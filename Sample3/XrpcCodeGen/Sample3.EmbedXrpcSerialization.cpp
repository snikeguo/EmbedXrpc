#include"Sample3.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void Student_Serialize(SerializationManager *sm,Student *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->c,sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->d,sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->e,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
}

void Student_Deserialize(SerializationManager *sm,Student *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->a,sm,1,sizeof(uint8_t),isIsr);
DeserializeField((uint8_t *)&obj->b,sm,1,sizeof(uint8_t),isIsr);
DeserializeField((uint8_t *)&obj->c,sm,1,sizeof(uint8_t),isIsr);
DeserializeField((uint8_t *)&obj->d,sm,1,sizeof(uint8_t),isIsr);
DeserializeField((uint8_t *)&obj->e,sm,4,sizeof(int32_t),isIsr);
}

//! void Student_FreeData(Student *obj)
//! {
//! }

void DynamicStudentArray_Serialize(SerializationManager *sm,DynamicStudentArray *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->dataLen,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
for(int32_t FalseFixedStudent_index=0;FalseFixedStudent_index<obj->dataLen;FalseFixedStudent_index++)
{
Student_Serialize(sm,&obj->FalseFixedStudent[FalseFixedStudent_index]);
}
for(int32_t TrueFixedStudent_index=0;TrueFixedStudent_index<obj->dataLen;TrueFixedStudent_index++)
{
Student_Serialize(sm,&obj->TrueFixedStudent[TrueFixedStudent_index]);
}
 //NoSerializationStudent:NoSerialization
}

void DynamicStudentArray_Deserialize(SerializationManager *sm,DynamicStudentArray *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->dataLen,sm,4,sizeof(int32_t),isIsr);
obj->FalseFixedStudent=(Student *)El_Malloc(sizeof(Student)*obj->dataLen);
El_Memset(obj->FalseFixedStudent,0,sizeof(Student)*obj->dataLen);
for(int32_t FalseFixedStudent_index=0;FalseFixedStudent_index<obj->dataLen;FalseFixedStudent_index++)
{
Student_Deserialize(sm,&obj->FalseFixedStudent[FalseFixedStudent_index],isIsr);
}
for(int32_t TrueFixedStudent_index=0;TrueFixedStudent_index<obj->dataLen;TrueFixedStudent_index++)
{
Student_Deserialize(sm,&obj->TrueFixedStudent[TrueFixedStudent_index],isIsr);
}
 //NoSerializationStudent:NoSerialization
}

 void DynamicStudentArray_FreeData(DynamicStudentArray *obj)
 {
  //!!!! for(int32_t FalseFixedStudent_index=0;FalseFixedStudent_index<obj->dataLen;FalseFixedStudent_index++)
  //!!!! {
  //!!!! Student_FreeData(&obj->FalseFixedStudent[FalseFixedStudent_index]);
  //!!!! }
  El_Free(obj->FalseFixedStudent);
  //!!!! for(int32_t TrueFixedStudent_index=0;TrueFixedStudent_index<obj->dataLen;TrueFixedStudent_index++)
  //!!!! {
  //!!!! Student_FreeData(&obj->TrueFixedStudent[TrueFixedStudent_index]);
  //!!!! }
 //NoSerializationStudent:NoSerialization 
 }

void MixedStudentArray_Serialize(SerializationManager *sm,MixedStudentArray *obj)
{
DynamicStudentArray_Serialize(sm,&obj->DynamicStudent);
Student_Serialize(sm,&obj->OneStudent);
}

void MixedStudentArray_Deserialize(SerializationManager *sm,MixedStudentArray *obj,int isIsr)
{
DynamicStudentArray_Deserialize(sm,&obj->DynamicStudent,isIsr);
Student_Deserialize(sm,&obj->OneStudent,isIsr);
}

 void MixedStudentArray_FreeData(MixedStudentArray *obj)
 {
   DynamicStudentArray_FreeData(&obj->DynamicStudent);
  //!! Student_FreeData(&obj->OneStudent);
 }

