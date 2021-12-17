#ifndef Sample2_EmbedXrpcSerialization_H
#define Sample2_EmbedXrpcSerialization_H
#include"Sample2.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

#ifndef offsetof
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
#endif
template<class DTL>
void GetSumResult_Serialize(SerializationManager<DTL> *sm,GetSumResult *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->IsSuccess,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Assert(sm->Index<=sm->BufferLen);
El_Memcpy(&sm->Buf[sm->Index],&obj->Value,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Assert(sm->Index<=sm->BufferLen);
}

template<class DTL>
void GetSumResult_Deserialize(SerializationManager<DTL> *sm,GetSumResult *obj)
{
DeserializeField((uint8_t *)&obj->IsSuccess,sm,sizeof(UInt8));
DeserializeField((uint8_t *)&obj->Value,sm,sizeof(Int32));
}

template<class DTL>
void GetSumResult_FreeData(GetSumResult *obj)
{
}

template<class DTL>
void Inter_GetSum_Parameter_Serialize(SerializationManager<DTL> *sm,Inter_GetSum_Parameter *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Assert(sm->Index<=sm->BufferLen);
El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Assert(sm->Index<=sm->BufferLen);
}

template<class DTL>
void Inter_GetSum_Parameter_Deserialize(SerializationManager<DTL> *sm,Inter_GetSum_Parameter *obj)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->b,sm,sizeof(Int32));
}

template<class DTL>
void Inter_GetSum_Parameter_FreeData(Inter_GetSum_Parameter *obj)
{
}

#define Inter_GetSum_ServiceId 16   //0x10
template<class DTL>
void Inter_GetSum_Return_Serialize(SerializationManager<DTL> *sm,Inter_GetSum_Return *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Assert(sm->Index<=sm->BufferLen);
GetSumResult_Serialize(sm,&obj->ReturnValue);
}

template<class DTL>
void Inter_GetSum_Return_Deserialize(SerializationManager<DTL> *sm,Inter_GetSum_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
GetSumResult_Deserialize(sm,&obj->ReturnValue);
}

template<class DTL>
void Inter_GetSum_Return_FreeData(Inter_GetSum_Return *obj)
{
GetSumResult_FreeData<DTL>(&obj->ReturnValue);
}


#endif
