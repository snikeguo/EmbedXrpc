#include"Sample1.Client.h"
void DateTimeChangeClientImpl::Invoke(SerializationManager& recManager)
{
	static DateTimeChangeStruct request;
	DateTimeChangeStruct_Type.Deserialize(recManager, &request);
	EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum() == recManager.BlockBufferProvider->GetCalculateSum());
	DateTimeChange(request.now);
	DateTimeChangeStruct_Type.Free(&request);
}
DateTimeChangeClientImpl DateTimeChangeClientImplInstance;
Inter_Add_RequestResponseContent& InterClientImpl::Add(Byte a, Byte b)
{
	//write serialization code:Add(a,b,)
	static Inter_Add_Request sendData;
	RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
	RpcClientObject->ResponseBlockBufferProvider->Reset();
	SerializationManager sm;
	sm.Reset();
	sm.Buf = &RpcClientObject->Buffer[4];
	sm.BufferLen = RpcClientObject->BufferLen - 4;
	sendData.a = a;
	sendData.b = b;
	Inter_Add_Request_Type.Serialize(sm, 0, &sendData);
	RpcClientObject->Buffer[0] = (uint8_t)(Inter_Add_ServiceId & 0xff);
	RpcClientObject->Buffer[1] = (uint8_t)(Inter_Add_ServiceId >> 8 & 0xff);
	RpcClientObject->Buffer[2] = (uint8_t)(RpcClientObject->TimeOut >> 0 & 0xff);
	RpcClientObject->Buffer[3] = (uint8_t)(RpcClientObject->TimeOut >> 8 & 0xff);
	static Inter_Add_RequestResponseContent reqresp;
	auto result = RpcClientObject->Send(RpcClientObject, sm.Index + 4, RpcClientObject->Buffer);
	sm.Reset();
	if (result == false)
	{
		reqresp.State = RequestState_Failed;
		goto exi;
	}
	else
	{
		reqresp.State = RequestState_Ok;
	}
	auto waitstate = RpcClientObject->Wait(Inter_Add_ServiceId, &Inter_Add_RequestResponseContent_Type, &reqresp);
	reqresp.State = waitstate;
exi:
	RpcClientObject->porter->ReleaseMutex(RpcClientObject->ObjectMutexHandle);
	return reqresp;
}
void InterClientImpl::Free_Add(Inter_Add_RequestResponseContent* response)
{
	if (response->State == ResponseState_Ok || response->State == ResponseState_SidError)
	{
		Inter_Add_RequestResponseContent_Type.Free(response);
	}
}


Inter_NoArg_RequestResponseContent& InterClientImpl::NoArg()
{
	//write serialization code:NoArg()
	static Inter_NoArg_Request sendData;
	RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
	RpcClientObject->ResponseBlockBufferProvider->Reset();
	SerializationManager sm;
	sm.Reset();
	sm.Buf = &RpcClientObject->Buffer[4];
	sm.BufferLen = RpcClientObject->BufferLen - 4;
	Inter_NoArg_Request_Type.Serialize(sm, 0, &sendData);
	RpcClientObject->Buffer[0] = (uint8_t)(Inter_NoArg_ServiceId & 0xff);
	RpcClientObject->Buffer[1] = (uint8_t)(Inter_NoArg_ServiceId >> 8 & 0xff);
	RpcClientObject->Buffer[2] = (uint8_t)(RpcClientObject->TimeOut >> 0 & 0xff);
	RpcClientObject->Buffer[3] = (uint8_t)(RpcClientObject->TimeOut >> 8 & 0xff);
	static Inter_NoArg_RequestResponseContent reqresp;
	auto result = RpcClientObject->Send(RpcClientObject, sm.Index + 4, RpcClientObject->Buffer);
	sm.Reset();
	if (result == false)
	{
		reqresp.State = RequestState_Failed;
		goto exi;
	}
	else
	{
		reqresp.State = RequestState_Ok;
	}
	auto waitstate = RpcClientObject->Wait(Inter_NoArg_ServiceId, &Inter_NoArg_RequestResponseContent_Type, &reqresp);
	reqresp.State = waitstate;
exi:
	RpcClientObject->porter->ReleaseMutex(RpcClientObject->ObjectMutexHandle);
	return reqresp;
}
void InterClientImpl::Free_NoArg(Inter_NoArg_RequestResponseContent* response)
{
	if (response->State == ResponseState_Ok || response->State == ResponseState_SidError)
	{
		Inter_NoArg_RequestResponseContent_Type.Free(response);
	}
}


