#pragma once
#ifndef EmbedXrpcClientObject_H
#define EmbedXrpcClientObject_H
#include "EmbedXrpcCommon.h"
class EmbedXrpcClientObject
{
public:
	//SerializationManager BufManager;


	uint8_t *Buffer;
	uint32_t BufferLen;

	uint32_t TimeOut;
	SendPack_t Send;
	IEmbeXrpcPort* porter;
	EmbedXrpc_Mutex_t ObjectMutexHandle;
	EmbedXrpc_Queue_t DelegateMessageQueueHandle;
	//EmbedXrpc_Queue_t ResponseMessageQueueHandle;
	EmbedXrpc_Semaphore_t ResponseMessageSemaphoreHandle;
	EmbeXrpcRawData ResponseMessageEmbedXrpcRawData;//回复数据



	EmbedXrpc_Thread_t ServiceThreadHandle;

	uint32_t CollectionCount;
	//ResponseDelegateMessageMap* MessageMaps;

	ResponseDelegateMessageMapCollection *MapCollection;

	void* UserData;

	bool DeInitFlag = false;
	EmbedXrpcClientObject(
		SendPack_t send,
		uint32_t timeOut,
		uint8_t* buf,
		uint32_t bufLen,
		IEmbeXrpcPort *port,
		uint32_t collectionCount,
		ResponseDelegateMessageMapCollection* mapCollection,
		void* ud = nullptr)
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
		ServiceThreadHandle = porter->CreateThread("ServiceThread", Client_ThreadPriority, ServiceThread,this);
		ObjectMutexHandle = porter->CreateMutex("ObjectMutex");
		DelegateMessageQueueHandle = porter->CreateQueue("DelegateMessageQueueHandle",sizeof(EmbeXrpcRawData), Client_DelegateMessageQueue_MaxItemNumber);
		ResponseMessageSemaphoreHandle = porter->CreateSemaphore("ResponseMessageSemaphoreHandle");
		porter->ThreadStart(ServiceThreadHandle);
	}
	void DeInit()
	{
		DeInitFlag = true;
		porter->DeleteThread(ServiceThreadHandle);
		porter->DeleteMutex(ObjectMutexHandle);
		porter->DeleteQueue(DelegateMessageQueueHandle);
		porter->DeleteSemaphore(ResponseMessageSemaphoreHandle);
	}
	void ReceivedMessage(uint32_t allDataLen, uint8_t* allData)
	{
		if (allDataLen < 4)
			return;
		EmbeXrpcRawData raw;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout= (uint16_t)(allData[2] | allData[3] << 8);
		raw.Data = nullptr;
		raw.DataLen = 0;
		raw.Sid = 0;
		uint32_t dataLen = allDataLen - 4;
		uint8_t* data = &allData[4];
		//XrpcDebug("Client ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
		if (serviceId == EmbedXrpcSuspendSid)
		{
			raw.Sid = serviceId;
			if (dataLen > 0)
			{
				raw.Data = (uint8_t*)porter->Malloc(dataLen);
				porter->Memcpy(raw.Data, data, dataLen);
			}
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			ResponseMessageEmbedXrpcRawData = raw;
			porter->ReleaseSemaphore(ResponseMessageSemaphoreHandle);
			return;
		}
		for (uint32_t collectionIndex = 0; collectionIndex < CollectionCount; collectionIndex++)
		{
			auto MessageMaps = MapCollection[collectionIndex].Map;
			for (uint32_t i = 0; i < MapCollection[collectionIndex].Count; i++)
			{

				auto iter = &MessageMaps[i];
				if (iter->Sid == serviceId)
				{
					raw.Sid = serviceId;
					//raw.MessageType = iter->MessageType;
					if (dataLen > 0)
					{
						raw.Data = (uint8_t*)porter->Malloc(dataLen);
						porter->Memcpy(raw.Data, data, dataLen);
					}
					raw.DataLen = dataLen;
					raw.TargetTimeout = targettimeout;
					if (iter->ReceiveType == ReceiveType_Response)
					{
						ResponseMessageEmbedXrpcRawData = raw;
						porter->ReleaseSemaphore(ResponseMessageSemaphoreHandle);
					}
					else if (iter->ReceiveType == ReceiveType_Delegate)
					{
						auto r = porter->SendQueue(DelegateMessageQueueHandle, &raw, sizeof(raw));
						if (r != QueueState_OK)
						{
							if (dataLen > 0)
								porter->Free(raw.Data);
						}
					}
					return;//只要匹配相等的SID 就返回了
				}		
			}
		}
		
	}
	static void ServiceThread(void* arg)
	{
		EmbedXrpcClientObject* obj = (EmbedXrpcClientObject*)arg;
		EmbeXrpcRawData recData;
		uint32_t i = 0;
		for (;;)
		{
			if (obj->porter->ReceiveQueue(obj->DelegateMessageQueueHandle, &recData, sizeof(EmbeXrpcRawData), 1)==QueueState_OK)
			{
				for (uint32_t collectionIndex = 0; collectionIndex < obj->CollectionCount; collectionIndex++)
				{
					auto MessageMaps = obj->MapCollection[collectionIndex].Map;
					for (i = 0; i < obj->MapCollection[collectionIndex].Count; i++)
					{
						if ((MessageMaps[i].ReceiveType == ReceiveType_Delegate) && (MessageMaps[i].Delegate->GetSid() == recData.Sid))
						{
							SerializationManager rsm;
							rsm.Reset();
							rsm.Buf = recData.Data;
							rsm.BufferLen = recData.DataLen;
							MessageMaps[i].Delegate->Invoke(rsm);
						}
					}
				}

				obj->porter->Free(recData.Data);
				XrpcDebug("Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
			}
			if (obj->DeInitFlag == true)
			{
				return;
			}
		}
	}

	ResponseState Wait(uint32_t sid, const IType *type,void * response)
	{
		ResponseState ret= ResponseState_Ok;
		EmbeXrpcRawData* recData = &ResponseMessageEmbedXrpcRawData;
		while (true)
		{
			if (porter->TakeSemaphore(ResponseMessageSemaphoreHandle, TimeOut) == false)
			{
				return ResponseState_Timeout;
			}
			if (recData->Sid == EmbedXrpcSuspendSid)
			{
				XrpcDebug("Client:recData.Sid == EmbedXrpcSuspendSid\n");
				if (recData->DataLen > 0)
				{
					porter->Free(recData->Data);
				}
				continue;
			}
			if (sid != recData->Sid)
			{
				ret = ResponseState_SidError;
			}
			else
			{
				XrpcDebug("sid == recData.Sid\n");
				ret = ResponseState_Ok;
			}
			break;
		}
		if (ret == ResponseState_Ok)
		{
			SerializationManager rsm;
			rsm.Reset();
			rsm.Buf = recData->Data;
			rsm.BufferLen = recData->DataLen;
			type->Deserialize(rsm, response);
		}
		if (recData->DataLen > 0)
		{
			porter->Free(recData->Data);
		}
		return ret;
	}
	
};

#endif
