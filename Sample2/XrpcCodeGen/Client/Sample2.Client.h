#ifndef Sample2_Client_H
#define Sample2_Client_H
#include"EmbedXrpcObject.h"
#include"Sample2.EmbedXrpcSerialization.h"
template<class DTL>
class InterClientImpl
{
public:
EmbedXrpcObject<DTL> *RpcObject=nullptr;
InterClientImpl(EmbedXrpcObject<DTL> *rpcobj):RpcObject(rpcobj)
{}
Inter_GetSum_Parameter GetSum_SendData;
Inter_GetSum_Return GetSum_reqresp;
Inter_GetSum_Return& GetSum(DTL* userDataOfTransportLayer,Int32 a,Int32 b)
{
//write serialization code:GetSum(a,b,)
SerializationManager<DTL> sm;
sm.Reset();
auto result=false;
auto waitstate=ResponseState_Timeout;
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
RpcObject->ResponseBlockBufferProvider->Reset();
}
else
{
El_ResetQueue(RpcObject->ResponseBlockQueue);
}

sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;
GetSum_SendData.a=a;
GetSum_SendData.b=b;
Inter_GetSum_Parameter_Serialize(&sm,&GetSum_SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(Inter_GetSum_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(Inter_GetSum_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Sender.Invoke(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
if(result==false)
{
GetSum_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
GetSum_reqresp.State=RequestState_Ok;
}
ReceiveItemInfo<DTL> recData;
waitstate=RpcObject->Wait(Inter_GetSum_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;
}
else
{
sm.Reset();
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig->CheckSumValid==true)
{
sm.SetCalculateSum(0);
sm.SetReferenceSum(recData.CheckSum);
}
Inter_GetSum_Return_Deserialize(&sm,&GetSum_reqresp);
if(RpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(sm.GetReferenceSum()==sm.GetCalculateSum());
}
}
if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==false)
{
if(waitstate != RequestResponseState::ResponseState_Timeout)
{
if (recData.DataLen > 0)
{
El_Free(recData.Data);
}
}
}//if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==false)
GetSum_reqresp.State=waitstate;
exi:
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
return GetSum_reqresp;
}
void Free_GetSum(Inter_GetSum_Return *response)
{
if(response->State==ResponseState_Ok)
{
Inter_GetSum_Return_FreeData<DTL>(response);
}
}




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
