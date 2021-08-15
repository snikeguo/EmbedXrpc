#ifndef Sample1_Server_H
#define Sample1_Server_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChangeDelegate
{
public:
EmbedXrpcObject *RpcObject=nullptr;
DateTimeChangeDelegate(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return DateTimeChange_ServiceId;}
DateTimeChange_Parameter SendData;
void  Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,DateTime_t now[1]);
};


class TestDelegateDelegate
{
public:
EmbedXrpcObject *RpcObject=nullptr;
TestDelegateDelegate(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return TestDelegate_ServiceId;}
TestDelegate_Parameter SendData;
void  Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer);};


class Inter_AddService:public IService
{
public:
uint16_t GetSid(){return Inter_Add_ServiceId;}
Inter_Add_Return Response;
void Add(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,Int32 a,Int32 b,Int32 dataLen,UInt8* data);
Inter_Add_Parameter request;
void Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager &recManager, SerializationManager& sendManager);
};


class Inter_NoArgService:public IService
{
public:
uint16_t GetSid(){return Inter_NoArg_ServiceId;}
Inter_NoArg_Return Response;
void NoArg(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut);
Inter_NoArg_Parameter request;
void Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager &recManager, SerializationManager& sendManager);
};


class Inter_NoReturnService:public IService
{
public:
uint16_t GetSid(){return Inter_NoReturn_ServiceId;}
void NoReturn(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,Int32 a);
Inter_NoReturn_Parameter request;
void Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager &recManager, SerializationManager& sendManager);
};


class Inter_NoArgAndReturnService:public IService
{
public:
uint16_t GetSid(){return Inter_NoArgAndReturn_ServiceId;}
void NoArgAndReturn(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut);
Inter_NoArgAndReturn_Parameter request;
void Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager &recManager, SerializationManager& sendManager);
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
