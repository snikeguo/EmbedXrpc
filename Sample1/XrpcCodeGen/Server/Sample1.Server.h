#ifndef Sample1_Server_H
#define Sample1_Server_H
#include"EmbedXrpcServerObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChangeDelegate
{
public:
EmbedXrpcServerObject *RpcServerObject=nullptr;
DateTimeChangeDelegate(EmbedXrpcServerObject *rpcobj)
{
this->RpcServerObject=rpcobj;
}
uint16_t GetSid(){return DateTimeChange_ServiceId;}
void  Invoke(DateTime_t now[1]);
};
class Inter_AddService:public IService
{
public:
uint16_t GetSid(){return Inter_Add_ServiceId;}
Inter_Add_RequestResponseContent Response;
void Add(Byte a,Byte b);
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class Inter_NoArgService:public IService
{
public:
uint16_t GetSid(){return Inter_NoArg_ServiceId;}
Inter_NoArg_RequestResponseContent Response;
void NoArg();
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class Inter_NoReturnService:public IService
{
public:
uint16_t GetSid(){return Inter_NoReturn_ServiceId;}
void NoReturn();
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class Inter_NoArgAndReturnService:public IService
{
public:
uint16_t GetSid(){return Inter_NoArgAndReturn_ServiceId;}
void NoArgAndReturn();
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
#define Inter_RequestMessages_Count  4
extern RequestMessageMap Inter_RequestMessages[Inter_RequestMessages_Count];

#endif
