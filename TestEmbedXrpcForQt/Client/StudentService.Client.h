#ifndef StudentService_Client_H
#define StudentService_Client_H
#include"StudentService.h"
#include"EmbedXrpcClientObject.h"
#include"StudentService.EmbedXrpcSerialization.h"
class BroadcastDataTimeClientImpl:public IDelegate
{
public:
uint16_t GetSid(){return BroadcastDataTime_ServiceId;}
void BroadcastDataTime(DateTime_t t);
void Invoke(SerializationManager &recManager);
};
class IMyInterfaceClientImpl
{
public:
EmbedXrpcClientObject *RpcClientObject=nullptr;
IMyInterfaceClientImpl(EmbedXrpcClientObject *rpcobj)
{
this->RpcClientObject=rpcobj;
}
IMyInterface_GetStudentInfoFormStudentId_Response& GetStudentInfoFormStudentId(Byte StudentIdLen,Byte StudentId[100],Int32 arg2,Int32 arg3)
{
//write serialization code:GetStudentInfoFormStudentId(StudentIdLen,StudentId,arg2,arg3,)
static IMyInterface_GetStudentInfoFormStudentId_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcClientObject->porter->ResetQueue(RpcClientObject->ResponseMessageQueueHandle);
SerializationManager sm;
sm.Reset();
sm.Buf = &RpcClientObject->Buffer[4];
sm.BufferLen = RpcClientObject->BufferLen-4;
sendData.StudentIdLen=StudentIdLen;
for(auto index=0;index<StudentIdLen;index++)
{
  sendData.StudentId[index]=StudentId[index];
}
sendData.arg2=arg2;
sendData.arg3=arg3;
IMyInterface_GetStudentInfoFormStudentId_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Buffer[0]=(uint8_t)(IMyInterface_GetStudentInfoFormStudentId_ServiceId&0xff);
RpcClientObject->Buffer[1]=(uint8_t)(IMyInterface_GetStudentInfoFormStudentId_ServiceId>>8&0xff);
RpcClientObject->Buffer[2]=(uint8_t)(RpcClientObject->TimeOut>>0&0xff);
RpcClientObject->Buffer[3]=(uint8_t)(RpcClientObject->TimeOut>>8&0xff);
RpcClientObject->Send(RpcClientObject,sm.Index+4,RpcClientObject->Buffer);
sm.Reset();
static IMyInterface_GetStudentInfoFormStudentId_Response response;
ResponseState result=RpcClientObject->Wait(IMyInterface_GetStudentInfoFormStudentId_ServiceId,&IMyInterface_GetStudentInfoFormStudentId_Response_Type,&response);
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
void Free_GetStudentInfoFormStudentId(IMyInterface_GetStudentInfoFormStudentId_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
IMyInterface_GetStudentInfoFormStudentId_Response_Type.Free(response);
}
}
IMyInterface_GetStudentsInfoFormAge_Response& GetStudentsInfoFormAge()
{
//write serialization code:GetStudentsInfoFormAge()
static IMyInterface_GetStudentsInfoFormAge_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcClientObject->porter->ResetQueue(RpcClientObject->ResponseMessageQueueHandle);
SerializationManager sm;
sm.Reset();
sm.Buf = &RpcClientObject->Buffer[4];
sm.BufferLen = RpcClientObject->BufferLen-4;
IMyInterface_GetStudentsInfoFormAge_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Buffer[0]=(uint8_t)(IMyInterface_GetStudentsInfoFormAge_ServiceId&0xff);
RpcClientObject->Buffer[1]=(uint8_t)(IMyInterface_GetStudentsInfoFormAge_ServiceId>>8&0xff);
RpcClientObject->Buffer[2]=(uint8_t)(RpcClientObject->TimeOut>>0&0xff);
RpcClientObject->Buffer[3]=(uint8_t)(RpcClientObject->TimeOut>>8&0xff);
RpcClientObject->Send(RpcClientObject,sm.Index+4,RpcClientObject->Buffer);
sm.Reset();
static IMyInterface_GetStudentsInfoFormAge_Response response;
ResponseState result=RpcClientObject->Wait(IMyInterface_GetStudentsInfoFormAge_ServiceId,&IMyInterface_GetStudentsInfoFormAge_Response_Type,&response);
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
void Free_GetStudentsInfoFormAge(IMyInterface_GetStudentsInfoFormAge_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
IMyInterface_GetStudentsInfoFormAge_Response_Type.Free(response);
}
}
IMyInterface_Test_Response& Test(Byte noLen[1])
{
//write serialization code:Test(noLen,)
static IMyInterface_Test_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcClientObject->porter->ResetQueue(RpcClientObject->ResponseMessageQueueHandle);
SerializationManager sm;
sm.Reset();
sm.Buf = &RpcClientObject->Buffer[4];
sm.BufferLen = RpcClientObject->BufferLen-4;
sendData.noLen[0]=noLen[0];
IMyInterface_Test_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Buffer[0]=(uint8_t)(IMyInterface_Test_ServiceId&0xff);
RpcClientObject->Buffer[1]=(uint8_t)(IMyInterface_Test_ServiceId>>8&0xff);
RpcClientObject->Buffer[2]=(uint8_t)(RpcClientObject->TimeOut>>0&0xff);
RpcClientObject->Buffer[3]=(uint8_t)(RpcClientObject->TimeOut>>8&0xff);
RpcClientObject->Send(RpcClientObject,sm.Index+4,RpcClientObject->Buffer);
sm.Reset();
static IMyInterface_Test_Response response;
ResponseState result=RpcClientObject->Wait(IMyInterface_Test_ServiceId,&IMyInterface_Test_Response_Type,&response);
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
void Free_Test(IMyInterface_Test_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
IMyInterface_Test_Response_Type.Free(response);
}
}
};
extern ResponseDelegateMessageMap IMyInterface_ResponseDelegateMessages[4];

#endif
