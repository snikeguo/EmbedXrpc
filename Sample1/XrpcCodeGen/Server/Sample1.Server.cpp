#include"Sample1.Server.h"
void  DateTimeChangeDelegate::Invoke(DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
static DateTimeChange_Parameter sendData;
SerializationManager sm;
sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;
EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
sm.Reset();
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
sendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(sm,&sendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);
RpcObject->Send(RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
sm.Reset();
EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);
}
void Inter_AddService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_Add_Parameter request;
Inter_Add_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1 && EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#elif EmbedXrpc_UseRingBufferWhenReceiving==0 && EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
Add(request.a,request.b,request.dataLen,request.data);
Inter_Add_Parameter_FreeData(&request);
Inter_Add_Return_Serialize(sendManager,&Response);
Inter_Add_Return_FreeData(&Response);
}
Inter_AddService Inter_AddServiceInstance;
void Inter_NoArgService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoArg_Parameter request;
Inter_NoArg_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1 && EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#elif EmbedXrpc_UseRingBufferWhenReceiving==0 && EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
NoArg();
Inter_NoArg_Parameter_FreeData(&request);
Inter_NoArg_Return_Serialize(sendManager,&Response);
Inter_NoArg_Return_FreeData(&Response);
}
Inter_NoArgService Inter_NoArgServiceInstance;
void Inter_NoReturnService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoReturn_Parameter request;
Inter_NoReturn_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1 && EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#elif EmbedXrpc_UseRingBufferWhenReceiving==0 && EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
NoReturn(request.a);
Inter_NoReturn_Parameter_FreeData(&request);
}
Inter_NoReturnService Inter_NoReturnServiceInstance;
void Inter_NoArgAndReturnService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoArgAndReturn_Parameter request;
Inter_NoArgAndReturn_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1 && EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#elif EmbedXrpc_UseRingBufferWhenReceiving==0 && EmbedXrpc_CheckSumValid==1
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
NoArgAndReturn();
Inter_NoArgAndReturn_Parameter_FreeData(&request);
}
Inter_NoArgAndReturnService Inter_NoArgAndReturnServiceInstance;
RequestMessageMap Inter_RequestMessages[]=
{
{"Inter_Add",&Inter_AddServiceInstance},
{"Inter_NoArg",&Inter_NoArgServiceInstance},
{"Inter_NoReturn",&Inter_NoReturnServiceInstance},
{"Inter_NoArgAndReturn",&Inter_NoArgAndReturnServiceInstance},
};
