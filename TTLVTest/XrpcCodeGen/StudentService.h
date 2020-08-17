#ifndef StudentService_H
#define StudentService_H
#include"EmbedSerialization.h"
#include"EmbedXrpcCommon.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef struct _Test
{
Byte IntPtrsLen;   //FieldNumber:10
Int32* IntPtr;   //FieldNumber:11
}Test;
typedef struct _Achievement
{
Byte Arlen;   //FieldNumber:10
Test* Ar;   //FieldNumber:8
}Achievement;
typedef struct _Student
{
Achievement ChineseAchievement;   //FieldNumber:4
}Student;
typedef struct _DelegateTestStruct
{
Int32 index;   //FieldNumber:1
Student student;   //FieldNumber:2
}DelegateTestStruct;
typedef struct _MyInerface_GetStudent_Request
{
Int32 index;   //FieldNumber:1
}MyInerface_GetStudent_Request;
typedef struct _MyInerface_GetStudent_RequestResponseContent
{
RequestResponseState State;   //FieldNumber:1
Student ReturnValue;   //FieldNumber:2
}MyInerface_GetStudent_RequestResponseContent;

#endif
