
#include "EmbedXrpcObject.h"

void EmbedXrpcObject::Init()
{
	DelegateServiceThreadExitState = false;
	RequestProcessServiceThreadExitState = false;
	DeInitFlag = false;

	//ObjectMutexHandle = El_CreateMutex("ObjectMutex");

	if (RpcConfig.UseRingBufferWhenReceiving == false)
	{
		DelegateBlockQueue = El_CreateQueue("DelegateBlockQueue", sizeof(ReceiveItemInfo), RpcConfig.DynamicMemoryConfig.DelegateBlockQueue_MaxItemNumber);
		ResponseBlockQueue = El_CreateQueue("ResponseBlockQueue", sizeof(ReceiveItemInfo), RpcConfig.DynamicMemoryConfig.ResponseBlockQueue_MaxItemNumber);
		RequestBlockQueue = El_CreateQueue("RequestBlockQueue", sizeof(ReceiveItemInfo), RpcConfig.DynamicMemoryConfig.RequestBlockQueue_MaxItemNumber);
	}

	if (
		((RpcConfig.UseRingBufferWhenReceiving == false)
			&& (RpcConfig.DynamicMemoryConfig.DelegateBlockQueue_MaxItemNumber > 0)
			&& (RpcConfig.DynamicMemoryConfig.IsSendToQueue == true))//动态内存模式
		|| ((RpcConfig.UseRingBufferWhenReceiving == true) && (RpcConfig.RingBufferConfig.DelegateBlockBufferProvider != nullptr))//ringbuffer模式
		)
	{
		DelegateServiceThreadHandle = El_CreateThread("DelegateServiceThread", RpcConfig.ClientThreadPriority, DelegateServiceThread, this, 2048);
		El_ThreadStart(DelegateServiceThreadHandle);
	}

	if (
		((RpcConfig.UseRingBufferWhenReceiving == false)
			&& (RpcConfig.DynamicMemoryConfig.RequestBlockQueue_MaxItemNumber > 0)
			&& (RpcConfig.DynamicMemoryConfig.IsSendToQueue == true))//动态内存模式
		|| ((RpcConfig.UseRingBufferWhenReceiving == true) && (RpcConfig.RingBufferConfig.RequestBlockBufferProvider != nullptr))//ringbuffer模式
		)
	{
		RequestProcessServiceThreadHandle = El_CreateThread("RequestProcessServiceThread", RpcConfig.ServerThreadPriority, RequestProcessServiceThread, this, 2048);
		El_ThreadStart(RequestProcessServiceThreadHandle);
	}
	SuspendTimer = El_CreateTimer("SuspendTimer", El_WAIT_FOREVER, SuspendTimerCallBack, this);
	}
