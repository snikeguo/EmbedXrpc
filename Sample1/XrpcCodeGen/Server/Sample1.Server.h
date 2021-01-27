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
void  Invoke(DateTime_t now[1]);
};
class TestDelegateDelegate
{
public:
EmbedXrpcObject *RpcObject=nullptr;
TestDelegateDelegate(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return TestDelegate_ServiceId;}
TestDelegate_Parameter SendData;
void  Invoke();};
class Inter_AddService:public IService
{
public:
uint16_t GetSid(){return Inter_Add_ServiceId;}
Inter_Add_Return Response;
void Add(Int32 a,Int32 b,Int32 dataLen,UInt8* data);
Inter_Add_Parameter request;
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class Inter_NoArgService:public IService
{
public:
uint16_t GetSid(){return Inter_NoArg_ServiceId;}
Inter_NoArg_Return Response;
void NoArg();
Inter_NoArg_Parameter request;
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class Inter_NoReturnService:public IService
{
public:
uint16_t GetSid(){return Inter_NoReturn_ServiceId;}
void NoReturn(Int32 a);
Inter_NoReturn_Parameter request;
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class Inter_NoArgAndReturnService:public IService
{
public:
uint16_t GetSid(){return Inter_NoArgAndReturn_ServiceId;}
void NoArgAndReturn();
Inter_NoArgAndReturn_Parameter request;
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
#define Inter_RequestMessages_Count  4
extern RequestMessageMap Inter_RequestMessages[Inter_RequestMessages_Count];

#endif
