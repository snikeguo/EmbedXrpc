#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void AddResult_Serialize(SerializationManager *sm,AddResult *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->boolTest,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
uint32_t bitsTempValue0=0;
bitsTempValue0 |= ((uint32_t)(obj->Sum))<< 0 ;
bitsTempValue0 |= ((uint32_t)(obj->Sum2))<< 11 ;
bitsTempValue0 |= ((uint32_t)(obj->Sum3))<< 30 ;
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue0,sizeof(uint32_t));
sm->Index+=sizeof(uint32_t);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->dataLen,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
//! //data:NoSerialization
uint64_t bitsTempValue1=0;
bitsTempValue1 |= ((uint64_t)(obj->Sum4))<< 0 ;
bitsTempValue1 |= ((uint64_t)(obj->Sum5))<< 11 ;
bitsTempValue1 |= ((uint64_t)(obj->Sum6))<< 30 ;
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue1,sizeof(uint64_t));
sm->Index+=sizeof(uint64_t);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Sum7,sizeof(uint32_t));
sm->Index+=sizeof(uint32_t);
El_Assert(sm->Index<=sm->BufferLen);
//! //PtrTest:NoSerialization
}

void AddResult_Deserialize(SerializationManager *sm,AddResult *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->boolTest,sm,sizeof(uint8_t),isIsr);
uint32_t bitsTempValue0=0;
DeserializeField((uint8_t *)&bitsTempValue0,sm,sizeof(uint32_t),isIsr);
obj->Sum=bitsTempValue0>>0;
obj->Sum2=bitsTempValue0>>11;
obj->Sum3=bitsTempValue0>>30;
DeserializeField((uint8_t *)&obj->dataLen,sm,sizeof(int32_t),isIsr);
//! //data:NoSerialization
uint64_t bitsTempValue1=0;
DeserializeField((uint8_t *)&bitsTempValue1,sm,sizeof(uint64_t),isIsr);
obj->Sum4=bitsTempValue1>>0;
obj->Sum5=bitsTempValue1>>11;
obj->Sum6=bitsTempValue1>>30;
DeserializeField((uint8_t *)&obj->Sum7,sm,sizeof(uint32_t),isIsr);
//! //PtrTest:NoSerialization
}

//! void AddResult_FreeData(AddResult *obj)
//! {
//! //data:NoSerialization 
//! //PtrTest:NoSerialization 
//! }

void Student_Serialize(SerializationManager *sm,Student *obj)
{
#if MyMacro==TRUE
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
#endif // #if MyMacro==TRUE
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->AddressType,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
switch(obj->AddressType)
{
#if MyMacro==TRUE
case Student_u1_FieldNumber:
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->u1,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u2_FieldNumber:
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->u2,sizeof(uint64_t));
sm->Index+=sizeof(uint64_t);
El_Assert(sm->Index<=sm->BufferLen);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u3_FieldNumber:
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->u3,sizeof(uint64_t));
sm->Index+=sizeof(uint64_t);
El_Assert(sm->Index<=sm->BufferLen);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u4_FieldNumber:
AddResult_Serialize(sm,&obj->u4);
break;
#endif // #if MyMacro==TRUE
}
#if MyMacro==TRUE
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->uend1,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->uend2,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
#endif // #if MyMacro==TRUE
}

void Student_Deserialize(SerializationManager *sm,Student *obj,int isIsr)
{
#if MyMacro==TRUE
DeserializeField((uint8_t *)&obj->a,sm,sizeof(int32_t),isIsr);
#endif // #if MyMacro==TRUE
DeserializeField((uint8_t *)&obj->b,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->AddressType,sm,sizeof(int32_t),isIsr);
switch(obj->AddressType)
{
#if MyMacro==TRUE
case Student_u1_FieldNumber:
DeserializeField((uint8_t *)&obj->u1,sm,sizeof(uint8_t),isIsr);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u2_FieldNumber:
DeserializeField((uint8_t *)&obj->u2,sm,sizeof(uint64_t),isIsr);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u3_FieldNumber:
DeserializeField((uint8_t *)&obj->u3,sm,sizeof(uint64_t),isIsr);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u4_FieldNumber:
AddResult_Deserialize(sm,&obj->u4,isIsr);
break;
#endif // #if MyMacro==TRUE
}
#if MyMacro==TRUE
DeserializeField((uint8_t *)&obj->uend1,sm,sizeof(uint8_t),isIsr);
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
DeserializeField((uint8_t *)&obj->uend2,sm,sizeof(uint8_t),isIsr);
#endif // #if MyMacro==TRUE
}

