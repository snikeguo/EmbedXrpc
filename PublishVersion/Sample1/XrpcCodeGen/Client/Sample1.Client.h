#ifndef Sample1_Client_H
#define Sample1_Client_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChange_Service:public IService
{
public:
uint16_t GetSid(){return DateTimeChange_ServiceId;}
virtual void DateTimeChange(ServiceInvokeParameter * serviceInvokeParameter,DateTime_t now[1]){}
DateTimeChange_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class Test2_Service:public IService
{
public:
uint16_t GetSid(){return Test2_ServiceId;}
virtual void Test2(ServiceInvokeParameter * serviceInvokeParameter,DateTime_t now[1]){}
Test2_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class Add_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
Add_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return Add_ServiceId;}
Add_Parameter Add_SendData;
Add_Return Add_reqresp;
Add_Return& Add(UserDataOfTransportLayer_t* userDataOfTransportLayer);void Free_Add(Add_Return *response);

};

class NoArg_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
NoArg_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return NoArg_ServiceId;}
NoArg_Parameter NoArg_SendData;
NoArg_Return NoArg_reqresp;
NoArg_Return& NoArg(UserDataOfTransportLayer_t* userDataOfTransportLayer);
void Free_NoArg(NoArg_Return *response);

};

class NoReturn_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
NoReturn_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return NoReturn_ServiceId;}
NoReturn_Parameter NoReturn_SendData;
NoReturn_Return NoReturn_reqresp;
NoReturn_Return& NoReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a);
};

class NoArgAndReturn_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
NoArgAndReturn_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return NoArgAndReturn_ServiceId;}
NoArgAndReturn_Parameter NoArgAndReturn_SendData;
NoArgAndReturn_Return NoArgAndReturn_reqresp;
NoArgAndReturn_Return& NoArgAndReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer);
};


#endif
