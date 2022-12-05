#include"Sample1.Server.h"
DateTimeChange_Return& DateTimeChange_Requester::DateTimeChange(UserDataOfTransportLayer_t* userDataOfTransportLayer,DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
osMutexAcquire(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService);
}
else
{
osMessageQueueReset(RpcObject->MessageQueueOfRequestService);
}

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
DateTimeChange_SendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(&sm,&DateTimeChange_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
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
DateTimeChange_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
DateTimeChange_reqresp.State=RequestState_Ok;
}
exi:
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
osMutexRelease(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return DateTimeChange_reqresp;
}

Test2_Return& Test2_Requester::Test2(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{//write serialization code:Test2()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
osMutexAcquire(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService);
}
else
{
osMessageQueueReset(RpcObject->MessageQueueOfRequestService);
}

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
Test2_Parameter_Serialize(&sm,&Test2_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Test2_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Test2_ServiceId>>8&0xff);
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
Test2_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
Test2_reqresp.State=RequestState_Ok;
}
exi:
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
osMutexRelease(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return Test2_reqresp;
}

void Add_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Add_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
Add(serviceInvokeParameter,request.a,request.b,request.dataLen,request.data);
Add_Parameter_FreeData(&request);
Add_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true){
//!Add_Return_FreeData(&Response);
}
}


void NoArg_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
NoArg_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoArg(serviceInvokeParameter);
//!NoArg_Parameter_FreeData(&request);
NoArg_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true){
//!NoArg_Return_FreeData(&Response);
}
}


void NoReturn_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
NoReturn_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoReturn(serviceInvokeParameter,request.a);
//!NoReturn_Parameter_FreeData(&request);
}


void NoArgAndReturn_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
NoArgAndReturn_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoArgAndReturn(serviceInvokeParameter);
//!NoArgAndReturn_Parameter_FreeData(&request);
}