//! void Student_FreeData(Student *obj)
//! {
//!  #if MyMacro==TRUE
//!  #endif // #if MyMacro==TRUE
//! switch(obj->AddressType)
//! {
//!  #if MyMacro==TRUE
//! case Student_u1_FieldNumber:
//! break;
//!  #endif // #if MyMacro==TRUE
//!  #if MyMacro==TRUE
//! case Student_u2_FieldNumber:
//! break;
//!  #endif // #if MyMacro==TRUE
//!  #if MyMacro==TRUE
//! case Student_u3_FieldNumber:
//! break;
//!  #endif // #if MyMacro==TRUE
//!  #if MyMacro==TRUE
//! case Student_u4_FieldNumber:
//!  //!! AddResult_FreeData(&obj->u4);
//! break;
//!  #endif // #if MyMacro==TRUE
//! }
//!  #if MyMacro==TRUE
//!  #endif // #if MyMacro==TRUE
//!  #if MyMacro==TRUE
//!  #endif // #if MyMacro==TRUE
//! }

void DateTime_t_Serialize(SerializationManager *sm,DateTime_t *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Year,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Month,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Day,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Hour,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Min,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Sec,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Sex,sizeof(uint64_t));
sm->Index+=sizeof(uint64_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->DateStringLen,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
#if MyMacro==TRUE
for(uint8_t DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->DateString[DateString_index],sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
}
#endif // #if MyMacro==TRUE
Student_Serialize(sm,&obj->David);
}

void DateTime_t_Deserialize(SerializationManager *sm,DateTime_t *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->Year,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Month,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Day,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Hour,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Min,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Sec,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Sex,sm,sizeof(uint64_t),isIsr);
DeserializeField((uint8_t *)&obj->DateStringLen,sm,sizeof(uint8_t),isIsr);
#if MyMacro==TRUE
obj->DateString=(uint8_t *)El_Malloc(sizeof(uint8_t)*obj->DateStringLen);
El_Memset(obj->DateString,0,sizeof(uint8_t)*obj->DateStringLen);
for(uint8_t DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
{
DeserializeField((uint8_t *)&obj->DateString[DateString_index],sm,sizeof(uint8_t),isIsr);
}
#endif // #if MyMacro==TRUE
Student_Deserialize(sm,&obj->David,isIsr);
}

 void DateTime_t_FreeData(DateTime_t *obj)
 {
  #if MyMacro==TRUE
  //!!!! for(uint8_t DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
  //!!!! {
  //!!!! }
  El_Free(obj->DateString);
  #endif // #if MyMacro==TRUE
  //!! Student_FreeData(&obj->David);
 }

void DateTimeChange_Return_Serialize(SerializationManager *sm,DateTimeChange_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
}

void DateTimeChange_Return_Deserialize(SerializationManager *sm,DateTimeChange_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(uint8_t),isIsr);
}

//! void DateTimeChange_Return_FreeData(DateTimeChange_Return *obj)
//! {
//! }

void DateTimeChange_Parameter_Serialize(SerializationManager *sm,DateTimeChange_Parameter *obj)
{
for(uint32_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Serialize(sm,&obj->now[now_index]);
}
}

void DateTimeChange_Parameter_Deserialize(SerializationManager *sm,DateTimeChange_Parameter *obj,int isIsr)
{
for(uint32_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Deserialize(sm,&obj->now[now_index],isIsr);
}
}

 void DateTimeChange_Parameter_FreeData(DateTimeChange_Parameter *obj)
 {
   for(uint32_t now_index=0;now_index<1;now_index++)
   {
   DateTime_t_FreeData(&obj->now[now_index]);
   }
 }

void Test2_Return_Serialize(SerializationManager *sm,Test2_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
}

void Test2_Return_Deserialize(SerializationManager *sm,Test2_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(uint8_t),isIsr);
}

//! void Test2_Return_FreeData(Test2_Return *obj)
//! {
//! }

void Test2_Parameter_Serialize(SerializationManager *sm,Test2_Parameter *obj)
{
for(uint32_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Serialize(sm,&obj->now[now_index]);
}
}

