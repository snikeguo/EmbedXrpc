#ifndef Sample1_H
#define Sample1_H
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef enum _Sex
{
Man = 2,
WoMan = 9,
}Sex;
typedef struct _AddResult
{
Int32 Sum;   //FieldNumber:1    
Int32 dataLen;   //FieldNumber:2    
UInt8* data;   //FieldNumber:3    
}AddResult;
#define AddResult_Sum_FieldNumber  1
#define AddResult_dataLen_FieldNumber  2
#define AddResult_data_FieldNumber  3


typedef struct _Student
{
Int32 a;   //FieldNumber:1    
Int32 b;   //FieldNumber:2    
Int32 AddressType;   //FieldNumber:3    Union Target Type
union{
UInt8 u1;   //FieldNumber:4    
UInt64 u2;   //FieldNumber:5    
Sex u3;   //FieldNumber:6    
AddResult u4;   //FieldNumber:7    
};//union end
UInt8 uend1;   //FieldNumber:100    
UInt8 uend2;   //FieldNumber:8    
}Student;
#define Student_a_FieldNumber  1
#define Student_b_FieldNumber  2
#define Student_AddressType_FieldNumber  3
#define Student_u1_FieldNumber  4
#define Student_u2_FieldNumber  5
#define Student_u3_FieldNumber  6
#define Student_u4_FieldNumber  7
#define Student_uend1_FieldNumber  100
#define Student_uend2_FieldNumber  8


typedef struct _DateTime_t
{
Int32 Year;   //FieldNumber:10086    
Int32 Month;   //FieldNumber:2    
Int32 Day;   //FieldNumber:3    
Int32 Hour;   //FieldNumber:4    
Int32 Min;   //FieldNumber:5    
Int32 Sec;   //FieldNumber:6    
Sex Sex;   //FieldNumber:8    
UInt8 DateStringLen;   //FieldNumber:1    
UInt8* DateString;   //FieldNumber:7    
Student David;   //FieldNumber:9    
}DateTime_t;
#define DateTime_t_Year_FieldNumber  10086
#define DateTime_t_Month_FieldNumber  2
#define DateTime_t_Day_FieldNumber  3
#define DateTime_t_Hour_FieldNumber  4
#define DateTime_t_Min_FieldNumber  5
#define DateTime_t_Sec_FieldNumber  6
#define DateTime_t_Sex_FieldNumber  8
#define DateTime_t_DateStringLen_FieldNumber  1
#define DateTime_t_DateString_FieldNumber  7
#define DateTime_t_David_FieldNumber  9


typedef struct _DateTimeChange_Parameter
{
DateTime_t now[1];   //FieldNumber:1    
}DateTimeChange_Parameter;
#define DateTimeChange_Parameter_now_FieldNumber  1


typedef struct _TestDelegate_Parameter
{
DateTime_t now[1];   //FieldNumber:1    
}TestDelegate_Parameter;
#define TestDelegate_Parameter_now_FieldNumber  1


typedef struct _Inter_Add_Parameter
{
Int32 a;   //FieldNumber:1    
Int32 b;   //FieldNumber:2    
Int32 dataLen;   //FieldNumber:3    
UInt8* data;   //FieldNumber:4    
}Inter_Add_Parameter;
#define Inter_Add_Parameter_a_FieldNumber  1
#define Inter_Add_Parameter_b_FieldNumber  2
#define Inter_Add_Parameter_dataLen_FieldNumber  3
#define Inter_Add_Parameter_data_FieldNumber  4


typedef struct _Inter_Add_Return
{
RequestResponseState State;   //FieldNumber:1    
AddResult ReturnValue;   //FieldNumber:2    
}Inter_Add_Return;
#define Inter_Add_Return_State_FieldNumber  1
#define Inter_Add_Return_ReturnValue_FieldNumber  2


typedef struct _Inter_NoArg_Parameter
{
}Inter_NoArg_Parameter;


typedef struct _Inter_NoArg_Return
{
RequestResponseState State;   //FieldNumber:1    
UInt8 ReturnValue;   //FieldNumber:2    
}Inter_NoArg_Return;
#define Inter_NoArg_Return_State_FieldNumber  1
#define Inter_NoArg_Return_ReturnValue_FieldNumber  2


typedef struct _Inter_NoReturn_Parameter
{
Int32 a;   //FieldNumber:1    
}Inter_NoReturn_Parameter;
#define Inter_NoReturn_Parameter_a_FieldNumber  1


typedef struct _Inter_NoReturn_Return
{
RequestResponseState State;   //FieldNumber:1    
}Inter_NoReturn_Return;
#define Inter_NoReturn_Return_State_FieldNumber  1


typedef struct _Inter_NoArgAndReturn_Parameter
{
}Inter_NoArgAndReturn_Parameter;


typedef struct _Inter_NoArgAndReturn_Return
{
RequestResponseState State;   //FieldNumber:1    
}Inter_NoArgAndReturn_Return;
#define Inter_NoArgAndReturn_Return_State_FieldNumber  1



#endif
