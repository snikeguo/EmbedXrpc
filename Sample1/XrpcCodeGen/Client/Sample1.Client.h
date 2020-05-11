#ifndef Sample1_Client_H
#define Sample1_Client_H
#include"Sample1.h"
#include"EmbedXrpcClientObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChangeClientImpl:public IDelegate
{
public:
uint16_t GetSid(){return DateTimeChange_ServiceId;}
void DateTimeChange(DateTime_t now[1]);
void Invoke(SerializationManager &recManager);
};
class InterClientImpl
{
public:
EmbedXrpcClientObject *RpcClientObject=nullptr;
InterClientImpl(EmbedXrpcClientObject *rpcobj)
{
this->RpcClientObject=rpcobj;
}
Inter_Add_Response& Add(Int32 a,Int32 b);
void Free_Add(Inter_Add_Response *response);

};
#define Inter_ResponseDelegateMessages_Count 2
extern ResponseDelegateMessageMap Inter_ResponseDelegateMessages[Inter_ResponseDelegateMessages_Count];

#endif
