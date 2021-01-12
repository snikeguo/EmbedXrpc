#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedXrpcCommon.h"
#if EmbedXrpc_UseRingBufferWhenReceiving==1
#include "BlockBufferProvider.h"
#endif

class EmbedXrpcObject
{
public:

	uint8_t DataLinkLayoutBuffer[EmbedXrpc_SendBufferSize];

	uint32_t TimeOut;
	SendPack_t Send;
	EmbedXrpc_Mutex_t ObjectMutexHandle;

#if EmbedXrpc_UseRingBufferWhenReceiving==1
	uint8_t DelegateRingBuffer[EmbedXrpc_DelegateRingBufferSize];
	BlockRingBufferProvider *DelegateBlockBufferProvider=nullptr;

	uint8_t ResponseRingBuffer[EmbedXrpc_ResponseRingBufferSize];
	BlockRingBufferProvider *ResponseBlockBufferProvider = nullptr;
#else
	EmbedXrpc_Queue_t DelegateBlockQueue = nullptr;
	EmbedXrpc_Queue_t ResponseBlockQueue = nullptr;
#endif


	EmbedXrpc_Thread_t DelegateServiceThreadHandle;

	uint32_t ResponseDelegateMessageMapsCount;
	ResponseDelegateMessageMapCollection * ResponseDelegateMessageMaps;

	void* UserData;

	bool DeInitFlag;

	bool IsEnableMataDataEncode;

	//server:
	EmbedXrpc_Thread_t ResponseServiceThreadHandle;
	//EmbedXrpc_Mutex_t ObjectMutexHandle;
#if EmbedXrpc_UseRingBufferWhenReceiving==1
	uint8_t RequestRingBuffer[EmbedXrpc_RequestRingBufferSize];
	BlockRingBufferProvider* RequestBlockBufferProvider;
#else
	EmbedXrpc_Queue_t	RequestBlockQueue;
#endif
	EmbedXrpc_Timer_t SuspendTimer;

	uint32_t RequestMessageMapsCount;
	RequestMessageMapCollection* RequestMessageMaps;

	EmbedXrpcObject(
		SendPack_t send,
		uint32_t timeOut,
		ResponseDelegateMessageMapCollection* responseDelegateMessageMaps,//代理的services
		uint32_t responseDelegateMessageMapsCount,

		RequestMessageMapCollection* requestMessageMaps,//server 请求的services
		uint32_t requestMessageMapsCount,//server

		bool isEnableMataDataEncode,
		void* ud = nullptr):Send(send),
		TimeOut(timeOut),
		ResponseDelegateMessageMaps(responseDelegateMessageMaps),
		ResponseDelegateMessageMapsCount(responseDelegateMessageMapsCount),
		RequestMessageMaps(requestMessageMaps),
		RequestMessageMapsCount(requestMessageMapsCount),
		IsEnableMataDataEncode(isEnableMataDataEncode),
		UserData(ud),
		//last
		DeInitFlag(false),
		ObjectMutexHandle(nullptr),
		DelegateServiceThreadHandle(nullptr),
		ResponseServiceThreadHandle(nullptr),
		SuspendTimer(nullptr)
	{
		
	}
	//client节点构造函数
	EmbedXrpcObject(SendPack_t send,
		uint32_t timeOut,

		ResponseDelegateMessageMapCollection* mapCollection,
		uint32_t collectionCount,

		bool isEnableMataDataEncode,
		void* ud = nullptr):EmbedXrpcObject(send,
			timeOut,

			mapCollection,
			collectionCount,

			nullptr,
			0,

			isEnableMataDataEncode,
			ud
		)
	{
		
	}
	//server节点的构造函数
	EmbedXrpcObject(SendPack_t send,
		uint32_t timeOut,

		RequestMessageMapCollection* mapCollection,
		uint32_t collectionCount,

		bool isEnableMataDataEncode,
		void* ud = nullptr):EmbedXrpcObject(send,
			timeOut,
			nullptr,
			0,
			mapCollection,
			collectionCount,

			isEnableMataDataEncode,
			ud
		)
	{
		
	}

