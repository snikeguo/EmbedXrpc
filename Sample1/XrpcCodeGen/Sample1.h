#ifndef Sample1_H
#define Sample1_H
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"
#include"MyInclude.h"
/* 这是一个注释! */

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

#ifdef  __cplusplus
extern "C" {
#endif

typedef enum Sex
{
Man = 2,
WoMan = 9,
}Sex;
typedef struct AddResult
{
uint8_t boolTest;   //FieldNumber:1     
uint32_t Sum:11;   //FieldNumber:2     
uint32_t Sum2:19;   //FieldNumber:3     
uint32_t Sum3:2;   //FieldNumber:4     
int32_t dataLen;   //FieldNumber:5     
uint8_t* data;   //FieldNumber:6     NoSerialization
uint64_t Sum4:11;   //FieldNumber:7     
uint64_t Sum5:19;   //FieldNumber:8     
uint64_t Sum6:2;   //FieldNumber:9     
uint32_t Sum7;   //FieldNumber:10     
void * PtrTest;   //FieldNumber:11     NoSerialization
}AddResult;
#define AddResult_boolTest_FieldNumber  1
#define AddResult_Sum_FieldNumber  2
#define AddResult_Sum2_FieldNumber  3
#define AddResult_Sum3_FieldNumber  4
#define AddResult_dataLen_FieldNumber  5
#define AddResult_data_FieldNumber  6
#define AddResult_Sum4_FieldNumber  7
#define AddResult_Sum5_FieldNumber  8
#define AddResult_Sum6_FieldNumber  9
#define AddResult_Sum7_FieldNumber  10
#define AddResult_PtrTest_FieldNumber  11


typedef struct Student
{
#if MyMacro==TRUE
int32_t a;   //FieldNumber:1     
#endif // #if MyMacro==TRUE
int32_t b;   //FieldNumber:2     
int32_t AddressType;   //FieldNumber:3    Union Target Type 
union{
#if MyMacro==TRUE
uint8_t u1;   //FieldNumber:4     
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
uint64_t u2;   //FieldNumber:5     
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
Sex u3;   //FieldNumber:6     
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
AddResult u4;   //FieldNumber:7     
#endif // #if MyMacro==TRUE
};//union end
#if MyMacro==TRUE
uint8_t uend1;   //FieldNumber:100     
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
uint8_t uend2;   //FieldNumber:8     
#endif // #if MyMacro==TRUE
}Student;
#if MyMacro==TRUE
#define Student_a_FieldNumber  1
#endif // #if MyMacro==TRUE
#define Student_b_FieldNumber  2
#define Student_AddressType_FieldNumber  3
#if MyMacro==TRUE
#define Student_u1_FieldNumber  4
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
#define Student_u2_FieldNumber  5
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
#define Student_u3_FieldNumber  6
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
#define Student_u4_FieldNumber  7
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
#define Student_uend1_FieldNumber  100
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
#define Student_uend2_FieldNumber  8
#endif // #if MyMacro==TRUE


typedef struct DateTime_t
{
int32_t Year;   //FieldNumber:10086     
int32_t Month;   //FieldNumber:2     
int32_t Day;   //FieldNumber:3     
int32_t Hour;   //FieldNumber:4     
int32_t Min;   //FieldNumber:5     
int32_t Sec;   //FieldNumber:6     
Sex Sex;   //FieldNumber:8     
uint8_t DateStringLen;   //FieldNumber:1     
#if MyMacro==TRUE
uint8_t* DateString;   //FieldNumber:7     
#endif // #if MyMacro==TRUE
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
#if MyMacro==TRUE
#define DateTime_t_DateString_FieldNumber  7
#endif // #if MyMacro==TRUE
#define DateTime_t_David_FieldNumber  9


typedef struct DateTimeChange_Return
{
RequestResponseState State;   //FieldNumber:1     
}DateTimeChange_Return;
#define DateTimeChange_Return_State_FieldNumber  1


typedef struct DateTimeChange_Parameter
{
DateTime_t now[1];   //FieldNumber:1     
}DateTimeChange_Parameter;
#define DateTimeChange_Parameter_now_FieldNumber  1


typedef struct Test2_Return
{
RequestResponseState State;   //FieldNumber:1     
}Test2_Return;
#define Test2_Return_State_FieldNumber  1


typedef struct Test2_Parameter
{
DateTime_t now[1];   //FieldNumber:1     
}Test2_Parameter;
#define Test2_Parameter_now_FieldNumber  1


typedef struct Add_Return
{
RequestResponseState State;   //FieldNumber:1     
AddResult ReturnValue;   //FieldNumber:2     
}Add_Return;
#define Add_Return_State_FieldNumber  1
#define Add_Return_ReturnValue_FieldNumber  2


typedef struct Add_Parameter
{
int32_t a;   //FieldNumber:1     
int32_t b;   //FieldNumber:2     
int32_t dataLen;   //FieldNumber:3     
uint8_t* data;   //FieldNumber:4     
}Add_Parameter;
#define Add_Parameter_a_FieldNumber  1
#define Add_Parameter_b_FieldNumber  2
#define Add_Parameter_dataLen_FieldNumber  3
#define Add_Parameter_data_FieldNumber  4


typedef struct NoArg_Return
{
RequestResponseState State;   //FieldNumber:1     
uint8_t ReturnValue;   //FieldNumber:2     
}NoArg_Return;
#define NoArg_Return_State_FieldNumber  1
#define NoArg_Return_ReturnValue_FieldNumber  2


typedef struct NoArg_Parameter
{
}NoArg_Parameter;


typedef struct NoReturn_Return
{
RequestResponseState State;   //FieldNumber:1     
}NoReturn_Return;
#define NoReturn_Return_State_FieldNumber  1


typedef struct NoReturn_Parameter
{
int32_t a;   //FieldNumber:1     
}NoReturn_Parameter;
#define NoReturn_Parameter_a_FieldNumber  1


typedef struct NoArgAndReturn_Return
{
RequestResponseState State;   //FieldNumber:1     
}NoArgAndReturn_Return;
#define NoArgAndReturn_Return_State_FieldNumber  1


typedef struct NoArgAndReturn_Parameter
{
}NoArgAndReturn_Parameter;


#ifdef  __cplusplus
}
#endif


#endif
