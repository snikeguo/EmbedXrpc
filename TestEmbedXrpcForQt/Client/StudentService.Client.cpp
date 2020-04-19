#include"StudentService.Client.h"
void BroadcastDataTimeClientImpl::Invoke(SerializationManager &recManager)
{
static BroadcastDataTimeStruct request;
BroadcastDataTimeStruct_Type.Deserialize(recManager,&request);
BroadcastDataTime(request.t);
BroadcastDataTimeStruct_Type.Free(&request);
}
BroadcastDataTimeClientImpl BroadcastDataTimeClientImplInstance;
ResponseDelegateMessageMap IMyInterface_ResponseDelegateMessages[]=
{
{"BroadcastDataTime",BroadcastDataTime_ServiceId,ReceiveType_Delegate,&BroadcastDataTimeClientImplInstance},
{"GetStudentInfoFormStudentId",GetStudentInfoFormStudentId_ServiceId,ReceiveType_Response,nullptr},
{"GetStudentsInfoFormAge",GetStudentsInfoFormAge_ServiceId,ReceiveType_Response,nullptr},
{"Test",Test_ServiceId,ReceiveType_Response,nullptr},
};
