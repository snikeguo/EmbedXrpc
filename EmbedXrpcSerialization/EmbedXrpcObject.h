#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedLibrary.h"
#include "EmbedXrpcCommon.h"
#if EmbedXrpc_UseRingBufferWhenReceiving==1
#include "BlockBufferProvider.h"
#endif
#define EmbedXrpcObjectVersion	"2.0.1"
class EmbedXrpcObject
{
public:

	uint8_t DataLinkLayoutBuffer[EmbedXrpc_SendBufferSize];

	uint32_t TimeOut;
	SendPack_t Send;
	El_Mutex_t ObjectMutexHandle;

#if EmbedXrpc_UseRingBufferWhenReceiving==1
	uint8_t DelegateRingBuffer[EmbedXrpc_DelegateRingBufferSize];
	BlockRingBufferProvider* DelegateBlockBufferProvider = nullptr;

	uint8_t ResponseRingBuffer[EmbedXrpc_ResponseRingBufferSize];
	BlockRingBufferProvider* ResponseBlockBufferProvider = nullptr;
#else
	El_Queue_t DelegateBlockQueue = nullptr;
	El_Queue_t ResponseBlockQueue = nullptr;
#endif


	El_Thread_t DelegateServiceThreadHandle;

	uint32_t DelegatesCount;
	DelegateDescribe* Delegates;

	uint32_t ResponsesCount;
	ResponseDescribe* Responses;

	void* UserData;

	volatile bool DeInitFlag;
	volatile bool DelegateServiceThreadExitState;
	volatile bool RequestProcessServiceThreadExitState;
	//bool IsEnableMataDataEncode;

	//server:
	El_Thread_t RequestProcessServiceThreadHandle; 
	//El_Mutex_t ObjectMutexHandle;
#if EmbedXrpc_UseRingBufferWhenReceiving==1
	uint8_t RequestRingBuffer[EmbedXrpc_RequestRingBufferSize];
	BlockRingBufferProvider* RequestBlockBufferProvider;
#else
	El_Queue_t	RequestBlockQueue;
#endif
	El_Timer_t SuspendTimer;
	UserDataOfTransportLayer_t UserDataOfTransportLayerOfSuspendTimerUsed;

	uint32_t RequestsCount;
	RequestDescribe* Requests;

	EmbedXrpcObject(
		SendPack_t send,
		uint32_t timeOut,

		ResponseDescribe* responses,
		uint32_t responsesCount,

		DelegateDescribe* delegates,//代理的services
		uint32_t delegatesCount,

		RequestDescribe* requests,//server 请求的services
		uint32_t requestsCount,//server

		//bool isEnableMataDataEncode,
		void* ud = nullptr) :
		TimeOut(timeOut),
		Send(send),
		ObjectMutexHandle(nullptr),
		DelegateServiceThreadHandle(nullptr),

		DelegatesCount(delegatesCount),
		Delegates(delegates),

		ResponsesCount(responsesCount),
		Responses(responses),

		UserData(ud),
		DeInitFlag(false),
		//IsEnableMataDataEncode(isEnableMataDataEncode),

		RequestProcessServiceThreadHandle(nullptr),
		SuspendTimer(nullptr),

		RequestsCount(requestsCount),
		Requests(requests)
	{

	}
	//client节点构造函数
	EmbedXrpcObject(SendPack_t send,
		uint32_t timeOut,

		ResponseDescribe* responses,
		uint32_t responsesCount,

		DelegateDescribe* delegates,
		uint32_t delegatesCount,

		//bool isEnableMataDataEncode,
		void* ud = nullptr) :EmbedXrpcObject(send,
			timeOut,

			responses,
			responsesCount,

			delegates,
			delegatesCount,

			nullptr,
			0,

			//isEnableMataDataEncode,
			ud
		)
	{

	}
	//server节点的构造函数
	EmbedXrpcObject(SendPack_t send,
		uint32_t timeOut,

		RequestDescribe* requests,//server 请求的services
		uint32_t requestsCount,//server

		//bool isEnableMataDataEncode,
		void* ud = nullptr) :EmbedXrpcObject(send,
			timeOut,

			nullptr,
			0,

			nullptr,
			0,

			requests,
			requestsCount,

			//isEnableMataDataEncode,
			ud
		)
	{

	}

