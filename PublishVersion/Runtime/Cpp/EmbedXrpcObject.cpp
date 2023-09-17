
#include "EmbedXrpcObject.h"
#include "SoftCrc32.h"
void EmbedXrpcObject::Init(InitConfig *cfg)
{
	ServiceThreadExitState = false;
	DeInitFlag = false;

	El_Strncpy(Name, cfg->Name, EmbedXrpc_NameMaxLen);

	DataLinkBufferForRequest.Buffer = cfg->DataLinkBufferConfigForRequest.Buffer;
	DataLinkBufferForRequest.BufferLen = cfg->DataLinkBufferConfigForRequest.BufferLen;
	if (cfg->DataLinkBufferConfigForRequest.IsUseMutex == true)
	{
		DataLinkBufferForRequest.MutexHandle = El_CreateMutex("DataLinkBufferForRequest.MutexHandle");
	}
	else
	{
		DataLinkBufferForRequest.MutexHandle = nullptr;
	}

	DataLinkBufferForResponse.Buffer = cfg->DataLinkBufferConfigForResponse.Buffer;
	DataLinkBufferForResponse.BufferLen = cfg->DataLinkBufferConfigForResponse.BufferLen;
	if (cfg->DataLinkBufferConfigForResponse.IsUseMutex == true)
	{
		DataLinkBufferForResponse.MutexHandle = El_CreateMutex("DataLinkBufferForResponse.MutexHandle");
	}
	else
	{
		DataLinkBufferForResponse.MutexHandle = nullptr;
	}

	TimeOut = cfg->TimeOut;
	Send = cfg->Sender;

	UserData = cfg->UserData;

	ServiceThreadHandle = nullptr;
	SuspendTimer = nullptr;

	Services = cfg->Services;
	ServicesCount = cfg->ServicesCount;
	RpcConfig = cfg->RpcConfig;

	MessageQueueOfRequestService = El_CreateQueue("MessageQueueOfRequestService", sizeof(ReceiveItemInfo), RpcConfig.DynamicMemoryConfig.MessageQueueOfRequestService_MaxItemNumber);

	if (RpcConfig.UseRingBufferWhenReceiving == false)
	{
		if (RpcConfig.DynamicMemoryConfig.IsSendToQueue == true)
		{
			ServiceMessageQueue = El_CreateQueue("ServiceMessageQueue", sizeof(ReceiveItemInfo), RpcConfig.DynamicMemoryConfig.ServiceMessageQueue_MaxItemNumber);
		}
		else
		{
		}
	}
	else
	{
		BlockBufferProviderOfRequestService = new BlockRingBufferProvider();
		BlockRingBufferProvider_Init(BlockBufferProviderOfRequestService,
									 RpcConfig.RingBufferConfig.BlockBufferOfRequestService_Config.Buffer,
									 RpcConfig.RingBufferConfig.BlockBufferOfRequestService_Config.Size,
									 RpcConfig.RingBufferConfig.BlockBufferOfRequestService_Config.MaxItemNumber);

		ServiceBlockBufferProvider = new BlockRingBufferProvider();
		BlockRingBufferProvider_Init(ServiceBlockBufferProvider,
									 RpcConfig.RingBufferConfig.ServiceBlockBufferConfig.Buffer,
									 RpcConfig.RingBufferConfig.ServiceBlockBufferConfig.Size,
									 RpcConfig.RingBufferConfig.ServiceBlockBufferConfig.MaxItemNumber);
	}

	if (
		((RpcConfig.UseRingBufferWhenReceiving == false) && (RpcConfig.DynamicMemoryConfig.ServiceMessageQueue_MaxItemNumber > 0) && (RpcConfig.DynamicMemoryConfig.IsSendToQueue == true)) // 动态内存模式
		|| (RpcConfig.UseRingBufferWhenReceiving == true)																																	// ringbuffer模式
	)
	{
		ServiceThreadHandle = El_CreateThread("ServiceThread", RpcConfig.ServiceThreadPriority, ServiceThread, this, RpcConfig.ServiceThreadStackSize);
		El_ThreadStart(ServiceThreadHandle, 0);
	}
	SuspendTimer = El_CreateTimer("SuspendTimer", El_WAIT_FOREVER, SuspendTimerCallBack, this);
}
void EmbedXrpcObject::DeInit()
{
	DeInitFlag = true;
	if (DataLinkBufferForRequest.MutexHandle != nullptr)
	{
		El_DeleteMutex(DataLinkBufferForRequest.MutexHandle);
	}
	if (DataLinkBufferForResponse.MutexHandle != nullptr)
	{
		El_DeleteMutex(DataLinkBufferForResponse.MutexHandle);
	}

	if (RpcConfig.UseRingBufferWhenReceiving == 1)
	{
		delete BlockBufferProviderOfRequestService;
	}
	else
	{
		El_DeleteQueue(MessageQueueOfRequestService);
	}

	if (ServiceThreadHandle != nullptr)
	{
		while (ServiceThreadExitState == false)
		{
			El_Delay(10);
		}
		El_DeleteThread(ServiceThreadHandle);
	}

	if (RpcConfig.UseRingBufferWhenReceiving == true)
	{
		delete ServiceBlockBufferProvider;
	}
	else if (ServiceMessageQueue != nullptr)
	{
		El_DeleteQueue(ServiceMessageQueue); // server接受到的request队列数据
	}

	El_DeleteTimer(SuspendTimer);
}
uint32_t GetBufferCrc(uint32_t len, uint8_t *Buf)
{
	CrcCalculate_t crcer;
	El_Memset(&crcer, 0, sizeof(crcer));
	Crc32Init(&crcer, Crc32_InitValue);
	Crc32Compute(&crcer, Buf, len);
	Crc32Finish(&crcer);
	return crcer.CurrentValue;
}
void EmbedXrpcObject::ServiceExecute(EmbedXrpcObject *obj, ReceiveItemInfo &recData, bool isFreeData, int isIsr)
{
	bool isContain;
	isContain = false;
	(void)isContain;
	bool isFindService;
	isFindService = false;
	ServiceInvokeParameter serviceInvokeParameter;
	for (uint32_t collectionIndex = 0; collectionIndex < obj->ServicesCount; collectionIndex++)
	{
		if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
		{
			isContain = true;
		}

		auto iter = &obj->Services[collectionIndex];
		if (iter->Service->GetSid() == recData.Sid)
		{
			isFindService = true;
			SerializationManager rsm;
			SerializationManager sendsm;
			El_Memset(&rsm, 0, sizeof(SerializationManager));
			El_Memset(&sendsm, 0, sizeof(SerializationManager));
			// rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
			rsm.BufferLen = recData.DataLen;
			if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
			{
				rsm.BlockBufferProvider = obj->ServiceBlockBufferProvider;
			}
			else
			{
				rsm.Buf = recData.Data;
			}

			// if (obj->RpcConfig.CheckSumValid == true)
			//{
			//	SerializationManager_SetCalculateSum(&rsm, 0);
			//	SerializationManager_SetReferenceSum(&rsm, recData.CheckSum);
			// }
			if (obj->DataLinkBufferForResponse.MutexHandle != nullptr)
			{
				El_TakeMutex(obj->DataLinkBufferForResponse.MutexHandle, El_WAIT_FOREVER, isIsr); // 由于使用DataLinkBufferForResponse，所以添加锁
			}
			// sendsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
			sendsm.Buf = &obj->DataLinkBufferForResponse.Buffer[12];
			sendsm.BufferLen = obj->DataLinkBufferForResponse.BufferLen - 12;

			// EmbedXrpc_TimerReset(obj->SuspendTimer);
			// EmbedXrpc_TimerStart(obj->SuspendTimer, recData.TargetTimeout / 2);
			serviceInvokeParameter.Request_UserDataOfTransportLayer = &recData.UserDataOfTransportLayer;
			serviceInvokeParameter.Response_UserDataOfTransportLayer = recData.UserDataOfTransportLayer;
			serviceInvokeParameter.RpcObject = obj;
			serviceInvokeParameter.TargetTimeOut = recData.TargetTimeout;
			serviceInvokeParameter.IsIsr = isIsr;
			iter->Service->Invoke(&serviceInvokeParameter, &rsm, &sendsm);
			El_TimerStop(obj->SuspendTimer, isIsr);

			if (sendsm.Index > 0) //
			{
				obj->DataLinkBufferForResponse.Buffer[0] = (uint8_t)(recData.Sid >> 0 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
				obj->DataLinkBufferForResponse.Buffer[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);

				obj->DataLinkBufferForResponse.Buffer[4] = (uint8_t)(sendsm.Index >> 0 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[5] = (uint8_t)(sendsm.Index >> 8 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[6] = (uint8_t)(sendsm.Index >> 16 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[7] = (uint8_t)(sendsm.Index >> 24 & 0xff);

				uint32_t bufcrc = GetBufferCrc(sendsm.Index, sendsm.Buf);
				obj->DataLinkBufferForResponse.Buffer[8] = (uint8_t)(bufcrc >> 0 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[9] = (uint8_t)(bufcrc >> 8 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[10] = (uint8_t)(bufcrc >> 16 & 0xff);
				obj->DataLinkBufferForResponse.Buffer[11] = (uint8_t)(bufcrc >> 24 & 0xff);

				RequestParameter rp;
				rp.Udtl = &serviceInvokeParameter.Response_UserDataOfTransportLayer;
				rp.IsIsr = isIsr;
				obj->Send(&rp, obj, sendsm.Index + 12, obj->DataLinkBufferForResponse.Buffer);
			}
			if (obj->DataLinkBufferForResponse.MutexHandle != nullptr)
			{
				El_ReleaseMutex(obj->DataLinkBufferForResponse.MutexHandle, isIsr);
			}

			if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
			{
				isContain = true;
			}
			// goto _break;
			break;
		}
	}
	//_break:
	if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
	{
		if (isContain == false)
		{
			BlockRingBufferProvider_PopChars(obj->ServiceBlockBufferProvider, nullptr, recData.DataLen, isIsr);
		}
	}
	else
	{
		if (recData.DataLen > 0 && isFreeData == true)
			El_Free(recData.Data);
	}
	if (isFindService == false)
	{
		uint8_t sb[12];
		sb[0] = (uint8_t)(EmbedXrpcUnsupportedSid >> 0 & 0xff);
		sb[1] = (uint8_t)(EmbedXrpcUnsupportedSid >> 8 & 0xff);
		sb[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
		sb[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
		sb[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);

		sb[4] = (uint8_t)(0 >> 0 & 0xff);
		sb[5] = (uint8_t)(0 >> 8 & 0xff);
		sb[6] = (uint8_t)(0 >> 16 & 0xff);
		sb[7] = (uint8_t)(0 >> 24 & 0xff);

		uint32_t bufcrc = GetBufferCrc(0, nullptr);
		sb[8] = (uint8_t)(bufcrc >> 0 & 0xff);
		sb[9] = (uint8_t)(bufcrc >> 8 & 0xff);
		sb[10] = (uint8_t)(bufcrc >> 16 & 0xff);
		sb[11] = (uint8_t)(bufcrc >> 24 & 0xff);
		RequestParameter rp;
		rp.Udtl = &serviceInvokeParameter.Response_UserDataOfTransportLayer;
		rp.IsIsr = isIsr;
		obj->Send(&rp, obj, 12, sb);
	}
}

#define TrySendDataToQueue(queue, datalen, data, raw, gotolib, itemType, result, isIsr)                                                                                \
	{                                                                                                                                                                  \
		if (El_QueueSpacesAvailable(queue, isIsr) > 0)                                                                                                                 \
		{                                                                                                                                                              \
			if (datalen > 0)                                                                                                                                           \
			{                                                                                                                                                          \
				raw.Data = (uint8_t *)El_Malloc(dataLen);                                                                                                              \
				if (raw.Data == nullptr)                                                                                                                               \
				{                                                                                                                                                      \
					goto gotolib;                                                                                                                                      \
				}                                                                                                                                                      \
				El_Memcpy(raw.Data, data, dataLen);                                                                                                                    \
			}                                                                                                                                                          \
			result = El_SendQueue(queue, &raw, sizeof(itemType), isIsr) == QueueState::QueueState_Full ? ReceivedMessageStatus::QueueFull : ReceivedMessageStatus::Ok; \
		}                                                                                                                                                              \
		else                                                                                                                                                           \
		{                                                                                                                                                              \
			result = ReceivedMessageStatus::QueueFull;                                                                                                                 \
		}                                                                                                                                                              \
	}

ReceivedMessageStatus EmbedXrpcObject::ReceivedMessage(uint32_t allDataLen, uint8_t *allData, UserDataOfTransportLayer_t userDataOfTransportLayer, int isIsr)
{
	if (allDataLen < 12)
		return ReceivedMessageStatus::InvalidData;
	ReceivedMessageStatus El_SendQueueResult = QueueFull;

	ReceiveItemInfo raw;
	uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
	uint16_t targettimeout = (uint16_t)(allData[2] | ((allData[3] & 0x3f) << 8));
	uint32_t dataLen = allDataLen - 12;
	uint8_t *data = &allData[12];
	ReceiveType_t rt = (ReceiveType_t)(allData[3] >> 6);

	uint32_t wantedDataLen = (uint32_t)(allData[4] | allData[5] << 8 | allData[6] << 16 | allData[7] << 24);
	uint32_t wantedBufCrc = (uint32_t)(allData[8] | allData[9] << 8 | allData[10] << 16 | allData[11] << 24);
	if (wantedDataLen != dataLen)
	{
		return ReceivedMessageStatus::InvalidData;
	}
	uint32_t calBufCrc = GetBufferCrc(wantedDataLen, data);
	if (wantedBufCrc != calBufCrc)
	{
		return ReceivedMessageStatus::InvalidData;
	}

	El_Memset(&raw, 0, sizeof(ReceiveItemInfo));
	raw.UserDataOfTransportLayer = userDataOfTransportLayer;
	raw.Sid = serviceId;
	raw.DataLen = dataLen;
	raw.TargetTimeout = targettimeout;

#ifndef EmbedXrpc_UsingOs
#error "EmbedXrpc_UsingOs is not defined!"
#endif

#if EmbedXrpc_UsingOs == 0
	raw.Data = data;
	if (rt == ReceiveType_Response)
	{
		CurrentReceivedData = raw;
	}
	else if (rt == ReceiveType_Request)
	{
		ServiceExecute(this, raw, false, isIsr);
	}
	El_SendQueueResult = ReceivedMessageStatus::Ok;
#else
	if (rt == ReceiveType_Response)
	{
		// EmbedSerializationShowMessage("EmbedXrpcObject","Client ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
		if (RpcConfig.UseRingBufferWhenReceiving == true)
		{
			El_SendQueueResult = BlockRingBufferProvider_Send(BlockBufferProviderOfRequestService, &raw, data, isIsr) == True ? ReceivedMessageStatus::Ok : ReceivedMessageStatus::QueueFull;
		}
		else
		{
			TrySendDataToQueue(MessageQueueOfRequestService, dataLen, data, raw, sqr, ReceiveItemInfo, El_SendQueueResult, isIsr);
		}
	}
	else if (rt == ReceiveType_Request) // server
	{
		// EmbedSerializationShowMessage("EmbedXrpcObject","Server ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
		if (RpcConfig.UseRingBufferWhenReceiving == true)
		{
			El_SendQueueResult = BlockRingBufferProvider_Send(ServiceBlockBufferProvider, &raw, data, isIsr) == True ? ReceivedMessageStatus::Ok : ReceivedMessageStatus::QueueFull;
		}
		else if (RpcConfig.DynamicMemoryConfig.IsSendToQueue == true)
		{
			TrySendDataToQueue(ServiceMessageQueue, dataLen, data, raw, sqr, ReceiveItemInfo, El_SendQueueResult, isIsr);
		}
		else
		{
			raw.Data = data;
			ServiceExecute(this, raw, false, isIsr);
			El_SendQueueResult = ReceivedMessageStatus::Ok;
		}
	}
sqr:
#endif
	return El_SendQueueResult;
}

void EmbedXrpcObject::SuspendTimerCallBack(void *arg)
{
	EmbedXrpcObject *obj = (EmbedXrpcObject *)arg;
	/*if (obj->DataLinkBufferForSuspendTimer.MutexHandle != nullptr)
	{
		El_TakeMutex(obj->DataLinkBufferForSuspendTimer.MutexHandle, El_WAIT_FOREVER);
	}*/
	uint8_t sb[12];
	sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
	sb[1] = (uint8_t)(EmbedXrpcSuspendSid >> 8 & 0xff);
	sb[2] = (uint8_t)(obj->TimeOut & 0xff);
	sb[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
	sb[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);

	sb[4] = (uint8_t)(0);
	sb[5] = (uint8_t)(0);
	sb[6] = (uint8_t)(0);
	sb[7] = (uint8_t)(0);

	uint32_t bufcrc = GetBufferCrc(0, nullptr);
	sb[8] = (uint8_t)(bufcrc & 0xff);
	sb[9] = (uint8_t)(bufcrc >> 8 & 0xff);
	sb[10] = (uint8_t)(bufcrc >> 16 & 0xff);
	sb[11] = (uint8_t)(bufcrc >> 24 & 0xff);
	RequestParameter rp;
	rp.Udtl = &obj->UserDataOfTransportLayerOfSuspendTimerUsed;
	rp.IsIsr = 0;
	obj->Send(&rp, obj, 12, sb);
	/*if (obj->DataLinkBufferForSuspendTimer.MutexHandle != nullptr)
	{
		El_ReleaseMutex(obj->DataLinkBufferForSuspendTimer.MutexHandle);
	}*/
}

void EmbedXrpcObject::ServiceThread(void *arg)
{
	ThreadBeginHook();
	EmbedXrpcObject *obj = (EmbedXrpcObject *)arg;
	ReceiveItemInfo recData;
	bool waitResult = true;
	for (;;)
	{

		if (obj->RpcConfig.UseRingBufferWhenReceiving == true)
		{
			waitResult = BlockRingBufferProvider_Receive(obj->ServiceBlockBufferProvider, &recData, 1, 0);
		}
		else
		{
			waitResult = El_ReceiveQueue(obj->ServiceMessageQueue, &recData, sizeof(ReceiveItemInfo), 1, 0) == QueueState_OK ? true : false;
		}

		if (waitResult == true)
		{
			ServiceExecute(obj, recData, true, 0);
		}

		if (obj->DeInitFlag == true)
		{
			obj->ServiceThreadExitState = true;
			ThreadExitHook();
			return;
		}
	}
}

RequestResponseState EmbedXrpcObject::Wait(uint32_t sid, ReceiveItemInfo *recData, int IsIsr)
{
	RequestResponseState ret = ResponseState_Ok;
	// ReceiveItemInfo recData;
	bool waitResult = true;
	while (true)
	{
		if (RpcConfig.UseRingBufferWhenReceiving == true)
		{
			waitResult = BlockRingBufferProvider_Receive(BlockBufferProviderOfRequestService, recData, TimeOut, IsIsr);
		}
		else
		{
			waitResult = El_ReceiveQueue(MessageQueueOfRequestService, recData, sizeof(ReceiveItemInfo), TimeOut, IsIsr) == QueueState_OK ? true : false;
		}
		if (waitResult != true)
		{
			return ResponseState_Timeout;
		}

		if (recData->Sid == EmbedXrpcSuspendSid)
		{
			EmbedSerializationShowMessage("EmbedXrpcObject", "Client:recData.Sid == EmbedXrpcSuspendSid\n");
			continue;
		}
		else if (recData->Sid == EmbedXrpcUnsupportedSid)
		{
			EmbedSerializationShowMessage("EmbedXrpcObject", "Client:recData.Sid == EmbedXrpcUnsupportedSid\n");
			ret = ResponseState_UnsupportedSid;
		}
		else if (sid != recData->Sid)
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
