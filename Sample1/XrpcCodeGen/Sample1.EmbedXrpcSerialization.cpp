#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void AddResult_Serialize(SerializationManager *sm,AddResult *obj)
{
UInt32 bitsTempValue0=0;
bitsTempValue0 |= ((UInt32)(obj->Sum))<< 0 ;
bitsTempValue0 |= ((UInt32)(obj->Sum2))<< 11 ;
bitsTempValue0 |= ((UInt32)(obj->Sum3))<< 30 ;
El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue0,sizeof(UInt32));
sm->Index+=sizeof(UInt32);

El_Memcpy(&sm->Buf[sm->Index],&obj->dataLen,sizeof(Int32));
sm->Index+=sizeof(Int32);
//data :NoSerialization
UInt64 bitsTempValue1=0;
bitsTempValue1 |= ((UInt64)(obj->Sum4))<< 0 ;
bitsTempValue1 |= ((UInt64)(obj->Sum5))<< 11 ;
bitsTempValue1 |= ((UInt64)(obj->Sum6))<< 30 ;
El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue1,sizeof(UInt64));
sm->Index+=sizeof(UInt64);

El_Memcpy(&sm->Buf[sm->Index],&obj->Sum7,sizeof(UInt32));
sm->Index+=sizeof(UInt32);
El_Assert(sm->Index<=sm->BufferLen);
}


void AddResult_Deserialize(SerializationManager *sm,AddResult *obj)
{
UInt32 bitsTempValue0=0;
DeserializeField((uint8_t *)&bitsTempValue0,sm,sizeof(UInt32));
obj->Sum=bitsTempValue0>>0;
obj->Sum2=bitsTempValue0>>11;
obj->Sum3=bitsTempValue0>>30;

DeserializeField((uint8_t *)&obj->dataLen,sm,sizeof(Int32));
//data :NoSerialization
UInt64 bitsTempValue1=0;
DeserializeField((uint8_t *)&bitsTempValue1,sm,sizeof(UInt64));
obj->Sum4=bitsTempValue1>>0;
obj->Sum5=bitsTempValue1>>11;
obj->Sum6=bitsTempValue1>>30;

DeserializeField((uint8_t *)&obj->Sum7,sm,sizeof(UInt32));
}


void AddResult_FreeData(AddResult *obj)
{
//data :NoSerialization 
}


void Student_Serialize(SerializationManager *sm,Student *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->AddressType,sizeof(Int32));
sm->Index+=sizeof(Int32);
 if(obj->AddressType==Student_u1_FieldNumber)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->u1,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
}
else if(obj->AddressType==Student_u2_FieldNumber)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->u2,sizeof(UInt64));
sm->Index+=sizeof(UInt64);
}
else if(obj->AddressType==Student_u3_FieldNumber)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->u3,sizeof(UInt64));
sm->Index+=sizeof(UInt64);

}
else if(obj->AddressType==Student_u4_FieldNumber)
{
AddResult_Serialize(sm,&obj->u4);

}
El_Memcpy(&sm->Buf[sm->Index],&obj->uend1,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Memcpy(&sm->Buf[sm->Index],&obj->uend2,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Assert(sm->Index<=sm->BufferLen);
}


void Student_Deserialize(SerializationManager *sm,Student *obj)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->b,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->AddressType,sm,sizeof(Int32));
 if(obj->AddressType==Student_u1_FieldNumber)
{
DeserializeField((uint8_t *)&obj->u1,sm,sizeof(UInt8));
}
else if(obj->AddressType==Student_u2_FieldNumber)
{
DeserializeField((uint8_t *)&obj->u2,sm,sizeof(UInt64));
}
else if(obj->AddressType==Student_u3_FieldNumber)
{
DeserializeField((uint8_t *)&obj->u3,sm,sizeof(UInt64));
}
else if(obj->AddressType==Student_u4_FieldNumber)
{
AddResult_Deserialize(sm,&obj->u4);

}
DeserializeField((uint8_t *)&obj->uend1,sm,sizeof(UInt8));
DeserializeField((uint8_t *)&obj->uend2,sm,sizeof(UInt8));
}


