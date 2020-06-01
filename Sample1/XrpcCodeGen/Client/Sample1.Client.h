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
Inter_Add_Response& Add(Byte a,Byte b);
void Free_Add(Inter_Add_Response *response);

Inter_NoArg_Response& NoArg();
void Free_NoArg(Inter_NoArg_Response *response);

void NoReturn();
void NoArgAndReturn();
};
#define Inter_ResponseDelegateMessages_Count 3
extern ResponseDelegateMessageMap Inter_ResponseDelegateMessages[Inter_ResponseDelegateMessages_Count];

#endif
