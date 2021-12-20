#ifndef Sample2_Client_H
#define Sample2_Client_H
#include"EmbedXrpcObject.h"
#include"Sample2.EmbedXrpcSerialization.h"
class Inter_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
Inter_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
Inter_GetSum_Parameter GetSum_SendData;
Inter_GetSum_Return GetSum_reqresp;
Inter_GetSum_Return& GetSum(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a,Int32 b);
void Free_GetSum(Inter_GetSum_Return *response);



};

/*
The Delegates Of Inter:
name                   type


DelegatesCount:0



The Responses Of Inter:
name                   sid

"Inter_GetSum"        Inter_GetSum_ServiceId

ResponsesCount:1
*/


#endif
