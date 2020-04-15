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

	uint32_t MessageMapCount;
	RequestMessageMap* MessageMaps;

	EmbedXrpcServerObject(SendPack_t send, uint32_t timeOut, uint8_t* buf, uint32_t bufLen, IEmbeXrpcPort* port, uint32_t messageMapCount, RequestMessageMap* messageMaps)
	{
		Send = send;
		TimeOut = timeOut;
		Buffer = buf;
		BufferLen = bufLen;
		porter = port;
		MessageMapCount = messageMapCount;
		MessageMaps = messageMaps;
	}
	void Init()
	{
		ServiceThreadHandle = porter->CreateThread("ServiceThread", ServiceThread,this);
		SendMutexHandle = porter->CreateMutex("SendBufMutex");
		RequestQueueHandle = porter->CreateQueue("RequestQueueHandle", sizeof(EmbeXrpcRawData), 10);
		SuspendTimer = porter->CreateTimer("SuspendTimer", TimeOut/2,this, SuspendTimerCallBack);

		porter->ThreadStart(ServiceThreadHandle);
	}

	void ReceivedMessage(uint32_t allDataLen, uint8_t* allData)
	{
		if (allDataLen < 4)
			return;
		EmbeXrpcRawData raw;
		uint32_t serviceId = (uint32_t)(allData[0] | allData[1] << 8 | allData[2] << 16 | allData[3] << 24);
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
		porter->SendQueue(RequestQueueHandle, &raw, sizeof(EmbeXrpcRawData));

	}
	static void SuspendTimerCallBack(void* arg)
	{
		EmbedXrpcServerObject* obj = (EmbedXrpcServerObject*)arg;
		obj->porter->TakeMutex(obj->SendMutexHandle, EmbedXrpc_WAIT_FOREVER);
		uint8_t *sb= obj->Buffer;
		sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
		sb[1] = (uint8_t)(EmbedXrpcSuspendSid>>8 & 0xff);
		sb[2] = (uint8_t)(EmbedXrpcSuspendSid>>16 & 0xff);
		sb[3] = (uint8_t)(EmbedXrpcSuspendSid>>24 & 0xff);
		obj->Send(obj,4, sb);
		obj->porter->ReleaseMutex(obj->SendMutexHandle);
	}
	static void ServiceThread(void* arg)
	{
		EmbedXrpcServerObject* obj = (EmbedXrpcServerObject*)arg;
		EmbeXrpcRawData recData;
		uint32_t i = 0;
		for (;;)
		{
			if (obj->porter->ReceiveQueue(obj->RequestQueueHandle, &recData, sizeof(EmbeXrpcRawData), 1) != QueueState_OK)
			{
				continue;
			}
			for (i = 0; i < obj->MessageMapCount; i++)
			{
				if (obj->MessageMaps[i].Service->GetSid() == recData.Sid)
				{
					
					SerializationManager rsm;
					SerializationManager sendsm;
					rsm.Reset();
					rsm.Buf = recData.Data;
					rsm.BufferLen = recData.DataLen;

					//obj->porter->TakeMutex(obj->BufMutexHandle, EmbedXrpc_WAIT_FOREVER);
					sendsm.Reset();
					sendsm.Buf = &obj->Buffer[4];
					sendsm.BufferLen = obj->BufferLen-4;

					obj->porter->TimerReset(obj->SuspendTimer);
					obj->porter->TimerStart(obj->SuspendTimer);
					obj->MessageMaps[i].Service->Invoke(rsm, sendsm);
					obj->porter->TimerStop(obj->SuspendTimer);
					if (sendsm.Index > 0)//
					{
						obj->porter->TakeMutex(obj->SendMutexHandle, EmbedXrpc_WAIT_FOREVER);

						obj->Buffer[0] = (uint8_t)(recData.Sid & 0xff);
						obj->Buffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
						obj->Buffer[2] = (uint8_t)(recData.Sid >> 16 & 0xff);
						obj->Buffer[3] = (uint8_t)(recData.Sid >> 24 & 0xff);

						obj->Send(obj,sendsm.Index+4, obj->Buffer);

						obj->porter->ReleaseMutex(obj->SendMutexHandle);
					}
						
					//obj->porter->ReleaseMutex(obj->BufMutexHandle);
				}
			}
			obj->porter->Free(recData.Data);
			//XrpcDebug("Server ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
		}
	}
};
#endif