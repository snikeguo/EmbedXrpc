#include"Sample1.Client.h"
void DateTimeChangeClientImpl::Invoke(SerializationManager &recManager)
{
static DateTimeChange_Parameter request;
recManager.Deserialize(&DateTimeChange_Parameter_TypeInstance,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#else
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
DateTimeChange(request.now);
SerializationManager::FreeData(&DateTimeChange_Parameter_TypeInstance,&request);
}
DateTimeChangeClientImpl DateTimeChangeClientImplInstance;
Inter_Add_Return& InterClientImpl::Add(Int32 a,Int32 b,Int32 dataLen,UInt8* data)
{
//write serialization code:Add(a,b,dataLen,data,)
static Inter_Add_Parameter sendData;
SerializationManager sm;
sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;
static Inter_Add_Return reqresp;
auto result=false;
auto waitstate=ResponseState_Timeout;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
RpcObject->ResponseBlockBufferProvider->Reset();
#else
EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);
#endif
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
sendData.a=a;
sendData.b=b;
sendData.dataLen=dataLen;
sendData.data=data;
sm.Serialize(&Inter_Add_Parameter_TypeInstance,&sendData,0);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_Add_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_Add_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
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
waitstate=RpcObject->Wait(Inter_Add_ServiceId,&Inter_Add_Return_TypeInstance,&reqresp);
reqresp.State=waitstate;
exi:
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
return reqresp;
}
void InterClientImpl::Free_Add(Inter_Add_Return *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
SerializationManager::FreeData(&Inter_Add_Return_TypeInstance,response);
}
}


Inter_NoArg_Return& InterClientImpl::NoArg()
{
//write serialization code:NoArg()
static Inter_NoArg_Parameter sendData;
SerializationManager sm;
sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;
static Inter_NoArg_Return reqresp;
auto result=false;
auto waitstate=ResponseState_Timeout;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
RpcObject->ResponseBlockBufferProvider->Reset();
#else
EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);
#endif
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
sm.Serialize(&Inter_NoArg_Parameter_TypeInstance,&sendData,0);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoArg_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoArg_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
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
waitstate=RpcObject->Wait(Inter_NoArg_ServiceId,&Inter_NoArg_Return_TypeInstance,&reqresp);
reqresp.State=waitstate;
exi:
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
return reqresp;
}
void InterClientImpl::Free_NoArg(Inter_NoArg_Return *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
SerializationManager::FreeData(&Inter_NoArg_Return_TypeInstance,response);
}
}


Inter_NoReturn_Return& InterClientImpl::NoReturn(Int32 a)
{
//write serialization code:NoReturn(a,)
static Inter_NoReturn_Parameter sendData;
SerializationManager sm;
sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;
static Inter_NoReturn_Return reqresp;
auto result=false;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
RpcObject->ResponseBlockBufferProvider->Reset();
#else
EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);
#endif
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
sendData.a=a;
sm.Serialize(&Inter_NoReturn_Parameter_TypeInstance,&sendData,0);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoReturn_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoReturn_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
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
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
return reqresp;
}


Inter_NoArgAndReturn_Return& InterClientImpl::NoArgAndReturn()
{
//write serialization code:NoArgAndReturn()
static Inter_NoArgAndReturn_Parameter sendData;
SerializationManager sm;
sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;
static Inter_NoArgAndReturn_Return reqresp;
auto result=false;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
RpcObject->ResponseBlockBufferProvider->Reset();
#else
EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);
#endif
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
sm.Serialize(&Inter_NoArgAndReturn_Parameter_TypeInstance,&sendData,0);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoArgAndReturn_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoArgAndReturn_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
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
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
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
