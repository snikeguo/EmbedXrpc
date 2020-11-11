#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

extern const Int32Field Student_Field_a=
{ 
  { 
    TYPE_INT32,
  },
  "Student.a" ,
  offsetof(Student,a) ,
  &Int32_TypeInstance,  
  1,
  false,
};
extern const Int32Field Student_Field_b=
{ 
  { 
    TYPE_INT32,
  },
  "Student.b" ,
  offsetof(Student,b) ,
  &Int32_TypeInstance,  
  2,
  false,
};
const IField*  Student_Type_Field_Desc []=
{
  (const IField*)&Student_Field_a,
  (const IField*)&Student_Field_b,
};
extern const ObjectType Student_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Student_Type_Field_Desc)/sizeof(IField*),
  Student_Type_Field_Desc,
};

extern const Int32Field DateTime_t_Field_Year=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Year" ,
  offsetof(DateTime_t,Year) ,
  &Int32_TypeInstance,  
  10086,
  false,
};
extern const Int32Field DateTime_t_Field_Month=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Month" ,
  offsetof(DateTime_t,Month) ,
  &Int32_TypeInstance,  
  2,
  false,
};
extern const Int32Field DateTime_t_Field_Day=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Day" ,
  offsetof(DateTime_t,Day) ,
  &Int32_TypeInstance,  
  3,
  false,
};
extern const Int32Field DateTime_t_Field_Hour=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Hour" ,
  offsetof(DateTime_t,Hour) ,
  &Int32_TypeInstance,  
  4,
  false,
};
extern const Int32Field DateTime_t_Field_Min=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Min" ,
  offsetof(DateTime_t,Min) ,
  &Int32_TypeInstance,  
  5,
  false,
};
extern const Int32Field DateTime_t_Field_Sec=
{ 
  { 
    TYPE_INT32,
  },
  "DateTime_t.Sec" ,
  offsetof(DateTime_t,Sec) ,
  &Int32_TypeInstance,  
  6,
  false,
};
extern const UInt64Field DateTime_t_Field_Sex=
{ 
  { 
    TYPE_UINT64,
  },
  "DateTime_t.Sex" ,
  offsetof(DateTime_t,Sex) ,
  &UInt64_TypeInstance,  
  8,
  false,
};
extern const ObjectType Student_TypeInstance;
extern const ObjectField DateTime_t_Field_David=
{ 
  {
    TYPE_OBJECT,
  },
  "DateTime_t.David",
  offsetof(DateTime_t,David),
  &Student_TypeInstance,
  7,
};
const IField*  DateTime_t_Type_Field_Desc []=
{
  (const IField*)&DateTime_t_Field_Year,
  (const IField*)&DateTime_t_Field_Month,
  (const IField*)&DateTime_t_Field_Day,
  (const IField*)&DateTime_t_Field_Hour,
  (const IField*)&DateTime_t_Field_Min,
  (const IField*)&DateTime_t_Field_Sec,
  (const IField*)&DateTime_t_Field_Sex,
  (const IField*)&DateTime_t_Field_David,
};
extern const ObjectType DateTime_t_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(DateTime_t_Type_Field_Desc)/sizeof(IField*),
  DateTime_t_Type_Field_Desc,
};

