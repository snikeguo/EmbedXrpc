#include"Sample1.Client.h"
void DateTimeChangeClientImpl::Invoke(SerializationManager &recManager)
{
static DateTimeChangeStruct request;
DateTimeChangeStruct_Type.Deserialize(recManager,&request);
DateTimeChange(request.now);
DateTimeChangeStruct_Type.Free(&request);
}
DateTimeChangeClientImpl DateTimeChangeClientImplInstance;
Inter_Add_Response& InterClientImpl::Add(Int32 a,Int32 b)
{
//write serialization code:Add(a,b,)
static Inter_Add_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcClientObject->porter->ResetQueue(RpcClientObject->ResponseMessageQueueHandle);
SerializationManager sm;
sm.Reset();
sm.Buf = &RpcClientObject->Buffer[4];
sm.BufferLen = RpcClientObject->BufferLen-4;
sendData.a=a;
sendData.b=b;
Inter_Add_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Buffer[0]=(uint8_t)(Inter_Add_ServiceId&0xff);
RpcClientObject->Buffer[1]=(uint8_t)(Inter_Add_ServiceId>>8&0xff);
RpcClientObject->Buffer[2]=(uint8_t)(RpcClientObject->TimeOut>>0&0xff);
RpcClientObject->Buffer[3]=(uint8_t)(RpcClientObject->TimeOut>>8&0xff);
RpcClientObject->Send(RpcClientObject,sm.Index+4,RpcClientObject->Buffer);
sm.Reset();
static Inter_Add_Response response;
ResponseState result=RpcClientObject->Wait(Inter_Add_ServiceId,&Inter_Add_Response_Type,&response);
if(result==ResponseState_SidError)
{
response.State=ResponseState_SidError;
}
else if(result==ResponseState_Ok)
{
response.State=ResponseState_Ok;
}
else if(result==ResponseState_Timeout)
{
response.State=ResponseState_Timeout;
}
RpcClientObject->porter->ReleaseMutex(RpcClientObject->ObjectMutexHandle);
return response;
}
void InterClientImpl::Free_Add(Inter_Add_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
Inter_Add_Response_Type.Free(response);
}
}


ResponseDelegateMessageMap Inter_ResponseDelegateMessages[]=
{
{"DateTimeChange",DateTimeChange_ServiceId,ReceiveType_Delegate,&DateTimeChangeClientImplInstance},
{"Inter_Add",Inter_Add_ServiceId,ReceiveType_Response,nullptr},
};
