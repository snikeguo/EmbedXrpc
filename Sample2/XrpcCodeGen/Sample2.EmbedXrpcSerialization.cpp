#include"Sample2.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void GetSumResult_Serialize(SerializationManager *sm,GetSumResult *obj)
{
if(sm->Buf) *(bool *)(&sm->Buf[sm->Index])=obj->IsSuccess;
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(int32_t *)(&sm->Buf[sm->Index])=obj->Value;
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
}

void GetSumResult_Deserialize(SerializationManager *sm,GetSumResult *obj,int isIsr,void *cmp_field_ptr,uint32_t index)
{
    if(cmp_ptr!=NULL&&cmp_ptr==(void*)&obj->IsSuccess)
    {
        //DeserializeField(NULL,sm,1,sizeof(bool),isIsr);
        return;
    }
    else if(obj!=NULL)
    {
        DeserializeField((uint8_t *)&obj->IsSuccess,sm,1,sizeof(bool),isIsr);
    }
    else
    {
        DeserializeField((uint8_t *)NULL,sm,1,sizeof(bool),isIsr);
    }
DeserializeField((uint8_t *)&obj->Value,sm,4,sizeof(int32_t),isIsr);
}

//! void GetSumResult_FreeData(GetSumResult *obj)
//! {
//! }

void GetSum_Return_Serialize(SerializationManager *sm,GetSum_Return *obj)
{
if(sm->Buf) *(RequestResponseState *)(&sm->Buf[sm->Index])=obj->State;
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
GetSumResult_Serialize(sm,&obj->ReturnValue);
}

void GetSum_Return_Deserialize(SerializationManager *sm,GetSum_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,1,sizeof(RequestResponseState),isIsr);
GetSumResult_Deserialize(sm,&obj->ReturnValue,isIsr);
}

//! void GetSum_Return_FreeData(GetSum_Return *obj)
//! {
//!  //!! GetSumResult_FreeData(&obj->ReturnValue);
//! }

void GetSum_Parameter_Serialize(SerializationManager *sm,GetSum_Parameter *obj)
{
if(sm->Buf) *(int32_t *)(&sm->Buf[sm->Index])=obj->a;
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) *(int32_t *)(&sm->Buf[sm->Index])=obj->b;
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
}

void GetSum_Parameter_Deserialize(SerializationManager *sm,GetSum_Parameter *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->a,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->b,sm,4,sizeof(int32_t),isIsr);
}

//! void GetSum_Parameter_FreeData(GetSum_Parameter *obj)
//! {
//! }

