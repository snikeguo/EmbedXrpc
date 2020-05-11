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
Add(request.a,request.b);
Inter_Add_Request_Type.Free(&request);
Inter_Add_Response_Type.Serialize(sendManager,0,&Response);
Inter_Add_Response_Type.Free(&Response);
}
Inter_AddService Inter_AddServiceInstance;
RequestMessageMap Inter_RequestMessages[]=
{
{"Inter_Add",&Inter_AddServiceInstance},
};
