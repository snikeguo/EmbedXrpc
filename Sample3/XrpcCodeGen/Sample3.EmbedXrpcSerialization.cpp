#include"Sample3.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void Student_Serialize(SerializationManager *sm,Student *obj)
{
if(sm->Buf) *(uint8_t *)(&sm->Buf[sm->Index])=obj->u8;
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(int8_t *)(&sm->Buf[sm->Index])=obj->s8;
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(uint16_t *)(&sm->Buf[sm->Index])=obj->u16;
sm->Index+=2;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(int16_t *)(&sm->Buf[sm->Index])=obj->i16;
sm->Index+=2;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(uint32_t *)(&sm->Buf[sm->Index])=obj->u32;
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(int32_t *)(&sm->Buf[sm->Index])=obj->i32;
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(uint64_t *)(&sm->Buf[sm->Index])=obj->u64;
sm->Index+=8;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(int64_t *)(&sm->Buf[sm->Index])=obj->i64;
sm->Index+=8;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(float *)(&sm->Buf[sm->Index])=obj->f;
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(double *)(&sm->Buf[sm->Index])=obj->d;
sm->Index+=8;
El_Assert(sm->Index<=sm->BufferLen);
}

void Student_Deserialize(SerializationManager *sm,Student *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->u8,sm,1,sizeof(uint8_t),isIsr);
DeserializeField((uint8_t *)&obj->s8,sm,1,sizeof(int8_t),isIsr);
DeserializeField((uint8_t *)&obj->u16,sm,2,sizeof(uint16_t),isIsr);
DeserializeField((uint8_t *)&obj->i16,sm,2,sizeof(int16_t),isIsr);
DeserializeField((uint8_t *)&obj->u32,sm,4,sizeof(uint32_t),isIsr);
DeserializeField((uint8_t *)&obj->i32,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->u64,sm,8,sizeof(uint64_t),isIsr);
DeserializeField((uint8_t *)&obj->i64,sm,8,sizeof(int64_t),isIsr);
DeserializeField((uint8_t *)&obj->f,sm,4,sizeof(float),isIsr);
DeserializeField((uint8_t *)&obj->d,sm,8,sizeof(double),isIsr);
}

//! void Student_FreeData(Student *obj)
//! {
//! }

