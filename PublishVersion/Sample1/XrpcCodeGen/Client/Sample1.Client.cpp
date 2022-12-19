#include"Sample1.Client.h"
void DateTimeChange_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
DateTimeChange_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
DateTimeChange(serviceInvokeParameter,request.now);
DateTimeChange_Parameter_FreeData(&request);
}


void Test2_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Test2_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
Test2(serviceInvokeParameter,request.now);
Test2_Parameter_FreeData(&request);
}


Add_Return& Add_Requester::Add(UserDataOfTransportLayer_t* userDataOfTransportLayer)
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
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService);
}

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
Add_Parameter_Serialize(&sm,&Add_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Add_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Add_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
Add_reqresp.State=RequestState_Failed;
}
else
{
Add_reqresp.State=RequestState_Ok;

ReceiveItemInfo recData;
waitstate=RpcObject->Wait(Add_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;
}
else
{
sm.Index=0;
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig.CheckSumValid==true)
{
SerializationManager_SetCalculateSum(&sm,0);
SerializationManager_SetReferenceSum(&sm,recData.CheckSum);
}
Add_Return_Deserialize(&sm,&Add_reqresp);
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
}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return Add_reqresp;
}
void Add_Requester::Free_Add(Add_Return *response)
{
if(response->State==ResponseState_Ok)
{
//!Add_Return_FreeData(response);
}
}

NoArg_Return& NoArg_Requester::NoArg(UserDataOfTransportLayer_t* userDataOfTransportLayer)
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
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService);
}

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoArg_Parameter_Serialize(&sm,&NoArg_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoArg_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoArg_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
NoArg_reqresp.State=RequestState_Failed;
}
else
{
NoArg_reqresp.State=RequestState_Ok;

ReceiveItemInfo recData;
waitstate=RpcObject->Wait(NoArg_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;
}
else
{
sm.Index=0;
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig.CheckSumValid==true)
{
SerializationManager_SetCalculateSum(&sm,0);
SerializationManager_SetReferenceSum(&sm,recData.CheckSum);
}
NoArg_Return_Deserialize(&sm,&NoArg_reqresp);
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
}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return NoArg_reqresp;
}
void NoArg_Requester::Free_NoArg(NoArg_Return *response)
{
if(response->State==ResponseState_Ok)
{
//!NoArg_Return_FreeData(response);
}
}

NoReturn_Return& NoReturn_Requester::NoReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a)
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
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService);
}

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoReturn_SendData.a=a;
NoReturn_Parameter_Serialize(&sm,&NoReturn_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoReturn_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoReturn_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
NoReturn_reqresp.State=RequestState_Failed;
}
else
{
NoReturn_reqresp.State=RequestState_Ok;

}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return NoReturn_reqresp;
}

NoArgAndReturn_Return& NoArgAndReturn_Requester::NoArgAndReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer)
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
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService);
}

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoArgAndReturn_Parameter_Serialize(&sm,&NoArgAndReturn_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoArgAndReturn_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoArgAndReturn_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
NoArgAndReturn_reqresp.State=RequestState_Failed;
}
else
{
NoArgAndReturn_reqresp.State=RequestState_Ok;

}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return NoArgAndReturn_reqresp;
}

