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
class IMyInterface_GetStudentInfoFormStudentIdService:public IService
{
public:
uint16_t GetSid(){return IMyInterface_GetStudentInfoFormStudentId_ServiceId;}
IMyInterface_GetStudentInfoFormStudentId_Response Response;
void GetStudentInfoFormStudentId(Byte StudentIdLen,Byte StudentId[100],Int32 arg2,Int32 arg3);
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class IMyInterface_GetStudentsInfoFormAgeService:public IService
{
public:
uint16_t GetSid(){return IMyInterface_GetStudentsInfoFormAge_ServiceId;}
IMyInterface_GetStudentsInfoFormAge_Response Response;
void GetStudentsInfoFormAge();
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
class IMyInterface_TestService:public IService
{
public:
uint16_t GetSid(){return IMyInterface_Test_ServiceId;}
IMyInterface_Test_Response Response;
void Test(Byte noLen[1]);
void Invoke(SerializationManager &recManager, SerializationManager& sendManager);
};
extern RequestMessageMap IMyInterface_RequestMessages[3];

#endif
