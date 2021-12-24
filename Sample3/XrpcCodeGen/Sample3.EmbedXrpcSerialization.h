#ifndef Sample3_EmbedXrpcSerialization_H
#define Sample3_EmbedXrpcSerialization_H
#include"Sample3.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

#ifndef offsetof
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
#endif
void Student_Serialize(SerializationManager *sm,Student *obj);

void Student_Deserialize(SerializationManager *sm,Student *obj);

void Student_FreeData(Student *obj);


#endif
