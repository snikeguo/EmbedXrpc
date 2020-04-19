#include"StudentService.h"
#include"StudentService.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

const Int32Field Result_t_Field_Value("Result_t.Value",offsetof(Result_t,Value));
const UInt8Field Result_t_Field_NameLen("Result_t.NameLen",offsetof(Result_t,NameLen));
const ArrayField Result_t_Field_Name("Result_t.Name",true,&UInt8TypeInstance,sizeof(Byte),offsetof(Result_t,Name),&Result_t_Field_NameLen);
const Int8Field Result_t_Field_Arg1("Result_t.Arg1",offsetof(Result_t,Arg1));
const IField* Result_tDesc []=
{
&Result_t_Field_Value,
&Result_t_Field_NameLen,
&Result_t_Field_Name,
&Result_t_Field_Arg1,
};
const ObjectType Result_t_Type(sizeof(Result_tDesc)/sizeof(IField*),Result_tDesc);

const UInt16Field Student_t_Field_ResultsLen("Student_t.ResultsLen",offsetof(Student_t,ResultsLen));
const ArrayField Student_t_Field_Results("Student_t.Results",true,&Result_t_Type,sizeof(Result_t),offsetof(Student_t,Results),&Student_t_Field_ResultsLen);
const Int32Field Student_t_Field_Age("Student_t.Age",offsetof(Student_t,Age));
const ArrayField Student_t_Field_Name("Student_t.Name",false,&UInt8TypeInstance,sizeof(Byte),offsetof(Student_t,Name),nullptr);
const UInt8Field Student_t_Field_StudentIdLen("Student_t.StudentIdLen",offsetof(Student_t,StudentIdLen));
const ArrayField Student_t_Field_StudentId("Student_t.StudentId",true,&UInt8TypeInstance,sizeof(Byte),offsetof(Student_t,StudentId),&Student_t_Field_StudentIdLen);
const UInt32Field Student_t_Field_Sex("Student_t.Sex",offsetof(Student_t,Sex));
const IField* Student_tDesc []=
{
&Student_t_Field_ResultsLen,
&Student_t_Field_Results,
&Student_t_Field_Age,
&Student_t_Field_Name,
&Student_t_Field_StudentIdLen,
&Student_t_Field_StudentId,
&Student_t_Field_Sex,
};
const ObjectType Student_t_Type(sizeof(Student_tDesc)/sizeof(IField*),Student_tDesc);

const Int32Field DateTime_t_Field_Year("DateTime_t.Year",offsetof(DateTime_t,Year));
const Int32Field DateTime_t_Field_Month("DateTime_t.Month",offsetof(DateTime_t,Month));
const Int32Field DateTime_t_Field_Day("DateTime_t.Day",offsetof(DateTime_t,Day));
const Int32Field DateTime_t_Field_Hour("DateTime_t.Hour",offsetof(DateTime_t,Hour));
const Int32Field DateTime_t_Field_Min("DateTime_t.Min",offsetof(DateTime_t,Min));
const Int32Field DateTime_t_Field_Sec("DateTime_t.Sec",offsetof(DateTime_t,Sec));
const IField* DateTime_tDesc []=
{
&DateTime_t_Field_Year,
&DateTime_t_Field_Month,
&DateTime_t_Field_Day,
&DateTime_t_Field_Hour,
&DateTime_t_Field_Min,
&DateTime_t_Field_Sec,
};
const ObjectType DateTime_t_Type(sizeof(DateTime_tDesc)/sizeof(IField*),DateTime_tDesc);

const UInt8Field StudentArray_t_Field_StudentIdLen("StudentArray_t.StudentIdLen",offsetof(StudentArray_t,StudentIdLen));
const ArrayField StudentArray_t_Field_Students("StudentArray_t.Students",true,&Student_t_Type,sizeof(Student_t),offsetof(StudentArray_t,Students),&StudentArray_t_Field_StudentIdLen);
const IField* StudentArray_tDesc []=
{
&StudentArray_t_Field_StudentIdLen,
&StudentArray_t_Field_Students,
};
const ObjectType StudentArray_t_Type(sizeof(StudentArray_tDesc)/sizeof(IField*),StudentArray_tDesc);

