#include"Sample1.Client.h"
void DateTimeChangeClientImpl::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,SerializationManager *recManager)
{
DateTimeChange_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
#endif
DateTimeChange(userDataOfTransportLayer,request.now);
DateTimeChange_Parameter_FreeData(&request);
}


void TestDelegateClientImpl::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,SerializationManager *recManager)
{
TestDelegate_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
#endif
TestDelegate(userDataOfTransportLayer,request.now);
TestDelegate_Parameter_FreeData(&request);
}


Inter_Add_Return& InterClientImpl::Add(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{//write serialization code:Add()
SerializationManager sm;
EmbedXrpc_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
auto waitstate=ResponseState_Timeout;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
BlockRingBufferProvider_Reset(RpcObject->ResponseBlockBufferProvider);
#else
EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);
#endif
SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
Inter_Add_Parameter_Serialize(&sm,&Add_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_Add_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_Add_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
SerializationManager_Reset(&sm);
if(result==false)
{
Add_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
Add_reqresp.State=RequestState_Ok;
}
ReceiveItemInfo recData;
waitstate=RpcObject->Wait(Inter_Add_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
#if  EmbedXrpc_UseRingBufferWhenReceiving==1
sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;
#else
SerializationManager_Reset(&sm);
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
#endif
#if  EmbedXrpc_CheckSumValid==1
SerializationManager_SetCalculateSum(&sm,0);
SerializationManager_SetReferenceSum(&sm,recData.CheckSum);
#endif
Inter_Add_Return_Deserialize(&sm,&Add_reqresp);
#if  EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(SerializationManager_GetReferenceSum(&sm)==SerializationManager_GetCalculateSum(&sm));
#endif
}
#if  EmbedXrpc_UseRingBufferWhenReceiving==0
if(waitstate != RequestResponseState::ResponseState_Timeout)
{
if (recData.DataLen > 0)
{
EmbedXrpc_Free(recData.Data);
}
}
#endif
Add_reqresp.State=waitstate;
exi:
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
return Add_reqresp;
}
void InterClientImpl::Free_Add(Inter_Add_Return *response)
{
if(response->State==ResponseState_Ok)
{
Inter_Add_Return_FreeData(response);
}
}




Inter_NoArg_Return& InterClientImpl::NoArg(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{
//write serialization code:NoArg()
SerializationManager sm;
EmbedXrpc_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
auto waitstate=ResponseState_Timeout;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
BlockRingBufferProvider_Reset(RpcObject->ResponseBlockBufferProvider);
#else
EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);
#endif
SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
Inter_NoArg_Parameter_Serialize(&sm,&NoArg_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoArg_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoArg_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
SerializationManager_Reset(&sm);
if(result==false)
{
NoArg_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
NoArg_reqresp.State=RequestState_Ok;
}
ReceiveItemInfo recData;
waitstate=RpcObject->Wait(Inter_NoArg_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
#if  EmbedXrpc_UseRingBufferWhenReceiving==1
sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;
#else
SerializationManager_Reset(&sm);
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
#endif
#if  EmbedXrpc_CheckSumValid==1
SerializationManager_SetCalculateSum(&sm,0);
SerializationManager_SetReferenceSum(&sm,recData.CheckSum);
#endif
Inter_NoArg_Return_Deserialize(&sm,&NoArg_reqresp);
#if  EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(SerializationManager_GetReferenceSum(&sm)==SerializationManager_GetCalculateSum(&sm));
#endif
}
#if  EmbedXrpc_UseRingBufferWhenReceiving==0
if(waitstate != RequestResponseState::ResponseState_Timeout)
{
if (recData.DataLen > 0)
{
EmbedXrpc_Free(recData.Data);
}
}
#endif
NoArg_reqresp.State=waitstate;
exi:
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
return NoArg_reqresp;
}
void InterClientImpl::Free_NoArg(Inter_NoArg_Return *response)
{
if(response->State==ResponseState_Ok)
{
Inter_NoArg_Return_FreeData(response);
}
}




Inter_NoReturn_Return& InterClientImpl::NoReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a)
{
//write serialization code:NoReturn(a,)
SerializationManager sm;
EmbedXrpc_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
BlockRingBufferProvider_Reset(RpcObject->ResponseBlockBufferProvider);
#else
EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);
#endif
SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
NoReturn_SendData.a=a;
Inter_NoReturn_Parameter_Serialize(&sm,&NoReturn_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoReturn_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoReturn_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
SerializationManager_Reset(&sm);
if(result==false)
{
NoReturn_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
NoReturn_reqresp.State=RequestState_Ok;
}
exi:
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
return NoReturn_reqresp;
}




Inter_NoArgAndReturn_Return& InterClientImpl::NoArgAndReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{
//write serialization code:NoArgAndReturn()
SerializationManager sm;
EmbedXrpc_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
BlockRingBufferProvider_Reset(RpcObject->ResponseBlockBufferProvider);
#else
EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);
#endif
SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
Inter_NoArgAndReturn_Parameter_Serialize(&sm,&NoArgAndReturn_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoArgAndReturn_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoArgAndReturn_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
SerializationManager_Reset(&sm);
if(result==false)
{
NoArgAndReturn_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
NoArgAndReturn_reqresp.State=RequestState_Ok;
}
exi:
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
return NoArgAndReturn_reqresp;
}




