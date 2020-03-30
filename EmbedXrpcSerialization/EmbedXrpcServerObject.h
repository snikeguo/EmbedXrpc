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
	std::list<std::string> ServicesName;

	uint8_t* Buffer;
	uint32_t BufferLen;

	uint32_t TimeOut;
	SendPack_t Send;
	IEmbeXrpcPort* porter;
	EmbeXrpc_Thread_t ServiceThreadHandle;
	EmbeXrpc_Mutex_t BufMutexHandle;
	EmbeXrpc_Queue_t RequestQueueHandle;

	uint32_t ServiceCount;
	IService* Services;

	EmbedXrpcServerObject(uint32_t timeOut, uint8_t* buf, uint32_t bufLen, IEmbeXrpcPort* port, uint32_t serviceCount, IService* services)
	{
		TimeOut = timeOut;
		Buffer = buf;
		BufferLen = bufLen;
		porter = port;
		ServiceCount = serviceCount;
		Services = services;
	}
	void Init()
	{
		ServiceThreadHandle = porter->CreateThread("ServiceThread", ServiceThread);
		BufMutexHandle = porter->CreateMutex("BufMutex");
		RequestQueueHandle = porter->CreateQueue("RequestQueueHandle", sizeof(EmbeXrpcRawData), 10);

		porter->ThreadStart(ServiceThreadHandle);
	}
	static void ServiceThread(void* arg)
	{
		EmbedXrpcServerObject* obj = (EmbedXrpcServerObject*)arg;
		EmbeXrpcRawData recData;
		uint32_t i = 0;
		for (;;)
		{
			if (obj->porter->ReceiveQueue(obj->RequestQueueHandle, &recData, sizeof(EmbeXrpcRawData), WAIT_FOREVER) != QueueState_OK)
			{
				continue;
			}

			for (i = 0; i < obj->ServiceCount; i++)
			{
				if (obj->Services[i].Sid == recData.Sid)
				{
					
					SerializationManager rsm;
					SerializationManager sendsm;
					rsm.Reset();
					rsm.Buf = recData.Data;
					rsm.BufferLen = recData.DataLen;

					sendsm.Reset();
					sendsm.Buf = obj->Buffer;
					sendsm.BufferLen = obj->BufferLen;

					obj->Services[i].Invoke(rsm, sendsm);

					if (sendsm.Index > 0)//
						obj->Send(recData.Sid, sendsm.Index, sendsm.Buf);
					obj->porter->Free(recData.Data);

				}
			}
		}
	}
};
#endif