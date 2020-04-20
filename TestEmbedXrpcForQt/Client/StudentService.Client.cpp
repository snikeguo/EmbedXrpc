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
{"IMyInterface_GetStudentInfoFormStudentId",IMyInterface_GetStudentInfoFormStudentId_ServiceId,ReceiveType_Response,nullptr},
{"IMyInterface_GetStudentsInfoFormAge",IMyInterface_GetStudentsInfoFormAge_ServiceId,ReceiveType_Response,nullptr},
{"IMyInterface_Test",IMyInterface_Test_ServiceId,ReceiveType_Response,nullptr},
};
