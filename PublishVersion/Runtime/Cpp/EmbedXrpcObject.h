#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedLibrary.h"
#include "EmbedXrpcCommon.h"
#define EmbedXrpcObjectVersion	"2.1.0"



template<class DTL>
class EmbedXrpcObject
{
public:

	uint8_t *DataLinkLayoutBuffer;
	uint32_t DataLinkLayoutBufferLen;

	uint32_t TimeOut;
	SendAction<DTL> Sender;
	El_Mutex_t ObjectMutexHandle;

//#if EmbedXrpc_UseRingBufferWhenReceiving==1
	BlockRingBufferProvider* DelegateBlockBufferProvider = nullptr;
	BlockRingBufferProvider* ResponseBlockBufferProvider = nullptr;
//#else
	El_Queue_t DelegateBlockQueue = nullptr;
	El_Queue_t ResponseBlockQueue = nullptr;
//#endif


	El_Thread_t DelegateServiceThreadHandle;

	uint32_t DelegatesCount;
	DelegateDescribe<DTL>* Delegates;

	uint32_t ResponsesCount;
	ResponseDescribe* Responses;

	void* UserData;

	volatile bool DeInitFlag;
	volatile bool DelegateServiceThreadExitState;
	volatile bool RequestProcessServiceThreadExitState;
	EmbedXrpcConfig* RpcConfig;

	//server:
	El_Thread_t RequestProcessServiceThreadHandle; 
	//El_Mutex_t ObjectMutexHandle;
//#if EmbedXrpc_UseRingBufferWhenReceiving==1
	BlockRingBufferProvider* RequestBlockBufferProvider;
//#else
	El_Queue_t	RequestBlockQueue;
//#endif
	El_Timer_t SuspendTimer;
	DTL UserDataOfTransportLayerOfSuspendTimerUsed;

	uint32_t RequestsCount;
	RequestDescribe<DTL>* Requests;

	EmbedXrpcObject(
		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,
		SendAction<DTL> send,
		uint32_t timeOut,

		ResponseDescribe* responses,
		uint32_t responsesCount,

		DelegateDescribe<DTL>* delegates,//代理的services
		uint32_t delegatesCount,

		RequestDescribe<DTL>* requests,//server 请求的services
		uint32_t requestsCount,//server

		EmbedXrpcConfig*rpcConfig,
		void* ud = nullptr) :

		DataLinkLayoutBuffer(dataLinkLayoutBuffer),
		DataLinkLayoutBufferLen(dataLinkLayoutBufferLen),

		TimeOut(timeOut),
		Sender(send),
		ObjectMutexHandle(nullptr),
		DelegateServiceThreadHandle(nullptr),

		DelegatesCount(delegatesCount),
		Delegates(delegates),

		ResponsesCount(responsesCount),
		Responses(responses),

		UserData(ud),
		DeInitFlag(false),

		DelegateBlockBufferProvider(rpcConfig->DelegateBlockBufferProvider),
		ResponseBlockBufferProvider(rpcConfig->ResponseBlockBufferProvider),
		RequestBlockBufferProvider(rpcConfig->RequestBlockBufferProvider),

		RequestProcessServiceThreadHandle(nullptr),
		SuspendTimer(nullptr),

