#include"StudentService.Server.h"
void  BroadcastDataTimeDelegate::Invoke(DateTime_t t)
{
//write serialization code:BroadcastDataTime(t,)
static BroadcastDataTimeStruct sendData;
RpcServerObject->porter->TakeMutex(RpcServerObject->SendMutexHandle, EmbedXrpc_WAIT_FOREVER);
SerializationManager sm;
sm.Reset();
sm.Buf = &RpcServerObject->Buffer[4];
sm.BufferLen = RpcServerObject->BufferLen-4;
sendData.t=t;
BroadcastDataTimeStruct_Type.Serialize(sm,0,&sendData);
RpcServerObject->Buffer[0]=(uint8_t)(BroadcastDataTime_ServiceId&0xff);
RpcServerObject->Buffer[1]=(uint8_t)(BroadcastDataTime_ServiceId>>8&0xff);
RpcServerObject->Buffer[2]=(uint8_t)(RpcServerObject->TimeOut>>0&0xff);
RpcServerObject->Buffer[3]=(uint8_t)(RpcServerObject->TimeOut>>8&0xff);
RpcServerObject->Send(RpcServerObject,sm.Index+4,RpcServerObject->Buffer);
sm.Reset();
RpcServerObject->porter->ReleaseMutex(RpcServerObject->SendMutexHandle);
}
void GetStudentInfoFormStudentIdService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static GetStudentInfoFormStudentId_Request request;
GetStudentInfoFormStudentId_Request_Type.Deserialize(recManager,&request);
GetStudentInfoFormStudentId(request.StudentIdLen,request.StudentId,request.arg2,request.arg3);
GetStudentInfoFormStudentId_Request_Type.Free(&request);
GetStudentInfoFormStudentId_Response_Type.Serialize(sendManager,0,&Response);
GetStudentInfoFormStudentId_Response_Type.Free(&Response);
}
GetStudentInfoFormStudentIdService GetStudentInfoFormStudentIdServiceInstance;
void GetStudentsInfoFormAgeService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static GetStudentsInfoFormAge_Request request;
GetStudentsInfoFormAge_Request_Type.Deserialize(recManager,&request);
GetStudentsInfoFormAge();
GetStudentsInfoFormAge_Request_Type.Free(&request);
GetStudentsInfoFormAge_Response_Type.Serialize(sendManager,0,&Response);
GetStudentsInfoFormAge_Response_Type.Free(&Response);
}
GetStudentsInfoFormAgeService GetStudentsInfoFormAgeServiceInstance;
void TestService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static Test_Request request;
Test_Request_Type.Deserialize(recManager,&request);
Test();
Test_Request_Type.Free(&request);
Test_Response_Type.Serialize(sendManager,0,&Response);
Test_Response_Type.Free(&Response);
}
TestService TestServiceInstance;
RequestMessageMap IMyInterface_RequestMessages[]=
{
{"GetStudentInfoFormStudentId",&GetStudentInfoFormStudentIdServiceInstance},
{"GetStudentsInfoFormAge",&GetStudentsInfoFormAgeServiceInstance},
{"Test",&TestServiceInstance},
};