	void Init()
	{
		DelegateServiceThreadExitState = false;
		RequestProcessServiceThreadExitState = false;
		DeInitFlag = false;

		ObjectMutexHandle = El_CreateMutex("ObjectMutex");




#if EmbedXrpc_UseRingBufferWhenReceiving==1
		DelegateBlockBufferProvider = new BlockRingBufferProvider();
		BlockRingBufferProvider_Init(DelegateBlockBufferProvider,DelegateRingBuffer, EmbedXrpc_DelegateRingBufferSize, EmbedXrpc_DelegateBlockQueue_MaxItemNumber);

		ResponseBlockBufferProvider = new BlockRingBufferProvider();
		BlockRingBufferProvider_Init(ResponseBlockBufferProvider,ResponseRingBuffer, EmbedXrpc_ResponseRingBufferSize, EmbedXrpc_ResponseBlockQueue_MaxItemNumber);

		RequestBlockBufferProvider = new BlockRingBufferProvider();
		BlockRingBufferProvider_Init(RequestBlockBufferProvider,RequestRingBuffer, EmbedXrpc_RequestRingBufferSize, EmbedXrpc_RequestBlockQueue_MaxItemNumber);
#else
		DelegateBlockQueue = El_CreateQueue("DelegateBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_DelegateBlockQueue_MaxItemNumber);
		ResponseBlockQueue = El_CreateQueue("ResponseBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_ResponseBlockQueue_MaxItemNumber);
		RequestBlockQueue = El_CreateQueue("RequestBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_RequestBlockQueue_MaxItemNumber);
#endif

#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0&&EmbedXrpc_IsSendToQueue==1 //client
		DelegateServiceThreadHandle = El_CreateThread("DelegateServiceThread", Client_ThreadPriority, DelegateServiceThread, this);
		El_ThreadStart(DelegateServiceThreadHandle);
#endif

		//server
#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0&&EmbedXrpc_IsSendToQueue==1
		RequestProcessServiceThreadHandle = El_CreateThread("RequestProcessServiceThread", Server_ThreadPriority, RequestProcessServiceThread, this);
		El_ThreadStart(RequestProcessServiceThreadHandle);
#endif
		SuspendTimer = El_CreateTimer("SuspendTimer", El_WAIT_FOREVER, SuspendTimerCallBack, this);
	}
	void DeInit()
	{
		DeInitFlag = true;
		El_DeleteMutex(ObjectMutexHandle);
		
#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0&&EmbedXrpc_IsSendToQueue==1
		El_DeleteThread(DelegateServiceThreadHandle);
#else
		DelegateServiceThreadExitState = true;
#endif

#if EmbedXrpc_UseRingBufferWhenReceiving==1
		delete DelegateBlockBufferProvider;
		delete ResponseBlockBufferProvider;
#else
		El_DeleteQueue(DelegateBlockQueue);
		El_DeleteQueue(ResponseBlockQueue);
#endif

#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0 && EmbedXrpc_IsSendToQueue==1
		El_DeleteThread(RequestProcessServiceThreadHandle);
#else
		RequestProcessServiceThreadExitState = true;
#endif
#if EmbedXrpc_UseRingBufferWhenReceiving==1	
		delete RequestBlockBufferProvider;
#else
		El_DeleteQueue(RequestBlockQueue);//server接受到的request队列数据
#endif
		El_DeleteTimer(SuspendTimer);
		
	}
	static void DelegateServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData)
	{
		bool isContain;
		isContain = false;
		(void)isContain;
		for (uint32_t collectionIndex = 0; collectionIndex < obj->DelegatesCount; collectionIndex++)
		{
			isContain = true;
			auto iter = &obj->Delegates[collectionIndex];
			if (iter->Delegate->GetSid() == recData.Sid)
			{
				SerializationManager rsm;
				El_Memset(&rsm, 0, sizeof(SerializationManager));
				//rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
				SerializationManager_Reset(&rsm);
				rsm.BufferLen = recData.DataLen;
#if EmbedXrpc_UseRingBufferWhenReceiving==1
				rsm.BlockBufferProvider = obj->DelegateBlockBufferProvider;
#else
				rsm.Buf = recData.Data;
#endif
#if EmbedXrpc_CheckSumValid==1
				SerializationManager_SetCalculateSum(&rsm,0);
				SerializationManager_SetReferenceSum(&rsm, recData.CheckSum);
#endif
				iter->Delegate->Invoke(&recData.UserDataOfTransportLayer,&rsm);
				isContain = true;
				goto _break;
			}
		}
	_break:
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		if (isContain == false)
		{
			BlockRingBufferProvider_PopChars(obj->DelegateBlockBufferProvider,nullptr, recData.DataLen);
		}
#else
		if ((recData.DataLen > 0) && (isFreeData == true))
			El_Free(recData.Data);
#endif
		//EmbedSerializationShowMessage("EmbedXrpcObject","Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);

	}
	static void ResponseServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData)
	{
		bool isContain;
		isContain = false;
		(void)isContain;
		UserDataOfTransportLayer_t response_UserDataOfTransportLayer;
		for (uint32_t collectionIndex = 0; collectionIndex < obj->RequestsCount; collectionIndex++)
		{
			isContain = true;
			auto iter = &obj->Requests[collectionIndex];
			if (iter->Service->GetSid() == recData.Sid)
			{

				SerializationManager rsm;
				SerializationManager sendsm;
				El_Memset(&rsm, 0, sizeof(SerializationManager));
				El_Memset(&sendsm, 0, sizeof(SerializationManager));
				//rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
				SerializationManager_Reset(&rsm);
				rsm.BufferLen = recData.DataLen;
#if EmbedXrpc_UseRingBufferWhenReceiving==1
				rsm.BlockBufferProvider = obj->RequestBlockBufferProvider;
#else
				rsm.Buf = recData.Data;
#endif
#if EmbedXrpc_CheckSumValid==1
				SerializationManager_SetCalculateSum(&rsm,0);
				SerializationManager_SetReferenceSum(&rsm,recData.CheckSum);
#endif
				El_TakeMutex(obj->ObjectMutexHandle, El_WAIT_FOREVER);//由于使用obj->Buffer这个全局变量，所以添加锁
				//sendsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
				SerializationManager_Reset(&sendsm);
				sendsm.Buf = &obj->DataLinkLayoutBuffer[4];
				sendsm.BufferLen = EmbedXrpc_SendBufferSize - 4;

				//EmbedXrpc_TimerReset(obj->SuspendTimer);
				//EmbedXrpc_TimerStart(obj->SuspendTimer, recData.TargetTimeout / 2);
				iter->Service->Invoke(&recData.UserDataOfTransportLayer, &response_UserDataOfTransportLayer, obj, recData.TargetTimeout, &rsm, &sendsm);
				El_TimerStop(obj->SuspendTimer);

				if (sendsm.Index > 0)//
				{
					obj->DataLinkLayoutBuffer[0] = (uint8_t)(recData.Sid >> 0 & 0xff);
					obj->DataLinkLayoutBuffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
					obj->DataLinkLayoutBuffer[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
					obj->DataLinkLayoutBuffer[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
					obj->DataLinkLayoutBuffer[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
					obj->Send(&response_UserDataOfTransportLayer,obj, sendsm.Index + 4, obj->DataLinkLayoutBuffer);
				}
				El_ReleaseMutex(obj->ObjectMutexHandle);

				isContain = true;
				goto _break;
			}
		}
	_break:
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		if (isContain == false)
		{
			BlockRingBufferProvider_PopChars(obj->RequestBlockBufferProvider,nullptr, recData.DataLen);
		}
#else
		if (recData.DataLen > 0 && isFreeData == true)
			El_Free(recData.Data);
#endif
	}

#if EmbedXrpc_IsSendToQueue==1
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData,UserDataOfTransportLayer_t userDataOfTransportLayer)
	{
		if (allDataLen < 4)
			return false;
		QueueState El_SendQueueResult = QueueState_Full;

		ReceiveItemInfo raw;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout = (uint16_t)(allData[2] | ((allData[3] & 0x3f) << 8));
		uint32_t dataLen = allDataLen - 4;
		uint8_t* data = &allData[4];
		ReceiveType_t rt = (ReceiveType_t)(allData[3] >> 6);
		raw.UserDataOfTransportLayer = userDataOfTransportLayer;
		if (rt == ReceiveType_Response)
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Client ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			if (serviceId == EmbedXrpcSuspendSid)//挂起的SID要特殊处理
			{
				raw.Sid = serviceId;
				raw.DataLen = dataLen;
				raw.TargetTimeout = targettimeout;
				raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
				El_SendQueueResult = BlockRingBufferProvider_Send(ResponseBlockBufferProvider ,&raw, nullptr, 0) == True ? QueueState_OK : QueueState_Full;
#else
				raw.Data = nullptr;
				if (El_QueueSpacesAvailable(ResponseBlockQueue) > 0)
				{
					El_SendQueueResult = El_SendQueue(ResponseBlockQueue, &raw, sizeof(ReceiveItemInfo));
				}
				else
				{
					El_SendQueueResult = QueueState_Full;
				}
#endif
				goto sqr;
			}
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			El_SendQueueResult = BlockRingBufferProvider_Send(ResponseBlockBufferProvider,&raw, data, dataLen) == True ? QueueState_OK : QueueState_Full;
#else
			if (El_QueueSpacesAvailable(ResponseBlockQueue) > 0)
			{
				if (dataLen > 0)
				{
					raw.Data = (uint8_t*)El_Malloc(dataLen);
					if (raw.Data == nullptr)
					{
						goto sqr;
					}
					El_Memcpy(raw.Data, data, dataLen);
				}
				El_SendQueueResult = El_SendQueue(ResponseBlockQueue, &raw, sizeof(ReceiveItemInfo));
			}
			else
			{
				El_SendQueueResult = QueueState_Full;
			}
#endif			
		}
		else if (rt == ReceiveType_Delegate)
		{
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			El_SendQueueResult = BlockRingBufferProvider_Send(DelegateBlockBufferProvider ,&raw, data, dataLen) == True ? QueueState_OK : QueueState_Full;
#else
			if (El_QueueSpacesAvailable(DelegateBlockQueue) > 0)
			{
				if (dataLen > 0)
				{
					raw.Data = (uint8_t*)El_Malloc(dataLen);
					if (raw.Data == nullptr)
					{
						goto sqr;
					}
					El_Memcpy(raw.Data, data, dataLen);
				}
				El_SendQueueResult = El_SendQueue(DelegateBlockQueue, &raw, sizeof(ReceiveItemInfo));
			}
			else
			{
				El_SendQueueResult = QueueState_Full;
			}
#endif			
		}
		else if (rt == ReceiveType_Request)//server
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Server ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			El_SendQueueResult = BlockRingBufferProvider_Send(RequestBlockBufferProvider ,&raw, data, dataLen) == True ? QueueState_OK : QueueState_Full;
#else 		
			if (El_QueueSpacesAvailable(RequestBlockQueue) > 0)
			{
				if (dataLen > 0)
				{
					raw.Data = (uint8_t*)El_Malloc(dataLen);
					if (raw.Data == nullptr)
					{
						goto sqr;
					}
					El_Memcpy(raw.Data, data, dataLen);
				}
				El_SendQueueResult = El_SendQueue(RequestBlockQueue, &raw, sizeof(ReceiveItemInfo));
			}
			else
			{
				El_SendQueueResult = QueueState_Full;
			}
#endif
		}
	sqr:
		return El_SendQueueResult == QueueState_OK ? true : false;
	}
#else
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData, UserDataOfTransportLayer_t userDataOfTransportLayer)
	{
		if (allDataLen < 4)
			return false;
		QueueState El_SendQueueResult = QueueState_Full;

		ReceiveItemInfo raw;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout = (uint16_t)(allData[2] | ((allData[3] & 0x3f) << 8));
		uint32_t dataLen = allDataLen - 4;
		uint8_t* data = &allData[4];
		ReceiveType_t rt = (ReceiveType_t)(allData[3] >> 6);
		raw.UserDataOfTransportLayer = userDataOfTransportLayer;
		if (rt == ReceiveType_Response)
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Client ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			if (serviceId == EmbedXrpcSuspendSid)//挂起的SID要特殊处理
			{
				raw.Sid = serviceId;
				raw.DataLen = dataLen;
				raw.TargetTimeout = targettimeout;
				raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
				El_SendQueueResult = ResponseBlockBufferProvider->Send(&raw, nullptr, 0) == true ? QueueState_OK : QueueState_Full;
#else
				raw.Data = nullptr;
				if (El_QueueSpacesAvailable(ResponseBlockQueue) > 0)
				{
					El_SendQueueResult = El_SendQueue(ResponseBlockQueue, &raw, sizeof(ReceiveItemInfo));
				}
				else
				{
					El_SendQueueResult = QueueState_Full;
				}
#endif
				goto sqr;
			}
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			El_SendQueueResult = ResponseBlockBufferProvider->Send(&raw, data, dataLen) == true ? QueueState_OK : QueueState_Full;
#else
			if (El_QueueSpacesAvailable(ResponseBlockQueue) > 0)
			{
				if (dataLen > 0)
				{
					raw.Data = (uint8_t*)El_Malloc(dataLen);
					if (raw.Data == nullptr)
					{
						goto sqr;
					}
					El_Memcpy(raw.Data, data, dataLen);
				}
				El_SendQueueResult = El_SendQueue(ResponseBlockQueue, &raw, sizeof(ReceiveItemInfo));
			}
			else
			{
				El_SendQueueResult = QueueState_Full;
			}
#endif			
		}
		else if (rt == ReceiveType_Delegate)
		{
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			El_SendQueueResult = DelegateBlockBufferProvider->Send(&raw, data, dataLen) == true ? QueueState_OK : QueueState_Full;
#else
			raw.Data = data;
			DelegateServiceExecute(this, raw, false);
			El_SendQueueResult = QueueState_OK;
#endif			
		}
		else if (rt == ReceiveType_Request)//server
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Server ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			raw.CheckSum = GetSum(data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			El_SendQueueResult = RequestBlockBufferProvider->Send(&raw, data, dataLen) == true ? QueueState_OK : QueueState_Full;
#else 		
			raw.Data = data;
			ResponseServiceExecute(this, raw, false);
			El_SendQueueResult = QueueState_OK;
#endif
		}
	sqr:
		return El_SendQueueResult == QueueState_OK ? true : false;
	}
