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
sm.Serialize(&DateTimeChange_Parameter_Object_Type,&sendData,0);
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
recManager.Deserialize(&Inter_Add_Parameter_Object_Type,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#else
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
Add(request.a,request.b,request.dataLen,request.data);
SerializationManager::FreeData(&Inter_Add_Parameter_Object_Type,&request);
sendManager.Serialize(&Inter_Add_Return_Object_Type,&Response,0);
SerializationManager::FreeData(&Inter_Add_Return_Object_Type,&Response);
}
Inter_AddService Inter_AddServiceInstance;
void Inter_NoArgService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoArg_Parameter request;
recManager.Deserialize(&Inter_NoArg_Parameter_Object_Type,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#else
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
NoArg();
SerializationManager::FreeData(&Inter_NoArg_Parameter_Object_Type,&request);
sendManager.Serialize(&Inter_NoArg_Return_Object_Type,&Response,0);
SerializationManager::FreeData(&Inter_NoArg_Return_Object_Type,&Response);
}
Inter_NoArgService Inter_NoArgServiceInstance;
void Inter_NoReturnService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoReturn_Parameter request;
recManager.Deserialize(&Inter_NoReturn_Parameter_Object_Type,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#else
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
NoReturn(request.a);
SerializationManager::FreeData(&Inter_NoReturn_Parameter_Object_Type,&request);
}
Inter_NoReturnService Inter_NoReturnServiceInstance;
void Inter_NoArgAndReturnService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoArgAndReturn_Parameter request;
recManager.Deserialize(&Inter_NoArgAndReturn_Parameter_Object_Type,&request);
#if EmbedXrpc_UseRingBufferWhenReceiving==1
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
#else
EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);
#endif
NoArgAndReturn();
SerializationManager::FreeData(&Inter_NoArgAndReturn_Parameter_Object_Type,&request);
}
Inter_NoArgAndReturnService Inter_NoArgAndReturnServiceInstance;
RequestMessageMap Inter_RequestMessages[]=
{
{"Inter_Add",&Inter_AddServiceInstance},
{"Inter_NoArg",&Inter_NoArgServiceInstance},
{"Inter_NoReturn",&Inter_NoReturnServiceInstance},
{"Inter_NoArgAndReturn",&Inter_NoArgAndReturnServiceInstance},
};
