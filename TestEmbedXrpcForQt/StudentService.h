#ifndef StudentService_H
#define StudentService_H
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef enum _Sex_t
{
Man = 0,
Woman = 1,
HHHH = 2,
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
Byte StudentId[100];
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

#endif
