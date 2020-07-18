#ifndef Sample1_H
#define Sample1_H
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef struct _DateTime_t
{
Int32 Year;
Int32 Month;
Int32 Day;
Int32 Hour;
Int32 Min;
Int32 Sec;
}DateTime_t;
typedef struct _DateTimeChangeStruct
{
DateTime_t now[1];
}DateTimeChangeStruct;
typedef struct _Inter_Add_Request
{
Int32 a;
Int32 b;
}Inter_Add_Request;
typedef struct _Inter_Add_RequestResponseContent
{
RequestResponseState State;
Int32 ReturnValue;
}Inter_Add_RequestResponseContent;
typedef struct _Inter_NoArg_Request
{
}Inter_NoArg_Request;
typedef struct _Inter_NoArg_RequestResponseContent
{
RequestResponseState State;
Boolean ReturnValue;
}Inter_NoArg_RequestResponseContent;
typedef struct _Inter_NoReturn_Request
{
}Inter_NoReturn_Request;
typedef struct _Inter_NoReturn_RequestResponseContent
{
RequestResponseState State;
}Inter_NoReturn_RequestResponseContent;
typedef struct _Inter_NoArgAndReturn_Request
{
}Inter_NoArgAndReturn_Request;
typedef struct _Inter_NoArgAndReturn_RequestResponseContent
{
RequestResponseState State;
}Inter_NoArgAndReturn_RequestResponseContent;

#endif