void Student_FreeData(Student *obj)
{
 if(obj->AddressType==Student_u1_FieldNumber)
{
}
else if(obj->AddressType==Student_u2_FieldNumber)
{
}
else if(obj->AddressType==Student_u3_FieldNumber)
{
}
else if(obj->AddressType==Student_u4_FieldNumber)
{
AddResult_FreeData(&obj->u4);

}
}


void DateTime_t_Serialize(SerializationManager *sm,DateTime_t *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->Year,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->Month,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->Day,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->Hour,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->Min,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->Sec,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->Sex,sizeof(UInt64));
sm->Index+=sizeof(UInt64);

El_Memcpy(&sm->Buf[sm->Index],&obj->DateStringLen,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
for(UInt8 DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->DateString[DateString_index],sizeof(UInt8));
sm->Index+=sizeof(UInt8);

}

Student_Serialize(sm,&obj->David);

El_Assert(sm->Index<=sm->BufferLen);
}


void DateTime_t_Deserialize(SerializationManager *sm,DateTime_t *obj)
{
DeserializeField((uint8_t *)&obj->Year,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->Month,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->Day,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->Hour,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->Min,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->Sec,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->Sex,sm,sizeof(UInt64));
DeserializeField((uint8_t *)&obj->DateStringLen,sm,sizeof(UInt8));
obj->DateString=(UInt8 *)El_Malloc(sizeof(UInt8)*obj->DateStringLen);
El_Memset(obj->DateString,0,sizeof(UInt8)*obj->DateStringLen);
for(UInt8 DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
{
DeserializeField((uint8_t *)&obj->DateString[DateString_index],sm,sizeof(UInt8));
}

Student_Deserialize(sm,&obj->David);

}


void DateTime_t_FreeData(DateTime_t *obj)
{
for(UInt8 DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
{
}

El_Free(obj->DateString);

Student_FreeData(&obj->David);

}


void DateTimeChange_Parameter_Serialize(SerializationManager *sm,DateTimeChange_Parameter *obj)
{
for(uint8_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Serialize(sm,&obj->now[now_index]);

}

El_Assert(sm->Index<=sm->BufferLen);
}


void DateTimeChange_Parameter_Deserialize(SerializationManager *sm,DateTimeChange_Parameter *obj)
{
for(uint8_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Deserialize(sm,&obj->now[now_index]);

}

}


void DateTimeChange_Parameter_FreeData(DateTimeChange_Parameter *obj)
{
for(uint8_t now_index=0;now_index<1;now_index++)
{
DateTime_t_FreeData(&obj->now[now_index]);

}

}


void TestDelegate_Parameter_Serialize(SerializationManager *sm,TestDelegate_Parameter *obj)
{
for(uint8_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Serialize(sm,&obj->now[now_index]);

}

El_Assert(sm->Index<=sm->BufferLen);
}


void TestDelegate_Parameter_Deserialize(SerializationManager *sm,TestDelegate_Parameter *obj)
{
for(uint8_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Deserialize(sm,&obj->now[now_index]);

}

}


void TestDelegate_Parameter_FreeData(TestDelegate_Parameter *obj)
{
for(uint8_t now_index=0;now_index<1;now_index++)
{
DateTime_t_FreeData(&obj->now[now_index]);

}

}


void Inter_Add_Parameter_Serialize(SerializationManager *sm,Inter_Add_Parameter *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->b,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Memcpy(&sm->Buf[sm->Index],&obj->dataLen,sizeof(Int32));
sm->Index+=sizeof(Int32);
for(Int32 data_index=0;data_index<obj->dataLen;data_index++)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->data[data_index],sizeof(UInt8));
sm->Index+=sizeof(UInt8);

}

El_Assert(sm->Index<=sm->BufferLen);
}


void Inter_Add_Parameter_Deserialize(SerializationManager *sm,Inter_Add_Parameter *obj)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->b,sm,sizeof(Int32));
DeserializeField((uint8_t *)&obj->dataLen,sm,sizeof(Int32));
obj->data=(UInt8 *)El_Malloc(sizeof(UInt8)*obj->dataLen);
El_Memset(obj->data,0,sizeof(UInt8)*obj->dataLen);
for(Int32 data_index=0;data_index<obj->dataLen;data_index++)
{
DeserializeField((uint8_t *)&obj->data[data_index],sm,sizeof(UInt8));
}

}