	void Init()
	{
		DeInitFlag = false;

		ObjectMutexHandle = EmbedXrpc_CreateMutex("ObjectMutex");

#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0 && EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
	if (ResponseDelegateMessageMapsCount > 0)
	{
	#if EmbedXrpc_UseRingBufferWhenReceiving==1

		#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0
					DelegateBlockBufferProvider = new BlockRingBufferProvider(DelegateRingBuffer, EmbedXrpc_DelegateRingBufferSize, EmbedXrpc_DelegateBlockQueue_MaxItemNumber);
		#endif

		#if EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
					ResponseBlockBufferProvider = new BlockRingBufferProvider(ResponseRingBuffer, EmbedXrpc_ResponseRingBufferSize, EmbedXrpc_ResponseBlockQueue_MaxItemNumber);
		#endif

	#else
		#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0
					DelegateBlockQueue = EmbedXrpc_CreateQueue("DelegateBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_DelegateBlockQueue_MaxItemNumber);
		#endif
		#if EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
					ResponseBlockQueue = EmbedXrpc_CreateQueue("ResponseBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_ResponseBlockQueue_MaxItemNumber);
		#endif
	#endif
			DelegateServiceThreadHandle = EmbedXrpc_CreateThread("DelegateServiceThread", Client_ThreadPriority, DelegateServiceThread, this);			
			EmbedXrpc_ThreadStart(DelegateServiceThreadHandle);
	}
#endif		

#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0
	if (RequestMessageMapsCount > 0)
	{
		//server
	#if EmbedXrpc_UseRingBufferWhenReceiving==1
		#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0
			RequestBlockBufferProvider = new BlockRingBufferProvider(RequestRingBuffer, EmbedXrpc_RequestRingBufferSize, EmbedXrpc_RequestBlockQueue_MaxItemNumber);
		#endif
	#else
		#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0
				RequestBlockQueue = EmbedXrpc_CreateQueue("RequestBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_RequestBlockQueue_MaxItemNumber);
		#endif
	#endif
			ResponseServiceThreadHandle = EmbedXrpc_CreateThread("ResponseServiceThread", Server_ThreadPriority, ResponseServiceThread, this);
			SuspendTimer = EmbedXrpc_CreateTimer("SuspendTimer", EmbedXrpc_WAIT_FOREVER, SuspendTimerCallBack, this);
			EmbedXrpc_ThreadStart(ResponseServiceThreadHandle);
	}
#endif		

	}
	void DeInit()
	{
		DeInitFlag = true;
		EmbedXrpc_DeleteMutex(ObjectMutexHandle);
#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0 && EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
		if (ResponseDelegateMessageMapsCount > 0)
		{
			EmbedXrpc_DeleteThread(DelegateServiceThreadHandle);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
	#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0
			delete DelegateBlockBufferProvider;
	#endif
	#if EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
			delete ResponseBlockBufferProvider;
	#endif
#endif
		}
#endif

#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0
		if (RequestMessageMapsCount > 0)
		{
			//server
			EmbedXrpc_DeleteThread(ResponseServiceThreadHandle);
			EmbedXrpc_DeleteMutex(ObjectMutexHandle);
	#if EmbedXrpc_UseRingBufferWhenReceiving==1
		#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0
			delete RequestBlockBufferProvider;
		#endif
	#endif
		}
#endif		

	}
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData)
	{
		if (allDataLen < 4)
			return false;
		bool FindFlag = false;
		QueueState EmbedXrpc_SendQueueResult = QueueState_Full;

		ReceiveItemInfo raw;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout = (uint16_t)(allData[2] | allData[3] << 8)&0x3FFF;
		uint32_t dataLen = allDataLen - 4;
		uint8_t* data = &allData[4];
		ReceiveType_t rt = (ReceiveType_t)(allData[3] >> 6);
		if (rt== ReceiveType_Response|| rt == ReceiveType_Delegate)
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Client ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			if (serviceId == EmbedXrpcSuspendSid)//挂起的SID要特殊处理
			{
				raw.Sid = serviceId;
				raw.DataLen = dataLen;
				raw.TargetTimeout = targettimeout;
				raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
				EmbedXrpc_SendQueueResult=ResponseBlockBufferProvider->Send(&raw, nullptr, 0)==true? QueueState_OK: QueueState_Full;
#else
				raw.Data = nullptr;
				if(EmbedXrpc_QueueSpacesAvailable(ResponseBlockQueue)>0)
				{
					EmbedXrpc_SendQueueResult = EmbedXrpc_SendQueue(ResponseBlockQueue, &raw, sizeof(ReceiveItemInfo));
				}
				else
				{
					EmbedXrpc_SendQueueResult = QueueState_Full;
				}
#endif
				goto sqr;
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
						raw.CheckSum = GetSum(data, dataLen);
						if (iter->ReceiveType == ReceiveType_Response)
						{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
							EmbedXrpc_SendQueueResult = ResponseBlockBufferProvider->Send(&raw, data, dataLen) == true ? QueueState_OK : QueueState_Full;
#else
							if(EmbedXrpc_QueueSpacesAvailable(ResponseBlockQueue)>0)
							{
								if (dataLen > 0)
								{
									raw.Data = (uint8_t *)Malloc(dataLen);
									if (raw.Data == nullptr)
									{
										goto sqr;
									}
									Memcpy(raw.Data, data, dataLen);
								}
								EmbedXrpc_SendQueueResult = EmbedXrpc_SendQueue(ResponseBlockQueue, &raw, sizeof(ReceiveItemInfo));
							}
							else
							{
								EmbedXrpc_SendQueueResult = QueueState_Full;
							}
#endif
						}
						else if (iter->ReceiveType == ReceiveType_Delegate)
						{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
							EmbedXrpc_SendQueueResult = DelegateBlockBufferProvider->Send(&raw, data, dataLen) == true ? QueueState_OK : QueueState_Full;
#else
							if(EmbedXrpc_QueueSpacesAvailable(DelegateBlockQueue)>0)
							{
								if (dataLen > 0)
								{
									raw.Data = (uint8_t*)Malloc(dataLen);
									if (raw.Data == nullptr)
									{
										goto sqr;
									}
									Memcpy(raw.Data, data, dataLen);
								}
								EmbedXrpc_SendQueueResult = EmbedXrpc_SendQueue(DelegateBlockQueue, &raw, sizeof(ReceiveItemInfo));
							}
							else
							{
								EmbedXrpc_SendQueueResult = QueueState_Full;
							}
#endif
						}
						goto sqr;
					}
				}
			}
		}
		else if (rt == ReceiveType_Request)//server
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Server ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			EmbedXrpc_SendQueueResult=RequestBlockBufferProvider->Send(&raw, data, dataLen) == true ? QueueState_OK : QueueState_Full;
#else 		
			if(EmbedXrpc_QueueSpacesAvailable(RequestBlockQueue)>0)
			{
				if (dataLen > 0)
				{
					raw.Data = (uint8_t*)Malloc(dataLen);
					if (raw.Data == nullptr)
					{
						goto sqr;
					}
					Memcpy(raw.Data, data, dataLen);
				}
				EmbedXrpc_SendQueueResult = EmbedXrpc_SendQueue(RequestBlockQueue, &raw, sizeof(ReceiveItemInfo));
			}
			else
			{
				EmbedXrpc_SendQueueResult = QueueState_Full;
			}
#endif
		}
		sqr:
		return EmbedXrpc_SendQueueResult== QueueState_OK?true:false;
	}
	static void SuspendTimerCallBack(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		//obj->EmbedXrpc_TakeMutex(obj->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);//不需要加锁 这里不使用obj->buffer全局buffer
		uint8_t sb[4];
		sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
		sb[1] = (uint8_t)(EmbedXrpcSuspendSid >> 8 & 0xff);
		sb[2] = (uint8_t)(obj->TimeOut & 0xff);
		sb[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff)&0x3FFF);
		sb[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
		obj->Send(obj, 4, sb);
		//obj->EmbedXrpc_ReleaseMutex(obj->ObjectMutexHandle);
	}
