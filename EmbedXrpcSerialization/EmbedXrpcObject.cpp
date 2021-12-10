
#include "EmbedXrpcObject.h"

void EmbedXrpcObject::Init()
{
	DelegateServiceThreadExitState = false;
	RequestProcessServiceThreadExitState = false;
	DeInitFlag = false;

	ObjectMutexHandle = El_CreateMutex("ObjectMutex");

#if EmbedXrpc_UseRingBufferWhenReceiving==1
	DelegateBlockBufferProvider = new BlockRingBufferProvider();
	BlockRingBufferProvider_Init(DelegateBlockBufferProvider, DelegateRingBuffer, EmbedXrpc_DelegateRingBufferSize, EmbedXrpc_DelegateBlockQueue_MaxItemNumber);

	ResponseBlockBufferProvider = new BlockRingBufferProvider();
	BlockRingBufferProvider_Init(ResponseBlockBufferProvider, ResponseRingBuffer, EmbedXrpc_ResponseRingBufferSize, EmbedXrpc_ResponseBlockQueue_MaxItemNumber);

	RequestBlockBufferProvider = new BlockRingBufferProvider();
	BlockRingBufferProvider_Init(RequestBlockBufferProvider, RequestRingBuffer, EmbedXrpc_RequestRingBufferSize, EmbedXrpc_RequestBlockQueue_MaxItemNumber);
#else
	DelegateBlockQueue = El_CreateQueue("DelegateBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_DelegateBlockQueue_MaxItemNumber);
	ResponseBlockQueue = El_CreateQueue("ResponseBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_ResponseBlockQueue_MaxItemNumber);
	RequestBlockQueue = El_CreateQueue("RequestBlockQueue", sizeof(ReceiveItemInfo), EmbedXrpc_RequestBlockQueue_MaxItemNumber);
#endif

#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0&&EmbedXrpc_IsSendToQueue==1 //client
	DelegateServiceThreadHandle = El_CreateThread("DelegateServiceThread", Client_ThreadPriority, DelegateServiceThread, this, 2048);
	El_ThreadStart(DelegateServiceThreadHandle);
#endif

	//server
#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0&&EmbedXrpc_IsSendToQueue==1
	RequestProcessServiceThreadHandle = El_CreateThread("RequestProcessServiceThread", Server_ThreadPriority, RequestProcessServiceThread, this, 2048);
	El_ThreadStart(RequestProcessServiceThreadHandle);
#endif
	SuspendTimer = El_CreateTimer("SuspendTimer", El_WAIT_FOREVER, SuspendTimerCallBack, this);
}
void EmbedXrpcObject::DeInit()
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
void EmbedXrpcObject::DelegateServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData)
{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
	bool isContain;
	isContain = false;
	(void)isContain;
#endif
	for (uint32_t collectionIndex = 0; collectionIndex < obj->DelegatesCount; collectionIndex++)
	{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		isContain = true;
#endif
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
			SerializationManager_SetCalculateSum(&rsm, 0);
			SerializationManager_SetReferenceSum(&rsm, recData.CheckSum);
#endif
			iter->Delegate->Invoke(&recData.UserDataOfTransportLayer, &rsm);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			isContain = true;
#endif
			goto _break;
		}
	}
_break:
#if EmbedXrpc_UseRingBufferWhenReceiving==1
	if (isContain == false)
	{
		BlockRingBufferProvider_PopChars(obj->DelegateBlockBufferProvider, nullptr, recData.DataLen);
	}
#else
	if ((recData.DataLen > 0) && (isFreeData == true))
		El_Free(recData.Data);
#endif
	//EmbedSerializationShowMessage("EmbedXrpcObject","Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);

}
void EmbedXrpcObject::ResponseServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData)
{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
	bool isContain;
	isContain = false;
	(void)isContain;
#endif
	ServiceInvokeParameter serviceInvokeParameter;
	for (uint32_t collectionIndex = 0; collectionIndex < obj->RequestsCount; collectionIndex++)
	{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		isContain = true;
#endif
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
			SerializationManager_SetCalculateSum(&rsm, 0);
			SerializationManager_SetReferenceSum(&rsm, recData.CheckSum);
#endif
			El_TakeMutex(obj->ObjectMutexHandle, El_WAIT_FOREVER);//由于使用obj->Buffer这个全局变量，所以添加锁
			//sendsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
			SerializationManager_Reset(&sendsm);
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
				obj->Send(&serviceInvokeParameter.response_UserDataOfTransportLayer, obj, sendsm.Index + 4, obj->DataLinkLayoutBuffer);
			}
			El_ReleaseMutex(obj->ObjectMutexHandle);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			isContain = true;
#endif
			goto _break;
		}
	}
