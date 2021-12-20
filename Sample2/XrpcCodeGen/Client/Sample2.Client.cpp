﻿#include"Sample2.Client.h"
Inter_GetSum_Return& Inter_Requester::GetSum(UserDataOfTransportLayer_t* userDataOfTransportLayer,Int32 a,Int32 b)
{
//write serialization code:GetSum(a,b,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
auto waitstate=ResponseState_Timeout;
if(RpcObject->RequestBuffer.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->RequestBuffer.MutexHandle, El_WAIT_FOREVER);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->ResponseBlockBufferProvider);
}
else
{
El_ResetQueue(RpcObject->ResponseBlockQueue);
}

SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->RequestBuffer.Buffer[4];
sm.BufferLen = RpcObject->RequestBuffer.BufferLen-4;
GetSum_SendData.a=a;
GetSum_SendData.b=b;
Inter_GetSum_Parameter_Serialize(&sm,&GetSum_SendData);
RpcObject->RequestBuffer.Buffer[0]=(uint8_t)(Inter_GetSum_ServiceId&0xff);
RpcObject->RequestBuffer.Buffer[1]=(uint8_t)(Inter_GetSum_ServiceId>>8&0xff);
RpcObject->RequestBuffer.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->RequestBuffer.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->RequestBuffer.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);
result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->RequestBuffer.Buffer);
SerializationManager_Reset(&sm);
if(result==false)
{
GetSum_reqresp.State=RequestState_Failed;
goto exi;
}
else
{
GetSum_reqresp.State=RequestState_Ok;
}
ReceiveItemInfo recData;
waitstate=RpcObject->Wait(Inter_GetSum_ServiceId,&recData);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;
}
else
{
SerializationManager_Reset(&sm);
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
if(RpcObject->RpcConfig.CheckSumValid==true)
{
SerializationManager_SetCalculateSum(&sm,0);
SerializationManager_SetReferenceSum(&sm,recData.CheckSum);
}
Inter_GetSum_Return_Deserialize(&sm,&GetSum_reqresp);
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
exi:
if(RpcObject->RequestBuffer.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->RequestBuffer.MutexHandle);
}
return GetSum_reqresp;
}
void Inter_Requester::Free_GetSum(Inter_GetSum_Return *response)
{
if(response->State==ResponseState_Ok)
{
Inter_GetSum_Return_FreeData(response);
}
}




