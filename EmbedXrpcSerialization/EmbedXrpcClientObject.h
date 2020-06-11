#pragma once
#ifndef EmbedXrpcClientObject_H
#define EmbedXrpcClientObject_H
#include "EmbedXrpcCommon.h"
#include "BlockBufferProvider.h"
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

	uint8_t *DelegateRingBuffer;
	int16_t DelegateRingBufferSize;
	BlockRingBufferProvider *DelegateBlockBufferProvider;

	uint8_t *ResponseRingBuffer;
	int16_t ResponseRingBufferSize;
	BlockRingBufferProvider *ResponseBlockBufferProvider;



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

		uint8_t *responseRingBuffer,
		int16_t responseRingBufferSize,

		uint8_t *delegateRingBuffer,
		int16_t delegateRingBufferSize,

		IEmbeXrpcPort *port,
		uint32_t collectionCount,
		ResponseDelegateMessageMapCollection* mapCollection,
		void* ud = nullptr)
	{
		Send = send;
		TimeOut = timeOut;
		Buffer = buf;
		BufferLen = bufLen;

		ResponseRingBuffer=responseRingBuffer;
		ResponseRingBufferSize=responseRingBufferSize;

		DelegateRingBuffer=delegateRingBuffer;
		DelegateRingBufferSize=delegateRingBufferSize;

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
		
		DelegateBlockBufferProvider=new BlockRingBufferProvider(DelegateRingBuffer,DelegateRingBufferSize,porter);
		//DelegateMessageQueueHandle = porter->CreateQueue("DelegateMessageQueueHandle",sizeof(EmbeXrpcRawData), Client_DelegateMessageQueue_MaxItemNumber);
		
		ResponseBlockBufferProvider=new BlockRingBufferProvider(ResponseRingBuffer,ResponseRingBufferSize,porter);
		//ResponseMessageSemaphoreHandle = porter->CreateSemaphore("ResponseMessageSemaphoreHandle");
		porter->ThreadStart(ServiceThreadHandle);
	}
	void DeInit()
	{
		DeInitFlag = true;
		porter->DeleteThread(ServiceThreadHandle);
		porter->DeleteMutex(ObjectMutexHandle);

		delete DelegateBlockBufferProvider;
		//porter->DeleteQueue(DelegateMessageQueueHandle);
		
		delete ResponseBlockBufferProvider;
		//porter->DeleteSemaphore(ResponseMessageSemaphoreHandle);

	}
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData)
	{
		if (allDataLen < 4)
			return false;
		BlockBufferItemInfo raw;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout= (uint16_t)(allData[2] | allData[3] << 8);
		uint32_t dataLen = allDataLen - 4;
		uint8_t* data = &allData[4];
		bool SendQueueResult=false;
		//XrpcDebug("Client ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
		if (serviceId == EmbedXrpcSuspendSid)
		{
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			return ResponseBlockBufferProvider->Send(&raw,nullptr,0);
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
					raw.DataLen = dataLen;
					raw.TargetTimeout = targettimeout;
					if (iter->ReceiveType == ReceiveType_Response)
					{
						SendQueueResult=ResponseBlockBufferProvider->Send(&raw, data,dataLen);
					}
					else if (iter->ReceiveType == ReceiveType_Delegate)
					{
						SendQueueResult=DelegateBlockBufferProvider->Send(&raw, data,dataLen);
					}
					return SendQueueResult;//只要匹配相等的SID 就返回了
				}		
			}
		}
		return SendQueueResult;
	}
	static void ServiceThread(void* arg)
	{
		EmbedXrpcClientObject* obj = (EmbedXrpcClientObject*)arg;
		BlockBufferItemInfo recData;
		uint32_t i = 0;
		for (;;)
		{
			if (obj->DelegateBlockBufferProvider->Receive(&recData, 1)==true)
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
