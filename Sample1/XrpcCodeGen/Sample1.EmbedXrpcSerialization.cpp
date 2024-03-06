#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void AddResult_Serialize(SerializationManager *sm,AddResult *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->boolTest,sizeof(bool));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
uint32_t bitsTempValue0=0;
bitsTempValue0 |= ((uint32_t)(obj->Sum))<< 0 ;
bitsTempValue0 |= ((uint32_t)(obj->Sum2))<< 11 ;
bitsTempValue0 |= ((uint32_t)(obj->Sum3))<< 30 ;
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue0,sizeof(uint32_t));
sm->Index+=4;
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->dataLen,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
//data:NoSerialization
uint64_t bitsTempValue1=0;
bitsTempValue1 |= ((uint64_t)(obj->Sum4))<< 0 ;
bitsTempValue1 |= ((uint64_t)(obj->Sum5))<< 11 ;
bitsTempValue1 |= ((uint64_t)(obj->Sum6))<< 30 ;
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue1,sizeof(uint64_t));
sm->Index+=8;
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Sum7,sizeof(uint32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
//PtrTest:NoSerialization
}

void AddResult_Deserialize(SerializationManager *sm,AddResult *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->boolTest,sm,1,sizeof(bool),isIsr);
uint32_t bitsTempValue0=0;
DeserializeField((uint8_t *)&bitsTempValue0,sm,4,sizeof(uint32_t),isIsr);
obj->Sum=bitsTempValue0>>0;
obj->Sum2=bitsTempValue0>>11;
obj->Sum3=bitsTempValue0>>30;
DeserializeField((uint8_t *)&obj->dataLen,sm,4,sizeof(int32_t),isIsr);
//data:NoSerialization
uint64_t bitsTempValue1=0;
DeserializeField((uint8_t *)&bitsTempValue1,sm,8,sizeof(uint64_t),isIsr);
obj->Sum4=bitsTempValue1>>0;
obj->Sum5=bitsTempValue1>>11;
obj->Sum6=bitsTempValue1>>30;
DeserializeField((uint8_t *)&obj->Sum7,sm,4,sizeof(uint32_t),isIsr);
//PtrTest:NoSerialization
}

//StructFreeLabel void AddResult_FreeData(AddResult *obj)
//StructFreeLabel {
//StructFreeLabel //NoSerializeAttrLabel //data:NoSerialization 
//StructFreeLabel //NoSerializeAttrLabel //PtrTest:NoSerialization 
//StructFreeLabel }

void Student_Serialize(SerializationManager *sm,Student *obj)
{
#if MyMacro==TRUE
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
#endif // #if MyMacro==TRUE
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->AddressType,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
switch(obj->AddressType)
{
#if MyMacro==TRUE
case Student_u1_FieldNumber:
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->u1,sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u2_FieldNumber:
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->u2,sizeof(uint64_t));
sm->Index+=8;
El_Assert(sm->Index<=sm->BufferLen);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u3_FieldNumber:
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->u3,sizeof(Sex)<=8?sizeof(Sex):8);
sm->Index+=8;
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
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->uend2,sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
#endif // #if MyMacro==TRUE
}

void Student_Deserialize(SerializationManager *sm,Student *obj,int isIsr)
{
#if MyMacro==TRUE
DeserializeField((uint8_t *)&obj->a,sm,4,sizeof(int32_t),isIsr);
#endif // #if MyMacro==TRUE
DeserializeField((uint8_t *)&obj->b,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->AddressType,sm,4,sizeof(int32_t),isIsr);
switch(obj->AddressType)
{
#if MyMacro==TRUE
case Student_u1_FieldNumber:
DeserializeField((uint8_t *)&obj->u1,sm,1,sizeof(uint8_t),isIsr);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u2_FieldNumber:
DeserializeField((uint8_t *)&obj->u2,sm,8,sizeof(uint64_t),isIsr);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u3_FieldNumber:
DeserializeField((uint8_t *)&obj->u3,sm,8,sizeof(Sex),isIsr);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u4_FieldNumber:
AddResult_Deserialize(sm,&obj->u4,isIsr);
break;
#endif // #if MyMacro==TRUE
}
#if MyMacro==TRUE
DeserializeField((uint8_t *)&obj->uend1,sm,1,sizeof(uint8_t),isIsr);
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
DeserializeField((uint8_t *)&obj->uend2,sm,1,sizeof(uint8_t),isIsr);
#endif // #if MyMacro==TRUE
}

