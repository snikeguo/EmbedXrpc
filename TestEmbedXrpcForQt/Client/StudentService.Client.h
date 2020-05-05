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
IMyInterface_GetStudentInfoFormStudentId_Response& GetStudentInfoFormStudentId(Byte StudentIdLen,Byte StudentId[100],Int32 arg2,Int32 arg3);
void Free_GetStudentInfoFormStudentId(IMyInterface_GetStudentInfoFormStudentId_Response *response);

IMyInterface_GetStudentsInfoFormAge_Response& GetStudentsInfoFormAge();
void Free_GetStudentsInfoFormAge(IMyInterface_GetStudentsInfoFormAge_Response *response);

IMyInterface_Test_Response& Test(Byte noLen[1]);
void Free_Test(IMyInterface_Test_Response *response);

};
extern ResponseDelegateMessageMap IMyInterface_ResponseDelegateMessages[4];

#endif
