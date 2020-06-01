﻿#include"Sample1.h"
#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

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

const ArrayField DateTimeChangeStruct_Field_now("DateTimeChangeStruct.now",true,&DateTime_t_Type,sizeof(DateTime_t),offsetof(DateTimeChangeStruct,now),nullptr);
const IField* DateTimeChangeStructDesc []=
{
&DateTimeChangeStruct_Field_now,
};
const ObjectType DateTimeChangeStruct_Type(sizeof(DateTimeChangeStructDesc)/sizeof(IField*),DateTimeChangeStructDesc);

const UInt8Field Inter_Add_Request_Field_a("Inter_Add_Request.a",offsetof(Inter_Add_Request,a));
const UInt8Field Inter_Add_Request_Field_b("Inter_Add_Request.b",offsetof(Inter_Add_Request,b));
const IField* Inter_Add_RequestDesc []=
{
&Inter_Add_Request_Field_a,
&Inter_Add_Request_Field_b,
};
const ObjectType Inter_Add_Request_Type(sizeof(Inter_Add_RequestDesc)/sizeof(IField*),Inter_Add_RequestDesc);

const UInt8Field Inter_Add_Response_Field_State("Inter_Add_Response.State",offsetof(Inter_Add_Response,State));
const Int32Field Inter_Add_Response_Field_ReturnValue("Inter_Add_Response.ReturnValue",offsetof(Inter_Add_Response,ReturnValue));
const IField* Inter_Add_ResponseDesc []=
{
&Inter_Add_Response_Field_State,
&Inter_Add_Response_Field_ReturnValue,
};
const ObjectType Inter_Add_Response_Type(sizeof(Inter_Add_ResponseDesc)/sizeof(IField*),Inter_Add_ResponseDesc);

const ObjectType Inter_NoArg_Request_Type(0,nullptr);

const UInt8Field Inter_NoArg_Response_Field_State("Inter_NoArg_Response.State",offsetof(Inter_NoArg_Response,State));
const UInt8Field Inter_NoArg_Response_Field_ReturnValue("Inter_NoArg_Response.ReturnValue",offsetof(Inter_NoArg_Response,ReturnValue));
const IField* Inter_NoArg_ResponseDesc []=
{
&Inter_NoArg_Response_Field_State,
&Inter_NoArg_Response_Field_ReturnValue,
};
const ObjectType Inter_NoArg_Response_Type(sizeof(Inter_NoArg_ResponseDesc)/sizeof(IField*),Inter_NoArg_ResponseDesc);

const ObjectType Inter_NoReturn_Request_Type(0,nullptr);

const ObjectType Inter_NoArgAndReturn_Request_Type(0,nullptr);

