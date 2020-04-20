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
void IMyInterface_GetStudentInfoFormStudentIdService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static IMyInterface_GetStudentInfoFormStudentId_Request request;
IMyInterface_GetStudentInfoFormStudentId_Request_Type.Deserialize(recManager,&request);
GetStudentInfoFormStudentId(request.StudentIdLen,request.StudentId,request.arg2,request.arg3);
IMyInterface_GetStudentInfoFormStudentId_Request_Type.Free(&request);
IMyInterface_GetStudentInfoFormStudentId_Response_Type.Serialize(sendManager,0,&Response);
IMyInterface_GetStudentInfoFormStudentId_Response_Type.Free(&Response);
}
IMyInterface_GetStudentInfoFormStudentIdService IMyInterface_GetStudentInfoFormStudentIdServiceInstance;
void IMyInterface_GetStudentsInfoFormAgeService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static IMyInterface_GetStudentsInfoFormAge_Request request;
IMyInterface_GetStudentsInfoFormAge_Request_Type.Deserialize(recManager,&request);
GetStudentsInfoFormAge();
IMyInterface_GetStudentsInfoFormAge_Request_Type.Free(&request);
IMyInterface_GetStudentsInfoFormAge_Response_Type.Serialize(sendManager,0,&Response);
IMyInterface_GetStudentsInfoFormAge_Response_Type.Free(&Response);
}
IMyInterface_GetStudentsInfoFormAgeService IMyInterface_GetStudentsInfoFormAgeServiceInstance;
void IMyInterface_TestService::Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
static IMyInterface_Test_Request request;
IMyInterface_Test_Request_Type.Deserialize(recManager,&request);
Test(request.noLen);
IMyInterface_Test_Request_Type.Free(&request);
IMyInterface_Test_Response_Type.Serialize(sendManager,0,&Response);
IMyInterface_Test_Response_Type.Free(&Response);
}
IMyInterface_TestService IMyInterface_TestServiceInstance;
RequestMessageMap IMyInterface_RequestMessages[]=
{
{"IMyInterface_GetStudentInfoFormStudentId",&IMyInterface_GetStudentInfoFormStudentIdServiceInstance},
{"IMyInterface_GetStudentsInfoFormAge",&IMyInterface_GetStudentsInfoFormAgeServiceInstance},
{"IMyInterface_Test",&IMyInterface_TestServiceInstance},
};
