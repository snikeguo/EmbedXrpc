#include"StudentService.h"
#include"StudentService.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

Int32Field Result_t_Field_Value("Result_t.Value",offsetof(Result_t,Value));
Uint8Field Result_t_Field_NameLen("Result_t.NameLen",offsetof(Result_t,NameLen));
ArrayField Result_t_Field_Name("Result_t.Name",true,&Uint8TypeInstance,sizeof(Byte),offsetof(Result_t,Name),&Result_t_Field_NameLen);
Int8Field Result_t_Field_Arg1("Result_t.Arg1",offsetof(Result_t,Arg1));
IField* Result_tDesc []=
{
&Result_t_Field_Value,
&Result_t_Field_NameLen,
&Result_t_Field_Name,
&Result_t_Field_Arg1,
};
ObjectType Result_t_Type(sizeof(Result_tDesc)/sizeof(IField*),Result_tDesc);

Uint16Field Student_t_Field_ResultsLen("Student_t.ResultsLen",offsetof(Student_t,ResultsLen));
ArrayField Student_t_Field_Results("Student_t.Results",true,&Result_t_Type,sizeof(Result_t),offsetof(Student_t,Results),&Student_t_Field_ResultsLen);
Int32Field Student_t_Field_Age("Student_t.Age",offsetof(Student_t,Age));
ArrayField Student_t_Field_Name("Student_t.Name",false,&Uint8TypeInstance,sizeof(Byte),offsetof(Student_t,Name),nullptr);
Uint8Field Student_t_Field_StudentIdLen("Student_t.StudentIdLen",offsetof(Student_t,StudentIdLen));
ArrayField Student_t_Field_StudentId("Student_t.StudentId",true,&Int16TypeInstance,sizeof(Int16),offsetof(Student_t,StudentId),&Student_t_Field_StudentIdLen);
Uint8Field Student_t_Field_Sex("Student_t.Sex",offsetof(Student_t,Sex));
IField* Student_tDesc []=
{
&Student_t_Field_ResultsLen,
&Student_t_Field_Results,
&Student_t_Field_Age,
&Student_t_Field_Name,
&Student_t_Field_StudentIdLen,
&Student_t_Field_StudentId,
&Student_t_Field_Sex,
};
ObjectType Student_t_Type(sizeof(Student_tDesc)/sizeof(IField*),Student_tDesc);

Int32Field DateTime_t_Field_Year("DateTime_t.Year",offsetof(DateTime_t,Year));
Int32Field DateTime_t_Field_Month("DateTime_t.Month",offsetof(DateTime_t,Month));
Int32Field DateTime_t_Field_Day("DateTime_t.Day",offsetof(DateTime_t,Day));
Int32Field DateTime_t_Field_Hour("DateTime_t.Hour",offsetof(DateTime_t,Hour));
Int32Field DateTime_t_Field_Min("DateTime_t.Min",offsetof(DateTime_t,Min));
Int32Field DateTime_t_Field_Sec("DateTime_t.Sec",offsetof(DateTime_t,Sec));
IField* DateTime_tDesc []=
{
&DateTime_t_Field_Year,
&DateTime_t_Field_Month,
&DateTime_t_Field_Day,
&DateTime_t_Field_Hour,
&DateTime_t_Field_Min,
&DateTime_t_Field_Sec,
};
ObjectType DateTime_t_Type(sizeof(DateTime_tDesc)/sizeof(IField*),DateTime_tDesc);

Uint8Field StudentArray_t_Field_StudentIdLen("StudentArray_t.StudentIdLen",offsetof(StudentArray_t,StudentIdLen));
ArrayField StudentArray_t_Field_Students("StudentArray_t.Students",true,&Student_t_Type,sizeof(Student_t),offsetof(StudentArray_t,Students),&StudentArray_t_Field_StudentIdLen);
IField* StudentArray_tDesc []=
{
&StudentArray_t_Field_StudentIdLen,
&StudentArray_t_Field_Students,
};
ObjectType StudentArray_t_Type(sizeof(StudentArray_tDesc)/sizeof(IField*),StudentArray_tDesc);

ObjectField BroadcastDataTimeStruct_Field_t("BroadcastDataTimeStruct.t",sizeof(DateTime_tDesc)/sizeof(IField*),DateTime_tDesc,offsetof(BroadcastDataTimeStruct,t));
IField* BroadcastDataTimeStructDesc []=
{
&BroadcastDataTimeStruct_Field_t,
};
ObjectType BroadcastDataTimeStruct_Type(sizeof(BroadcastDataTimeStructDesc)/sizeof(IField*),BroadcastDataTimeStructDesc);

Uint8Field GetStudentInfoFormStudentId_Request_Field_StudentIdLen("GetStudentInfoFormStudentId_Request.StudentIdLen",offsetof(GetStudentInfoFormStudentId_Request,StudentIdLen));
ArrayField GetStudentInfoFormStudentId_Request_Field_StudentId("GetStudentInfoFormStudentId_Request.StudentId",true,&Uint8TypeInstance,sizeof(Byte),offsetof(GetStudentInfoFormStudentId_Request,StudentId),&GetStudentInfoFormStudentId_Request_Field_StudentIdLen);
Int32Field GetStudentInfoFormStudentId_Request_Field_arg2("GetStudentInfoFormStudentId_Request.arg2",offsetof(GetStudentInfoFormStudentId_Request,arg2));
Int32Field GetStudentInfoFormStudentId_Request_Field_arg3("GetStudentInfoFormStudentId_Request.arg3",offsetof(GetStudentInfoFormStudentId_Request,arg3));
IField* GetStudentInfoFormStudentId_RequestDesc []=
{
&GetStudentInfoFormStudentId_Request_Field_StudentIdLen,
&GetStudentInfoFormStudentId_Request_Field_StudentId,
&GetStudentInfoFormStudentId_Request_Field_arg2,
&GetStudentInfoFormStudentId_Request_Field_arg3,
};
ObjectType GetStudentInfoFormStudentId_Request_Type(sizeof(GetStudentInfoFormStudentId_RequestDesc)/sizeof(IField*),GetStudentInfoFormStudentId_RequestDesc);

ObjectField GetStudentInfoFormStudentId_Response_Field_ReturnValue("GetStudentInfoFormStudentId_Response.ReturnValue",sizeof(Student_tDesc)/sizeof(IField*),Student_tDesc,offsetof(GetStudentInfoFormStudentId_Response,ReturnValue));
IField* GetStudentInfoFormStudentId_ResponseDesc []=
{
&GetStudentInfoFormStudentId_Response_Field_ReturnValue,
};
ObjectType GetStudentInfoFormStudentId_Response_Type(sizeof(GetStudentInfoFormStudentId_ResponseDesc)/sizeof(IField*),GetStudentInfoFormStudentId_ResponseDesc);

ObjectType GetStudentsInfoFormAge_Request_Type(0,nullptr);

ObjectField GetStudentsInfoFormAge_Response_Field_ReturnValue("GetStudentsInfoFormAge_Response.ReturnValue",sizeof(StudentArray_tDesc)/sizeof(IField*),StudentArray_tDesc,offsetof(GetStudentsInfoFormAge_Response,ReturnValue));
IField* GetStudentsInfoFormAge_ResponseDesc []=
{
&GetStudentsInfoFormAge_Response_Field_ReturnValue,
};
ObjectType GetStudentsInfoFormAge_Response_Type(sizeof(GetStudentsInfoFormAge_ResponseDesc)/sizeof(IField*),GetStudentsInfoFormAge_ResponseDesc);

ObjectType Test_Request_Type(0,nullptr);