void EmbedXrpcObject::DeInit()
{
	DeInitFlag = true;
	if (RequestBuffer.MutexHandle != nullptr)
	{
		El_DeleteMutex(RequestBuffer.MutexHandle);
	}
	if (ResponseBuffer.MutexHandle != nullptr)
	{
		El_DeleteMutex(ResponseBuffer.MutexHandle);
	}
	if (DelegateBuffer.MutexHandle != nullptr)
	{
		El_DeleteMutex(DelegateBuffer.MutexHandle);
	}
	if (SuspendTimerBuffer.MutexHandle != nullptr)
	{
		El_DeleteMutex(SuspendTimerBuffer.MutexHandle);
}


	if (DelegateServiceThreadHandle != nullptr)
	{
		El_DeleteThread(DelegateServiceThreadHandle);
}
	else
	{
		DelegateServiceThreadExitState = true;
	}

	if (RpcConfig.UseRingBufferWhenReceiving == 1)
	{
		delete DelegateBlockBufferProvider;
		delete ResponseBlockBufferProvider;
	}
	else
	{
		El_DeleteQueue(DelegateBlockQueue);
		El_DeleteQueue(ResponseBlockQueue);
	}

	if (RequestProcessServiceThreadHandle != nullptr)
	{
		El_DeleteThread(RequestProcessServiceThreadHandle);
	}
	else
	{
		RequestProcessServiceThreadExitState = true;
	}

	if (RpcConfig.UseRingBufferWhenReceiving == true)
	{
		delete RequestBlockBufferProvider;
	}
	else
	{
		El_DeleteQueue(RequestBlockQueue);//server接受到的request队列数据
	}

	El_DeleteTimer(SuspendTimer);

}
void EmbedXrpcObject::DelegateServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData)
{
	bool isContain;
	isContain = false;
	(void)isContain;
	for (uint32_t collectionIndex = 0; collectionIndex < obj->DelegatesCount; collectionIndex++)
	{
		if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
		{
			isContain = true;
		}
		auto iter = &obj->Delegates[collectionIndex];
		if (iter->Delegate->GetSid() == recData.Sid)
		{
			SerializationManager rsm;
			El_Memset(&rsm, 0, sizeof(SerializationManager));
			//rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
			SerializationManager_Reset(&rsm);
			rsm.BufferLen = recData.DataLen;
			if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
			{
				rsm.BlockBufferProvider = obj->DelegateBlockBufferProvider;
			}
			else
			{
				rsm.Buf = recData.Data;
		}

			if (obj->RpcConfig.CheckSumValid == true)
			{
				SerializationManager_SetCalculateSum(&rsm, 0);
				SerializationManager_SetReferenceSum(&rsm, recData.CheckSum);
			}

			iter->Delegate->Invoke(&obj->RpcConfig, &recData.UserDataOfTransportLayer, &rsm);

			if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
			{
				isContain = true;
			}

			goto _break;
			}
	}
_break:
	if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
	{
		if (isContain == false)
		{
			BlockRingBufferProvider_PopChars(obj->DelegateBlockBufferProvider,nullptr, recData.DataLen);
		}
	}
	else
	{
		if ((recData.DataLen > 0) && (isFreeData == true))
			El_Free(recData.Data);
	}
	//EmbedSerializationShowMessage("EmbedXrpcObject","Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);

}
void EmbedXrpcObject::ResponseServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData)
{
	bool isContain;
	isContain = false;
	(void)isContain;

	ServiceInvokeParameter serviceInvokeParameter;
	for (uint32_t collectionIndex = 0; collectionIndex < obj->RequestsCount; collectionIndex++)
	{
		if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
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
			SerializationManager_Reset(&rsm);
			rsm.BufferLen = recData.DataLen;
			if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
			{
				rsm.BlockBufferProvider = obj->RequestBlockBufferProvider;
			}
			else
			{
				rsm.Buf = recData.Data;
			}

			if (obj->RpcConfig.CheckSumValid == true)
			{
				SerializationManager_SetCalculateSum(&rsm,0);
				SerializationManager_SetReferenceSum(&rsm, recData.CheckSum);
			}
			if (obj->ResponseBuffer.MutexHandle != nullptr)
			{
				El_TakeMutex(obj->ResponseBuffer.MutexHandle, El_WAIT_FOREVER);//由于使用ResponseBuffer，所以添加锁
			}
			//sendsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
			SerializationManager_Reset(&sendsm);
			sendsm.Buf = &obj->ResponseBuffer.Buffer[4];
			sendsm.BufferLen = obj->ResponseBuffer.BufferLen - 4;

			//EmbedXrpc_TimerReset(obj->SuspendTimer);
			//EmbedXrpc_TimerStart(obj->SuspendTimer, recData.TargetTimeout / 2);
			serviceInvokeParameter.Request_UserDataOfTransportLayer = &recData.UserDataOfTransportLayer;
			serviceInvokeParameter.Response_UserDataOfTransportLayer = recData.UserDataOfTransportLayer;
			serviceInvokeParameter.RpcObject = obj;
			serviceInvokeParameter.TargetTimeOut = recData.TargetTimeout;
			iter->Service->Invoke(&serviceInvokeParameter, &rsm, &sendsm);
			El_TimerStop(obj->SuspendTimer);

			if (sendsm.Index > 0)//
			{
				obj->ResponseBuffer.Buffer[0] = (uint8_t)(recData.Sid >> 0 & 0xff);
				obj->ResponseBuffer.Buffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
				obj->ResponseBuffer.Buffer[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
				obj->ResponseBuffer.Buffer[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
				obj->ResponseBuffer.Buffer[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
				obj->Send(&serviceInvokeParameter.Response_UserDataOfTransportLayer, obj, sendsm.Index + 4, obj->ResponseBuffer.Buffer);
			}
			if (obj->ResponseBuffer.MutexHandle != nullptr)
			{
				El_ReleaseMutex(obj->ResponseBuffer.MutexHandle);
			}

			if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
			{
				isContain = true;
			}
			goto _break;
		}
	}
_break:
	if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
	{
		if (isContain == false)
		{
			BlockRingBufferProvider_PopChars(obj->RequestBlockBufferProvider,nullptr, recData.DataLen);
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
	El_Memset(&raw, 0, sizeof(ReceiveItemInfo));
	raw.UserDataOfTransportLayer = userDataOfTransportLayer;
	raw.Sid = serviceId;
	raw.DataLen = dataLen;
	raw.TargetTimeout = targettimeout;
	if (RpcConfig.CheckSumValid == true)
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
			if (RpcConfig.UseRingBufferWhenReceiving == true)
			{
				El_SendQueueResult = BlockRingBufferProvider_Send(ResponseBlockBufferProvider,&raw, nullptr) == True ? QueueState_OK : QueueState_Full;
			}
			else
			{
				raw.Data = nullptr;
				if (El_QueueSpacesAvailable(ResponseBlockQueue) > 0)
				{
					El_SendQueueResult = El_SendQueue(ResponseBlockQueue, &raw, sizeof(ReceiveItemInfo));
				}
				else
				{
					El_SendQueueResult = QueueState_Full;
				}
			}
			goto sqr;
				}

		if (RpcConfig.UseRingBufferWhenReceiving == true)
		{
			El_SendQueueResult = BlockRingBufferProvider_Send(ResponseBlockBufferProvider ,&raw, data) == True ? QueueState_OK : QueueState_Full;
		}
		else
		{
			TrySendDataToQueue(ResponseBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo, El_SendQueueResult);
		}

			}
	else if (rt == ReceiveType_Delegate)
	{
		if (RpcConfig.UseRingBufferWhenReceiving == true)
		{
			El_SendQueueResult = BlockRingBufferProvider_Send(DelegateBlockBufferProvider,&raw, data) == True ? QueueState_OK : QueueState_Full;
		}
		else if (RpcConfig.DynamicMemoryConfig.IsSendToQueue == true)
		{
			TrySendDataToQueue(DelegateBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo, El_SendQueueResult);
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
		if (RpcConfig.UseRingBufferWhenReceiving == true)
		{
			El_SendQueueResult = BlockRingBufferProvider_Send(RequestBlockBufferProvider ,&raw, data) == True ? QueueState_OK : QueueState_Full;
		}
		else if (RpcConfig.DynamicMemoryConfig.IsSendToQueue == true)
		{
			TrySendDataToQueue(RequestBlockQueue, dataLen, data, raw, sqr, ReceiveItemInfo, El_SendQueueResult);
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


void EmbedXrpcObject::SuspendTimerCallBack(void* arg)
{
	EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
	if (obj->SuspendTimerBuffer.MutexHandle != nullptr)
	{
		El_TakeMutex(obj->SuspendTimerBuffer.MutexHandle, El_WAIT_FOREVER);
	}
	uint8_t sb[4];
	sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
	sb[1] = (uint8_t)(EmbedXrpcSuspendSid >> 8 & 0xff);
	sb[2] = (uint8_t)(obj->TimeOut & 0xff);
	sb[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
	sb[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
	obj->Send(&obj->UserDataOfTransportLayerOfSuspendTimerUsed, obj, 4, sb);
	if (obj->SuspendTimerBuffer.MutexHandle != nullptr)
	{
		El_ReleaseMutex(obj->SuspendTimerBuffer.MutexHandle);
	}
}



void EmbedXrpcObject::DelegateServiceThread(void* arg)
{
	EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
	ReceiveItemInfo recData;
	bool waitResult = false;
	for (;;)
	{
		if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
		{
			waitResult = BlockRingBufferProvider_Receive(obj->DelegateBlockBufferProvider ,&recData, 1);
		}
		else
		{
			waitResult = El_ReceiveQueue(obj->DelegateBlockQueue, &recData, sizeof(ReceiveItemInfo), 1) == QueueState_OK ? true : false;
		}
		if (waitResult == true)
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

void EmbedXrpcObject::RequestProcessServiceThread(void* arg)
{
	EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
	ReceiveItemInfo recData;
	bool waitResult = false;
	for (;;)
	{

		if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
		{
			waitResult = BlockRingBufferProvider_Receive(obj->RequestBlockBufferProvider ,&recData, 1);
		}
		else
		{
			waitResult = El_ReceiveQueue(obj->RequestBlockQueue, &recData, sizeof(ReceiveItemInfo), 1) == QueueState_OK ? true : false;
		}

		if (waitResult == true)
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

RequestResponseState EmbedXrpcObject::Wait(uint32_t sid, ReceiveItemInfo* recData)
{
	RequestResponseState ret = ResponseState_Ok;
	//ReceiveItemInfo recData;
	bool waitResult = false;
	while (true)
	{
		if (RpcConfig.UseRingBufferWhenReceiving == true)
		{
			waitResult = BlockRingBufferProvider_Receive(ResponseBlockBufferProvider,recData, TimeOut);
		}
		else
		{
			auto wr = El_ReceiveQueue(ResponseBlockQueue, recData, sizeof(ReceiveItemInfo), TimeOut);
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

