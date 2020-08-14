#ifndef StudentService_H
#define StudentService_H
#include"EmbedSerialization.h"

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

#endif
