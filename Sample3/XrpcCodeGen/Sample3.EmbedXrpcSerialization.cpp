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

//!void Student_FreeData(Student *obj)
//!{
//!}

void DynamicStudentArray_Serialize(SerializationManager *sm,DynamicStudentArray *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->dataLen,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Assert(sm->Index<=sm->BufferLen);
for(Int32 FalseFixedStudent_index=0;FalseFixedStudent_index<obj->dataLen;FalseFixedStudent_index++)
{
Student_Serialize(sm,&obj->FalseFixedStudent[FalseFixedStudent_index]);
}
for(Int32 TrueFixedStudent_index=0;TrueFixedStudent_index<obj->dataLen;TrueFixedStudent_index++)
{
Student_Serialize(sm,&obj->TrueFixedStudent[TrueFixedStudent_index]);
}
//NoSerializationStudent :NoSerialization
}

void DynamicStudentArray_Deserialize(SerializationManager *sm,DynamicStudentArray *obj)
{
DeserializeField((uint8_t *)&obj->dataLen,sm,sizeof(Int32));
obj->FalseFixedStudent=(Student *)El_Malloc(sizeof(Student)*obj->dataLen);
El_Memset(obj->FalseFixedStudent,0,sizeof(Student)*obj->dataLen);
for(Int32 FalseFixedStudent_index=0;FalseFixedStudent_index<obj->dataLen;FalseFixedStudent_index++)
{
Student_Deserialize(sm,&obj->FalseFixedStudent[FalseFixedStudent_index]);
}
for(Int32 TrueFixedStudent_index=0;TrueFixedStudent_index<obj->dataLen;TrueFixedStudent_index++)
{
Student_Deserialize(sm,&obj->TrueFixedStudent[TrueFixedStudent_index]);
}
//NoSerializationStudent :NoSerialization
}

void DynamicStudentArray_FreeData(DynamicStudentArray *obj)
{
//!!for(Int32 FalseFixedStudent_index=0;FalseFixedStudent_index<obj->dataLen;FalseFixedStudent_index++)
//!!{
//!!Student_FreeData(&obj->FalseFixedStudent[FalseFixedStudent_index]);
//!!}
El_Free(obj->FalseFixedStudent);
//!!for(Int32 TrueFixedStudent_index=0;TrueFixedStudent_index<obj->dataLen;TrueFixedStudent_index++)
//!!{
//!!Student_FreeData(&obj->TrueFixedStudent[TrueFixedStudent_index]);
//!!}
//NoSerializationStudent :NoSerialization 
}

void MixedStudentArray_Serialize(SerializationManager *sm,MixedStudentArray *obj)
{
DynamicStudentArray_Serialize(sm,&obj->DynamicStudent);
Student_Serialize(sm,&obj->OneStudent);
}

void MixedStudentArray_Deserialize(SerializationManager *sm,MixedStudentArray *obj)
{
DynamicStudentArray_Deserialize(sm,&obj->DynamicStudent);
Student_Deserialize(sm,&obj->OneStudent);
}

void MixedStudentArray_FreeData(MixedStudentArray *obj)
{
DynamicStudentArray_FreeData(&obj->DynamicStudent);
//!!!Student_FreeData(&obj->OneStudent);
}