Inter_NoReturn_RequestResponseContent& InterClientImpl::NoReturn()
{
	//write serialization code:NoReturn()
	static Inter_NoReturn_Request sendData;
	RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
	RpcClientObject->ResponseBlockBufferProvider->Reset();
	SerializationManager sm;
	sm.Reset();
	sm.Buf = &RpcClientObject->Buffer[4];
	sm.BufferLen = RpcClientObject->BufferLen - 4;
	Inter_NoReturn_Request_Type.Serialize(sm, 0, &sendData);
	RpcClientObject->Buffer[0] = (uint8_t)(Inter_NoReturn_ServiceId & 0xff);
	RpcClientObject->Buffer[1] = (uint8_t)(Inter_NoReturn_ServiceId >> 8 & 0xff);
	RpcClientObject->Buffer[2] = (uint8_t)(RpcClientObject->TimeOut >> 0 & 0xff);
	RpcClientObject->Buffer[3] = (uint8_t)(RpcClientObject->TimeOut >> 8 & 0xff);
	static Inter_NoReturn_RequestResponseContent reqresp;
	auto result = RpcClientObject->Send(RpcClientObject, sm.Index + 4, RpcClientObject->Buffer);
	sm.Reset();
	if (result == false)
	{
		reqresp.State = RequestState_Failed;
		goto exi;
	}
	else
	{
		reqresp.State = RequestState_Ok;
	}
exi:
	RpcClientObject->porter->ReleaseMutex(RpcClientObject->ObjectMutexHandle);
	return reqresp;
}


Inter_NoArgAndReturn_RequestResponseContent& InterClientImpl::NoArgAndReturn()
{
	//write serialization code:NoArgAndReturn()
	static Inter_NoArgAndReturn_Request sendData;
	RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
	RpcClientObject->ResponseBlockBufferProvider->Reset();
	SerializationManager sm;
	sm.Reset();
	sm.Buf = &RpcClientObject->Buffer[4];
	sm.BufferLen = RpcClientObject->BufferLen - 4;
	Inter_NoArgAndReturn_Request_Type.Serialize(sm, 0, &sendData);
	RpcClientObject->Buffer[0] = (uint8_t)(Inter_NoArgAndReturn_ServiceId & 0xff);
	RpcClientObject->Buffer[1] = (uint8_t)(Inter_NoArgAndReturn_ServiceId >> 8 & 0xff);
	RpcClientObject->Buffer[2] = (uint8_t)(RpcClientObject->TimeOut >> 0 & 0xff);
	RpcClientObject->Buffer[3] = (uint8_t)(RpcClientObject->TimeOut >> 8 & 0xff);
	static Inter_NoArgAndReturn_RequestResponseContent reqresp;
	auto result = RpcClientObject->Send(RpcClientObject, sm.Index + 4, RpcClientObject->Buffer);
	sm.Reset();
	if (result == false)
	{
		reqresp.State = RequestState_Failed;
		goto exi;
	}
	else
	{
		reqresp.State = RequestState_Ok;
	}
exi:
	RpcClientObject->porter->ReleaseMutex(RpcClientObject->ObjectMutexHandle);
	return reqresp;
}


ResponseDelegateMessageMap Inter_ResponseDelegateMessages[] =
{
{"DateTimeChange",DateTimeChange_ServiceId,ReceiveType_Delegate,&DateTimeChangeClientImplInstance},
{"Inter_Add",Inter_Add_ServiceId,ReceiveType_Response,nullptr},
{"Inter_NoArg",Inter_NoArg_ServiceId,ReceiveType_Response,nullptr},
{"Inter_NoReturn",Inter_NoReturn_ServiceId,ReceiveType_Response,nullptr},
{"Inter_NoArgAndReturn",Inter_NoArgAndReturn_ServiceId,ReceiveType_Response,nullptr},
};
