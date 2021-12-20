#ifndef Sample1_Client_H
#define Sample1_Client_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChange_DelegateReceiver:public IDelegate
{
public:
uint16_t GetSid(){return DateTimeChange_ServiceId;}
virtual void DateTimeChange(UserDataOfTransportLayer_t* userDataOfTransportLayer,DateTime_t now[1]){}
DateTimeChange_Parameter request;
void Invoke(EmbedXrpcConfig *rpcConfig,UserDataOfTransportLayer_t* userDataOfTransportLayer,SerializationManager *recManager);
};


class TestDelegate_DelegateReceiver:public IDelegate
{
public:
uint16_t GetSid(){return TestDelegate_ServiceId;}
virtual void TestDelegate(UserDataOfTransportLayer_t* userDataOfTransportLayer,DateTime_t now[1]){}
TestDelegate_Parameter request;
void Invoke(EmbedXrpcConfig *rpcConfig,UserDataOfTransportLayer_t* userDataOfTransportLayer,SerializationManager *recManager);
};


class Inter_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
Inter_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
Inter_Add_Parameter Add_SendData;
Inter_Add_Return Add_reqresp;
Inter_Add_Return& Add(UserDataOfTransportLayer_t* userDataOfTransportLayer);void Free_Add(Inter_Add_Return *response);



Inter_NoArg_Parameter NoArg_SendData;
Inter_NoArg_Return NoArg_reqresp;
Inter_NoArg_Return& NoArg(UserDataOfTransportLayer_t* userDataOfTransportLayer);
void Free_NoArg(Inter_NoArg_Return *response);



Inter_NoReturn_Parameter NoReturn_SendData;
Inter_NoReturn_Return NoReturn_reqresp;
Inter_NoReturn_Return& NoReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a);


Inter_NoArgAndReturn_Parameter NoArgAndReturn_SendData;
Inter_NoArgAndReturn_Return NoArgAndReturn_reqresp;
Inter_NoArgAndReturn_Return& NoArgAndReturn(UserDataOfTransportLayer_t* userDataOfTransportLayer);


};

/*
The Delegates Of Inter:
name                   type

"DateTimeChange"        DateTimeChangeClientImpl,
"TestDelegate"        TestDelegateClientImpl,

DelegatesCount:2



The Responses Of Inter:
name                   sid

"Inter_Add"        Inter_Add_ServiceId
"Inter_NoArg"        Inter_NoArg_ServiceId
"Inter_NoReturn"        Inter_NoReturn_ServiceId
"Inter_NoArgAndReturn"        Inter_NoArgAndReturn_ServiceId

ResponsesCount:4
*/


#endif
