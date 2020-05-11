#ifndef Sample1_Server_H
#define Sample1_Server_H
#include"Sample1.h"
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
Inter_Add_Response Response;
void Add(Int32 a,Int32 b);
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
#define Inter_RequestMessages_Count  1
extern RequestMessageMap Inter_RequestMessages[Inter_RequestMessages_Count];

#endif
