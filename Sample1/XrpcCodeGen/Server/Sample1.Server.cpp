#include"Sample1.Server.h"
void  DateTimeChange_DelegateSender::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
if(RpcObject->DataLinkBufferForDelegate.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForDelegate.MutexHandle, El_WAIT_FOREVER);
}
SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkBufferForDelegate.Buffer[4];
sm.BufferLen = RpcObject->DataLinkBufferForDelegate.BufferLen-4;
SendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(&sm,&SendData);
RpcObject->DataLinkBufferForDelegate.Buffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
RpcObject->DataLinkBufferForDelegate.Buffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForDelegate.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForDelegate.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForDelegate.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);
RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkBufferForDelegate.Buffer);
SerializationManager_Reset(&sm);
if(RpcObject->DataLinkBufferForDelegate.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForDelegate.MutexHandle);
}
}


void  TestDelegate_DelegateSender::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{//write serialization code:TestDelegate(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
if(RpcObject->DataLinkBufferForDelegate.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForDelegate.MutexHandle, El_WAIT_FOREVER);
}
SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkBufferForDelegate.Buffer[4];
sm.BufferLen = RpcObject->DataLinkBufferForDelegate.BufferLen-4;
TestDelegate_Parameter_Serialize(&sm,&SendData);
RpcObject->DataLinkBufferForDelegate.Buffer[0]=(uint8_t)(TestDelegate_ServiceId&0xff);
RpcObject->DataLinkBufferForDelegate.Buffer[1]=(uint8_t)(TestDelegate_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForDelegate.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForDelegate.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForDelegate.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);
RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkBufferForDelegate.Buffer);
SerializationManager_Reset(&sm);
if(RpcObject->DataLinkBufferForDelegate.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForDelegate.MutexHandle);
}
}


void Inter_Add_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_Add_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
Add(serviceInvokeParameter,request.a,request.b,request.dataLen,request.data);
Inter_Add_Parameter_FreeData(&request);
Inter_Add_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true) Inter_Add_Return_FreeData(&Response);
}


void Inter_NoArg_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoArg_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoArg(serviceInvokeParameter);
Inter_NoArg_Parameter_FreeData(&request);
Inter_NoArg_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true) Inter_NoArg_Return_FreeData(&Response);
}


void Inter_NoReturn_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoReturn_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoReturn(serviceInvokeParameter,request.a);
Inter_NoReturn_Parameter_FreeData(&request);
}


void Inter_NoArgAndReturn_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoArgAndReturn_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoArgAndReturn(serviceInvokeParameter);
Inter_NoArgAndReturn_Parameter_FreeData(&request);
}


