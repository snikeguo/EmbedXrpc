#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

const Int32Field Student_Field_a=
{ 
  { 
    TYPE_INT32,
  },
  "Student.a" ,
  offsetof(Student,a) ,
  &Int32TypeInstance,  
  1,
  false,
};
const Int32Field Student_Field_b=
{ 
  { 
    TYPE_INT32,
  },
  "Student.b" ,
  offsetof(Student,b) ,
  &Int32TypeInstance,  
  2,
  false,
};
const IField*  Student_Object_Type_Desc []=
{
  (const IField*)&Student_Field_a,
  (const IField*)&Student_Field_b,
};
const ObjectType Student_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Student_Object_Type_Desc)/sizeof(IField*),
  Student_Object_Type_Desc,
};

const Int32Field DateTime_t_Field_Year=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Year" ,
  offsetof(DateTime_t,Year) ,
  &Int32TypeInstance,  
  10086,
  false,
};
const Int32Field DateTime_t_Field_Month=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Month" ,
  offsetof(DateTime_t,Month) ,
  &Int32TypeInstance,  
  2,
  false,
};
const Int32Field DateTime_t_Field_Day=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Day" ,
  offsetof(DateTime_t,Day) ,
  &Int32TypeInstance,  
  3,
  false,
};
const Int32Field DateTime_t_Field_Hour=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Hour" ,
  offsetof(DateTime_t,Hour) ,
  &Int32TypeInstance,  
  4,
  false,
};
const Int32Field DateTime_t_Field_Min=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Min" ,
  offsetof(DateTime_t,Min) ,
  &Int32TypeInstance,  
  5,
  false,
};
const Int32Field DateTime_t_Field_Sec=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Sec" ,
  offsetof(DateTime_t,Sec) ,
  &Int32TypeInstance,  
  6,
  false,
};
const ObjectField DateTime_t_Field_David=
{ 
  {
    TYPE_OBJECT,
  },
  "DateTime_t.David",
  offsetof(DateTime_t,David),
  &Student_Object_Type,
  7,
};
const IField*  DateTime_t_Object_Type_Desc []=
{
  (const IField*)&DateTime_t_Field_Year,
  (const IField*)&DateTime_t_Field_Month,
  (const IField*)&DateTime_t_Field_Day,
  (const IField*)&DateTime_t_Field_Hour,
  (const IField*)&DateTime_t_Field_Min,
  (const IField*)&DateTime_t_Field_Sec,
  (const IField*)&DateTime_t_Field_David,
};
const ObjectType DateTime_t_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(DateTime_t_Object_Type_Desc)/sizeof(IField*),
  DateTime_t_Object_Type_Desc,
};

const Int32Field AddResult_Field_Sum=
{ 
  { 
    TYPE_INT32,
  },
  "AddResult.Sum" ,
  offsetof(AddResult,Sum) ,
  &Int32TypeInstance,  
  1,
  false,
};
const Int32Field AddResult_Field_dataLen=
{ 
  { 
    TYPE_INT32,
  },
  "AddResult.dataLen" ,
  offsetof(AddResult,dataLen) ,
  &Int32TypeInstance,  
  2,
  true,
};
const ArrayType Byte_Array_Type= 
{
  { 
    TYPE_ARRAY,
  },
  (const IType *)(&UInt8TypeInstance),
  sizeof(Byte),
};
const ArrayField AddResult_Field_data=
{ 
  {
    TYPE_ARRAY,
  },
  "AddResult.data",
  offsetof(AddResult,data),
  (const IField*)&AddResult_Field_dataLen,
  &Byte_Array_Type,
  false,
  3,
};
const IField*  AddResult_Object_Type_Desc []=
{
  (const IField*)&AddResult_Field_Sum,
  (const IField*)&AddResult_Field_dataLen,
  (const IField*)&AddResult_Field_data,
};
const ObjectType AddResult_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(AddResult_Object_Type_Desc)/sizeof(IField*),
  AddResult_Object_Type_Desc,
};

const ArrayType DateTime_t_Array_Type= 
{
  { 
    TYPE_ARRAY,
  },
  (const IType *)(&DateTime_t_Object_Type),
  sizeof(DateTime_t),
};
const ArrayType DateTime_t_Array_Type =
{
  {
    TYPE_ARRAY,
  },
  (const IType*)(&DateTime_t_Type),
  sizeof(DateTime_t),
};
const ArrayField DateTimeChange_Parameter_Field_now=
{ 
  {
    TYPE_ARRAY,
  },
  "DateTimeChange_Parameter.now",
  offsetof(DateTimeChange_Parameter,now),
  nullptr,
  &DateTime_t_Array_Type,
  true,
  1,
};
const IField*  DateTimeChange_Parameter_Object_Type_Desc []=
{
  (const IField*)&DateTimeChange_Parameter_Field_now,
};
const ObjectType DateTimeChange_Parameter_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(DateTimeChange_Parameter_Object_Type_Desc)/sizeof(IField*),
  DateTimeChange_Parameter_Object_Type_Desc,
};

