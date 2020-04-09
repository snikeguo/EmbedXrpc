#include"StudentService.Server.h"
GetStudentInfoFormStudentIdService GetStudentInfoFormStudentIdServiceInstance;
GetStudentsInfoFormAgeService GetStudentsInfoFormAgeServiceInstance;
TestService TestServiceInstance;
RequestMessageMap RequestMessages[]=
{
{"GetStudentInfoFormStudentId",&GetStudentInfoFormStudentIdServiceInstance},
{"GetStudentsInfoFormAge",&GetStudentsInfoFormAgeServiceInstance},
{"Test",&TestServiceInstance},
};
