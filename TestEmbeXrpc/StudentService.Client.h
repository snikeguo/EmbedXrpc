#ifndef StudentService_Client_H
#define StudentService_Client_H
#include"StudentService.h"
#include"EmbedXrpcClientObject.h"
#include"StudentService.EmbedXrpcSerialization.h"
class BroadcastDataTimeClientImpl:public IDelegate
{
public:
uint32_t Sid=0;
void BroadcastDataTime(DateTime_t t);
void Invoke(SerializationManager &recManager)
{
BroadcastDataTimeStruct request;
BroadcastDataTimeStruct_Type.Deserialize(recManager,&request);
BroadcastDataTime(request.t);
BroadcastDataTimeStruct_Type.Free(&request);
}
};
class IMyInterfaceClientImpl
{
public:
EmbedXrpcClientObject *RpcClientObject=nullptr;
IMyInterfaceClientImpl(EmbedXrpcClientObject *rpcobj)
{
this->RpcClientObject=rpcobj;
}
GetStudentInfoFormStudentId_Response GetStudentInfoFormStudentId(Byte StudentIdLen,Byte StudentId[100],Int32 arg2,Int32 arg3)
{
//write serialization code:GetStudentInfoFormStudentId(StudentIdLen,StudentId,arg2,arg3,)
GetStudentInfoFormStudentId_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->BufMutexHandle, 100);
SerializationManager sm;
sm.Reset();
sm.Buf = RpcClientObject->Buffer;
sm.BufferLen = RpcClientObject->BufferLen;
memcpy(&sendData.StudentIdLen,&StudentIdLen,sizeof(sendData.StudentIdLen));
memcpy(sendData.StudentId,StudentId,sizeof(sendData.StudentId)/sizeof(Byte));
memcpy(&sendData.arg2,&arg2,sizeof(sendData.arg2));
memcpy(&sendData.arg3,&arg3,sizeof(sendData.arg3));
GetStudentInfoFormStudentId_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Send(GetStudentInfoFormStudentId_ServiceId,sm.Index,sm.Buf);
sm.Reset();
RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);
EmbeXrpcRawData recData;
GetStudentInfoFormStudentId_Response response;
ResponseState result=RpcClientObject->Wait(GetStudentInfoFormStudentId_ServiceId,&recData,&response);
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
return response;
}
void Free_GetStudentInfoFormStudentId(GetStudentInfoFormStudentId_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
GetStudentInfoFormStudentId_Response_Type.Free(response);
}
}
GetStudentsInfoFormAge_Response GetStudentsInfoFormAge()
{
//write serialization code:GetStudentsInfoFormAge()
GetStudentsInfoFormAge_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->BufMutexHandle, 100);
SerializationManager sm;
sm.Reset();
sm.Buf = RpcClientObject->Buffer;
sm.BufferLen = RpcClientObject->BufferLen;
GetStudentsInfoFormAge_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Send(GetStudentsInfoFormAge_ServiceId,sm.Index,sm.Buf);
sm.Reset();
RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);
EmbeXrpcRawData recData;
GetStudentsInfoFormAge_Response response;
ResponseState result=RpcClientObject->Wait(GetStudentsInfoFormAge_ServiceId,&recData,&response);
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
return response;
}
void Free_GetStudentsInfoFormAge(GetStudentsInfoFormAge_Response *response)
{
if(response->State==ResponseState_Ok||response->State==ResponseState_SidError)
{
GetStudentsInfoFormAge_Response_Type.Free(response);
}
}
void Test()
{
//write serialization code:Test()
Test_Request sendData;
RpcClientObject->porter->TakeMutex(RpcClientObject->BufMutexHandle, 100);
SerializationManager sm;
sm.Reset();
sm.Buf = RpcClientObject->Buffer;
sm.BufferLen = RpcClientObject->BufferLen;
Test_Request_Type.Serialize(sm,0,&sendData);
RpcClientObject->Send(Test_ServiceId,sm.Index,sm.Buf);
sm.Reset();
RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);
}
};

#endif
