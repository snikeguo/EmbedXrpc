﻿#ifndef Sample1_H
#define Sample1_H
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef struct _DateTime_t
{
Int32 Year;   //FieldNumber:10086
Int32 Month;   //FieldNumber:2
Int32 Day;   //FieldNumber:3
Int32 Hour;   //FieldNumber:4
Int32 Min;   //FieldNumber:5
Int32 Sec;   //FieldNumber:6
}DateTime_t;
typedef struct _AddResult
{
Int32 Sum;   //FieldNumber:1
Int32 dataLen;   //FieldNumber:2
Byte* data;   //FieldNumber:3
}AddResult;
typedef struct _DateTimeChange_Parameter
{
DateTime_t now[1];   //FieldNumber:1
}DateTimeChange_Parameter;
typedef struct _Inter_Add_Parameter
{
Int32 a;   //FieldNumber:1
Int32 b;   //FieldNumber:2
Int32 dataLen;   //FieldNumber:3
Byte* data;   //FieldNumber:4
}Inter_Add_Parameter;
typedef struct _Inter_Add_Return
{
RequestResponseState State;   //FieldNumber:1
AddResult ReturnValue;   //FieldNumber:2
}Inter_Add_Return;
typedef struct _Inter_NoArg_Parameter
{
}Inter_NoArg_Parameter;
typedef struct _Inter_NoArg_Return
{
RequestResponseState State;   //FieldNumber:1
Boolean ReturnValue;   //FieldNumber:2
}Inter_NoArg_Return;
typedef struct _Inter_NoReturn_Parameter
{
Int32 a;   //FieldNumber:1
}Inter_NoReturn_Parameter;
typedef struct _Inter_NoReturn_Return
{
RequestResponseState State;   //FieldNumber:1
}Inter_NoReturn_Return;
typedef struct _Inter_NoArgAndReturn_Parameter
{
}Inter_NoArgAndReturn_Parameter;
typedef struct _Inter_NoArgAndReturn_Return
{
RequestResponseState State;   //FieldNumber:1
}Inter_NoArgAndReturn_Return;

#endif
