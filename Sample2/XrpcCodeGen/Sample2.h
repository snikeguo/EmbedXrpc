#ifndef Sample2_H
#define Sample2_H
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

#ifdef  __cplusplus
extern "C" {
#endif

typedef struct GetSumResult
{
UInt8 IsSuccess;   //FieldNumber:1     
Int32 Value;   //FieldNumber:2     
}GetSumResult;
#define GetSumResult_IsSuccess_FieldNumber  1
#define GetSumResult_Value_FieldNumber  2


typedef struct GetSum_Return
{
RequestResponseState State;   //FieldNumber:1     
GetSumResult ReturnValue;   //FieldNumber:2     
}GetSum_Return;
#define GetSum_Return_State_FieldNumber  1
#define GetSum_Return_ReturnValue_FieldNumber  2


typedef struct GetSum_Parameter
{
Int32 a;   //FieldNumber:1     
Int32 b;   //FieldNumber:2     
}GetSum_Parameter;
#define GetSum_Parameter_a_FieldNumber  1
#define GetSum_Parameter_b_FieldNumber  2


#ifdef  __cplusplus
}
#endif


#endif
