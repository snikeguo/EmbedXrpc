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
GetStudentInfoFormStudentId_Response& GetStudentInfoFormStudentId(Byte StudentIdLen,Byte StudentId[100],Int32 arg2,Int32 arg3)
{
//write serialization code:GetStudentInfoFormStudentId(StudentIdLen,StudentId,arg2,arg3,)
static GetStudentInfoFormStudentId_Request sendData;
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
GetStudentInfoFormStudentId_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Buffer[0]=(uint8_t)(GetStudentInfoFormStudentId_ServiceId&0xff);
RpcClientObject->Buffer[1]=(uint8_t)(GetStudentInfoFormStudentId_ServiceId>>8&0xff);
RpcClientObject->Buffer[2]=(uint8_t)(RpcClientObject->TimeOut>>0&0xff);
RpcClientObject->Buffer[3]=(uint8_t)(RpcClientObject->TimeOut>>8&0xff);
RpcClientObject->Send(RpcClientObject,sm.Index+4,RpcClientObject->Buffer);
sm.Reset();
static GetStudentInfoFormStudentId_Response response;
ResponseState result=RpcClientObject->Wait(GetStudentInfoFormStudentId_ServiceId,&GetStudentInfoFormStudentId_Response_Type,&response);
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
void Free_GetStudentInfoFormStudentId(GetStudentInfoFormStudentId_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
GetStudentInfoFormStudentId_Response_Type.Free(response);
}
}
GetStudentsInfoFormAge_Response& GetStudentsInfoFormAge()
{
//write serialization code:GetStudentsInfoFormAge()
static GetStudentsInfoFormAge_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcClientObject->porter->ResetQueue(RpcClientObject->ResponseMessageQueueHandle);
SerializationManager sm;
sm.Reset();
sm.Buf = &RpcClientObject->Buffer[4];
sm.BufferLen = RpcClientObject->BufferLen-4;
GetStudentsInfoFormAge_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Buffer[0]=(uint8_t)(GetStudentsInfoFormAge_ServiceId&0xff);
RpcClientObject->Buffer[1]=(uint8_t)(GetStudentsInfoFormAge_ServiceId>>8&0xff);
RpcClientObject->Buffer[2]=(uint8_t)(RpcClientObject->TimeOut>>0&0xff);
RpcClientObject->Buffer[3]=(uint8_t)(RpcClientObject->TimeOut>>8&0xff);
RpcClientObject->Send(RpcClientObject,sm.Index+4,RpcClientObject->Buffer);
sm.Reset();
static GetStudentsInfoFormAge_Response response;
ResponseState result=RpcClientObject->Wait(GetStudentsInfoFormAge_ServiceId,&GetStudentsInfoFormAge_Response_Type,&response);
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
void Free_GetStudentsInfoFormAge(GetStudentsInfoFormAge_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
GetStudentsInfoFormAge_Response_Type.Free(response);
}
}
Test_Response& Test()
{
//write serialization code:Test()
static Test_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);
RpcClientObject->porter->ResetQueue(RpcClientObject->ResponseMessageQueueHandle);
SerializationManager sm;
sm.Reset();
sm.Buf = &RpcClientObject->Buffer[4];
sm.BufferLen = RpcClientObject->BufferLen-4;
Test_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Buffer[0]=(uint8_t)(Test_ServiceId&0xff);
RpcClientObject->Buffer[1]=(uint8_t)(Test_ServiceId>>8&0xff);
RpcClientObject->Buffer[2]=(uint8_t)(RpcClientObject->TimeOut>>0&0xff);
RpcClientObject->Buffer[3]=(uint8_t)(RpcClientObject->TimeOut>>8&0xff);
RpcClientObject->Send(RpcClientObject,sm.Index+4,RpcClientObject->Buffer);
sm.Reset();
static Test_Response response;
ResponseState result=RpcClientObject->Wait(Test_ServiceId,&Test_Response_Type,&response);
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
void Free_Test(Test_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
Test_Response_Type.Free(response);
}
}
};
extern ResponseDelegateMessageMap IMyInterface_ResponseDelegateMessages[4];

#endif