void Inter_Add_Parameter_FreeData(Inter_Add_Parameter *obj)
{
for(Int32 data_index=0;data_index<obj->dataLen;data_index++)
{
}

El_Free(obj->data);

}


void Inter_Add_Return_Serialize(SerializationManager *sm,Inter_Add_Return *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(UInt8));
sm->Index+=sizeof(UInt8);

AddResult_Serialize(sm,&obj->ReturnValue);

El_Assert(sm->Index<=sm->BufferLen);
}


void Inter_Add_Return_Deserialize(SerializationManager *sm,Inter_Add_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
AddResult_Deserialize(sm,&obj->ReturnValue);

}


void Inter_Add_Return_FreeData(Inter_Add_Return *obj)
{
AddResult_FreeData(&obj->ReturnValue);

}


void Inter_NoArg_Parameter_Serialize(SerializationManager *sm,Inter_NoArg_Parameter *obj)
{
El_Assert(sm->Index<=sm->BufferLen);
}


void Inter_NoArg_Parameter_Deserialize(SerializationManager *sm,Inter_NoArg_Parameter *obj)
{
}


void Inter_NoArg_Parameter_FreeData(Inter_NoArg_Parameter *obj)
{
}


void Inter_NoArg_Return_Serialize(SerializationManager *sm,Inter_NoArg_Return *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(UInt8));
sm->Index+=sizeof(UInt8);

El_Memcpy(&sm->Buf[sm->Index],&obj->ReturnValue,sizeof(UInt8));
sm->Index+=sizeof(UInt8);
El_Assert(sm->Index<=sm->BufferLen);
}


void Inter_NoArg_Return_Deserialize(SerializationManager *sm,Inter_NoArg_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
DeserializeField((uint8_t *)&obj->ReturnValue,sm,sizeof(UInt8));
}


void Inter_NoArg_Return_FreeData(Inter_NoArg_Return *obj)
{
}


void Inter_NoReturn_Parameter_Serialize(SerializationManager *sm,Inter_NoReturn_Parameter *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->a,sizeof(Int32));
sm->Index+=sizeof(Int32);
El_Assert(sm->Index<=sm->BufferLen);
}


void Inter_NoReturn_Parameter_Deserialize(SerializationManager *sm,Inter_NoReturn_Parameter *obj)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(Int32));
}


void Inter_NoReturn_Parameter_FreeData(Inter_NoReturn_Parameter *obj)
{
}


void Inter_NoReturn_Return_Serialize(SerializationManager *sm,Inter_NoReturn_Return *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(UInt8));
sm->Index+=sizeof(UInt8);

El_Assert(sm->Index<=sm->BufferLen);
}


void Inter_NoReturn_Return_Deserialize(SerializationManager *sm,Inter_NoReturn_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
}


void Inter_NoReturn_Return_FreeData(Inter_NoReturn_Return *obj)
{
}


void Inter_NoArgAndReturn_Parameter_Serialize(SerializationManager *sm,Inter_NoArgAndReturn_Parameter *obj)
{
El_Assert(sm->Index<=sm->BufferLen);
}


void Inter_NoArgAndReturn_Parameter_Deserialize(SerializationManager *sm,Inter_NoArgAndReturn_Parameter *obj)
{
}


void Inter_NoArgAndReturn_Parameter_FreeData(Inter_NoArgAndReturn_Parameter *obj)
{
}


void Inter_NoArgAndReturn_Return_Serialize(SerializationManager *sm,Inter_NoArgAndReturn_Return *obj)
{
El_Memcpy(&sm->Buf[sm->Index],&obj->State,sizeof(UInt8));
sm->Index+=sizeof(UInt8);

El_Assert(sm->Index<=sm->BufferLen);
}


void Inter_NoArgAndReturn_Return_Deserialize(SerializationManager *sm,Inter_NoArgAndReturn_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
}


void Inter_NoArgAndReturn_Return_FreeData(Inter_NoArgAndReturn_Return *obj)
{
}