void Test2_Parameter_Deserialize(SerializationManager *sm,Test2_Parameter *obj,int isIsr)
{
for(uint32_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Deserialize(sm,&obj->now[now_index],isIsr);
}
}

 void Test2_Parameter_FreeData(Test2_Parameter *obj)
 {
   for(uint32_t now_index=0;now_index<1;now_index++)
   {
   DateTime_t_FreeData(&obj->now[now_index]);
   }
 }

void Add_Return_Serialize(SerializationManager *sm,Add_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
AddResult_Serialize(sm,&obj->ReturnValue);
}

void Add_Return_Deserialize(SerializationManager *sm,Add_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(uint8_t),isIsr);
AddResult_Deserialize(sm,&obj->ReturnValue,isIsr);
}

//! void Add_Return_FreeData(Add_Return *obj)
//! {
//!  //!! AddResult_FreeData(&obj->ReturnValue);
//! }

void Add_Parameter_Serialize(SerializationManager *sm,Add_Parameter *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->dataLen,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
for(int32_t data_index=0;data_index<obj->dataLen;data_index++)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->data[data_index],sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
}
}

void Add_Parameter_Deserialize(SerializationManager *sm,Add_Parameter *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->b,sm,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->dataLen,sm,sizeof(int32_t),isIsr);
obj->data=(uint8_t *)El_Malloc(sizeof(uint8_t)*obj->dataLen);
El_Memset(obj->data,0,sizeof(uint8_t)*obj->dataLen);
for(int32_t data_index=0;data_index<obj->dataLen;data_index++)
{
DeserializeField((uint8_t *)&obj->data[data_index],sm,sizeof(uint8_t),isIsr);
}
}

 void Add_Parameter_FreeData(Add_Parameter *obj)
 {
  //!!!! for(int32_t data_index=0;data_index<obj->dataLen;data_index++)
  //!!!! {
  //!!!! }
  El_Free(obj->data);
 }

void NoArg_Return_Serialize(SerializationManager *sm,NoArg_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->ReturnValue,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
}

void NoArg_Return_Deserialize(SerializationManager *sm,NoArg_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(uint8_t),isIsr);
DeserializeField((uint8_t *)&obj->ReturnValue,sm,sizeof(uint8_t),isIsr);
}

//! void NoArg_Return_FreeData(NoArg_Return *obj)
//! {
//! }

void NoArg_Parameter_Serialize(SerializationManager *sm,NoArg_Parameter *obj)
{
}

void NoArg_Parameter_Deserialize(SerializationManager *sm,NoArg_Parameter *obj,int isIsr)
{
}

//! void NoArg_Parameter_FreeData(NoArg_Parameter *obj)
//! {
//! }

void NoReturn_Return_Serialize(SerializationManager *sm,NoReturn_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
}

void NoReturn_Return_Deserialize(SerializationManager *sm,NoReturn_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(uint8_t),isIsr);
}

//! void NoReturn_Return_FreeData(NoReturn_Return *obj)
//! {
//! }

void NoReturn_Parameter_Serialize(SerializationManager *sm,NoReturn_Parameter *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(int32_t));
sm->Index+=sizeof(int32_t);
El_Assert(sm->Index<=sm->BufferLen);
}

void NoReturn_Parameter_Deserialize(SerializationManager *sm,NoReturn_Parameter *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(int32_t),isIsr);
}

//! void NoReturn_Parameter_FreeData(NoReturn_Parameter *obj)
//! {
//! }

void NoArgAndReturn_Return_Serialize(SerializationManager *sm,NoArgAndReturn_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(uint8_t));
sm->Index+=sizeof(uint8_t);
El_Assert(sm->Index<=sm->BufferLen);
}

void NoArgAndReturn_Return_Deserialize(SerializationManager *sm,NoArgAndReturn_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(uint8_t),isIsr);
}

//! void NoArgAndReturn_Return_FreeData(NoArgAndReturn_Return *obj)
//! {
//! }

void NoArgAndReturn_Parameter_Serialize(SerializationManager *sm,NoArgAndReturn_Parameter *obj)
{
}

void NoArgAndReturn_Parameter_Deserialize(SerializationManager *sm,NoArgAndReturn_Parameter *obj,int isIsr)
{
}

//! void NoArgAndReturn_Parameter_FreeData(NoArgAndReturn_Parameter *obj)
//! {
//! }

