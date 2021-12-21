#ifndef Sample1_Server_H
#define Sample1_Server_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChange_DelegateSender
{
public:
EmbedXrpcObject *RpcObject=nullptr;
DateTimeChange_DelegateSender(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return DateTimeChange_ServiceId;}
DateTimeChange_Parameter SendData;
void  Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,DateTime_t now[1]);
};


class TestDelegate_DelegateSender
{
public:
EmbedXrpcObject *RpcObject=nullptr;
TestDelegate_DelegateSender(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return TestDelegate_ServiceId;}
TestDelegate_Parameter SendData;
void  Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer);};


class Inter_Add_Service:public IService
{
public:
uint16_t GetSid(){return Inter_Add_ServiceId;}
Inter_Add_Return Response;
virtual void Add(ServiceInvokeParameter * serviceInvokeParameter,Int32 a,Int32 b,Int32 dataLen,UInt8* data){}
Inter_Add_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class Inter_NoArg_Service:public IService
{
public:
uint16_t GetSid(){return Inter_NoArg_ServiceId;}
Inter_NoArg_Return Response;
virtual void NoArg(ServiceInvokeParameter * serviceInvokeParameter){}
Inter_NoArg_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class Inter_NoReturn_Service:public IService
{
public:
uint16_t GetSid(){return Inter_NoReturn_ServiceId;}
virtual void NoReturn(ServiceInvokeParameter * serviceInvokeParameter,Int32 a){}
Inter_NoReturn_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class Inter_NoArgAndReturn_Service:public IService
{
public:
uint16_t GetSid(){return Inter_NoArgAndReturn_ServiceId;}
virtual void NoArgAndReturn(ServiceInvokeParameter * serviceInvokeParameter){}
Inter_NoArgAndReturn_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


/*
The Requests Of Inter:
name                   type

"Inter_Add",           Inter_Add_Service,// U need to inherit this class and override the method!
"Inter_NoArg",           Inter_NoArg_Service,// U need to inherit this class and override the method!
"Inter_NoReturn",           Inter_NoReturn_Service,// U need to inherit this class and override the method!
"Inter_NoArgAndReturn",           Inter_NoArgAndReturn_Service,// U need to inherit this class and override the method!

RequestsCount: 4
*/


#endif
