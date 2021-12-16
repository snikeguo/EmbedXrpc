#include"Sample1.Client.h"
template<class DTL>
void DateTimeChangeClientImpl<DTL>::Invoke(EmbedXrpcConfig *rpcConfig,DTL* userDataOfTransportLayer,SerializationManager *recManager)
{
DateTimeChange_Parameter_Deserialize(recManager,&request);
if(rpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}//if(rpcConfig->CheckSumValid==true)
DateTimeChange(userDataOfTransportLayer,request.now);
DateTimeChange_Parameter_FreeData(&request);
}


template<class DTL>
void TestDelegateClientImpl<DTL>::Invoke(EmbedXrpcConfig *rpcConfig,DTL* userDataOfTransportLayer,SerializationManager *recManager)
{
TestDelegate_Parameter_Deserialize(recManager,&request);
if(rpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}//if(rpcConfig->CheckSumValid==true)
TestDelegate(userDataOfTransportLayer,request.now);
TestDelegate_Parameter_FreeData(&request);
}


template<class DTL>
Inter_Add_Return& InterClientImpl<DTL>::Add(DTL* userDataOfTransportLayer)
{//write serialization code:Add()
SerializationManager sm;
sm.Reset();
auto result=false;
auto waitstate=ResponseState_Timeout;
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
RpcObject->ResponseBlockBufferProvider->Reset();
}
else
{
El_ResetQueue(RpcObject->ResponseBlockQueue);
}

sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
Inter_Add_Parameter_Serialize(&sm,&Add_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_Add_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_Add_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Sender::Invoke(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
if(result==false)
{
Add_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
Add_reqresp.State=RequestState_Ok;
}
ReceiveItemInfo<DTL> recData;
waitstate=RpcObject->Wait<DTL>(Inter_Add_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;
}
else
{
sm.Reset();
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig->CheckSumValid==true)
{
sm.SetCalculateSum(0);
sm.SetReferenceSum(recData.CheckSum);
}
Inter_Add_Return_Deserialize(&sm,&Add_reqresp);
if(RpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(sm.GetReferenceSum()==sm.GetCalculateSum());
}
}
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
if(waitstate != RequestResponseState::ResponseState_Timeout)
{
if (recData.DataLen > 0)
{
El_Free(recData.Data);
}
}
}//if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
Add_reqresp.State=waitstate;
exi:
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
return Add_reqresp;
}
template<class DTL>
void InterClientImpl<DTL>::Free_Add(Inter_Add_Return *response)
{
if(response->State==ResponseState_Ok)
{
Inter_Add_Return_FreeData(response);
}
}




template<class DTL>
Inter_NoArg_Return& InterClientImpl<DTL>::NoArg(DTL* userDataOfTransportLayer)
{
//write serialization code:NoArg()
SerializationManager sm;
sm.Reset();
auto result=false;
auto waitstate=ResponseState_Timeout;
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
RpcObject->ResponseBlockBufferProvider->Reset();
}
else
{
El_ResetQueue(RpcObject->ResponseBlockQueue);
}

sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
Inter_NoArg_Parameter_Serialize(&sm,&NoArg_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoArg_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoArg_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Sender::Invoke(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
if(result==false)
{
NoArg_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
NoArg_reqresp.State=RequestState_Ok;
}
ReceiveItemInfo<DTL> recData;
waitstate=RpcObject->Wait<DTL>(Inter_NoArg_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;
}
else
{
sm.Reset();
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig->CheckSumValid==true)
{
sm.SetCalculateSum(0);
sm.SetReferenceSum(recData.CheckSum);
}
Inter_NoArg_Return_Deserialize(&sm,&NoArg_reqresp);
if(RpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(sm.GetReferenceSum()==sm.GetCalculateSum());
}
}
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
if(waitstate != RequestResponseState::ResponseState_Timeout)
{
if (recData.DataLen > 0)
{
El_Free(recData.Data);
}
}
}//if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
NoArg_reqresp.State=waitstate;
exi:
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
return NoArg_reqresp;
}
template<class DTL>
void InterClientImpl<DTL>::Free_NoArg(Inter_NoArg_Return *response)
{
if(response->State==ResponseState_Ok)
{
Inter_NoArg_Return_FreeData(response);
}
}




template<class DTL>
Inter_NoReturn_Return& InterClientImpl<DTL>::NoReturn(DTL* userDataOfTransportLayer,Int32 a)
{
//write serialization code:NoReturn(a,)
SerializationManager sm;
sm.Reset();
auto result=false;
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
RpcObject->ResponseBlockBufferProvider->Reset();
}
else
{
El_ResetQueue(RpcObject->ResponseBlockQueue);
}

sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
NoReturn_SendData.a=a;
Inter_NoReturn_Parameter_Serialize(&sm,&NoReturn_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoReturn_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoReturn_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Sender::Invoke(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
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
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
return NoReturn_reqresp;
}




template<class DTL>
Inter_NoArgAndReturn_Return& InterClientImpl<DTL>::NoArgAndReturn(DTL* userDataOfTransportLayer)
{
//write serialization code:NoArgAndReturn()
SerializationManager sm;
sm.Reset();
auto result=false;
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
RpcObject->ResponseBlockBufferProvider->Reset();
}
else
{
El_ResetQueue(RpcObject->ResponseBlockQueue);
}

sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
Inter_NoArgAndReturn_Parameter_Serialize(&sm,&NoArgAndReturn_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_NoArgAndReturn_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_NoArgAndReturn_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Sender::Invoke(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
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
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
return NoArgAndReturn_reqresp;
}