//StructFreeLabel void Student_FreeData(Student *obj)
//StructFreeLabel {
//StructFreeLabel  #if MyMacro==TRUE
//StructFreeLabel  #endif // #if MyMacro==TRUE
//StructFreeLabel switch(obj->AddressType)
//StructFreeLabel {
//StructFreeLabel  #if MyMacro==TRUE
//StructFreeLabel case Student_u1_FieldNumber:
//StructFreeLabel break;
//StructFreeLabel  #endif // #if MyMacro==TRUE
//StructFreeLabel  #if MyMacro==TRUE
//StructFreeLabel case Student_u2_FieldNumber:
//StructFreeLabel break;
//StructFreeLabel  #endif // #if MyMacro==TRUE
//StructFreeLabel  #if MyMacro==TRUE
//StructFreeLabel case Student_u3_FieldNumber:
//StructFreeLabel break;
//StructFreeLabel  #endif // #if MyMacro==TRUE
//StructFreeLabel  #if MyMacro==TRUE
//StructFreeLabel case Student_u4_FieldNumber:
//StructFreeLabel  //FieldNeedFreeMemoryLabel AddResult_FreeData(&obj->u4);
//StructFreeLabel break;
//StructFreeLabel  #endif // #if MyMacro==TRUE
//StructFreeLabel }
//StructFreeLabel  #if MyMacro==TRUE
//StructFreeLabel  #endif // #if MyMacro==TRUE
//StructFreeLabel  #if MyMacro==TRUE
//StructFreeLabel  #endif // #if MyMacro==TRUE
//StructFreeLabel }

void DateTime_t_Serialize(SerializationManager *sm,DateTime_t *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Year,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Month,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Day,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Hour,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Min,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Sec,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->Sex,sizeof(Sex)<=8?sizeof(Sex):8);
sm->Index+=8;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->DateStringLen,sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
#if MyMacro==TRUE
El_Assert(obj->DateStringLen<=1);
for(uint8_t DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->DateString[DateString_index],sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
}
#endif // #if MyMacro==TRUE
Student_Serialize(sm,&obj->David);
}

void DateTime_t_Deserialize(SerializationManager *sm,DateTime_t *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->Year,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Month,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Day,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Hour,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Min,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Sec,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->Sex,sm,8,sizeof(Sex),isIsr);
DeserializeField((uint8_t *)&obj->DateStringLen,sm,1,sizeof(uint8_t),isIsr);
#if MyMacro==TRUE
obj->DateString=(uint8_t *)El_Malloc(sizeof(uint8_t)*obj->DateStringLen);
El_Memset(obj->DateString,0,sizeof(uint8_t)*obj->DateStringLen);
El_Assert(obj->DateStringLen<=1);
for(uint8_t DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
{
DeserializeField((uint8_t *)&obj->DateString[DateString_index],sm,1,sizeof(uint8_t),isIsr);
}
#endif // #if MyMacro==TRUE
Student_Deserialize(sm,&obj->David,isIsr);
}

 void DateTime_t_FreeData(DateTime_t *obj)
 {
  #if MyMacro==TRUE
  //ElementTypeFreeLabel for(uint8_t DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
  //ElementTypeFreeLabel {
  //ElementTypeFreeLabel }
  El_Free(obj->DateString);
  #endif // #if MyMacro==TRUE
  //FieldNeedFreeMemoryLabel Student_FreeData(&obj->David);
 }

void TestSerialize_Serialize(SerializationManager *sm,TestSerialize *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->EnumArrayLen,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
El_Assert(obj->EnumArrayLen<=1);
for(int32_t EnumArray_index=0;EnumArray_index<obj->EnumArrayLen;EnumArray_index++)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->EnumArray[EnumArray_index],sizeof(Sex)<=8?sizeof(Sex):8);
sm->Index+=8;
El_Assert(sm->Index<=sm->BufferLen);
}
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->ObjectArrayLen,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
El_Assert(obj->EnumArrayLen<=1);
for(int32_t DateTimeArray_index=0;DateTimeArray_index<obj->EnumArrayLen;DateTimeArray_index++)
{
DateTime_t_Serialize(sm,&obj->DateTimeArray[DateTimeArray_index]);
}
El_Assert(obj->EnumArrayLen<=16);
for(int32_t FiexDateTimeArray_index=0;FiexDateTimeArray_index<obj->EnumArrayLen;FiexDateTimeArray_index++)
{
DateTime_t_Serialize(sm,&obj->FiexDateTimeArray[FiexDateTimeArray_index]);
}
}