		RequestsCount(requestsCount),
		Requests(requests),
		RpcConfig(rpcConfig)
	{

	}
	//client节点构造函数
	EmbedXrpcObject(
		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,
		SendAction<DTL> send,
		uint32_t timeOut,

		ResponseDescribe* responses,
		uint32_t responsesCount,

		DelegateDescribe<DTL>* delegates,
		uint32_t delegatesCount,

		EmbedXrpcConfig*rpcConfig,
		void* ud = nullptr) :EmbedXrpcObject(dataLinkLayoutBuffer, 
			dataLinkLayoutBufferLen,

			send,
			timeOut,

			responses,
			responsesCount,

			delegates,
			delegatesCount,

			nullptr,
			0,

			rpcConfig,
			ud
		)
	{

	}
	//server节点的构造函数
	EmbedXrpcObject(
		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,
		SendAction<DTL> send,
		uint32_t timeOut,

		RequestDescribe<DTL>* requests,//server 请求的services
		uint32_t requestsCount,//server

		EmbedXrpcConfig*rpcConfig,
		void* ud = nullptr) :EmbedXrpcObject(dataLinkLayoutBuffer, 
			dataLinkLayoutBufferLen,

			send,
			timeOut,

			nullptr,
			0,

			nullptr,
			0,

			requests,
			requestsCount,

			rpcConfig,
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

//#if EmbedXrpc_UseRingBufferWhenReceiving==1
		//DelegateBlockBufferProvider = new BlockRingBufferProvider();
		//BlockRingBufferProvider_Init(DelegateBlockBufferProvider, DelegateRingBuffer, EmbedXrpc_DelegateRingBufferSize, EmbedXrpc_DelegateBlockQueue_MaxItemNumber);

		//ResponseBlockBufferProvider = new BlockRingBufferProvider();
		//BlockRingBufferProvider_Init(ResponseBlockBufferProvider, ResponseRingBuffer, EmbedXrpc_ResponseRingBufferSize, EmbedXrpc_ResponseBlockQueue_MaxItemNumber);

		//RequestBlockBufferProvider = new BlockRingBufferProvider();
		//BlockRingBufferProvider_Init(RequestBlockBufferProvider, RequestRingBuffer, EmbedXrpc_RequestRingBufferSize, EmbedXrpc_RequestBlockQueue_MaxItemNumber);
//#else
		if (RpcConfig->UseRingBufferWhenReceiving == false)
		{
			DelegateBlockQueue = El_CreateQueue("DelegateBlockQueue", sizeof(ReceiveItemInfo<DTL>), RpcConfig->DelegateBlockQueue_MaxItemNumber);
			ResponseBlockQueue = El_CreateQueue("ResponseBlockQueue", sizeof(ReceiveItemInfo<DTL>), RpcConfig->ResponseBlockQueue_MaxItemNumber);
			RequestBlockQueue = El_CreateQueue("RequestBlockQueue", sizeof(ReceiveItemInfo<DTL>), RpcConfig->RequestBlockQueue_MaxItemNumber);
		}
		
//#endif

//#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0&&EmbedXrpc_IsSendToQueue==1 //client
		if ((RpcConfig->DelegateBlockQueue_MaxItemNumber > 0) && (RpcConfig->IsSendToQueue == true))
		{
			DelegateServiceThreadHandle = El_CreateThread("DelegateServiceThread", RpcConfig->ClientThreadPriority, DelegateServiceThread, this, 2048);
			El_ThreadStart(DelegateServiceThreadHandle);
		}
		
//#endif

		//server
//#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0&&EmbedXrpc_IsSendToQueue==1
		if ((RpcConfig->RequestBlockQueue_MaxItemNumber > 0) && (RpcConfig->IsSendToQueue == true))
		{
			RequestProcessServiceThreadHandle = El_CreateThread("RequestProcessServiceThread", RpcConfig->ServerThreadPriority, RequestProcessServiceThread, this, 2048);
			El_ThreadStart(RequestProcessServiceThreadHandle);
		}
		
//#endif
		SuspendTimer = El_CreateTimer("SuspendTimer", El_WAIT_FOREVER, SuspendTimerCallBack, this);
	}
	void DeInit()
	{
		DeInitFlag = true;
		El_DeleteMutex(ObjectMutexHandle);

		if (RpcConfig->DelegateBlockQueue_MaxItemNumber > 0 && RpcConfig->IsSendToQueue == true)
		{
			El_DeleteThread(DelegateServiceThreadHandle);
		}
		else
		{
			DelegateServiceThreadExitState = true;
		}

		if (RpcConfig->UseRingBufferWhenReceiving == 1)
		{
			delete DelegateBlockBufferProvider;
			delete ResponseBlockBufferProvider;
		}
		else
		{
			El_DeleteQueue(DelegateBlockQueue);
			El_DeleteQueue(ResponseBlockQueue);
		}

		if ((RpcConfig->RequestBlockQueue_MaxItemNumber > 0) && RpcConfig->IsSendToQueue == true)
		{
			El_DeleteThread(RequestProcessServiceThreadHandle);
		}
		else
		{
			RequestProcessServiceThreadExitState = true;
		}

		if (RpcConfig->UseRingBufferWhenReceiving == true)
		{
			delete RequestBlockBufferProvider;
		}
		else
		{
			El_DeleteQueue(RequestBlockQueue);//server接受到的request队列数据
		}

		El_DeleteTimer(SuspendTimer);

	}
	static void DelegateServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo<DTL>& recData, bool isFreeData)
	{
		bool isContain;
		isContain = false;
		(void)isContain;
		for (uint32_t collectionIndex = 0; collectionIndex < obj->DelegatesCount; collectionIndex++)
		{
			if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
			{
				isContain = true;
			}
			

			auto iter = &obj->Delegates[collectionIndex];
			if (iter->Delegate->GetSid() == recData.Sid)
			{
				SerializationManager rsm;
				El_Memset(&rsm, 0, sizeof(SerializationManager));
				//rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
				rsm.Reset();
				rsm.BufferLen = recData.DataLen;
				if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
				{
					rsm.BlockBufferProvider = obj->DelegateBlockBufferProvider;
				}
				else
				{
					rsm.Buf = recData.Data;
				}

				if (obj->RpcConfig->CheckSumValid == true)
				{
					rsm.SetCalculateSum( 0);
					rsm.SetReferenceSum(recData.CheckSum);
				}

				iter->Delegate->Invoke(obj->RpcConfig,&recData.UserDataOfTransportLayer, &rsm);

				if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
				{
					isContain = true;
				}

				goto _break;
			}
		}
	_break:
		if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
		{
			if (isContain == false)
			{
				obj->DelegateBlockBufferProvider->PopChars(nullptr, recData.DataLen);
			}
		}
		else
		{
			if ((recData.DataLen > 0) && (isFreeData == true))
				El_Free(recData.Data);
		}
		//EmbedSerializationShowMessage("EmbedXrpcObject","Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);

	}
	static void ResponseServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo<DTL>& recData, bool isFreeData)
	{

		bool isContain;
		isContain = false;
		(void)isContain;

		ServiceInvokeParameter<DTL> serviceInvokeParameter;
		for (uint32_t collectionIndex = 0; collectionIndex < obj->RequestsCount; collectionIndex++)
		{
			if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
			{
				isContain = true;
			}
			
			auto iter = &obj->Requests[collectionIndex];
			if (iter->Service->GetSid() == recData.Sid)
			{

				SerializationManager rsm;
				SerializationManager sendsm;
				El_Memset(&rsm, 0, sizeof(SerializationManager));
				El_Memset(&sendsm, 0, sizeof(SerializationManager));
				//rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
				rsm.Reset();
				rsm.BufferLen = recData.DataLen;
				if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
				{
					rsm.BlockBufferProvider = obj->RequestBlockBufferProvider;
				}
				else
				{
					rsm.Buf = recData.Data;
				}

				if (obj->RpcConfig->CheckSumValid == true)
				{
					rsm.SetCalculateSum(0);
					rsm.SetReferenceSum(recData.CheckSum);
				}

				El_TakeMutex(obj->ObjectMutexHandle, El_WAIT_FOREVER);//由于使用obj->Buffer这个全局变量，所以添加锁
				//sendsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
				sendsm.Reset();
				sendsm.Buf = &obj->DataLinkLayoutBuffer[4];
				sendsm.BufferLen = obj->DataLinkLayoutBufferLen - 4;

				//EmbedXrpc_TimerReset(obj->SuspendTimer);
				//EmbedXrpc_TimerStart(obj->SuspendTimer, recData.TargetTimeout / 2);
				serviceInvokeParameter.request_UserDataOfTransportLayer = &recData.UserDataOfTransportLayer;
				serviceInvokeParameter.response_UserDataOfTransportLayer = recData.UserDataOfTransportLayer;
				serviceInvokeParameter.rpcObject = obj;
				serviceInvokeParameter.targetTimeOut = recData.TargetTimeout;
				iter->Service->Invoke(&serviceInvokeParameter, &rsm, &sendsm);
				El_TimerStop(obj->SuspendTimer);

				if (sendsm.Index > 0)//
				{
					obj->DataLinkLayoutBuffer[0] = (uint8_t)(recData.Sid >> 0 & 0xff);
					obj->DataLinkLayoutBuffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
					obj->DataLinkLayoutBuffer[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
					obj->DataLinkLayoutBuffer[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
					obj->DataLinkLayoutBuffer[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
					obj->Sender.Invoke(&serviceInvokeParameter.response_UserDataOfTransportLayer, obj, sendsm.Index + 4, obj->DataLinkLayoutBuffer);
				}
				El_ReleaseMutex(obj->ObjectMutexHandle);
				if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
				{
					isContain = true;
				}
				goto _break;
			}
		}
	_break:
		if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
		{
			if (isContain == false)
			{
				obj->RequestBlockBufferProvider->PopChars(nullptr, recData.DataLen);
			}
		}
		else
		{
			if (recData.DataLen > 0 && isFreeData == true)
				El_Free(recData.Data);
		}
	}
#define TrySendDataToQueue(queue,datalen,data,raw,gotolib,itemType,result)	\
		{\
			if(El_QueueSpacesAvailable(queue) > 0)\
			{\
				if(datalen>0)\
				{\
					raw.Data = (uint8_t*)El_Malloc(dataLen);\
					if (raw.Data == nullptr)\
					{\
						goto gotolib;\
					}\
					El_Memcpy(raw.Data, data, dataLen);\
				}\
				result = El_SendQueue(queue, &raw, sizeof(itemType));\
			}\
			else\
			{\
				result = QueueState_Full;\
			}\
		}
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData, DTL userDataOfTransportLayer)
	{
		if (allDataLen < 4)
			return false;
		QueueState El_SendQueueResult = QueueState_Full;

		ReceiveItemInfo<DTL> raw;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout = (uint16_t)(allData[2] | ((allData[3] & 0x3f) << 8));
		uint32_t dataLen = allDataLen - 4;
		uint8_t* data = &allData[4];
		ReceiveType_t rt = (ReceiveType_t)(allData[3] >> 6);
		raw.UserDataOfTransportLayer = userDataOfTransportLayer;
		raw.Sid = serviceId;
		raw.DataLen = dataLen;
		raw.TargetTimeout = targettimeout;
		if (RpcConfig->CheckSumValid == true)
		{
			raw.CheckSum = GetSum(data, dataLen);
		}
		else
		{
			raw.CheckSum = 0;
		}
		if (rt == ReceiveType_Response)
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Client ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			if (serviceId == EmbedXrpcSuspendSid)//挂起的SID要特殊处理
			{
				if (RpcConfig->UseRingBufferWhenReceiving == true)
				{
					El_SendQueueResult = ResponseBlockBufferProvider->Send(&raw, nullptr) == True ? QueueState_OK : QueueState_Full;
				}
				else
				{
					raw.Data = nullptr;
					if (El_QueueSpacesAvailable(ResponseBlockQueue) > 0)
					{
						El_SendQueueResult = El_SendQueue(ResponseBlockQueue, &raw, sizeof(ReceiveItemInfo<DTL>));
					}
					else
					{
						El_SendQueueResult = QueueState_Full;
					}
				}
				goto sqr;
			}

			if (RpcConfig->UseRingBufferWhenReceiving == true)
			{
				El_SendQueueResult = ResponseBlockBufferProvider->Send(&raw, data) == True ? QueueState_OK : QueueState_Full;
			}
			else
			{
				TrySendDataToQueue(ResponseBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo<DTL>, El_SendQueueResult);
			}
	
		}
		else if (rt == ReceiveType_Delegate)
		{
			if (RpcConfig->UseRingBufferWhenReceiving == true)
			{
				El_SendQueueResult = DelegateBlockBufferProvider->Send(&raw, data) == True ? QueueState_OK : QueueState_Full;
			}
			else if (RpcConfig->IsSendToQueue == true)
			{
				TrySendDataToQueue(DelegateBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo<DTL>, El_SendQueueResult);
			}
			else
			{
				raw.Data = data;
				DelegateServiceExecute(this, raw, false);
				El_SendQueueResult = QueueState_OK;
			}		
		}
		else if (rt == ReceiveType_Request)//server
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Server ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			if (RpcConfig->UseRingBufferWhenReceiving == true)
			{
				El_SendQueueResult = RequestBlockBufferProvider->Send( &raw, data) == True ? QueueState_OK : QueueState_Full;
			}
			else if (RpcConfig->IsSendToQueue == true)
			{
				TrySendDataToQueue(RequestBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo<DTL>, El_SendQueueResult);
			}
			else
			{
				raw.Data = data;
				ResponseServiceExecute(this, raw, false);
				El_SendQueueResult = QueueState_OK;
			}
		}
	sqr:
		return El_SendQueueResult == QueueState_OK ? true : false;
	}
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
		obj->Sender.Invoke(&obj->UserDataOfTransportLayerOfSuspendTimerUsed, obj, 4, sb);
		//obj->El_ReleaseMutex(obj->ObjectMutexHandle);
	}


	static void DelegateServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		ReceiveItemInfo<DTL> recData;
		bool waitResult = false;
		for (;;)
		{
			if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
			{
				waitResult= obj->DelegateBlockBufferProvider->Receive(&recData, 1);
			}
			else
			{
				waitResult = El_ReceiveQueue(obj->DelegateBlockQueue, &recData, sizeof(ReceiveItemInfo<DTL>), 1) == QueueState_OK ? true : false;
			}
			if(waitResult==true)
			{
				DelegateServiceExecute(obj, recData, true);
			}
			if (obj->DeInitFlag == true)
			{
				obj->DelegateServiceThreadExitState = true;
				return;
			}
		}
	}



	static void RequestProcessServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		ReceiveItemInfo<DTL> recData;
		bool waitResult = false;
		for (;;)
		{

			if (obj->RpcConfig->UseRingBufferWhenReceiving == true)
			{
				waitResult = obj->RequestBlockBufferProvider->Receive(&recData, 1);
			}
			else
			{
				waitResult = El_ReceiveQueue(obj->RequestBlockQueue, &recData, sizeof(ReceiveItemInfo<DTL>), 1) == QueueState_OK ? true : false;
			}

			if(waitResult==true)
			{
				ResponseServiceExecute(obj, recData, true);
			}

			if (obj->DeInitFlag == true)
			{
				obj->RequestProcessServiceThreadExitState = true;
				return;
			}
		}
	}

	RequestResponseState Wait(uint32_t sid, ReceiveItemInfo<DTL>* recData)
	{
		RequestResponseState ret = ResponseState_Ok;
		//ReceiveItemInfo recData;
		bool waitResult = false;
		while (true)
		{
			if (RpcConfig->UseRingBufferWhenReceiving == true)
			{
				waitResult = ResponseBlockBufferProvider->Receive(recData, TimeOut);
			}
			else
			{
				auto wr = El_ReceiveQueue(ResponseBlockQueue, recData, sizeof(ReceiveItemInfo<DTL>), TimeOut);
				if (wr != QueueState_OK)
				{
					return ResponseState_Timeout;
				}
				waitResult = true;
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
};

#endif
