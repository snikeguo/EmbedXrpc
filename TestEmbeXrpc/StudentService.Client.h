#ifndef StudentService_Client_H
#define StudentService_Client_H
#include"StudentService.h"
#include"EmbedXrpcClientObject.h"
#include"StudentService.EmbedXrpcSerialization.h"
class IMyInterfaceClientImpl
{
public:
EmbedXrpcClientObject *RpcClientObject=nullptr;
IMyInterfaceClientImpl(EmbedXrpcClientObject *rpcobj)
{
this->RpcClientObject=rpcobj;
}
GetValue_Response GetValue()
{
//write serialization code:GetValue()
GetValue_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->BufMutexHandle, 100);
RpcClientObject->BufManager.Reset();
GetValue_Request_Type.Serialize(RpcClientObject->BufManager,0,&sendData);
RpcClientObject->Send(GetValue_Request_ServiceId,RpcClientObject->BufManager.Index,RpcClientObject->BufManager.Buf);
RpcClientObject->BufManager.Reset();
RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);
EmbeXrpcRawData recData;
GetValue_Response response;
ResponseState result=RpcClientObject->Wait(GetValue_Request_ServiceId,&recData,&response);
if(result==ResponseState_SidError)
{
RpcClientObject->porter->Free(recData.Data);
response.State=ResponseState_SidError;
}
else if(result==ResponseState_Ok)
{
RpcClientObject->porter->Free(recData.Data);
response.State=ResponseState_Ok;
}
else if(result==ResponseState_Timeout)
{
response.State=ResponseState_Timeout;
}
return response;
}
void Free_GetValue(GetValue_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
GetValue_Response_Type.Free(response);
}
}
void SetValue(UInt16 v)
{
//write serialization code:SetValue(v,)
SetValue_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->BufMutexHandle, 100);
RpcClientObject->BufManager.Reset();
memcpy(&sendData.v,&v,sizeof(sendData.v));
SetValue_Request_Type.Serialize(RpcClientObject->BufManager,0,&sendData);
RpcClientObject->Send(SetValue_Request_ServiceId,RpcClientObject->BufManager.Index,RpcClientObject->BufManager.Buf);
RpcClientObject->BufManager.Reset();
RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);
}
};

#endif
