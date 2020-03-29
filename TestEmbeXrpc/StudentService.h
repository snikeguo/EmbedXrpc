#include"EmbedXrpcCommon.h"
#include"EmbedXrpcClientObject.h"
#include"StudentService.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef enum _Sex_t
{
Man = 0,
Woman = 1,
}Sex_t;
typedef struct _Result_t
{
Int32 Value;
Byte NameLen;
Byte Name[32];
SByte Arg1;
}Result_t;
typedef struct _Student_t
{
UInt16 ResultsLen;
Result_t Results[32];
Int32 Age;
Byte* Name;
Byte StudentIdLen;
Int16 StudentId[100];
Sex_t Sex;
}Student_t;
typedef struct _DateTime_t
{
Int32 Year;
Int32 Month;
Int32 Day;
Int32 Hour;
Int32 Min;
Int32 Sec;
}DateTime_t;
typedef struct _StudentArray_t
{
Byte StudentIdLen;
Student_t Students[2];
}StudentArray_t;
typedef struct _BroadcastDataTimeStruct
{
DateTime_t t;
}BroadcastDataTimeStruct;
typedef void (*BroadcastDataTime)(DateTime_t t);
typedef struct _GetStudentInfoFormStudentId_Request
{
Byte StudentIdLen;
Byte StudentId[100];
Int32 arg2;
Int32 arg3;
}GetStudentInfoFormStudentId_Request;
typedef struct _GetStudentInfoFormStudentId_Response
{
ResponseState State;
Student_t ReturnValue;
}GetStudentInfoFormStudentId_Response;
typedef struct _GetStudentsInfoFormAge_Request
{
}GetStudentsInfoFormAge_Request;
typedef struct _GetStudentsInfoFormAge_Response
{
ResponseState State;
StudentArray_t ReturnValue;
}GetStudentsInfoFormAge_Response;
typedef struct _Test_Request
{
}Test_Request;
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
RpcClientObject->BufManager.Reset();
memcpy(&sendData.StudentIdLen,&StudentIdLen,sizeof(sendData.StudentIdLen));
memcpy(sendData.StudentId,StudentId,sizeof(sendData.StudentId)/sizeof(Byte));
memcpy(&sendData.arg2,&arg2,sizeof(sendData.arg2));
memcpy(&sendData.arg3,&arg3,sizeof(sendData.arg3));
GetStudentInfoFormStudentId_Request_Type.Serialize(RpcClientObject->BufManager,0,&sendData);
RpcClientObject->Send(GetStudentInfoFormStudentId_Request_ServiceId,RpcClientObject->BufManager.Index,RpcClientObject->BufManager.Buf);
RpcClientObject->BufManager.Reset();
RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);
EmbeXrpcRawData recData;
GetStudentInfoFormStudentId_Response response;
ResponseState result=RpcClientObject->Wait(GetStudentInfoFormStudentId_Request_ServiceId,&recData,&response);
if(result==ResponseState_SidError)
{
RpcClientObject->porter->Free(recData.Data);
response.State=ResponseState_SidError;
}
else if(result==ResponseState_Ok)
{
RpcClientObject->porter->Free(recData.Data);
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
RpcClientObject->BufManager.Reset();
GetStudentsInfoFormAge_Request_Type.Serialize(RpcClientObject->BufManager,0,&sendData);
RpcClientObject->Send(GetStudentsInfoFormAge_Request_ServiceId,RpcClientObject->BufManager.Index,RpcClientObject->BufManager.Buf);
RpcClientObject->BufManager.Reset();
RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);
EmbeXrpcRawData recData;
GetStudentsInfoFormAge_Response response;
ResponseState result=RpcClientObject->Wait(GetStudentsInfoFormAge_Request_ServiceId,&recData,&response);
if(result==ResponseState_SidError)
{
RpcClientObject->porter->Free(recData.Data);
response.State=ResponseState_SidError;
}
else if(result==ResponseState_Ok)
{
RpcClientObject->porter->Free(recData.Data);
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
RpcClientObject->BufManager.Reset();
Test_Request_Type.Serialize(RpcClientObject->BufManager,0,&sendData);
RpcClientObject->Send(Test_Request_ServiceId,RpcClientObject->BufManager.Index,RpcClientObject->BufManager.Buf);
RpcClientObject->BufManager.Reset();
RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);
}
};
