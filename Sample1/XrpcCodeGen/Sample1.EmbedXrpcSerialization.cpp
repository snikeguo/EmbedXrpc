#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

const Int32Field DateTime_t_Field_Year(10086,"DateTime_t.Year",offsetof(DateTime_t,Year),false);
const Int32Field DateTime_t_Field_Month(2,"DateTime_t.Month",offsetof(DateTime_t,Month),false);
const Int32Field DateTime_t_Field_Day(3,"DateTime_t.Day",offsetof(DateTime_t,Day),false);
const Int32Field DateTime_t_Field_Hour(4,"DateTime_t.Hour",offsetof(DateTime_t,Hour),false);
const Int32Field DateTime_t_Field_Min(5,"DateTime_t.Min",offsetof(DateTime_t,Min),false);
const Int32Field DateTime_t_Field_Sec(6,"DateTime_t.Sec",offsetof(DateTime_t,Sec),false);
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

const Int32Field AddResult_Field_Sum(1,"AddResult.Sum",offsetof(AddResult,Sum),false);
const Int32Field AddResult_Field_dataLen(2,"AddResult.dataLen",offsetof(AddResult,dataLen),true);
const ArrayField AddResult_Field_data(3,"AddResult.data",false,&UInt8TypeInstance,sizeof(Byte),offsetof(AddResult,data),&AddResult_Field_dataLen);
const IField* AddResultDesc []=
{
&AddResult_Field_Sum,
&AddResult_Field_dataLen,
&AddResult_Field_data,
};
const ObjectType AddResult_Type(sizeof(AddResultDesc)/sizeof(IField*),AddResultDesc);

const ArrayField DateTimeChange_Parameter_Field_now(1,"DateTimeChange_Parameter.now",true,&DateTime_t_Type,sizeof(DateTime_t),offsetof(DateTimeChange_Parameter,now),nullptr);
const IField* DateTimeChange_ParameterDesc []=
{
&DateTimeChange_Parameter_Field_now,
};
const ObjectType DateTimeChange_Parameter_Type(sizeof(DateTimeChange_ParameterDesc)/sizeof(IField*),DateTimeChange_ParameterDesc);

const Int32Field Inter_Add_Parameter_Field_a(1,"Inter_Add_Parameter.a",offsetof(Inter_Add_Parameter,a),false);
const Int32Field Inter_Add_Parameter_Field_b(2,"Inter_Add_Parameter.b",offsetof(Inter_Add_Parameter,b),false);
const Int32Field Inter_Add_Parameter_Field_dataLen(3,"Inter_Add_Parameter.dataLen",offsetof(Inter_Add_Parameter,dataLen),true);
const ArrayField Inter_Add_Parameter_Field_data(4,"Inter_Add_Parameter.data",false,&UInt8TypeInstance,sizeof(Byte),offsetof(Inter_Add_Parameter,data),&Inter_Add_Parameter_Field_dataLen);
const IField* Inter_Add_ParameterDesc []=
{
&Inter_Add_Parameter_Field_a,
&Inter_Add_Parameter_Field_b,
&Inter_Add_Parameter_Field_dataLen,
&Inter_Add_Parameter_Field_data,
};
const ObjectType Inter_Add_Parameter_Type(sizeof(Inter_Add_ParameterDesc)/sizeof(IField*),Inter_Add_ParameterDesc);

const UInt8Field Inter_Add_Return_Field_State(1,"Inter_Add_Return.State",offsetof(Inter_Add_Return,State),false);
const ObjectField Inter_Add_Return_Field_ReturnValue(2,"Inter_Add_Return.ReturnValue",sizeof(AddResultDesc)/sizeof(IField*),AddResultDesc,offsetof(Inter_Add_Return,ReturnValue));
const IField* Inter_Add_ReturnDesc []=
{
&Inter_Add_Return_Field_State,
&Inter_Add_Return_Field_ReturnValue,
};
const ObjectType Inter_Add_Return_Type(sizeof(Inter_Add_ReturnDesc)/sizeof(IField*),Inter_Add_ReturnDesc);

const ObjectType Inter_NoArg_Parameter_Type(0,nullptr);

const UInt8Field Inter_NoArg_Return_Field_State(1,"Inter_NoArg_Return.State",offsetof(Inter_NoArg_Return,State),false);
const UInt8Field Inter_NoArg_Return_Field_ReturnValue(2,"Inter_NoArg_Return.ReturnValue",offsetof(Inter_NoArg_Return,ReturnValue),false);
const IField* Inter_NoArg_ReturnDesc []=
{
&Inter_NoArg_Return_Field_State,
&Inter_NoArg_Return_Field_ReturnValue,
};
const ObjectType Inter_NoArg_Return_Type(sizeof(Inter_NoArg_ReturnDesc)/sizeof(IField*),Inter_NoArg_ReturnDesc);

const Int32Field Inter_NoReturn_Parameter_Field_a(1,"Inter_NoReturn_Parameter.a",offsetof(Inter_NoReturn_Parameter,a),false);
const IField* Inter_NoReturn_ParameterDesc []=
{
&Inter_NoReturn_Parameter_Field_a,
};
const ObjectType Inter_NoReturn_Parameter_Type(sizeof(Inter_NoReturn_ParameterDesc)/sizeof(IField*),Inter_NoReturn_ParameterDesc);

const UInt8Field Inter_NoReturn_Return_Field_State(1,"Inter_NoReturn_Return.State",offsetof(Inter_NoReturn_Return,State),false);
const IField* Inter_NoReturn_ReturnDesc []=
{
&Inter_NoReturn_Return_Field_State,
};
const ObjectType Inter_NoReturn_Return_Type(sizeof(Inter_NoReturn_ReturnDesc)/sizeof(IField*),Inter_NoReturn_ReturnDesc);

const ObjectType Inter_NoArgAndReturn_Parameter_Type(0,nullptr);

const UInt8Field Inter_NoArgAndReturn_Return_Field_State(1,"Inter_NoArgAndReturn_Return.State",offsetof(Inter_NoArgAndReturn_Return,State),false);
const IField* Inter_NoArgAndReturn_ReturnDesc []=
{
&Inter_NoArgAndReturn_Return_Field_State,
};
const ObjectType Inter_NoArgAndReturn_Return_Type(sizeof(Inter_NoArgAndReturn_ReturnDesc)/sizeof(IField*),Inter_NoArgAndReturn_ReturnDesc);

