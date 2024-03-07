
#include "EmbedXrpcObject.h"
#include "SoftCrc32.h"
void EmbedXrpcObject::Init(InitConfig *cfg)
{
	ServiceThreadExitState = false;
	DeInitFlag = false;

	El_Strncpy(Name, cfg->Name, EmbedXrpc_NameMaxLen);

	ClientUseDataLinkBuffer.Buffer = cfg->ClientUseDataLinkBufferConfig.Buffer;
	ClientUseDataLinkBuffer.BufferLen = cfg->ClientUseDataLinkBufferConfig.BufferLen;
	if (cfg->ClientUseDataLinkBufferConfig.IsUseMutex == true)
	{
		ClientUseDataLinkBuffer.MutexHandle = El_CreateMutex("ClientUseDataLinkBuffer.MutexHandle");
	}
	else
	{
		ClientUseDataLinkBuffer.MutexHandle = nullptr;
	}

	ServerUseDataLinkBuffer.Buffer = cfg->ServerUseDataLinkBufferConfig.Buffer;
	ServerUseDataLinkBuffer.BufferLen = cfg->ServerUseDataLinkBufferConfig.BufferLen;
	if (cfg->ServerUseDataLinkBufferConfig.IsUseMutex == true)
	{
		ServerUseDataLinkBuffer.MutexHandle = El_CreateMutex("ServerUseDataLinkBuffer.MutexHandle");
	}
	else
	{
		ServerUseDataLinkBuffer.MutexHandle = nullptr;
	}

	TimeOut = cfg->TimeOut;
	Send = cfg->Sender;

	UserData = cfg->UserData;

	ServiceThreadHandle = nullptr;
	SuspendTimer = nullptr;

	Services = cfg->Services;
	ServicesCount = cfg->ServicesCount;
	RpcConfig = cfg->RpcConfig;


	ClientUseRespondedData = xMessageBufferCreate(RpcConfig.ClientUseRespondedDataMaxSize);
	ServerUseRequestedData = xMessageBufferCreate(RpcConfig.ServerUseRequestedDataMaxSize);

	if (RpcConfig.IsSendToQueue == true)
	{
		ServiceThreadHandle = El_CreateThread("ServiceThread", RpcConfig.ServiceThreadPriority, ServiceThread, this, RpcConfig.ServiceThreadStackSize);
		El_ThreadStart(ServiceThreadHandle, 0);
	}
	SuspendTimer = El_CreateTimer("SuspendTimer", El_WAIT_FOREVER, SuspendTimerCallBack, this);
}
void EmbedXrpcObject::DeInit()
{
	DeInitFlag = true;
	if (ClientUseDataLinkBuffer.MutexHandle != nullptr)
	{
		El_DeleteMutex(ClientUseDataLinkBuffer.MutexHandle);
	}
	if (ServerUseDataLinkBuffer.MutexHandle != nullptr)
	{
		El_DeleteMutex(ServerUseDataLinkBuffer.MutexHandle);
	}

	vMessageBufferDelete(ClientUseRespondedData);
	vMessageBufferDelete(ServerUseRequestedData);

	if (ServiceThreadHandle != nullptr)
	{
		while (ServiceThreadExitState == false)
		{
			El_Delay(10);
		}
		El_DeleteThread(ServiceThreadHandle);
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
	bool isFindService;
	isFindService = false;
	ServiceInvokeParameter serviceInvokeParameter;
	for (uint32_t collectionIndex = 0; collectionIndex < obj->ServicesCount; collectionIndex++)
	{
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
			rsm.Buf = recData.Data;


			if (obj->ServerUseDataLinkBuffer.MutexHandle != nullptr)
			{
				El_TakeMutex(obj->ServerUseDataLinkBuffer.MutexHandle, El_WAIT_FOREVER, isIsr); // 由于使用DataLinkBufferForResponse，所以添加锁
			}

			sendsm.Buf = &obj->ServerUseDataLinkBuffer.Buffer[12];//共用同一个内存
			sendsm.BufferLen = obj->ServerUseDataLinkBuffer.BufferLen - 12;

			// EmbedXrpc_TimerReset(obj->SuspendTimer);
			// EmbedXrpc_TimerStart(obj->SuspendTimer, recData.TargetTimeout / 2);
			serviceInvokeParameter.RpcObject = obj;
			serviceInvokeParameter.TargetTimeOut = recData.TargetTimeout;
			serviceInvokeParameter.IsIsr = isIsr;
			iter->Service->Invoke(&serviceInvokeParameter, &rsm, &sendsm);
			El_TimerStop(obj->SuspendTimer, isIsr);

			if (sendsm.Index > 0) //
			{
				obj->ServerUseDataLinkBuffer.Buffer[0] = (uint8_t)(recData.Sid >> 0 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
				obj->ServerUseDataLinkBuffer.Buffer[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);

				obj->ServerUseDataLinkBuffer.Buffer[4] = (uint8_t)(sendsm.Index >> 0 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[5] = (uint8_t)(sendsm.Index >> 8 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[6] = (uint8_t)(sendsm.Index >> 16 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[7] = (uint8_t)(sendsm.Index >> 24 & 0xff);

				uint32_t bufcrc = GetBufferCrc(sendsm.Index, sendsm.Buf);
				obj->ServerUseDataLinkBuffer.Buffer[8] = (uint8_t)(bufcrc >> 0 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[9] = (uint8_t)(bufcrc >> 8 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[10] = (uint8_t)(bufcrc >> 16 & 0xff);
				obj->ServerUseDataLinkBuffer.Buffer[11] = (uint8_t)(bufcrc >> 24 & 0xff);

				RequestParameter rp;
				rp.IsIsr = isIsr;
				obj->Send(&rp, obj, sendsm.Index + 12, obj->ServerUseDataLinkBuffer.Buffer);
			}
			if (obj->ServerUseDataLinkBuffer.MutexHandle != nullptr)
			{
				El_ReleaseMutex(obj->ServerUseDataLinkBuffer.MutexHandle, isIsr);
			}
			break;
		}
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
		rp.IsIsr = isIsr;
		obj->Send(&rp, obj, 12, sb);
	}
}

void EmbedXrpcObject::NoOs_ServiceExecute(int isIsr)
{
	ReceiveItemInfo requestServiceData;
	size_t allDataLen = xMessageBufferReceive(ServerUseRequestedData, ServerUseDataLinkBuffer.Buffer, ServerUseDataLinkBuffer.BufferLen, 0);
	if (allDataLen !=0)
	{
		uint8_t* allData = ServerUseDataLinkBuffer.Buffer;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout = (uint16_t)(allData[2] | ((allData[3] & 0x3f) << 8));
		uint32_t dataLen = allDataLen - 12;
		uint8_t* data = &allData[12];
		requestServiceData.Sid = serviceId;
		requestServiceData.DataLen = dataLen;
		requestServiceData.TargetTimeout = targettimeout;
		requestServiceData.Data = data;
		ServiceExecute(this, requestServiceData, false, isIsr);
	}
}


ReceivedMessageStatus EmbedXrpcObject::ReceivedMessage(uint32_t allDataLen, uint8_t *allData,  int isIsr)
{
	if (allDataLen < 12)
		return ReceivedMessageStatus::ReceivedMessageStatus_InvalidData;
	ReceivedMessageStatus El_SendQueueResult = ReceivedMessageStatus_QueueFull;

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
		return ReceivedMessageStatus::ReceivedMessageStatus_InvalidData;
	}
	uint32_t calBufCrc = GetBufferCrc(wantedDataLen, data);
	if (wantedBufCrc != calBufCrc)
	{
		return ReceivedMessageStatus::ReceivedMessageStatus_InvalidData;
	}

	El_Memset(&raw, 0, sizeof(ReceiveItemInfo));

	raw.Sid = serviceId;
	raw.DataLen = dataLen;
	raw.TargetTimeout = targettimeout;

#ifndef EmbedXrpc_UsingOs
#error "EmbedXrpc_UsingOs is not defined!"
#endif

//sqr:
	if (rt == ReceiveType_Response)
	{
		size_t size;
		if (isIsr)
		{
			size = xMessageBufferSendFromISR(ClientUseRespondedData, allData, allDataLen, NULL);
		}
		else
		{
			size = xMessageBufferSend(ClientUseRespondedData, allData, allDataLen, 0);
		}
		if (size == allDataLen)
		{
			El_SendQueueResult = ReceivedMessageStatus::ReceivedMessageStatus_Ok;
		}
	}
	else if (rt == ReceiveType_Request) // server
	{
		// EmbedSerializationShowMessage("EmbedXrpcObject","Server ReceivedMessage  El_Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
		if (RpcConfig.IsSendToQueue == true)
		{
			size_t size;
			if (isIsr)
			{
				size=xMessageBufferSendFromISR(ServerUseRequestedData, allData, allDataLen, NULL);
			}
			else
			{
				size=xMessageBufferSend(ServerUseRequestedData, allData, allDataLen, 0);
			}
			if (size == allDataLen)
			{
				El_SendQueueResult = ReceivedMessageStatus::ReceivedMessageStatus_Ok;
			}
		}
		else
		{
			raw.Data = data;
			ServiceExecute(this, raw, false, isIsr);
			El_SendQueueResult = ReceivedMessageStatus::ReceivedMessageStatus_Ok;
		}
	}
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

		size_t allDataLen = xMessageBufferReceive(obj->ServerUseRequestedData, obj->ServerUseDataLinkBuffer.Buffer, obj->ServerUseDataLinkBuffer.BufferLen, 10);
		if (allDataLen == 0)
		{
			waitResult = false;
		}
		else
		{
			waitResult = true;
		}
		if (waitResult == true)
		{
			uint8_t* allData = obj->ServerUseDataLinkBuffer.Buffer;
			uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
			uint16_t targettimeout = (uint16_t)(allData[2] | ((allData[3] & 0x3f) << 8));
			uint32_t dataLen = allDataLen - 12;
			uint8_t* data = &allData[12];
			recData.Sid = serviceId;
			recData.DataLen = dataLen;
			recData.TargetTimeout = targettimeout;
			recData.Data = data;
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
	RequestResponseState ret = ResponseState_Timeout;
	// ReceiveItemInfo recData;
	bool waitResult = true;
	uint32_t endTick = El_GetTick(IsIsr)+ TimeOut;
	uint32_t curTick = 0;
	uint32_t maxWaitingTickOfThisTime=0;
	memset(recData, 0, sizeof(ReceiveItemInfo));
	while (true)
	{
		curTick = El_GetTick(IsIsr);
		if (endTick < curTick)
		{
			break;//时间过了跳出
		}
		else
		{
			maxWaitingTickOfThisTime = endTick - curTick;
		}
		size_t allDataLen = xMessageBufferReceive(ClientUseRespondedData, ClientUseDataLinkBuffer.Buffer, ClientUseDataLinkBuffer.BufferLen, maxWaitingTickOfThisTime);
		if (allDataLen == 0)
		{
			waitResult = false;
		}
		else
		{
			waitResult = true;
		}
		if (waitResult != true)
		{
			ret= ResponseState_Timeout;
			break;//超时跳出
		}
		uint8_t* allData = ClientUseDataLinkBuffer.Buffer;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout = (uint16_t)(allData[2] | ((allData[3] & 0x3f) << 8));
		uint32_t dataLen = allDataLen - 12;
		uint8_t* data = &allData[12];
		recData->Sid = serviceId;
		recData->DataLen = dataLen;
		recData->TargetTimeout = targettimeout;
		recData->Data = data;
		if (sid == recData->Sid)
		{
			El_Debug("EmbedXrpcObject:sid == recData.Sid\n");
			ret = ResponseState_Ok;
			break;//接受OK跳出
		}
		else if (recData->Sid == EmbedXrpcSuspendSid)
		{
			El_Debug("EmbedXrpcObject:Client:recData.Sid == EmbedXrpcSuspendSid\n");
			endTick = El_GetTick(IsIsr) + TimeOut;
		}
		else if (recData->Sid == EmbedXrpcUnsupportedSid)
		{
			El_Debug("EmbedXrpcObject:Client:recData.Sid == EmbedXrpcUnsupportedSid\n");
			ret = ResponseState_Timeout;
		}
		else
		{
			ret = ResponseState_Timeout;
		}
	}
	return ret;
}
