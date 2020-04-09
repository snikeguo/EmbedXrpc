#include"StudentService.Client.h"
BroadcastDataTimeClientImpl BroadcastDataTimeClientImplInstance;
ResponseDelegateMessageMap ResponseDelegateMessages[]=
{
{"BroadcastDataTime",BroadcastDataTime_ServiceId,ReceiveType_Delegate,&BroadcastDataTimeClientImplInstance},
{"GetStudentInfoFormStudentId",GetStudentInfoFormStudentId_ServiceId,ReceiveType_Response,nullptr},
{"GetStudentsInfoFormAge",GetStudentsInfoFormAge_ServiceId,ReceiveType_Response,nullptr},
};
