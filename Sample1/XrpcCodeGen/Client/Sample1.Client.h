#ifndef Sample1_Client_H
#define Sample1_Client_H
#include"EmbedXrpcObject.h"
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
EmbedXrpcObject *RpcObject=nullptr;
InterClientImpl(EmbedXrpcObject *rpcobj)
{
this->RpcObject=rpcobj;
}
Inter_Add_RequestResponseContent& Add(Byte a,Byte b);
void Free_Add(Inter_Add_RequestResponseContent *response);

Inter_NoArg_RequestResponseContent& NoArg();
void Free_NoArg(Inter_NoArg_RequestResponseContent *response);

Inter_NoReturn_RequestResponseContent& NoReturn();
Inter_NoArgAndReturn_RequestResponseContent& NoArgAndReturn();
};
#define Inter_ResponseDelegateMessages_Count 5
extern ResponseDelegateMessageMap Inter_ResponseDelegateMessages[Inter_ResponseDelegateMessages_Count];

#endif
