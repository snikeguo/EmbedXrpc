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
	EmbedXrpc_Queue_t ResponseMessageQueueHandle;
	EmbedXrpc_Thread_t ServiceThreadHandle;

	uint32_t MessageMapsCount;
	ResponseDelegateMessageMap* MessageMaps;
	void* UserData;
	EmbedXrpcClientObject(
		SendPack_t send,
		uint32_t timeOut,
		uint8_t* buf,
		uint32_t bufLen,
		IEmbeXrpcPort *port,
		uint32_t messageMapsCount,
		ResponseDelegateMessageMap* messageMaps,
		void* ud = nullptr)
	{
		Send = send;
		TimeOut = timeOut;
		Buffer = buf;
		BufferLen = bufLen;
		porter = port;
		MessageMapsCount = messageMapsCount;
		MessageMaps = messageMaps;
		UserData = ud;
	}
	void Init()
	{
		ServiceThreadHandle = porter->CreateThread("ServiceThread", Client_ThreadPriority, ServiceThread,this);
		ObjectMutexHandle = porter->CreateMutex("ObjectMutex");
		DelegateMessageQueueHandle = porter->CreateQueue("DelegateMessageQueueHandle",sizeof(EmbeXrpcRawData), Client_DelegateMessageQueue_MaxItemNumber);
		ResponseMessageQueueHandle = porter->CreateQueue("ResponseMessageQueueHandle", sizeof(EmbeXrpcRawData), Client_ResponseMessageQueue_MaxItemNumber);
		porter->ThreadStart(ServiceThreadHandle);
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
			porter->SendQueue(ResponseMessageQueueHandle, &raw, sizeof(EmbeXrpcRawData));
			return;
		}

		for (uint32_t i=0;i< MessageMapsCount; i++)
		{
			auto iter = &MessageMaps[i];
			auto r = QueueState_OK;
			if (iter->Sid== serviceId)
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
					r=porter->SendQueue(ResponseMessageQueueHandle, &raw, sizeof(EmbeXrpcRawData));
				}
				else if (iter->ReceiveType == ReceiveType_Delegate)
				{
					r=porter->SendQueue(DelegateMessageQueueHandle, &raw, sizeof(raw));
				}
			}
			if (r != QueueState_OK)
			{
				if (dataLen > 0)
					porter->Free(raw.Data);
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
			if (obj->porter->ReceiveQueue(obj->DelegateMessageQueueHandle, &recData, sizeof(EmbeXrpcRawData), EmbedXrpc_WAIT_FOREVER)!=QueueState_OK)
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

	ResponseState Wait(uint32_t sid, const IType *type,void * response)
	{
		EmbeXrpcRawData recData;
		ResponseState ret= ResponseState_Ok;
		recData.Data = nullptr;
		recData.DataLen = 0;
		recData.Sid = 0;
		while (true)
		{
			if (porter->ReceiveQueue(ResponseMessageQueueHandle, &recData, sizeof(EmbeXrpcRawData), TimeOut) != QueueState_OK)
			{
				return ResponseState_Timeout;
			}
			if (recData.Sid == EmbedXrpcSuspendSid)
			{
				XrpcDebug("Client:recData.Sid == EmbedXrpcSuspendSid\n");
				if (recData.DataLen > 0)
				{
					porter->Free(recData.Data);
				}
				continue;
			}
			if (sid != recData.Sid)
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
			rsm.Buf = recData.Data;
			rsm.BufferLen = recData.DataLen;
			type->Deserialize(rsm, response);
		}
		if (recData.DataLen > 0)
		{
			porter->Free(recData.Data);
		}
		return ret;
	}
	
};

#endif
