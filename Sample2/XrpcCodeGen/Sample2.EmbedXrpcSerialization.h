#ifndef Sample2_EmbedXrpcSerialization_H
#define Sample2_EmbedXrpcSerialization_H
#include"Sample2.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

#ifndef offsetof
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
#endif
void GetSumResult_Serialize(SerializationManager *sm,GetSumResult *obj);

void GetSumResult_Deserialize(SerializationManager *sm,GetSumResult *obj);

void GetSumResult_FreeData(GetSumResult *obj);

void Inter_GetSum_Parameter_Serialize(SerializationManager *sm,Inter_GetSum_Parameter *obj);

void Inter_GetSum_Parameter_Deserialize(SerializationManager *sm,Inter_GetSum_Parameter *obj);

void Inter_GetSum_Parameter_FreeData(Inter_GetSum_Parameter *obj);

#define Inter_GetSum_ServiceId 16   //0x10
void Inter_GetSum_Return_Serialize(SerializationManager *sm,Inter_GetSum_Return *obj);

void Inter_GetSum_Return_Deserialize(SerializationManager *sm,Inter_GetSum_Return *obj);

void Inter_GetSum_Return_FreeData(Inter_GetSum_Return *obj);


#endif
