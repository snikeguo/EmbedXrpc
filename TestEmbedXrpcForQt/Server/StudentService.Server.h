#ifndef StudentService_Server_H
#define StudentService_Server_H
#include"StudentService.h"
#include"EmbedXrpcServerObject.h"
#include"StudentService.EmbedXrpcSerialization.h"
class BroadcastDataTimeDelegate
{
public:
EmbedXrpcServerObject *RpcServerObject=nullptr;
BroadcastDataTimeDelegate(EmbedXrpcServerObject *rpcobj)
{
this->RpcServerObject=rpcobj;
}
uint16_t GetSid(){return BroadcastDataTime_ServiceId;}
void  Invoke(DateTime_t t);
};
class GetStudentInfoFormStudentIdService:public IService
{
public:
uint16_t GetSid(){return GetStudentInfoFormStudentId_ServiceId;}
GetStudentInfoFormStudentId_Response Response;
void GetStudentInfoFormStudentId(Byte StudentIdLen,Byte StudentId[100],Int32 arg2,Int32 arg3);
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class GetStudentsInfoFormAgeService:public IService
{
public:
uint16_t GetSid(){return GetStudentsInfoFormAge_ServiceId;}
GetStudentsInfoFormAge_Response Response;
void GetStudentsInfoFormAge();
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class TestService:public IService
{
public:
uint16_t GetSid(){return Test_ServiceId;}
Test_Response Response;
void Test();
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
extern RequestMessageMap IMyInterface_RequestMessages[3];

#endif