#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0
	static void DelegateServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		ReceiveItemInfo recData;
		uint32_t i = 0;
		bool isContain = false;
		for (;;)
		{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			if (obj->DelegateBlockBufferProvider->Receive(&recData, 1)==true)
#else
			if (EmbedXrpc_ReceiveQueue(obj->DelegateBlockQueue,&recData,sizeof(ReceiveItemInfo),1) == QueueState_OK)
#endif
			{
				isContain = false;
				for (uint32_t collectionIndex = 0; collectionIndex < obj->ResponseDelegateMessageMapsCount; collectionIndex++)
				{
					auto MessageMaps = obj->ResponseDelegateMessageMaps[collectionIndex].Map;
					for (i = 0; i < obj->ResponseDelegateMessageMaps[collectionIndex].Count; i++)
					{
						if ((MessageMaps[i].ReceiveType == ReceiveType_Delegate) && (MessageMaps[i].Delegate->GetSid() == recData.Sid))
						{
							SerializationManager rsm;
							rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
							rsm.Reset();
							rsm.BufferLen = recData.DataLen;
#if EmbedXrpc_UseRingBufferWhenReceiving==1
							rsm.BlockBufferProvider=obj->DelegateBlockBufferProvider;
#else
							rsm.Buf = recData.Data;
#endif
#if EmbedXrpc_CheckSumValid==1
							rsm.SetCalculateSum(0);
							rsm.SetReferenceSum(recData.CheckSum);
#endif
							MessageMaps[i].Delegate->Invoke(rsm);
							isContain = true;
							goto _break;
						}
					}
				}
			_break:
#if EmbedXrpc_UseRingBufferWhenReceiving==1
				if (isContain == false)
				{
					obj->DelegateBlockBufferProvider->PopChars(nullptr, recData.DataLen);
				}
#else
				if(recData.DataLen>0)
					Free(recData.Data);
#endif
				//EmbedSerializationShowMessage("EmbedXrpcObject","Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
			}
			if (obj->DeInitFlag == true)
			{
				return;
			}
		}
	}
