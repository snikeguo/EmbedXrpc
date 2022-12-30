#ifndef Sample1_Server_H
#define Sample1_Server_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChange_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
DateTimeChange_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return DateTimeChange_ServiceId;}
DateTimeChange_Parameter DateTimeChange_SendData;
DateTimeChange_Return DateTimeChange_reqresp;
DateTimeChange_Return& DateTimeChange(RequestParameter* rp,DateTime_t now[1]);
};

class Test2_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
Test2_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return Test2_ServiceId;}
Test2_Parameter Test2_SendData;
Test2_Return Test2_reqresp;
Test2_Return& Test2(RequestParameter* rp);};

class Add_Service:public IService
{
public:
uint16_t GetSid(){return Add_ServiceId;}
Add_Return Response;
virtual void Add(ServiceInvokeParameter * serviceInvokeParameter,Int32 a,Int32 b,Int32 dataLen,UInt8* data){}
Add_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class NoArg_Service:public IService
{
public:
uint16_t GetSid(){return NoArg_ServiceId;}
NoArg_Return Response;
virtual void NoArg(ServiceInvokeParameter * serviceInvokeParameter){}
NoArg_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class NoReturn_Service:public IService
{
public:
uint16_t GetSid(){return NoReturn_ServiceId;}
virtual void NoReturn(ServiceInvokeParameter * serviceInvokeParameter,Int32 a){}
NoReturn_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class NoArgAndReturn_Service:public IService
{
public:
uint16_t GetSid(){return NoArgAndReturn_ServiceId;}
virtual void NoArgAndReturn(ServiceInvokeParameter * serviceInvokeParameter){}
NoArgAndReturn_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};



#endif
