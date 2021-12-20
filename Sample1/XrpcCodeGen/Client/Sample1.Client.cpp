#include"Sample1.Client.h"
void DateTimeChange_DelegateReceiver::Invoke(EmbedXrpcConfig *rpcConfig,UserDataOfTransportLayer_t* userDataOfTransportLayer,SerializationManager *recManager)
{
DateTimeChange_Parameter_Deserialize(recManager,&request);
if(rpcConfig->CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}//if(rpcConfig->CheckSumValid==true)
DateTimeChange(userDataOfTransportLayer,request.now);
DateTimeChange_Parameter_FreeData(&request);
}


void TestDelegate_DelegateReceiver::Invoke(EmbedXrpcConfig *rpcConfig,UserDataOfTransportLayer_t* userDataOfTransportLayer,SerializationManager *recManager)
{
TestDelegate_Parameter_Deserialize(recManager,&request);
if(rpcConfig->CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}//if(rpcConfig->CheckSumValid==true)
TestDelegate(userDataOfTransportLayer,request.now);
TestDelegate_Parameter_FreeData(&request);
}


Inter_Add_Return& Inter_Requester::Add(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{//write serialization code:Add()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
auto waitstate=ResponseState_Timeout;
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->ResponseMessageBlockBufferProvider);
}
else
{
El_ResetQueue(RpcObject->ResponseMessageQueue);
}

SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4;
Inter_Add_Parameter_Serialize(&sm,&Add_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Inter_Add_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Inter_Add_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->ResponseMessageBlockBufferProvider;
}
else
{
SerializationManager_Reset(&sm);
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig.CheckSumValid==true)
{
SerializationManager_SetCalculateSum(&sm,0);
SerializationManager_SetReferenceSum(&sm,recData.CheckSum);
}
Inter_Add_Return_Deserialize(&sm,&Add_reqresp);
if(RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(&sm)==SerializationManager_GetCalculateSum(&sm));
}
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
{
if(waitstate != RequestResponseState::ResponseState_Timeout)
{
if (recData.DataLen > 0)
{
El_Free(recData.Data);
}
}
}//if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
Add_reqresp.State=waitstate;
exi:
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return Add_reqresp;
}
void Inter_Requester::Free_Add(Inter_Add_Return *response)
{
if(response->State==ResponseState_Ok)
{
Inter_Add_Return_FreeData(response);
}
}




Inter_NoArg_Return& Inter_Requester::NoArg(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{
//write serialization code:NoArg()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
auto waitstate=ResponseState_Timeout;
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->ResponseMessageBlockBufferProvider);
}
else
{
El_ResetQueue(RpcObject->ResponseMessageQueue);
}

SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4;
Inter_NoArg_Parameter_Serialize(&sm,&NoArg_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Inter_NoArg_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Inter_NoArg_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->ResponseMessageBlockBufferProvider;
}
else
{
SerializationManager_Reset(&sm);
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig.CheckSumValid==true)
{
SerializationManager_SetCalculateSum(&sm,0);
SerializationManager_SetReferenceSum(&sm,recData.CheckSum);
}
Inter_NoArg_Return_Deserialize(&sm,&NoArg_reqresp);
if(RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(&sm)==SerializationManager_GetCalculateSum(&sm));
}
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
{
if(waitstate != RequestResponseState::ResponseState_Timeout)
{
if (recData.DataLen > 0)
{
El_Free(recData.Data);
}
}
}//if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
NoArg_reqresp.State=waitstate;
exi:
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return NoArg_reqresp;
}
void Inter_Requester::Free_NoArg(Inter_NoArg_Return *response)
{
if(response->State==ResponseState_Ok)
{
Inter_NoArg_Return_FreeData(response);
}
}




Inter_NoReturn_Return& Inter_Requester::NoReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a)
{
//write serialization code:NoReturn(a,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->ResponseMessageBlockBufferProvider);
}
else
{
El_ResetQueue(RpcObject->ResponseMessageQueue);
}

SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4;
NoReturn_SendData.a=a;
Inter_NoReturn_Parameter_Serialize(&sm,&NoReturn_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Inter_NoReturn_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Inter_NoReturn_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return NoReturn_reqresp;
}




Inter_NoArgAndReturn_Return& Inter_Requester::NoArgAndReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{
//write serialization code:NoArgAndReturn()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->ResponseMessageBlockBufferProvider);
}
else
{
El_ResetQueue(RpcObject->ResponseMessageQueue);
}

SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4;
Inter_NoArgAndReturn_Parameter_Serialize(&sm,&NoArgAndReturn_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Inter_NoArgAndReturn_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Inter_NoArgAndReturn_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return NoArgAndReturn_reqresp;
}