const Int32Field Inter_Add_Parameter_Field_a=
{ 
  { 
    TYPE_INT32,
  },
  "Inter_Add_Parameter.a" ,
  offsetof(Inter_Add_Parameter,a) ,
  &Int32TypeInstance,  
  1,
  false,
};
const Int32Field Inter_Add_Parameter_Field_b=
{ 
  { 
    TYPE_INT32,
  },
  "Inter_Add_Parameter.b" ,
  offsetof(Inter_Add_Parameter,b) ,
  &Int32TypeInstance,  
  2,
  false,
};
const Int32Field Inter_Add_Parameter_Field_dataLen=
{ 
  { 
    TYPE_INT32,
  },
  "Inter_Add_Parameter.dataLen" ,
  offsetof(Inter_Add_Parameter,dataLen) ,
  &Int32TypeInstance,  
  3,
  true,
};
const ArrayField Inter_Add_Parameter_Field_data=
{ 
  {
    TYPE_ARRAY,
  },
  "Inter_Add_Parameter.data",
  offsetof(Inter_Add_Parameter,data),
  (const IField*)&Inter_Add_Parameter_Field_dataLen,
  &Byte_Array_Type,
  false,
  4,
};
const IField*  Inter_Add_Parameter_Object_Type_Desc []=
{
  (const IField*)&Inter_Add_Parameter_Field_a,
  (const IField*)&Inter_Add_Parameter_Field_b,
  (const IField*)&Inter_Add_Parameter_Field_dataLen,
  (const IField*)&Inter_Add_Parameter_Field_data,
};
const ObjectType Inter_Add_Parameter_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_Add_Parameter_Object_Type_Desc)/sizeof(IField*),
  Inter_Add_Parameter_Object_Type_Desc,
};

const UInt8Field Inter_Add_Return_Field_State=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_Add_Return.State" ,
  offsetof(Inter_Add_Return,State) ,
  &UInt8TypeInstance,  
  1,
  false,
};
const ObjectField Inter_Add_Return_Field_ReturnValue=
{ 
  {
    TYPE_OBJECT,
  },
  "Inter_Add_Return.ReturnValue",
  offsetof(Inter_Add_Return,ReturnValue),
  &AddResult_Object_Type,
  2,
};
const IField*  Inter_Add_Return_Object_Type_Desc []=
{
  (const IField*)&Inter_Add_Return_Field_State,
  (const IField*)&Inter_Add_Return_Field_ReturnValue,
};
const ObjectType Inter_Add_Return_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_Add_Return_Object_Type_Desc)/sizeof(IField*),
  Inter_Add_Return_Object_Type_Desc,
};

const ObjectType Inter_NoArg_Parameter_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  0,
  nullptr,
};

const UInt8Field Inter_NoArg_Return_Field_State=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_NoArg_Return.State" ,
  offsetof(Inter_NoArg_Return,State) ,
  &UInt8TypeInstance,  
  1,
  false,
};
const UInt8Field Inter_NoArg_Return_Field_ReturnValue=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_NoArg_Return.ReturnValue" ,
  offsetof(Inter_NoArg_Return,ReturnValue) ,
  &UInt8TypeInstance,  
  2,
  false,
};
const IField*  Inter_NoArg_Return_Object_Type_Desc []=
{
  (const IField*)&Inter_NoArg_Return_Field_State,
  (const IField*)&Inter_NoArg_Return_Field_ReturnValue,
};
const ObjectType Inter_NoArg_Return_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_NoArg_Return_Object_Type_Desc)/sizeof(IField*),
  Inter_NoArg_Return_Object_Type_Desc,
};

const Int32Field Inter_NoReturn_Parameter_Field_a=
{ 
  { 
    TYPE_INT32,
  },
  "Inter_NoReturn_Parameter.a" ,
  offsetof(Inter_NoReturn_Parameter,a) ,
  &Int32TypeInstance,  
  1,
  false,
};
const IField*  Inter_NoReturn_Parameter_Object_Type_Desc []=
{
  (const IField*)&Inter_NoReturn_Parameter_Field_a,
};
const ObjectType Inter_NoReturn_Parameter_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_NoReturn_Parameter_Object_Type_Desc)/sizeof(IField*),
  Inter_NoReturn_Parameter_Object_Type_Desc,
};

const UInt8Field Inter_NoReturn_Return_Field_State=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_NoReturn_Return.State" ,
  offsetof(Inter_NoReturn_Return,State) ,
  &UInt8TypeInstance,  
  1,
  false,
};
const IField*  Inter_NoReturn_Return_Object_Type_Desc []=
{
  (const IField*)&Inter_NoReturn_Return_Field_State,
};
const ObjectType Inter_NoReturn_Return_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_NoReturn_Return_Object_Type_Desc)/sizeof(IField*),
  Inter_NoReturn_Return_Object_Type_Desc,
};

const ObjectType Inter_NoArgAndReturn_Parameter_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  0,
  nullptr,
};

const UInt8Field Inter_NoArgAndReturn_Return_Field_State=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_NoArgAndReturn_Return.State" ,
  offsetof(Inter_NoArgAndReturn_Return,State) ,
  &UInt8TypeInstance,  
  1,
  false,
};
const IField*  Inter_NoArgAndReturn_Return_Object_Type_Desc []=
{
  (const IField*)&Inter_NoArgAndReturn_Return_Field_State,
};
const ObjectType Inter_NoArgAndReturn_Return_Object_Type=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_NoArgAndReturn_Return_Object_Type_Desc)/sizeof(IField*),
  Inter_NoArgAndReturn_Return_Object_Type_Desc,
};

