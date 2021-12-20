#ifndef Sample2_H
#define Sample2_H
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

struct GetSumResult
{
UInt8 IsSuccess;   //FieldNumber:1     
Int32 Value;   //FieldNumber:2     
};
#define GetSumResult_IsSuccess_FieldNumber  1
#define GetSumResult_Value_FieldNumber  2


struct Inter_GetSum_Parameter
{
Int32 a;   //FieldNumber:1     
Int32 b;   //FieldNumber:2     
};
#define Inter_GetSum_Parameter_a_FieldNumber  1
#define Inter_GetSum_Parameter_b_FieldNumber  2


struct Inter_GetSum_Return
{
RequestResponseState State;   //FieldNumber:1     
GetSumResult ReturnValue;   //FieldNumber:2     
};
#define Inter_GetSum_Return_State_FieldNumber  1
#define Inter_GetSum_Return_ReturnValue_FieldNumber  2



#endif
