#ifndef Sample3_H
#define Sample3_H
#include"EmbedSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

struct Student
{
UInt8 a;   //FieldNumber:1     
UInt8 b;   //FieldNumber:2     
UInt8 c;   //FieldNumber:3     
UInt8 d;   //FieldNumber:4     
Int32 e;   //FieldNumber:5     
};
#define Student_a_FieldNumber  1
#define Student_b_FieldNumber  2
#define Student_c_FieldNumber  3
#define Student_d_FieldNumber  4
#define Student_e_FieldNumber  5


struct DynamicStudentArray
{
Int32 dataLen;   //FieldNumber:2     
Student* FalseFixedStudent;   //FieldNumber:1     
Student TrueFixedStudent[15];   //FieldNumber:3     
Student* NoSerializationStudent;   //FieldNumber:4     NoSerialization
};
#define DynamicStudentArray_dataLen_FieldNumber  2
#define DynamicStudentArray_FalseFixedStudent_FieldNumber  1
#define DynamicStudentArray_TrueFixedStudent_FieldNumber  3
#define DynamicStudentArray_NoSerializationStudent_FieldNumber  4


struct MixedStudentArray
{
DynamicStudentArray DynamicStudent;   //FieldNumber:1     
Student OneStudent;   //FieldNumber:2     
};
#define MixedStudentArray_DynamicStudent_FieldNumber  1
#define MixedStudentArray_OneStudent_FieldNumber  2



#endif
