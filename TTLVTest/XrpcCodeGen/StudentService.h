#ifndef StudentService_H
#define StudentService_H
#include"EmbedSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef struct _Test
{
Byte IntPtrsLen;
Int32* IntPtr;
}Test;
typedef struct _Achievement
{
Byte Arlen;
Test* Ar;
}Achievement;
typedef struct _Student
{
Achievement ChineseAchievement;
}Student;

#endif