extern const Int32Field AddResult_Field_Sum=
{ 
  { 
    TYPE_INT32,
  },
  "AddResult.Sum" ,
  offsetof(AddResult,Sum) ,
  &Int32_TypeInstance,  
  1,
  false,
};
extern const Int32Field AddResult_Field_dataLen=
{ 
  { 
    TYPE_INT32,
  },
  "AddResult.dataLen" ,
  offsetof(AddResult,dataLen) ,
  &Int32_TypeInstance,  
  2,
  true,
};
extern const ArrayType UInt8_Array_TypeInstance= 
{
  { 
    TYPE_ARRAY,
  },
  (const IType *)(&UInt8_TypeInstance),
  sizeof(UInt8),
};
extern const ArrayType UInt8_Array_TypeInstance;
extern const ArrayField AddResult_Field_data=
{ 
  {
    TYPE_ARRAY,
  },
  "AddResult.data",
  offsetof(AddResult,data),
  (const IField*)&AddResult_Field_dataLen,
  &UInt8_Array_TypeInstance,
  false,
  3,
};
const IField*  AddResult_Type_Field_Desc []=
{
  (const IField*)&AddResult_Field_Sum,
  (const IField*)&AddResult_Field_dataLen,
  (const IField*)&AddResult_Field_data,
};
extern const ObjectType AddResult_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(AddResult_Type_Field_Desc)/sizeof(IField*),
  AddResult_Type_Field_Desc,
};

extern const ArrayType DateTime_t_Array_TypeInstance= 
{
  { 
    TYPE_ARRAY,
  },
  (const IType *)(&DateTime_t_TypeInstance),
  sizeof(DateTime_t),
};
extern const ArrayType DateTime_t_Array_TypeInstance;
extern const ArrayField DateTimeChange_Parameter_Field_now=
{ 
  {
    TYPE_ARRAY,
  },
  "DateTimeChange_Parameter.now",
  offsetof(DateTimeChange_Parameter,now),
  nullptr,
  &DateTime_t_Array_TypeInstance,
  true,
  1,
};
const IField*  DateTimeChange_Parameter_Type_Field_Desc []=
{
  (const IField*)&DateTimeChange_Parameter_Field_now,
};
extern const ObjectType DateTimeChange_Parameter_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(DateTimeChange_Parameter_Type_Field_Desc)/sizeof(IField*),
  DateTimeChange_Parameter_Type_Field_Desc,
};

extern const Int32Field Inter_Add_Parameter_Field_a=
{ 
  { 
    TYPE_INT32,
  },
  "Inter_Add_Parameter.a" ,
  offsetof(Inter_Add_Parameter,a) ,
  &Int32_TypeInstance,  
  1,
  false,
};
extern const Int32Field Inter_Add_Parameter_Field_b=
{ 
  { 
    TYPE_INT32,
  },
  "Inter_Add_Parameter.b" ,
  offsetof(Inter_Add_Parameter,b) ,
  &Int32_TypeInstance,  
  2,
  false,
};
extern const Int32Field Inter_Add_Parameter_Field_dataLen=
{ 
  { 
    TYPE_INT32,
  },
  "Inter_Add_Parameter.dataLen" ,
  offsetof(Inter_Add_Parameter,dataLen) ,
  &Int32_TypeInstance,  
  3,
  true,
};
extern const ArrayType UInt8_Array_TypeInstance;
extern const ArrayField Inter_Add_Parameter_Field_data=
{ 
  {
    TYPE_ARRAY,
  },
  "Inter_Add_Parameter.data",
  offsetof(Inter_Add_Parameter,data),
  (const IField*)&Inter_Add_Parameter_Field_dataLen,
  &UInt8_Array_TypeInstance,
  false,
  4,
};
const IField*  Inter_Add_Parameter_Type_Field_Desc []=
{
  (const IField*)&Inter_Add_Parameter_Field_a,
  (const IField*)&Inter_Add_Parameter_Field_b,
  (const IField*)&Inter_Add_Parameter_Field_dataLen,
  (const IField*)&Inter_Add_Parameter_Field_data,
};
extern const ObjectType Inter_Add_Parameter_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_Add_Parameter_Type_Field_Desc)/sizeof(IField*),
  Inter_Add_Parameter_Type_Field_Desc,
};

