﻿#ifndef Sample2_Client_H
#define Sample2_Client_H
#include"EmbedXrpcObject.h"
#include"Sample2.EmbedXrpcSerialization.h"
class GetSum_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
GetSum_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return GetSum_ServiceId;}
GetSum_Parameter GetSum_SendData;
GetSum_Return GetSum_reqresp;
GetSum_Return& GetSum(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a,Int32 b);
void Free_GetSum(GetSum_Return *response);

};


#endif