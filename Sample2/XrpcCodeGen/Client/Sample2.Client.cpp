#include"Sample2.Client.h"
GetSum_Return& GetSum_Requester::GetSum(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a,Int32 b)
{
//write serialization code:GetSum(a,b,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
auto waitstate=ResponseState_Timeout;
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService);
}

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
GetSum_SendData.a=a;
GetSum_SendData.b=b;
GetSum_Parameter_Serialize(&sm,&GetSum_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(GetSum_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(GetSum_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
GetSum_reqresp.State=RequestState_Failed;
}
else
{
GetSum_reqresp.State=RequestState_Ok;

ReceiveItemInfo recData;
waitstate=RpcObject->Wait(GetSum_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;
}
else
{
sm.Index=0;
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig.CheckSumValid==true)
{
SerializationManager_SetCalculateSum(&sm,0);
SerializationManager_SetReferenceSum(&sm,recData.CheckSum);
}
GetSum_Return_Deserialize(&sm,&GetSum_reqresp);
if(RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(&sm)==SerializationManager_GetCalculateSum(&sm));
}
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
{
if(waitstate != RequestResponseState::ResponseState_Timeout)
{
if (recData.DataLen > 0)
{
El_Free(recData.Data);
}
}
}//if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
GetSum_reqresp.State=waitstate;
}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle);
}
return GetSum_reqresp;
}
void GetSum_Requester::Free_GetSum(GetSum_Return *response)
{
if(response->State==ResponseState_Ok)
{
//!GetSum_Return_FreeData(response);
}
}