const ObjectField BroadcastDataTimeStruct_Field_t("BroadcastDataTimeStruct.t",sizeof(DateTime_tDesc)/sizeof(IField*),DateTime_tDesc,offsetof(BroadcastDataTimeStruct,t));
const IField* BroadcastDataTimeStructDesc []=
{
&BroadcastDataTimeStruct_Field_t,
};
const ObjectType BroadcastDataTimeStruct_Type(sizeof(BroadcastDataTimeStructDesc)/sizeof(IField*),BroadcastDataTimeStructDesc);

const UInt8Field GetStudentInfoFormStudentId_Request_Field_StudentIdLen("GetStudentInfoFormStudentId_Request.StudentIdLen",offsetof(GetStudentInfoFormStudentId_Request,StudentIdLen));
const ArrayField GetStudentInfoFormStudentId_Request_Field_StudentId("GetStudentInfoFormStudentId_Request.StudentId",true,&UInt8TypeInstance,sizeof(Byte),offsetof(GetStudentInfoFormStudentId_Request,StudentId),&GetStudentInfoFormStudentId_Request_Field_StudentIdLen);
const Int32Field GetStudentInfoFormStudentId_Request_Field_arg2("GetStudentInfoFormStudentId_Request.arg2",offsetof(GetStudentInfoFormStudentId_Request,arg2));
const Int32Field GetStudentInfoFormStudentId_Request_Field_arg3("GetStudentInfoFormStudentId_Request.arg3",offsetof(GetStudentInfoFormStudentId_Request,arg3));
const IField* GetStudentInfoFormStudentId_RequestDesc []=
{
&GetStudentInfoFormStudentId_Request_Field_StudentIdLen,
&GetStudentInfoFormStudentId_Request_Field_StudentId,
&GetStudentInfoFormStudentId_Request_Field_arg2,
&GetStudentInfoFormStudentId_Request_Field_arg3,
};
const ObjectType GetStudentInfoFormStudentId_Request_Type(sizeof(GetStudentInfoFormStudentId_RequestDesc)/sizeof(IField*),GetStudentInfoFormStudentId_RequestDesc);

const UInt8Field GetStudentInfoFormStudentId_Response_Field_State("GetStudentInfoFormStudentId_Response.State",offsetof(GetStudentInfoFormStudentId_Response,State));
const ObjectField GetStudentInfoFormStudentId_Response_Field_ReturnValue("GetStudentInfoFormStudentId_Response.ReturnValue",sizeof(Student_tDesc)/sizeof(IField*),Student_tDesc,offsetof(GetStudentInfoFormStudentId_Response,ReturnValue));
const IField* GetStudentInfoFormStudentId_ResponseDesc []=
{
&GetStudentInfoFormStudentId_Response_Field_State,
&GetStudentInfoFormStudentId_Response_Field_ReturnValue,
};
const ObjectType GetStudentInfoFormStudentId_Response_Type(sizeof(GetStudentInfoFormStudentId_ResponseDesc)/sizeof(IField*),GetStudentInfoFormStudentId_ResponseDesc);

const ObjectType GetStudentsInfoFormAge_Request_Type(0,nullptr);

const UInt8Field GetStudentsInfoFormAge_Response_Field_State("GetStudentsInfoFormAge_Response.State",offsetof(GetStudentsInfoFormAge_Response,State));
const ObjectField GetStudentsInfoFormAge_Response_Field_ReturnValue("GetStudentsInfoFormAge_Response.ReturnValue",sizeof(StudentArray_tDesc)/sizeof(IField*),StudentArray_tDesc,offsetof(GetStudentsInfoFormAge_Response,ReturnValue));
const IField* GetStudentsInfoFormAge_ResponseDesc []=
{
&GetStudentsInfoFormAge_Response_Field_State,
&GetStudentsInfoFormAge_Response_Field_ReturnValue,
};
const ObjectType GetStudentsInfoFormAge_Response_Type(sizeof(GetStudentsInfoFormAge_ResponseDesc)/sizeof(IField*),GetStudentsInfoFormAge_ResponseDesc);

const ObjectType Test_Request_Type(0,nullptr);

const UInt8Field Test_Response_Field_State("Test_Response.State",offsetof(Test_Response,State));
const UInt8Field Test_Response_Field_ReturnValue("Test_Response.ReturnValue",offsetof(Test_Response,ReturnValue));
const IField* Test_ResponseDesc []=
{
&Test_Response_Field_State,
&Test_Response_Field_ReturnValue,
};
const ObjectType Test_Response_Type(sizeof(Test_ResponseDesc)/sizeof(IField*),Test_ResponseDesc);