void TestSerialize_Deserialize(SerializationManager *sm,TestSerialize *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->EnumArrayLen,sm,4,sizeof(int32_t),isIsr);
obj->EnumArray=(Sex *)El_Malloc(sizeof(Sex)*obj->EnumArrayLen);
El_Memset(obj->EnumArray,0,sizeof(Sex)*obj->EnumArrayLen);
El_Assert(obj->EnumArrayLen<=1);
for(int32_t EnumArray_index=0;EnumArray_index<obj->EnumArrayLen;EnumArray_index++)
{
DeserializeField((uint8_t *)&obj->EnumArray[EnumArray_index],sm,8,sizeof(Sex),isIsr);
}
DeserializeField((uint8_t *)&obj->ObjectArrayLen,sm,4,sizeof(int32_t),isIsr);
obj->DateTimeArray=(DateTime_t *)El_Malloc(sizeof(DateTime_t)*obj->EnumArrayLen);
El_Memset(obj->DateTimeArray,0,sizeof(DateTime_t)*obj->EnumArrayLen);
El_Assert(obj->EnumArrayLen<=1);
for(int32_t DateTimeArray_index=0;DateTimeArray_index<obj->EnumArrayLen;DateTimeArray_index++)
{
DateTime_t_Deserialize(sm,&obj->DateTimeArray[DateTimeArray_index],isIsr);
}
El_Assert(obj->EnumArrayLen<=16);
for(int32_t FiexDateTimeArray_index=0;FiexDateTimeArray_index<obj->EnumArrayLen;FiexDateTimeArray_index++)
{
DateTime_t_Deserialize(sm,&obj->FiexDateTimeArray[FiexDateTimeArray_index],isIsr);
}
}

 void TestSerialize_FreeData(TestSerialize *obj)
 {
  //ElementTypeFreeLabel for(int32_t EnumArray_index=0;EnumArray_index<obj->EnumArrayLen;EnumArray_index++)
  //ElementTypeFreeLabel {
  //ElementTypeFreeLabel }
  El_Free(obj->EnumArray);
   for(int32_t DateTimeArray_index=0;DateTimeArray_index<obj->EnumArrayLen;DateTimeArray_index++)
   {
   DateTime_t_FreeData(&obj->DateTimeArray[DateTimeArray_index]);
   }
  El_Free(obj->DateTimeArray);
   for(int32_t FiexDateTimeArray_index=0;FiexDateTimeArray_index<obj->EnumArrayLen;FiexDateTimeArray_index++)
   {
   DateTime_t_FreeData(&obj->FiexDateTimeArray[FiexDateTimeArray_index]);
   }
 }

void DateTimeChange_Return_Serialize(SerializationManager *sm,DateTimeChange_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(RequestResponseState)<=1?sizeof(RequestResponseState):1);
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
}

void DateTimeChange_Return_Deserialize(SerializationManager *sm,DateTimeChange_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,1,sizeof(RequestResponseState),isIsr);
}

//StructFreeLabel void DateTimeChange_Return_FreeData(DateTimeChange_Return *obj)
//StructFreeLabel {
//StructFreeLabel }

void DateTimeChange_Parameter_Serialize(SerializationManager *sm,DateTimeChange_Parameter *obj)
{
El_Assert(1<=1);
for(uint32_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Serialize(sm,&obj->now[now_index]);
}
}

void DateTimeChange_Parameter_Deserialize(SerializationManager *sm,DateTimeChange_Parameter *obj,int isIsr)
{
El_Assert(1<=1);
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
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(RequestResponseState)<=1?sizeof(RequestResponseState):1);
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
}

void Test2_Return_Deserialize(SerializationManager *sm,Test2_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,1,sizeof(RequestResponseState),isIsr);
}

//StructFreeLabel void Test2_Return_FreeData(Test2_Return *obj)
//StructFreeLabel {
//StructFreeLabel }

void Test2_Parameter_Serialize(SerializationManager *sm,Test2_Parameter *obj)
{
El_Assert(1<=1);
for(uint32_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Serialize(sm,&obj->now[now_index]);
}
}

void Test2_Parameter_Deserialize(SerializationManager *sm,Test2_Parameter *obj,int isIsr)
{
El_Assert(1<=1);
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
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(RequestResponseState)<=1?sizeof(RequestResponseState):1);
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
AddResult_Serialize(sm,&obj->ReturnValue);
}

void Add_Return_Deserialize(SerializationManager *sm,Add_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,1,sizeof(RequestResponseState),isIsr);
AddResult_Deserialize(sm,&obj->ReturnValue,isIsr);
}

//StructFreeLabel void Add_Return_FreeData(Add_Return *obj)
//StructFreeLabel {
//StructFreeLabel  //FieldNeedFreeMemoryLabel AddResult_FreeData(&obj->ReturnValue);
//StructFreeLabel }

void Add_Parameter_Serialize(SerializationManager *sm,Add_Parameter *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->dataLen,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
El_Assert(obj->dataLen<=1);
for(int32_t data_index=0;data_index<obj->dataLen;data_index++)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->data[data_index],sizeof(uint8_t));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
}
El_Assert(1<=1);
for(uint32_t test_index=0;test_index<1;test_index++)
{
TestSerialize_Serialize(sm,&obj->test[test_index]);
}
}

