#ifndef StudentService_H
#define StudentService_H
#include"EmbedSerializationBaseType.h"
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef struct _GetValue_Request
{
}GetValue_Request;
typedef struct _GetValue_Response
{
ResponseState State;
UInt16 ReturnValue;
}GetValue_Response;
typedef struct _SetValue_Request
{
UInt16 v;
}SetValue_Request;

#endif
