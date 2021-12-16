#ifndef Sample1_Server_H
#define Sample1_Server_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
template<class DTL>
class DateTimeChange_DelegateSender
{
public:
EmbedXrpcObject<DTL> *RpcObject=nullptr;
DateTimeChange_DelegateSender(EmbedXrpcObject<DTL> *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return DateTimeChange_ServiceId;}
DateTimeChange_Parameter SendData;
void  Invoke(DTL* userDataOfTransportLayer,DateTime_t now[1])
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
};


template<class DTL>
class TestDelegate_DelegateSender
{
public:
EmbedXrpcObject<DTL> *RpcObject=nullptr;
TestDelegate_DelegateSender(EmbedXrpcObject<DTL> *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return TestDelegate_ServiceId;}
TestDelegate_Parameter SendData;
void  Invoke(DTL* userDataOfTransportLayer)
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
};


template<class DTL>
class Inter_AddService:public IService<DTL>
{
public:
uint16_t GetSid(){return Inter_Add_ServiceId;}
Inter_Add_Return Response;
virtual void Add(ServiceInvokeParameter<DTL> * serviceInvokeParameter,Int32 a,Int32 b,Int32 dataLen,UInt8* data)=0;
Inter_Add_Parameter request;
void Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
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
};


template<class DTL>
class Inter_NoArgService:public IService<DTL>
{
public:
uint16_t GetSid(){return Inter_NoArg_ServiceId;}
Inter_NoArg_Return Response;
virtual void NoArg(ServiceInvokeParameter<DTL> * serviceInvokeParameter)=0;
Inter_NoArg_Parameter request;
void Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
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
};


template<class DTL>
class Inter_NoReturnService:public IService<DTL>
{
public:
uint16_t GetSid(){return Inter_NoReturn_ServiceId;}
virtual void NoReturn(ServiceInvokeParameter<DTL> * serviceInvokeParameter,Int32 a)=0;
Inter_NoReturn_Parameter request;
void Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoReturn_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->rpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}
NoReturn(serviceInvokeParameter,request.a);
Inter_NoReturn_Parameter_FreeData(&request);
}
};


template<class DTL>
class Inter_NoArgAndReturnService:public IService<DTL>
{
public:
uint16_t GetSid(){return Inter_NoArgAndReturn_ServiceId;}
virtual void NoArgAndReturn(ServiceInvokeParameter<DTL> * serviceInvokeParameter)=0;
Inter_NoArgAndReturn_Parameter request;
void Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoArgAndReturn_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->rpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}
NoArgAndReturn(serviceInvokeParameter);
Inter_NoArgAndReturn_Parameter_FreeData(&request);
}
};


/*
The Requests Of Inter:
name                   type

"Inter_Add",           Inter_AddService
"Inter_NoArg",           Inter_NoArgService
"Inter_NoReturn",           Inter_NoReturnService
"Inter_NoArgAndReturn",           Inter_NoArgAndReturnService

RequestsCount: 4
*/


#endif
