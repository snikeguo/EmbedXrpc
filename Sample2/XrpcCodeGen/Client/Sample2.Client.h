#ifndef Sample2_Client_H
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
GetSum_Return& GetSum(RequestParameter* rp,int32_t a,int32_t b);
void Free_GetSum(GetSum_Return *response);

//---------No Os-----------
uint32_t RequestTick;
GetSum_Return& NoOs_QueryServiceState(RequestParameter* rp);
GetSum_Return& NoOs_GetSum(RequestParameter* rp,int32_t a,int32_t b);
};


#endif