#endif

#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0
	static void ResponseServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		ReceiveItemInfo recData;
		uint32_t i = 0;
		bool isContain = false;
		for (;;)
		{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			if (obj->RequestBlockBufferProvider->Receive(&recData, 1) == true)
#else
			if (EmbedXrpc_ReceiveQueue(obj->RequestBlockQueue, &recData,sizeof(ReceiveItemInfo), 1) == QueueState_OK)
#endif
			{
				isContain = false;
				for (uint32_t collectionIndex = 0; collectionIndex < obj->RequestMessageMapsCount; collectionIndex++)
				{
					auto MessageMaps = obj->RequestMessageMaps[collectionIndex].Map;
					for (i = 0; i < obj->RequestMessageMaps[collectionIndex].Count; i++)
					{
						if (MessageMaps[i].Service->GetSid() == recData.Sid)
						{

							SerializationManager rsm;
							SerializationManager sendsm;
							rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
							rsm.Reset();
							rsm.BufferLen = recData.DataLen;
#if EmbedXrpc_UseRingBufferWhenReceiving==1
							rsm.BlockBufferProvider = obj->RequestBlockBufferProvider;
#else
							rsm.Buf = recData.Data;
#endif
#if EmbedXrpc_CheckSumValid==1
							rsm.SetCalculateSum(0);
							rsm.SetReferenceSum(recData.CheckSum);
#endif
							EmbedXrpc_TakeMutex(obj->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);//由于使用obj->Buffer这个全局变量，所以添加锁
							sendsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
							sendsm.Reset();
							sendsm.Buf = &obj->DataLinkLayoutBuffer[4];
							sendsm.BufferLen = EmbedXrpc_SendBufferSize - 4;

							EmbedXrpc_TimerReset(obj->SuspendTimer);
							EmbedXrpc_TimerStart(obj->SuspendTimer, recData.TargetTimeout / 2);
							MessageMaps[i].Service->Invoke(rsm, sendsm);
							EmbedXrpc_TimerStop(obj->SuspendTimer);

							if (sendsm.Index > 0)//
							{
								obj->DataLinkLayoutBuffer[0] = (uint8_t)(recData.Sid >> 0 & 0xff);
								obj->DataLinkLayoutBuffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
								obj->DataLinkLayoutBuffer[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
								obj->DataLinkLayoutBuffer[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
								obj->DataLinkLayoutBuffer[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
								obj->Send(obj, sendsm.Index + 4, obj->DataLinkLayoutBuffer);
							}
							EmbedXrpc_ReleaseMutex(obj->ObjectMutexHandle);

							isContain = true;
							goto _break;
						}
					}
				}
			_break:
#if EmbedXrpc_UseRingBufferWhenReceiving==1
				if (isContain == false)
				{
					obj->RequestBlockBufferProvider->PopChars(nullptr, recData.DataLen);
				}
#else
				if (recData.DataLen > 0)
					Free(recData.Data);
#endif
			}

			if (obj->DeInitFlag == true)
			{
				return;
			}
		}
	}
#endif

#if EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
	RequestResponseState Wait(uint32_t sid, ReceiveItemInfo *recData)
	{
		RequestResponseState ret= ResponseState_Ok;
		//ReceiveItemInfo recData;
		while (true)
		{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			if (ResponseBlockBufferProvider->Receive(recData, TimeOut) == false)
#else
			if (EmbedXrpc_ReceiveQueue(ResponseBlockQueue, recData,sizeof(ReceiveItemInfo), TimeOut)!= QueueState_OK)
#endif
			{
				return ResponseState_Timeout;
			}
			if (recData->Sid == EmbedXrpcSuspendSid)
			{
				EmbedSerializationShowMessage("EmbedXrpcObject", "Client:recData.Sid == EmbedXrpcSuspendSid\n");
				continue;
			}
			if (sid != recData->Sid)
			{
				ret = ResponseState_SidError;
			}
			else
			{
				EmbedSerializationShowMessage("EmbedXrpcObject", "sid == recData.Sid\n");
				ret = ResponseState_Ok;
			}
			break;
		}
#if 0
		if (ret == ResponseState_Ok)
		{
			SerializationManager rsm;
			rsm.IsEnableMataDataEncode = IsEnableMataDataEncode;
			rsm.Reset();
			rsm.BufferLen = recData.DataLen;
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			rsm.BlockBufferProvider = ResponseBlockBufferProvider;
			rsm.BlockBufferProvider->SetCalculateSum(0);
			rsm.BlockBufferProvider->SetReferenceSum(recData.CheckSum);
			rsm.Deserialize(type, response, 0);
			EmbedSerializationAssert(rsm.BlockBufferProvider->GetReferenceSum() == rsm.BlockBufferProvider->GetCalculateSum());
#else
			rsm.Buf = recData.Data;
			rsm.CalculateSum = 0;
			rsm.ReferenceSum = recData.CheckSum;
			rsm.Deserialize(type, response, 0);
			if(recData.DataLen>0)
				Free(recData.Data);
#endif
			
		}
#endif
		return ret;
	}
#endif	
};

#endif
