#ifndef StudentService_H
#define StudentService_H
#include"EmbedSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

typedef struct _Test
{
Byte Value2;
}Test;
typedef struct _Achievement
{
Byte Value;
Test test;
}Achievement;
typedef struct _Student
{
Achievement ChineseAchievement;
Int32 A;
Int32 B;
Int32 C;
}Student;

#endif