_break:
#if EmbedXrpc_UseRingBufferWhenReceiving==1
	if (isContain == false)
	{
		BlockRingBufferProvider_PopChars(obj->RequestBlockBufferProvider, nullptr, recData.DataLen);
	}
#else
	if (recData.DataLen > 0 && isFreeData == true)
		El_Free(recData.Data);
#endif
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


bool EmbedXrpcObject::ReceivedMessage(uint32_t allDataLen, uint8_t* allData, UserDataOfTransportLayer_t userDataOfTransportLayer)
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
	raw.Sid = serviceId;
	raw.DataLen = dataLen;
	raw.TargetTimeout = targettimeout;
#if EmbedXrpc_CheckSumValid==1
	raw.CheckSum = GetSum(data, dataLen);
#else
	raw.CheckSum = 0;
#endif
	if (rt == ReceiveType_Response)
	{
		//EmbedSerializationShowMessage("EmbedXrpcObject","Client ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
		if (serviceId == EmbedXrpcSuspendSid)//挂起的SID要特殊处理
		{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
			El_SendQueueResult = BlockRingBufferProvider_Send(ResponseBlockBufferProvider, &raw, nullptr, 0) == True ? QueueState_OK : QueueState_Full;
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
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		El_SendQueueResult = BlockRingBufferProvider_Send(ResponseBlockBufferProvider, &raw, data, dataLen) == True ? QueueState_OK : QueueState_Full;
#else
		TrySendDataToQueue(ResponseBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo, El_SendQueueResult);
#endif			
	}
	else if (rt == ReceiveType_Delegate)
	{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		El_SendQueueResult = BlockRingBufferProvider_Send(DelegateBlockBufferProvider, &raw, data, dataLen) == True ? QueueState_OK : QueueState_Full;
#elif EmbedXrpc_IsSendToQueue==1
		TrySendDataToQueue(DelegateBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo, El_SendQueueResult);
#else
		raw.Data = data;
		DelegateServiceExecute(this, raw, false);
		El_SendQueueResult = QueueState_OK;
#endif			
	}
	else if (rt == ReceiveType_Request)//server
	{
		//EmbedSerializationShowMessage("EmbedXrpcObject","Server ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		El_SendQueueResult = BlockRingBufferProvider_Send(RequestBlockBufferProvider, &raw, data, dataLen) == True ? QueueState_OK : QueueState_Full;
#elif EmbedXrpc_IsSendToQueue==1
		TrySendDataToQueue(RequestBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo, El_SendQueueResult);
#else		
		raw.Data = data;
		ResponseServiceExecute(this, raw, false);
		El_SendQueueResult = QueueState_OK;
#endif
	}
sqr:
	return El_SendQueueResult == QueueState_OK ? true : false;
}


void EmbedXrpcObject::SuspendTimerCallBack(void* arg)
{
	EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
	//obj->El_TakeMutex(obj->ObjectMutexHandle, El_WAIT_FOREVER);//不需要加锁 这里不使用obj->buffer全局buffer
	uint8_t sb[4];
	sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
	sb[1] = (uint8_t)(EmbedXrpcSuspendSid >> 8 & 0xff);
	sb[2] = (uint8_t)(obj->TimeOut & 0xff);
	sb[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
	sb[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
	obj->Send(&obj->UserDataOfTransportLayerOfSuspendTimerUsed, obj, 4, sb);
	//obj->El_ReleaseMutex(obj->ObjectMutexHandle);
}


#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0 &&EmbedXrpc_IsSendToQueue==1
void EmbedXrpcObject::DelegateServiceThread(void* arg)
{
	EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
	ReceiveItemInfo recData;
	for (;;)
	{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		if (BlockRingBufferProvider_Receive(obj->DelegateBlockBufferProvider, &recData, 1) == True)
#else
		if (El_ReceiveQueue(obj->DelegateBlockQueue, &recData, sizeof(ReceiveItemInfo), 1) == QueueState_OK)
#endif
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
#endif




#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0 && EmbedXrpc_IsSendToQueue==1
void EmbedXrpcObject::RequestProcessServiceThread(void* arg)
{
	EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
	ReceiveItemInfo recData;
	for (;;)
	{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		if (BlockRingBufferProvider_Receive(obj->RequestBlockBufferProvider, &recData, 1) == True)
#else
		if (El_ReceiveQueue(obj->RequestBlockQueue, &recData, sizeof(ReceiveItemInfo), 1) == QueueState_OK)
#endif
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
#endif


#if EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
RequestResponseState EmbedXrpcObject::Wait(uint32_t sid, ReceiveItemInfo* recData)
{
	RequestResponseState ret = ResponseState_Ok;
	//ReceiveItemInfo recData;
	while (true)
	{
#if EmbedXrpc_UseRingBufferWhenReceiving==1
		if (BlockRingBufferProvider_Receive(ResponseBlockBufferProvider, recData, TimeOut) == false)
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
