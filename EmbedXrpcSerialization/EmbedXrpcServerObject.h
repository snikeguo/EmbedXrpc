#ifndef EmbedXrpcServerObject_H
#define EmbedXrpcServerObject_H

#include "EmbedXrpcCommon.h"
#include "EmbedSerialization.h"
class EmbedXrpcServerObject
{
public:

	uint8_t* Buffer;
	uint32_t BufferLen;

	uint32_t TimeOut;
	SendPack_t Send;
	IEmbeXrpcPort* porter;
	EmbedXrpc_Thread_t ServiceThreadHandle;
	EmbedXrpc_Mutex_t SendMutexHandle;
	EmbedXrpc_Queue_t RequestQueueHandle;
	EmbedXrpc_Timer_t SuspendTimer;

	uint32_t CollectionCount;
	RequestMessageMapCollection* MapCollection;

	void* UserData;
	bool DeInitFlag = false;
	EmbedXrpcServerObject(SendPack_t send,
		uint32_t timeOut, 
		uint8_t* buf,
		uint32_t bufLen,
		IEmbeXrpcPort* port,
		uint32_t collectionCount,
		RequestMessageMapCollection* mapCollection,
		void *ud=nullptr)
	{
		Send = send;
		TimeOut = timeOut;
		Buffer = buf;
		BufferLen = bufLen;
		porter = port;
		CollectionCount = collectionCount;
		MapCollection = mapCollection;
		UserData = ud;
	}
	void Init()
	{
		DeInitFlag = false;
		ServiceThreadHandle = porter->CreateThread("ServiceThread",Server_ThreadPriority, ServiceThread,this);
		SendMutexHandle = porter->CreateMutex("SendBufMutex");
		RequestQueueHandle = porter->CreateQueue("RequestQueueHandle", sizeof(EmbeXrpcRawData), Server_RequestQueue_MaxItemNumber);
		SuspendTimer = porter->CreateTimer("SuspendTimer", EmbedXrpc_WAIT_FOREVER,this, SuspendTimerCallBack);
		porter->ThreadStart(ServiceThreadHandle);
	}
	void DeInit()
	{
		DeInitFlag = true;
		porter->DeleteThread(ServiceThreadHandle);
		porter->DeleteMutex(SendMutexHandle);
		porter->DeleteQueue(RequestQueueHandle);
	}

	void ReceivedMessage(uint32_t allDataLen, uint8_t* allData)
	{
		if (allDataLen < 4)
			return;
		EmbeXrpcRawData raw;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout = (uint16_t)(allData[2] | allData[3] << 8);
		raw.Data = nullptr;
		raw.DataLen = 0;
		raw.Sid = 0;
		uint32_t dataLen = allDataLen - 4;
		uint8_t* data = &allData[4];
		//XrpcDebug("Server ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);

		raw.Sid = serviceId;
		if (dataLen > 0)
		{
			raw.Data = (uint8_t*)porter->Malloc(dataLen);
			porter->Memcpy(raw.Data, data, dataLen);
		}
		raw.DataLen = dataLen;
		raw.TargetTimeout = targettimeout;
		auto r=porter->SendQueue(RequestQueueHandle, &raw, sizeof(EmbeXrpcRawData));
		if (r != QueueState_OK)
		{
			if (dataLen > 0)
				porter->Free(raw.Data);
		}

	}
	static void SuspendTimerCallBack(void* arg)
	{
		EmbedXrpcServerObject* obj = (EmbedXrpcServerObject*)arg;
		//obj->porter->TakeMutex(obj->SendMutexHandle, EmbedXrpc_WAIT_FOREVER);//不需要加锁 这里不使用obj->buffer全局buffer
		uint8_t sb[4];
		sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
		sb[1] = (uint8_t)(EmbedXrpcSuspendSid>>8 & 0xff);
		sb[2] = (uint8_t)(obj->TimeOut & 0xff);
		sb[3] = (uint8_t)(obj->TimeOut >>8 & 0xff);
		obj->Send(obj,4, sb);
		//obj->porter->ReleaseMutex(obj->SendMutexHandle);
	}
	static void ServiceThread(void* arg)
	{
		EmbedXrpcServerObject* obj = (EmbedXrpcServerObject*)arg;
		EmbeXrpcRawData recData;
		uint32_t i = 0;
		for (;;)
		{
			if (obj->porter->ReceiveQueue(obj->RequestQueueHandle, &recData, sizeof(EmbeXrpcRawData), EmbedXrpc_WAIT_FOREVER) != QueueState_OK)
			{
				continue;
			}
			for (uint32_t collectionIndex = 0; collectionIndex < obj->CollectionCount; collectionIndex++)
			{
				auto MessageMaps = obj->MapCollection[collectionIndex].Map;
				for (i = 0; i < obj->MapCollection[collectionIndex].Count; i++)
				{
					if (MessageMaps[i].Service->GetSid() == recData.Sid)
					{

						SerializationManager rsm;
						SerializationManager sendsm;
						rsm.Reset();
						rsm.Buf = recData.Data;
						rsm.BufferLen = recData.DataLen;

						//obj->porter->TakeMutex(obj->BufMutexHandle, EmbedXrpc_WAIT_FOREVER);
						obj->porter->TakeMutex(obj->SendMutexHandle, EmbedXrpc_WAIT_FOREVER);//由于使用obj->Buffer这个全局变量，所以添加锁
						sendsm.Reset();
						sendsm.Buf = &obj->Buffer[4];
						sendsm.BufferLen = obj->BufferLen - 4;

						obj->porter->TimerReset(obj->SuspendTimer);
						obj->porter->TimerStart(obj->SuspendTimer, recData.TargetTimeout / 2);
						MessageMaps[i].Service->Invoke(rsm, sendsm);
						obj->porter->TimerStop(obj->SuspendTimer);

						if (sendsm.Index > 0)//
						{
							obj->Buffer[0] = (uint8_t)(recData.Sid >> 0 & 0xff);
							obj->Buffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
							obj->Buffer[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
							obj->Buffer[3] = (uint8_t)(obj->TimeOut >> 8 & 0xff);
							obj->Send(obj, sendsm.Index + 4, obj->Buffer);
						}
						obj->porter->ReleaseMutex(obj->SendMutexHandle);
					}
				}
			}
			
			obj->porter->Free(recData.Data);
			//XrpcDebug("Server ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
		}
	}
};
#endif