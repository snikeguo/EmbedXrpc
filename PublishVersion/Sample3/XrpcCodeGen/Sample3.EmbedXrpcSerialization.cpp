#include"Sample3.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void Student_Serialize(SerializationManager *sm,Student *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Assert(sm->Index<=sm->BufferLen);
El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Assert(sm->Index<=sm->BufferLen);
El_Memcpy(&sm->Buf[sm->Index],&obj->c,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Assert(sm->Index<=sm->BufferLen);
El_Memcpy(&sm->Buf[sm->Index],&obj->d,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Assert(sm->Index<=sm->BufferLen);
El_Memcpy(&sm->Buf[sm->Index],&obj->e,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Assert(sm->Index<=sm->BufferLen);
}

void Student_Deserialize(SerializationManager *sm,Student *obj)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(UInt8));
DeserializeField((uint8_t *)&obj->b,sm,sizeof(UInt8));
DeserializeField((uint8_t *)&obj->c,sm,sizeof(UInt8));
DeserializeField((uint8_t *)&obj->d,sm,sizeof(UInt8));
DeserializeField((uint8_t *)&obj->e,sm,sizeof(Int32));
}

void Student_FreeData(Student *obj)
{
}

