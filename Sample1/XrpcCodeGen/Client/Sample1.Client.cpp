#include"Sample1.Client.h"
void DateTimeChangeClientImpl::Invoke(SerializationManager &recManager)
{
static DateTimeChangeStruct request;
DateTimeChangeStruct_Type.Deserialize(recManager,&request);
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
DateTimeChange(request.now);
DateTimeChangeStruct_Type.Free(&request);
}
DateTimeChangeClientImpl DateTimeChangeClientImplInstance;
Inter_Add_RequestResponseContent& InterClientImpl::Add(Int32 a,Int32 b)
{
//write serialization code:Add(a,b,)
static Inter_Add_Request sendData;
SerializationManager sm;
static Inter_Add_RequestResponseContent reqresp;
auto result=false;
auto waitstate=ResponseState_Timeout;
RpcObject->porter->TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcObject->ResponseBlockBufferProvider->Reset();
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
sendData.a=a;
sendData.b=b;
Inter_Add_Request_Type.Serialize(sm,0,&sendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_Add_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_Add_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)(RpcObject->TimeOut>>8&0xff);
result=RpcObject->Send(RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
if(result==false)
{
reqresp.State=RequestState_Failed;
goto exi;
}
else
{
reqresp.State=RequestState_Ok;
}
waitstate=RpcObject->Wait(Inter_Add_ServiceId,&Inter_Add_RequestResponseContent_Type,&reqresp);
reqresp.State=waitstate;
exi:
RpcObject->porter->ReleaseMutex(RpcObject->ObjectMutexHandle);
return reqresp;
}
void InterClientImpl::Free_Add(Inter_Add_RequestResponseContent *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
Inter_Add_RequestResponseContent_Type.Free(response);
}
}


Inter_NoArg_RequestResponseContent& InterClientImpl::NoArg()
{
//write serialization code:NoArg()
static Inter_NoArg_Request sendData;
SerializationManager sm;
static Inter_NoArg_RequestResponseContent reqresp;
auto result=false;
auto waitstate=ResponseState_Timeout;
RpcObject->porter->TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcObject->ResponseBlockBufferProvider->Reset();
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
Inter_NoArg_Request_Type.Serialize(sm,0,&sendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoArg_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoArg_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)(RpcObject->TimeOut>>8&0xff);
result=RpcObject->Send(RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
if(result==false)
{
reqresp.State=RequestState_Failed;
goto exi;
}
else
{
reqresp.State=RequestState_Ok;
}
waitstate=RpcObject->Wait(Inter_NoArg_ServiceId,&Inter_NoArg_RequestResponseContent_Type,&reqresp);
reqresp.State=waitstate;
exi:
RpcObject->porter->ReleaseMutex(RpcObject->ObjectMutexHandle);
return reqresp;
}
void InterClientImpl::Free_NoArg(Inter_NoArg_RequestResponseContent *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
Inter_NoArg_RequestResponseContent_Type.Free(response);
}
}


Inter_NoReturn_RequestResponseContent& InterClientImpl::NoReturn()
{
//write serialization code:NoReturn()
static Inter_NoReturn_Request sendData;
SerializationManager sm;
static Inter_NoReturn_RequestResponseContent reqresp;
auto result=false;
RpcObject->porter->TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcObject->ResponseBlockBufferProvider->Reset();
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
Inter_NoReturn_Request_Type.Serialize(sm,0,&sendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoReturn_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoReturn_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)(RpcObject->TimeOut>>8&0xff);
result=RpcObject->Send(RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
if(result==false)
{
reqresp.State=RequestState_Failed;
goto exi;
}
else
{
reqresp.State=RequestState_Ok;
}
exi:
RpcObject->porter->ReleaseMutex(RpcObject->ObjectMutexHandle);
return reqresp;
}


Inter_NoArgAndReturn_RequestResponseContent& InterClientImpl::NoArgAndReturn()
{
//write serialization code:NoArgAndReturn()
static Inter_NoArgAndReturn_Request sendData;
SerializationManager sm;
static Inter_NoArgAndReturn_RequestResponseContent reqresp;
auto result=false;
RpcObject->porter->TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcObject->ResponseBlockBufferProvider->Reset();
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
Inter_NoArgAndReturn_Request_Type.Serialize(sm,0,&sendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoArgAndReturn_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoArgAndReturn_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)(RpcObject->TimeOut>>8&0xff);
result=RpcObject->Send(RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
if(result==false)
{
reqresp.State=RequestState_Failed;
goto exi;
}
else
{
reqresp.State=RequestState_Ok;
}
exi:
RpcObject->porter->ReleaseMutex(RpcObject->ObjectMutexHandle);
return reqresp;
}


ResponseDelegateMessageMap Inter_ResponseDelegateMessages[]=
{
{"DateTimeChange",DateTimeChange_ServiceId,ReceiveType_Delegate,&DateTimeChangeClientImplInstance},
{"Inter_Add",Inter_Add_ServiceId,ReceiveType_Response,nullptr},
{"Inter_NoArg",Inter_NoArg_ServiceId,ReceiveType_Response,nullptr},
{"Inter_NoReturn",Inter_NoReturn_ServiceId,ReceiveType_Response,nullptr},
{"Inter_NoArgAndReturn",Inter_NoArgAndReturn_ServiceId,ReceiveType_Response,nullptr},
};
