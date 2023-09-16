#include"Sample2.Client.h"
GetSum_Return& GetSum_Requester::GetSum(RequestParameter* rp, int32_t a, int32_t b)
{
	//write serialization code:GetSum(a,b,)
	SerializationManager sm;
	El_Memset(&sm, 0, sizeof(SerializationManager));
	auto result = false;
	auto waitstate = ResponseState_Timeout;
	if (RpcObject->DataLinkBufferForRequest.MutexHandle != nullptr)
	{
		El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER, rp->IsIsr);
	}
	if (RpcObject->RpcConfig.UseRingBufferWhenReceiving == true)
	{
		BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService, rp->IsIsr);
	}
	else
	{
		El_ResetQueue(RpcObject->MessageQueueOfRequestService, rp->IsIsr);
	}

	sm.Index = 0;
	sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4 + 4 + 4];
	sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen - 4 - 4 - 4;
	GetSum_SendData.a = a;
	GetSum_SendData.b = b;
	GetSum_Parameter_Serialize(&sm, &GetSum_SendData);
	RpcObject->DataLinkBufferForRequest.Buffer[0] = (uint8_t)(GetSum_ServiceId & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[1] = (uint8_t)(GetSum_ServiceId >> 8 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[2] = (uint8_t)(RpcObject->TimeOut >> 0 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[3] = (uint8_t)((RpcObject->TimeOut >> 8 & 0xff) & 0x3FF);
	RpcObject->DataLinkBufferForRequest.Buffer[3] |= (uint8_t)((uint8_t)(ReceiveType_Request) << 6);

	RpcObject->DataLinkBufferForRequest.Buffer[4] = (uint8_t)(sm.Index & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[5] = (uint8_t)(sm.Index >> 8 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[6] = (uint8_t)(sm.Index >> 16 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[7] = (uint8_t)(sm.Index >> 24 & 0xff);

	uint32_t bufcrc = GetBufferCrc(sm.Index, sm.Buf);
	RpcObject->DataLinkBufferForRequest.Buffer[8] = (uint8_t)(bufcrc & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[9] = (uint8_t)(bufcrc >> 8 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[10] = (uint8_t)(bufcrc >> 16 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[11] = (uint8_t)(bufcrc >> 24 & 0xff);

	result = RpcObject->Send(rp, RpcObject, sm.Index + 4 + 4 + 4, RpcObject->DataLinkBufferForRequest.Buffer);
	sm.Index = 0;
	if (result == false)
	{
		GetSum_reqresp.State = RequestState_Failed;
	}
	else
	{
		GetSum_reqresp.State = RequestState_Ok;

		ReceiveItemInfo recData;
		waitstate = RpcObject->Wait(GetSum_ServiceId, &recData, rp->IsIsr);
		if (waitstate == RequestResponseState::ResponseState_Ok)
		{
			if (RpcObject->RpcConfig.UseRingBufferWhenReceiving == true)
			{
				sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;
			}
			else
			{
				sm.Index = 0;
				sm.BufferLen = recData.DataLen;
				sm.Buf = recData.Data;
			}
			GetSum_Return_Deserialize(&sm, &GetSum_reqresp, rp->IsIsr);
		}
		if (RpcObject->RpcConfig.UseRingBufferWhenReceiving == false)
		{
			if (waitstate != RequestResponseState::ResponseState_Timeout)
			{
				if (recData.DataLen > 0)
				{
					El_Free(recData.Data);
				}
			}
		}//if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
		GetSum_reqresp.State = waitstate;
	}
	if (RpcObject->DataLinkBufferForRequest.MutexHandle != nullptr)
	{
		El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, rp->IsIsr);
	}
	return GetSum_reqresp;
}
void GetSum_Requester::Free_GetSum(GetSum_Return* response)
{
	if (response->State == ResponseState_Ok)
	{
		//!GetSum_Return_FreeData(response);
	}
}

GetSum_Return& GetSum_Requester::NoOs_GetSum(RequestParameter* rp, int32_t a, int32_t b)
{
	//write serialization code:GetSum(a,b,)
	SerializationManager sm;
	El_Memset(&sm, 0, sizeof(SerializationManager));
	auto result = false;
	auto waitstate = ResponseState_Timeout;
	RequestTick = El_GetTick(rp->IsIsr);
	RpcObject->CurrentReceivedData.Sid = -1;
	RpcObject->CurrentReceivedData.DataLen = 0;
	RpcObject->CurrentReceivedData.Data = NULL;
	sm.Index = 0;
	sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4 + 4 + 4];
	sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen - 4 - 4 - 4;
	GetSum_SendData.a = a;
	GetSum_SendData.b = b;
	GetSum_Parameter_Serialize(&sm, &GetSum_SendData);
	RpcObject->DataLinkBufferForRequest.Buffer[0] = (uint8_t)(GetSum_ServiceId & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[1] = (uint8_t)(GetSum_ServiceId >> 8 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[2] = (uint8_t)(RpcObject->TimeOut >> 0 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[3] = (uint8_t)((RpcObject->TimeOut >> 8 & 0xff) & 0x3FF);
	RpcObject->DataLinkBufferForRequest.Buffer[3] |= (uint8_t)((uint8_t)(ReceiveType_Request) << 6);

	RpcObject->DataLinkBufferForRequest.Buffer[4] = (uint8_t)(sm.Index & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[5] = (uint8_t)(sm.Index >> 8 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[6] = (uint8_t)(sm.Index >> 16 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[7] = (uint8_t)(sm.Index >> 24 & 0xff);

	uint32_t bufcrc = GetBufferCrc(sm.Index, sm.Buf);
	RpcObject->DataLinkBufferForRequest.Buffer[8] = (uint8_t)(bufcrc & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[9] = (uint8_t)(bufcrc >> 8 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[10] = (uint8_t)(bufcrc >> 16 & 0xff);
	RpcObject->DataLinkBufferForRequest.Buffer[11] = (uint8_t)(bufcrc >> 24 & 0xff);

	result = RpcObject->Send(rp, RpcObject, sm.Index + 4 + 4 + 4, RpcObject->DataLinkBufferForRequest.Buffer);
	sm.Index = 0;
	if (result == false)
	{
		GetSum_reqresp.State = RequestState_Failed;
	}
	else
	{
		GetSum_reqresp.State = RequestState_Ok;

	}
	return GetSum_reqresp;
}

GetSum_Return& GetSum_Requester::NoOs_QueryServiceState(RequestParameter* rp)
{
	SerializationManager sm;
	uint32_t nowTick = El_GetTick(rp->IsIsr);
	if ((RequestTick + RpcObject->TimeOut) < nowTick) {
		GetSum_reqresp.State = RequestResponseState::ResponseState_Timeout;
		return GetSum_reqresp;
	}if (RpcObject->CurrentReceivedData.Sid == GetSum_ServiceId) {
		sm.Index = 0;
		sm.BufferLen = RpcObject->CurrentReceivedData.DataLen;
		sm.Buf = RpcObject->CurrentReceivedData.Data;
		GetSum_Return_Deserialize(&sm, &GetSum_reqresp, rp->IsIsr);
		GetSum_reqresp.State = RequestResponseState::ResponseState_Ok;
		return GetSum_reqresp;
	}
	else {
		GetSum_reqresp.State = RequestResponseState::ResponseState_SidError;
		return GetSum_reqresp;
	}
}