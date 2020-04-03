#pragma once
#ifndef EmbedXrpcServerObject_H
#define EmbedXrpcServerObject_H
#include <iostream>
#include <list>
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
	EmbeXrpc_Thread_t ServiceThreadHandle;
	EmbeXrpc_Mutex_t BufMutexHandle;
	EmbeXrpc_Queue_t RequestQueueHandle;

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
		BufMutexHandle = porter->CreateMutex("BufMutex");
		RequestQueueHandle = porter->CreateQueue("RequestQueueHandle", sizeof(EmbeXrpcRawData), 10);

		porter->ThreadStart(ServiceThreadHandle);
	}

	void ReceivedMessage(uint32_t serviceId, uint32_t dataLen, uint8_t* data)
	{
		EmbeXrpcRawData raw;
		raw.Sid = serviceId;
		raw.Data = (uint8_t*)porter->Malloc(dataLen);
		XrpcDebug("Client ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
		porter->Memcpy(raw.Data, data, dataLen);
		raw.DataLen = dataLen;
		porter->SendQueue(RequestQueueHandle, &raw, sizeof(EmbeXrpcRawData));

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

					obj->porter->TakeMutex(obj->BufMutexHandle, WAIT_FOREVER);
					sendsm.Reset();
					sendsm.Buf = obj->Buffer;
					sendsm.BufferLen = obj->BufferLen;

					obj->MessageMaps[i].Service->Invoke(rsm, sendsm);

					if (sendsm.Index > 0)//
						obj->Send(recData.Sid, sendsm.Index, sendsm.Buf);
					obj->porter->ReleaseMutex(obj->BufMutexHandle);
				}
			}
			obj->porter->Free(recData.Data);
			XrpcDebug("Server ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
		}
	}
};
#endif