void Add_Parameter_Deserialize(SerializationManager *sm,Add_Parameter *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->a,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->b,sm,4,sizeof(int32_t),isIsr);
DeserializeField((uint8_t *)&obj->dataLen,sm,4,sizeof(int32_t),isIsr);
obj->data=(uint8_t *)El_Malloc(sizeof(uint8_t)*obj->dataLen);
El_Memset(obj->data,0,sizeof(uint8_t)*obj->dataLen);
El_Assert(obj->dataLen<=1);
for(int32_t data_index=0;data_index<obj->dataLen;data_index++)
{
DeserializeField((uint8_t *)&obj->data[data_index],sm,1,sizeof(uint8_t),isIsr);
}
El_Assert(1<=1);
for(uint32_t test_index=0;test_index<1;test_index++)
{
TestSerialize_Deserialize(sm,&obj->test[test_index],isIsr);
}
}

 void Add_Parameter_FreeData(Add_Parameter *obj)
 {
  //ElementTypeFreeLabel for(int32_t data_index=0;data_index<obj->dataLen;data_index++)
  //ElementTypeFreeLabel {
  //ElementTypeFreeLabel }
  El_Free(obj->data);
   for(uint32_t test_index=0;test_index<1;test_index++)
   {
   TestSerialize_FreeData(&obj->test[test_index]);
   }
 }

void NoArg_Return_Serialize(SerializationManager *sm,NoArg_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(RequestResponseState)<=1?sizeof(RequestResponseState):1);
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->ReturnValue,sizeof(bool));
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
}

void NoArg_Return_Deserialize(SerializationManager *sm,NoArg_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,1,sizeof(RequestResponseState),isIsr);
DeserializeField((uint8_t *)&obj->ReturnValue,sm,1,sizeof(bool),isIsr);
}

//StructFreeLabel void NoArg_Return_FreeData(NoArg_Return *obj)
//StructFreeLabel {
//StructFreeLabel }

void NoArg_Parameter_Serialize(SerializationManager *sm,NoArg_Parameter *obj)
{
}

void NoArg_Parameter_Deserialize(SerializationManager *sm,NoArg_Parameter *obj,int isIsr)
{
}

//StructFreeLabel void NoArg_Parameter_FreeData(NoArg_Parameter *obj)
//StructFreeLabel {
//StructFreeLabel }

void NoReturn_Return_Serialize(SerializationManager *sm,NoReturn_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(RequestResponseState)<=1?sizeof(RequestResponseState):1);
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
}

void NoReturn_Return_Deserialize(SerializationManager *sm,NoReturn_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,1,sizeof(RequestResponseState),isIsr);
}

//StructFreeLabel void NoReturn_Return_FreeData(NoReturn_Return *obj)
//StructFreeLabel {
//StructFreeLabel }

void NoReturn_Parameter_Serialize(SerializationManager *sm,NoReturn_Parameter *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(int32_t));
sm->Index+=4;
El_Assert(sm->Index<=sm->BufferLen);
}

void NoReturn_Parameter_Deserialize(SerializationManager *sm,NoReturn_Parameter *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->a,sm,4,sizeof(int32_t),isIsr);
}

//StructFreeLabel void NoReturn_Parameter_FreeData(NoReturn_Parameter *obj)
//StructFreeLabel {
//StructFreeLabel }

void NoArgAndReturn_Return_Serialize(SerializationManager *sm,NoArgAndReturn_Return *obj)
{
if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(RequestResponseState)<=1?sizeof(RequestResponseState):1);
sm->Index+=1;
El_Assert(sm->Index<=sm->BufferLen);
}

void NoArgAndReturn_Return_Deserialize(SerializationManager *sm,NoArgAndReturn_Return *obj,int isIsr)
{
DeserializeField((uint8_t *)&obj->State,sm,1,sizeof(RequestResponseState),isIsr);
}

//StructFreeLabel void NoArgAndReturn_Return_FreeData(NoArgAndReturn_Return *obj)
//StructFreeLabel {
//StructFreeLabel }

void NoArgAndReturn_Parameter_Serialize(SerializationManager *sm,NoArgAndReturn_Parameter *obj)
{
}

void NoArgAndReturn_Parameter_Deserialize(SerializationManager *sm,NoArgAndReturn_Parameter *obj,int isIsr)
{
}

//StructFreeLabel void NoArgAndReturn_Parameter_FreeData(NoArgAndReturn_Parameter *obj)
//StructFreeLabel {
//StructFreeLabel }

