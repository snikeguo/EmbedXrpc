#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedXrpcCommon.h"
#include "BlockBufferProvider.h"
class EmbedXrpcObject
{
public:
	//SerializationManager BufManager;


	uint8_t *Buffer;
	uint32_t BufferLen;

	uint32_t TimeOut;
	SendPack_t Send;
	IEmbeXrpcPort* porter;
	EmbedXrpc_Mutex_t ObjectMutexHandle;

	uint8_t *DelegateRingBuffer;
	int16_t DelegateRingBufferSize;
	BlockRingBufferProvider *DelegateBlockBufferProvider;

	uint8_t *ResponseRingBuffer;
	int16_t ResponseRingBufferSize;
	BlockRingBufferProvider *ResponseBlockBufferProvider;



	EmbedXrpc_Thread_t DelegateServiceThreadHandle;

	uint32_t ResponseDelegateMessageMapsCount;
	ResponseDelegateMessageMapCollection * ResponseDelegateMessageMaps;

	void* UserData;

	bool DeInitFlag;



	//server:
	EmbedXrpc_Thread_t ResponseServiceThreadHandle;
	//EmbedXrpc_Mutex_t ObjectMutexHandle;

	uint8_t* RequestRingBuffer;
	int16_t RequestRingBufferSize;
	BlockRingBufferProvider* RequestBlockBufferProvider;

	EmbedXrpc_Timer_t SuspendTimer;

	uint32_t RequestMessageMapsCount;
	RequestMessageMapCollection* RequestMessageMaps;

	EmbedXrpcObject(
		SendPack_t send,
		uint32_t timeOut,

		uint8_t* buf,
		uint32_t bufLen,

		uint8_t *responseRingBuffer,//接收到回复数据
		int16_t responseRingBufferSize,

		uint8_t *delegateRingBuffer,//接收到代理广播数据
		int16_t delegateRingBufferSize,

		ResponseDelegateMessageMapCollection* responseDelegateMessageMaps,//代理的services
		uint32_t ResponseDelegateMessageMapsCount,
		

		uint8_t* requestRingBuffer,//server  接收到请求数据
		int16_t requestRingBufferSize,//server

		RequestMessageMapCollection* requestMessageMaps,//server 请求的services
		uint32_t requestMessageMapsCount,//server

		IEmbeXrpcPort* port,
		
		void* ud = nullptr):Send(send),
		TimeOut(timeOut),
		Buffer(buf), 
		BufferLen(bufLen),
		ResponseRingBuffer(responseRingBuffer),
		ResponseRingBufferSize(responseRingBufferSize),
		DelegateRingBuffer(delegateRingBuffer),
		DelegateRingBufferSize(delegateRingBufferSize),
		ResponseDelegateMessageMaps(responseDelegateMessageMaps),
		ResponseDelegateMessageMapsCount(ResponseDelegateMessageMapsCount),
		RequestMessageMaps(requestMessageMaps),
		RequestMessageMapsCount(requestMessageMapsCount),
		RequestRingBuffer(requestRingBuffer),
		RequestRingBufferSize(requestRingBufferSize),
		porter(port),
		UserData(ud),
		//last
		DeInitFlag(false),
		ObjectMutexHandle(nullptr),
		DelegateBlockBufferProvider(nullptr),
		ResponseBlockBufferProvider(nullptr),
		DelegateServiceThreadHandle(nullptr),
		ResponseServiceThreadHandle(nullptr),
		RequestBlockBufferProvider(nullptr),
		SuspendTimer(nullptr)
	{
		
	}

	EmbedXrpcObject(//client
		SendPack_t send,
		uint32_t timeOut,

		uint8_t* buf,
		uint32_t bufLen,

		uint8_t* responseRingBuffer,
		int16_t responseRingBufferSize,

		uint8_t* delegateRingBuffer,
		int16_t delegateRingBufferSize,

		ResponseDelegateMessageMapCollection* mapCollection,
		uint32_t collectionCount,

		IEmbeXrpcPort* port,
		void* ud = nullptr):EmbedXrpcObject(send,
			timeOut,

			buf,
			bufLen,

			responseRingBuffer,
			responseRingBufferSize,

			delegateRingBuffer,
			delegateRingBufferSize,

			mapCollection,
			collectionCount,

			nullptr,
			0,

			nullptr,
			0,

			port,
			ud
		)
	{
		
	}
	EmbedXrpcObject(SendPack_t send,
		uint32_t timeOut,

		uint8_t* buf,
		uint32_t bufLen,

		uint8_t* requestRingBuffer,
		int16_t requestRingBufferSize,

		RequestMessageMapCollection* mapCollection,
		uint32_t collectionCount,

		IEmbeXrpcPort* port,
		void* ud = nullptr):EmbedXrpcObject(send,
			timeOut,

			buf,
			bufLen,

			nullptr,
			0,

			nullptr,
			0,

			nullptr,
			0,

			requestRingBuffer,
			requestRingBufferSize,

			mapCollection,
			collectionCount,

			port,
			ud
		)
	{
		
	}