#endif

	static void SuspendTimerCallBack(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		//obj->El_TakeMutex(obj->ObjectMutexHandle, El_WAIT_FOREVER);//不需要加锁 这里不使用obj->buffer全局buffer
		uint8_t sb[4];
		sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
		sb[1] = (uint8_t)(EmbedXrpcSuspendSid >> 8 & 0xff);
		sb[2] = (uint8_t)(obj->TimeOut & 0xff);
		sb[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
		sb[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
		obj->Send(&obj->UserDataOfTransportLayerOfSuspendTimerUsed,obj, 4, sb);
		//obj->El_ReleaseMutex(obj->ObjectMutexHandle);
	}
	

#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0 &&EmbedXrpc_IsSendToQueue==1
	static void DelegateServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		ReceiveItemInfo recData;
		for (;;)
		{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			if (BlockRingBufferProvider_Receive(obj->DelegateBlockBufferProvider ,&recData, 1) == True)
#else
			if (El_ReceiveQueue(obj->DelegateBlockQueue, &recData, sizeof(ReceiveItemInfo), 1) == QueueState_OK)
#endif
			{
				DelegateServiceExecute(obj, recData,true);
			}
			if (obj->DeInitFlag == true)
			{
				obj->DelegateServiceThreadExitState = true;
				return;
			}
		}
	}
#endif


	

#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0 && EmbedXrpc_IsSendToQueue==1
	static void RequestProcessServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		ReceiveItemInfo recData;
		for (;;)
		{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			if (BlockRingBufferProvider_Receive(obj->RequestBlockBufferProvider ,&recData, 1) == True)
#else
			if (El_ReceiveQueue(obj->RequestBlockQueue, &recData, sizeof(ReceiveItemInfo), 1) == QueueState_OK)
#endif
			{
				ResponseServiceExecute(obj, recData,true);
			}

			if (obj->DeInitFlag == true)
			{
				obj->RequestProcessServiceThreadExitState = true;
				return;
			}
		}
	}
#endif


#if EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
	RequestResponseState Wait(uint32_t sid, ReceiveItemInfo* recData)
	{
		RequestResponseState ret = ResponseState_Ok;
		//ReceiveItemInfo recData;
		while (true)
		{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			if (BlockRingBufferProvider_Receive(ResponseBlockBufferProvider,recData, TimeOut) == false)
#else
			if (El_ReceiveQueue(ResponseBlockQueue, recData, sizeof(ReceiveItemInfo), TimeOut) != QueueState_OK)
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
		return ret;
	}
#endif	
};

#endif
