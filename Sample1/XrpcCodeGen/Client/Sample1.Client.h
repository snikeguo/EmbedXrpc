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
InterClientImpl(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
Inter_Add_Return& Add(Int32 a,Int32 b,Int32 dataLen,UInt8* data);
void Free_Add(Inter_Add_Return *response);

Inter_NoArg_Return& NoArg();
void Free_NoArg(Inter_NoArg_Return *response);

Inter_NoReturn_Return& NoReturn(Int32 a);
Inter_NoArgAndReturn_Return& NoArgAndReturn();
};
#define Inter_ResponseDelegateMessages_Count 5
extern ResponseDelegateMessageMap Inter_ResponseDelegateMessages[Inter_ResponseDelegateMessages_Count];

#endif
