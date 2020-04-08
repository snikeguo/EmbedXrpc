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

	void ReceivedMessage(uint32_t serviceId, uint32_t dataLen, uint8_t* data)
	{
		EmbeXrpcRawData raw;

		raw.Data = nullptr;
		raw.DataLen = 0;
		raw.Sid = 0;

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
		obj->porter->TakeMutex(obj->SendMutexHandle, WAIT_FOREVER);
		obj->Send(EmbedXrpcSuspendSid, 0, nullptr);
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

					//obj->porter->TakeMutex(obj->BufMutexHandle, WAIT_FOREVER);
					sendsm.Reset();
					sendsm.Buf = obj->Buffer;
					sendsm.BufferLen = obj->BufferLen;

					obj->porter->TimerReset(obj->SuspendTimer);
					obj->porter->TimerStart(obj->SuspendTimer);
					obj->MessageMaps[i].Service->Invoke(rsm, sendsm);
					obj->porter->TimerStop(obj->SuspendTimer);
					if (sendsm.Index > 0)//
					{
						obj->porter->TakeMutex(obj->SendMutexHandle, WAIT_FOREVER);
						obj->Send(recData.Sid, sendsm.Index, sendsm.Buf);
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