	void Init()
	{
		DeInitFlag = false;

		ObjectMutexHandle = porter->CreateMutex("ObjectMutex");

		if (ResponseDelegateMessageMapsCount > 0)
		{
			DelegateServiceThreadHandle = porter->CreateThread("DelegateServiceThread", Client_ThreadPriority, DelegateServiceThread, this);
			DelegateBlockBufferProvider = new BlockRingBufferProvider(DelegateRingBuffer, DelegateRingBufferSize, porter);
			ResponseBlockBufferProvider = new BlockRingBufferProvider(ResponseRingBuffer, ResponseRingBufferSize, porter);
			porter->ThreadStart(DelegateServiceThreadHandle);
		}
		
		if (RequestMessageMapsCount > 0)
		{
			//server
			ResponseServiceThreadHandle = porter->CreateThread("ResponseServiceThread", Server_ThreadPriority, ResponseServiceThread, this);
			RequestBlockBufferProvider = new BlockRingBufferProvider(RequestRingBuffer, RequestRingBufferSize, porter);
			SuspendTimer = porter->CreateTimer("SuspendTimer", EmbedXrpc_WAIT_FOREVER, SuspendTimerCallBack, this);
			porter->ThreadStart(ResponseServiceThreadHandle);
		}
		

	}
	void DeInit()
	{
		DeInitFlag = true;
		porter->DeleteMutex(ObjectMutexHandle);
		if (ResponseDelegateMessageMapsCount > 0)
		{
			porter->DeleteThread(DelegateServiceThreadHandle);
			delete DelegateBlockBufferProvider;
			delete ResponseBlockBufferProvider;
		}
		
		if (RequestMessageMapsCount > 0)
		{
			//server
			porter->DeleteThread(ResponseServiceThreadHandle);
			porter->DeleteMutex(ObjectMutexHandle);
			delete RequestBlockBufferProvider;
		}
		

	}
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData)
	{
		if (allDataLen < 4)
			return false;
		bool FindFlag = false;
		bool SendQueueResult = false;
		if (ResponseDelegateMessageMapsCount > 0)
		{
			BlockBufferItemInfo raw;
			uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
			uint16_t targettimeout = (uint16_t)(allData[2] | allData[3] << 8);
			uint32_t dataLen = allDataLen - 4;
			uint8_t* data = &allData[4];

			//XrpcDebug("Client ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			if (serviceId == EmbedXrpcSuspendSid)
			{
				raw.Sid = serviceId;
				raw.DataLen = dataLen;
				raw.TargetTimeout = targettimeout;
				return ResponseBlockBufferProvider->Send(&raw, nullptr, 0);
			}
			for (uint32_t collectionIndex = 0; collectionIndex < ResponseDelegateMessageMapsCount; collectionIndex++)
			{
				auto MessageMaps = ResponseDelegateMessageMaps[collectionIndex].Map;
				for (uint32_t i = 0; i < ResponseDelegateMessageMaps[collectionIndex].Count; i++)
				{

					auto iter = &MessageMaps[i];
					if (iter->Sid == serviceId)
					{
						FindFlag = true;
						raw.Sid = serviceId;
						raw.DataLen = dataLen;
						raw.TargetTimeout = targettimeout;
						if (iter->ReceiveType == ReceiveType_Response)
						{
							SendQueueResult = ResponseBlockBufferProvider->Send(&raw, data, dataLen);
						}
						else if (iter->ReceiveType == ReceiveType_Delegate)
						{
							SendQueueResult = DelegateBlockBufferProvider->Send(&raw, data, dataLen);
						}
						goto sqr;
					}
				}
			}
		}
		else if (RequestMessageMapsCount>0)//server
		{
			BlockBufferItemInfo raw;
			uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
			uint16_t targettimeout = (uint16_t)(allData[2] | allData[3] << 8);
			uint32_t dataLen = allDataLen - 4;
			uint8_t* data = &allData[4];
			//XrpcDebug("Server ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			SendQueueResult=RequestBlockBufferProvider->Send(&raw, data, dataLen);
		}
		sqr:
		return SendQueueResult;
	}
	static void SuspendTimerCallBack(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		//obj->porter->TakeMutex(obj->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);//不需要加锁 这里不使用obj->buffer全局buffer
		uint8_t sb[4];
		sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
		sb[1] = (uint8_t)(EmbedXrpcSuspendSid >> 8 & 0xff);
		sb[2] = (uint8_t)(obj->TimeOut & 0xff);
		sb[3] = (uint8_t)(obj->TimeOut >> 8 & 0xff);
		obj->Send(obj, 4, sb);
		//obj->porter->ReleaseMutex(obj->ObjectMutexHandle);
	}
	static void DelegateServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		BlockBufferItemInfo recData;
		uint32_t i = 0;
		for (;;)
		{
			if (obj->DelegateBlockBufferProvider->Receive(&recData, 1)==true)
			{
				for (uint32_t collectionIndex = 0; collectionIndex < obj->ResponseDelegateMessageMapsCount; collectionIndex++)
				{
					auto MessageMaps = obj->ResponseDelegateMessageMaps[collectionIndex].Map;
					for (i = 0; i < obj->ResponseDelegateMessageMaps[collectionIndex].Count; i++)
					{
						if ((MessageMaps[i].ReceiveType == ReceiveType_Delegate) && (MessageMaps[i].Delegate->GetSid() == recData.Sid))
						{
							SerializationManager rsm;
							rsm.Reset();
							rsm.BufferLen = recData.DataLen;
							rsm.BlockBufferProvider=obj->DelegateBlockBufferProvider;
							rsm.BlockBufferProvider->SetCalculateSum(0);
							rsm.BlockBufferProvider->SetReferenceSum(recData.CheckSum);
							MessageMaps[i].Delegate->Invoke(rsm);
						}
					}
				}
				//obj->porter->Free(recData.Data);
				//XrpcDebug("Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
			}
			if (obj->DeInitFlag == true)
			{
				return;
			}
		}
	}
	static void ResponseServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		BlockBufferItemInfo recData;
		uint32_t i = 0;
		for (;;)
		{
			if (obj->RequestBlockBufferProvider->Receive(&recData, EmbedXrpc_WAIT_FOREVER) != true)
			{
				continue;
			}
			for (uint32_t collectionIndex = 0; collectionIndex < obj->RequestMessageMapsCount; collectionIndex++)
			{
				auto MessageMaps = obj->RequestMessageMaps[collectionIndex].Map;
				for (i = 0; i < obj->RequestMessageMaps[collectionIndex].Count; i++)
				{
					if (MessageMaps[i].Service->GetSid() == recData.Sid)
					{

						SerializationManager rsm;
						SerializationManager sendsm;
						rsm.Reset();
						rsm.BufferLen = recData.DataLen;
						rsm.BlockBufferProvider = obj->RequestBlockBufferProvider;
						rsm.BlockBufferProvider->SetCalculateSum(0);
						rsm.BlockBufferProvider->SetReferenceSum(recData.CheckSum);
						obj->porter->TakeMutex(obj->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);//由于使用obj->Buffer这个全局变量，所以添加锁
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
						obj->porter->ReleaseMutex(obj->ObjectMutexHandle);
					}
				}
			}

			//obj->porter->Free(recData.Data);
			//XrpcDebug("Server ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
		}
	}
	RequestResponseState Wait(uint32_t sid, const IType *type,void * response)
	{
		RequestResponseState ret= ResponseState_Ok;
		BlockBufferItemInfo recData;
		while (true)
		{	
			if (ResponseBlockBufferProvider->Receive(&recData, TimeOut) == false)
			{
				return ResponseState_Timeout;
			}
			if (recData.Sid == EmbedXrpcSuspendSid)
			{
				XrpcDebug("Client:recData.Sid == EmbedXrpcSuspendSid\n");
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
			rsm.BufferLen = recData.DataLen;
			rsm.BlockBufferProvider = ResponseBlockBufferProvider;
			rsm.BlockBufferProvider->SetCalculateSum(0);
			rsm.BlockBufferProvider->SetReferenceSum(recData.CheckSum);
			type->Deserialize(rsm, response);
			EmbedSerializationAssert(rsm.BlockBufferProvider->GetReferenceSum() == rsm.BlockBufferProvider->GetCalculateSum());
		}
		return ret;
	}
	
};

#endif
