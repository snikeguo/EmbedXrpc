#include"StudentService.Client.h"
void BroadcastDataTimeClientImpl::Invoke(SerializationManager &recManager)
{
static BroadcastDataTimeStruct request;
BroadcastDataTimeStruct_Type.Deserialize(recManager,&request);
BroadcastDataTime(request.t);
BroadcastDataTimeStruct_Type.Free(&request);
}
BroadcastDataTimeClientImpl BroadcastDataTimeClientImplInstance;
IMyInterface_GetStudentInfoFormStudentId_Response& IMyInterfaceClientImpl::GetStudentInfoFormStudentId(Byte StudentIdLen,Byte StudentId[100],Int32 arg2,Int32 arg3)
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
void IMyInterfaceClientImpl::Free_GetStudentInfoFormStudentId(IMyInterface_GetStudentInfoFormStudentId_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
IMyInterface_GetStudentInfoFormStudentId_Response_Type.Free(response);
}
}


IMyInterface_GetStudentsInfoFormAge_Response& IMyInterfaceClientImpl::GetStudentsInfoFormAge()
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
void IMyInterfaceClientImpl::Free_GetStudentsInfoFormAge(IMyInterface_GetStudentsInfoFormAge_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
IMyInterface_GetStudentsInfoFormAge_Response_Type.Free(response);
}
}


IMyInterface_Test_Response& IMyInterfaceClientImpl::Test(Byte noLen[1])
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
void IMyInterfaceClientImpl::Free_Test(IMyInterface_Test_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
IMyInterface_Test_Response_Type.Free(response);
}
}


ResponseDelegateMessageMap IMyInterface_ResponseDelegateMessages[]=
{
{"BroadcastDataTime",BroadcastDataTime_ServiceId,ReceiveType_Delegate,&BroadcastDataTimeClientImplInstance},
{"IMyInterface_GetStudentInfoFormStudentId",IMyInterface_GetStudentInfoFormStudentId_ServiceId,ReceiveType_Response,nullptr},
{"IMyInterface_GetStudentsInfoFormAge",IMyInterface_GetStudentsInfoFormAge_ServiceId,ReceiveType_Response,nullptr},
{"IMyInterface_Test",IMyInterface_Test_ServiceId,ReceiveType_Response,nullptr},
};
