#include"Sample1.Server.h"
template<class DTL>
void  DateTimeChangeDelegate<DTL>::Invoke(DTL* userDataOfTransportLayer,DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
SendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(&sm,&SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);
RpcObject->Sender.Invoke(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
}


template<class DTL>
void  TestDelegateDelegate<DTL>::Invoke(DTL* userDataOfTransportLayer)
{//write serialization code:TestDelegate(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
TestDelegate_Parameter_Serialize(&sm,&SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(TestDelegate_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(TestDelegate_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);
RpcObject->Sender.Invoke(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
}


template<class DTL>
void Inter_AddService<DTL>::Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_Add_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->rpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}
Add(serviceInvokeParameter,request.a,request.b,request.dataLen,request.data);
Inter_Add_Parameter_FreeData(&request);
Inter_Add_Return_Serialize(sendManager,&Response);
if(this->IsFreeResponse==true) Inter_Add_Return_FreeData(&Response);
}


template<class DTL>
void Inter_NoArgService<DTL>::Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoArg_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->rpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}
NoArg(serviceInvokeParameter);
Inter_NoArg_Parameter_FreeData(&request);
Inter_NoArg_Return_Serialize(sendManager,&Response);
if(this->IsFreeResponse==true) Inter_NoArg_Return_FreeData(&Response);
}


template<class DTL>
void Inter_NoReturnService<DTL>::Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoReturn_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->rpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}
NoReturn(serviceInvokeParameter,request.a);
Inter_NoReturn_Parameter_FreeData(&request);
}


template<class DTL>
void Inter_NoArgAndReturnService<DTL>::Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoArgAndReturn_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->rpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}
NoArgAndReturn(serviceInvokeParameter);
Inter_NoArgAndReturn_Parameter_FreeData(&request);
}


