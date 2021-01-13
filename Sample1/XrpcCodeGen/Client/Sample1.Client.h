#ifndef Sample1_Client_H
#define Sample1_Client_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChangeClientImpl:public IDelegate
{
public:
uint16_t GetSid(){return DateTimeChange_ServiceId;}
void DateTimeChange(DateTime_t now[1]);
DateTimeChange_Parameter request;
void Invoke(SerializationManager &recManager);
};
class TestDelegateClientImpl:public IDelegate
{
public:
uint16_t GetSid(){return TestDelegate_ServiceId;}
void TestDelegate(DateTime_t now[1]);
TestDelegate_Parameter request;
void Invoke(SerializationManager &recManager);
};
class InterClientImpl
{
public:
EmbedXrpcObject *RpcObject=nullptr;
InterClientImpl(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
Inter_Add_Parameter Add_sendData;
Inter_Add_Return Add_reqresp;
Inter_Add_Return& Add();void Free_Add(Inter_Add_Return *response);

Inter_NoArg_Parameter NoArg_sendData;
Inter_NoArg_Return NoArg_reqresp;
Inter_NoArg_Return& NoArg();
void Free_NoArg(Inter_NoArg_Return *response);

Inter_NoReturn_Parameter NoReturn_sendData;
Inter_NoReturn_Return NoReturn_reqresp;
Inter_NoReturn_Return& NoReturn(Int32 a);
Inter_NoArgAndReturn_Parameter NoArgAndReturn_sendData;
Inter_NoArgAndReturn_Return NoArgAndReturn_reqresp;
Inter_NoArgAndReturn_Return& NoArgAndReturn();
};
#define Inter_ResponseDelegateMessages_Count 6
extern ResponseDelegateMessageMap Inter_ResponseDelegateMessages[Inter_ResponseDelegateMessages_Count];

#endif
