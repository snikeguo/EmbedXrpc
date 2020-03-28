#pragma once
#ifndef EmbedXrpcClientObject_H
#define EmbedXrpcClientObject_H
#include <iostream>
#include <list>
#include "EmbedXrpcCommon.h"
class EmbedXrpcClientObject
{
public:
	std::list<std::string> ServicesName;
	SerializationManager BufManager;
	uint32_t TimeOut;
	SendPack_t Send;
	IEmbeXrpcPort* porter;
	EmbeXrpc_Mutex_t BufMutexHandle;
	EmbeXrpc_Queue_t DelegateMessageQueueHandle;
	EmbeXrpc_Queue_t ResponseMessageQueueHandle;
	EmbeXrpc_Thread_t ServiceThreadHandle;

	uint32_t DeserializeMapsCount;
	DeserializeMap* DeserializeMaps;

	uint32_t DelegateCount;
	IDelegate* Delegates;
	
	EmbedXrpcClientObject(uint32_t timeOut,
		uint8_t* buf,
		uint32_t bufLen,
		IEmbeXrpcPort *port,
		uint32_t deserializeMapsCount,
		DeserializeMap* deserializeMaps,
		uint32_t delegateCount,
		IDelegate *dels)
	{
		TimeOut = timeOut;
		BufManager.Buf = buf;
		BufManager.BufferLen = bufLen;
		BufManager.Reset();
		porter = port;
		DeserializeMapsCount = deserializeMapsCount;
		DeserializeMaps = deserializeMaps;
		DelegateCount = delegateCount;
		Delegates = dels;
	}
	void Init()
	{
		ServiceThreadHandle = porter->CreateThread("ServiceThread", ServiceThread);
		BufMutexHandle = porter->CreateMutex("BufMutex");
		DelegateMessageQueueHandle = porter->CreateQueue("DelegateMessageQueueHandle",sizeof(EmbeXrpcRawData),10);
		porter->ThreadStart(ServiceThreadHandle);
	}
	void ReceivedMessage(uint32_t serviceId, uint8_t* data, uint32_t dataLen)
	{
		EmbeXrpcRawData raw;
		for (uint32_t i=0;i< DeserializeMapsCount; i++)
		{
			auto iter = &DeserializeMaps[i];
			if (iter->Sid == serviceId)
			{		
				raw.Sid = serviceId;
				raw.MessageType = iter->MessageType;
				raw.Data =(uint8_t *) porter->Malloc(dataLen);
				porter->Memcpy(raw.Data, data, dataLen);
				raw.DataLen = dataLen;
				if (iter->ReceiveType == ReceiveType_Response)
				{					
					porter->SendQueue(ResponseMessageQueueHandle, &raw, sizeof(EmbeXrpcRawData));
				}
				else if (iter->ReceiveType == ReceiveType_Delegate)
				{
					porter->SendQueue(DelegateMessageQueueHandle, &raw, sizeof(raw));
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
			if (obj->porter->ReceiveQueue(obj->DelegateMessageQueueHandle, &recData, sizeof(EmbeXrpcRawData), WAIT_FOREVER)!=QueueState_OK)
			{
				continue;
			}
			for (i = 0; i < obj->DelegateCount; i++)
			{
				if (obj->Delegates[i].Sid == recData.Sid)
				{
					obj->Delegates[i].Invoke(recData.Data, recData.DataLen);

					obj->porter->Free(recData.Data);
				}
			}
		}
	}
	/*
	send request;//如果返回值是void 那么直接生成返回代码即可。否则继续往下走。
	EmbeXrpcResponseRawData recData;
	auto result=rpcobj->Wait(sid,&recData,&response);
	if(result==ResponseState_SidError)
	{
		porter->Free(recData.Data);
		Option<responseType> ret;
		ret.ResponseState=ResponseState_SidError;
		return ret;
	}
	else if(result==ResponseState_Ok)
	{
		porter->Free(recData.Data);
		Option<responseType> ret;
		ret.ResponseState=ResponseState_Ok;
		ret.Value=response;
		return ret;
	}
	*/
	ResponseState Wait(uint32_t sid, EmbeXrpcRawData* recData,void * response)
	{
		if (porter->ReceiveQueue(ResponseMessageQueueHandle, recData, sizeof(EmbeXrpcRawData), TimeOut) != QueueState_OK)
		{
			return ResponseState_Timeout;
		}
		if (sid != recData->Sid)
		{
			return ResponseState_SidError;
		}
		recData->MessageType->Deserialize(BufManager, response);
		return ResponseState_Ok;
	}
	
};

#endif
