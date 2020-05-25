#include"Sample1.Server.h"
void  DateTimeChangeDelegate::Invoke(DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
static DateTimeChangeStruct sendData;
RpcServerObject->porter->TakeMutex(RpcServerObject->SendMutexHandle, EmbedXrpc_WAIT_FOREVER);
SerializationManager sm;
sm.Reset();
sm.Buf = &RpcServerObject->Buffer[4];
sm.BufferLen = RpcServerObject->BufferLen-4;
sendData.now[0]=now[0];
DateTimeChangeStruct_Type.Serialize(sm,0,&sendData);
RpcServerObject->Buffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
RpcServerObject->Buffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
RpcServerObject->Buffer[2]=(uint8_t)(RpcServerObject->TimeOut>>0&0xff);
RpcServerObject->Buffer[3]=(uint8_t)(RpcServerObject->TimeOut>>8&0xff);
RpcServerObject->Send(RpcServerObject,sm.Index+4,RpcServerObject->Buffer);
sm.Reset();
RpcServerObject->porter->ReleaseMutex(RpcServerObject->SendMutexHandle);
}
void Inter_AddService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_Add_Request request;
Inter_Add_Request_Type.Deserialize(recManager,&request);
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
Add(request.a,request.b);
Inter_Add_Request_Type.Free(&request);
Inter_Add_Response_Type.Serialize(sendManager,0,&Response);
Inter_Add_Response_Type.Free(&Response);
}
Inter_AddService Inter_AddServiceInstance;
void Inter_NoArgService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoArg_Request request;
Inter_NoArg_Request_Type.Deserialize(recManager,&request);
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
NoArg();
Inter_NoArg_Request_Type.Free(&request);
Inter_NoArg_Response_Type.Serialize(sendManager,0,&Response);
Inter_NoArg_Response_Type.Free(&Response);
}
Inter_NoArgService Inter_NoArgServiceInstance;
void Inter_NoReturnService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoReturn_Request request;
Inter_NoReturn_Request_Type.Deserialize(recManager,&request);
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
NoReturn();
Inter_NoReturn_Request_Type.Free(&request);
}
Inter_NoReturnService Inter_NoReturnServiceInstance;
void Inter_NoArgAndReturnService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Inter_NoArgAndReturn_Request request;
Inter_NoArgAndReturn_Request_Type.Deserialize(recManager,&request);
EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());
NoArgAndReturn();
Inter_NoArgAndReturn_Request_Type.Free(&request);
}
Inter_NoArgAndReturnService Inter_NoArgAndReturnServiceInstance;
RequestMessageMap Inter_RequestMessages[]=
{
{"Inter_Add",&Inter_AddServiceInstance},
{"Inter_NoArg",&Inter_NoArgServiceInstance},
{"Inter_NoReturn",&Inter_NoReturnServiceInstance},
{"Inter_NoArgAndReturn",&Inter_NoArgAndReturnServiceInstance},
};