extern const UInt8Field Inter_Add_Return_Field_State=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_Add_Return.State" ,
  offsetof(Inter_Add_Return,State) ,
  &UInt8_TypeInstance,  
  1,
  false,
};
extern const ObjectType AddResult_TypeInstance;
extern const ObjectField Inter_Add_Return_Field_ReturnValue=
{ 
  {
    TYPE_OBJECT,
  },
  "Inter_Add_Return.ReturnValue",
  offsetof(Inter_Add_Return,ReturnValue),
  &AddResult_TypeInstance,
  2,
};
const IField*  Inter_Add_Return_Type_Field_Desc []=
{
  (const IField*)&Inter_Add_Return_Field_State,
  (const IField*)&Inter_Add_Return_Field_ReturnValue,
};
extern const ObjectType Inter_Add_Return_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_Add_Return_Type_Field_Desc)/sizeof(IField*),
  Inter_Add_Return_Type_Field_Desc,
};

extern const ObjectType Inter_NoArg_Parameter_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  0,
  nullptr,
};

extern const UInt8Field Inter_NoArg_Return_Field_State=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_NoArg_Return.State" ,
  offsetof(Inter_NoArg_Return,State) ,
  &UInt8_TypeInstance,  
  1,
  false,
};
extern const UInt8Field Inter_NoArg_Return_Field_ReturnValue=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_NoArg_Return.ReturnValue" ,
  offsetof(Inter_NoArg_Return,ReturnValue) ,
  &UInt8_TypeInstance,  
  2,
  false,
};
const IField*  Inter_NoArg_Return_Type_Field_Desc []=
{
  (const IField*)&Inter_NoArg_Return_Field_State,
  (const IField*)&Inter_NoArg_Return_Field_ReturnValue,
};
extern const ObjectType Inter_NoArg_Return_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_NoArg_Return_Type_Field_Desc)/sizeof(IField*),
  Inter_NoArg_Return_Type_Field_Desc,
};

extern const Int32Field Inter_NoReturn_Parameter_Field_a=
{ 
  { 
    TYPE_INT32,
  },
  "Inter_NoReturn_Parameter.a" ,
  offsetof(Inter_NoReturn_Parameter,a) ,
  &Int32_TypeInstance,  
  1,
  false,
};
const IField*  Inter_NoReturn_Parameter_Type_Field_Desc []=
{
  (const IField*)&Inter_NoReturn_Parameter_Field_a,
};
extern const ObjectType Inter_NoReturn_Parameter_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_NoReturn_Parameter_Type_Field_Desc)/sizeof(IField*),
  Inter_NoReturn_Parameter_Type_Field_Desc,
};

extern const UInt8Field Inter_NoReturn_Return_Field_State=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_NoReturn_Return.State" ,
  offsetof(Inter_NoReturn_Return,State) ,
  &UInt8_TypeInstance,  
  1,
  false,
};
const IField*  Inter_NoReturn_Return_Type_Field_Desc []=
{
  (const IField*)&Inter_NoReturn_Return_Field_State,
};
extern const ObjectType Inter_NoReturn_Return_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_NoReturn_Return_Type_Field_Desc)/sizeof(IField*),
  Inter_NoReturn_Return_Type_Field_Desc,
};

extern const ObjectType Inter_NoArgAndReturn_Parameter_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  0,
  nullptr,
};

extern const UInt8Field Inter_NoArgAndReturn_Return_Field_State=
{ 
  { 
    TYPE_UINT8,
  },
  "Inter_NoArgAndReturn_Return.State" ,
  offsetof(Inter_NoArgAndReturn_Return,State) ,
  &UInt8_TypeInstance,  
  1,
  false,
};
const IField*  Inter_NoArgAndReturn_Return_Type_Field_Desc []=
{
  (const IField*)&Inter_NoArgAndReturn_Return_Field_State,
};
extern const ObjectType Inter_NoArgAndReturn_Return_TypeInstance=
{    
  {
    TYPE_OBJECT,
  },
  sizeof(Inter_NoArgAndReturn_Return_Type_Field_Desc)/sizeof(IField*),
  Inter_NoArgAndReturn_Return_Type_Field_Desc,
};

