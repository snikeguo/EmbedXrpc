#pragma once
#ifndef EmbedXrpcClientObject_H
#define EmbedXrpcClientObject_H
#include <iostream>
#include <list>
#include "EmbedXrpcCommon.h"
#include "EmbedSerialization.h"
class EmbedXrpcClientObject
{
public:
	//SerializationManager BufManager;


	uint8_t *Buffer;
	uint32_t BufferLen;

	uint32_t TimeOut;
	SendPack_t Send;
	IEmbeXrpcPort* porter;
	EmbeXrpc_Mutex_t BufMutexHandle;
	EmbeXrpc_Queue_t DelegateMessageQueueHandle;
	EmbeXrpc_Queue_t ResponseMessageQueueHandle;
	EmbeXrpc_Thread_t ServiceThreadHandle;

	uint32_t MessageMapsCount;
	ResponseDelegateMessageMap* MessageMaps;

	EmbedXrpcClientObject(
		SendPack_t send,
		uint32_t timeOut,
		uint8_t* buf,
		uint32_t bufLen,
		IEmbeXrpcPort *port,
		uint32_t messageMapsCount,
		ResponseDelegateMessageMap* messageMaps)
	{
		Send = send;
		TimeOut = timeOut;
		Buffer = buf;
		BufferLen = bufLen;
		porter = port;
		MessageMapsCount = messageMapsCount;
		MessageMaps = messageMaps;

	}
	void Init()
	{
		ServiceThreadHandle = porter->CreateThread("ServiceThread", ServiceThread,this);
		BufMutexHandle = porter->CreateMutex("BufMutex");
		DelegateMessageQueueHandle = porter->CreateQueue("DelegateMessageQueueHandle",sizeof(EmbeXrpcRawData),10);
		ResponseMessageQueueHandle = porter->CreateQueue("ResponseMessageQueueHandle", sizeof(EmbeXrpcRawData), 10);
		porter->ThreadStart(ServiceThreadHandle);
	}
	void ReceivedMessage(uint32_t serviceId, uint32_t dataLen, uint8_t* data)
	{
		EmbeXrpcRawData raw;

		for (uint32_t i=0;i< MessageMapsCount; i++)
		{
			auto iter = &MessageMaps[i];
			if (iter->Sid== serviceId)
			{		
				raw.Sid = serviceId;
				//raw.MessageType = iter->MessageType;
				raw.Data =(uint8_t *) porter->Malloc(dataLen);
				XrpcDebug("Client ReceivedMessage  Malloc :0x%x,size:%d\n",(uint32_t) raw.Data, dataLen);

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
			for (i = 0; i < obj->MessageMapsCount; i++)
			{
				if( (obj->MessageMaps[i].ReceiveType== ReceiveType_Delegate)&&(obj->MessageMaps[i].Delegate->GetSid() == recData.Sid))
				{
					SerializationManager rsm;
					rsm.Reset();
					rsm.Buf = recData.Data;
					rsm.BufferLen = recData.DataLen;
					obj->MessageMaps[i].Delegate->Invoke(rsm);
				}
			}
			obj->porter->Free(recData.Data);
			XrpcDebug("Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
		}
	}

	ResponseState Wait(uint32_t sid, IType *type,void * response)
	{
		EmbeXrpcRawData recData;
		if (porter->ReceiveQueue(ResponseMessageQueueHandle, &recData, sizeof(EmbeXrpcRawData), TimeOut) != QueueState_OK)
		{
			return ResponseState_Timeout;
		}
		ResponseState ret;
		if (sid != recData.Sid)
		{
			ret=ResponseState_SidError;
		}
		SerializationManager rsm;
		rsm.Reset();
		rsm.Buf = recData.Data;
		rsm.BufferLen = recData.DataLen;
		type->Deserialize(rsm, response);
		ret = ResponseState_Ok;
		porter->Free(recData.Data);
		return ret;
	}
	
};

#endif
