#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void AddResult_Serialize(SerializationManager &sm,AddResult *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->Sum,sizeof(obj->Sum));
sm.Index+=sizeof(obj->Sum);

Memcpy(&sm.Buf[sm.Index],&obj->dataLen,sizeof(obj->dataLen));
sm.Index+=sizeof(obj->dataLen);

//data :NoSerialization
}


void AddResult_Deserialize(SerializationManager &sm,AddResult *obj)
{
DeserializeField((uint8_t *)&obj->Sum,sm,sizeof(obj->Sum));
DeserializeField((uint8_t *)&obj->dataLen,sm,sizeof(obj->dataLen));
//data :NoSerialization
}


void AddResult_FreeData(AddResult *obj)
{
//data :NoSerialization 
}


void Student_Serialize(SerializationManager &sm,Student *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->a,sizeof(obj->a));
sm.Index+=sizeof(obj->a);

Memcpy(&sm.Buf[sm.Index],&obj->b,sizeof(obj->b));
sm.Index+=sizeof(obj->b);

Memcpy(&sm.Buf[sm.Index],&obj->AddressType,sizeof(obj->AddressType));
sm.Index+=sizeof(obj->AddressType);

 if(obj->AddressType==Student_u1_FieldNumber)
{
Memcpy(&sm.Buf[sm.Index],&obj->u1,sizeof(obj->u1));
sm.Index+=sizeof(obj->u1);

}
else if(obj->AddressType==Student_u2_FieldNumber)
{
Memcpy(&sm.Buf[sm.Index],&obj->u2,sizeof(obj->u2));
sm.Index+=sizeof(obj->u2);

}
else if(obj->AddressType==Student_u3_FieldNumber)
{
Memcpy(&sm.Buf[sm.Index],&obj->u3,sizeof(UInt64));
sm.Index+=sizeof(UInt64);

}
else if(obj->AddressType==Student_u4_FieldNumber)
{
AddResult_Serialize(sm,&obj->u4);

}
Memcpy(&sm.Buf[sm.Index],&obj->uend1,sizeof(obj->uend1));
sm.Index+=sizeof(obj->uend1);

Memcpy(&sm.Buf[sm.Index],&obj->uend2,sizeof(obj->uend2));
sm.Index+=sizeof(obj->uend2);

}


void Student_Deserialize(SerializationManager &sm,Student *obj)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(obj->a));
DeserializeField((uint8_t *)&obj->b,sm,sizeof(obj->b));
DeserializeField((uint8_t *)&obj->AddressType,sm,sizeof(obj->AddressType));
 if(obj->AddressType==Student_u1_FieldNumber)
{
DeserializeField((uint8_t *)&obj->u1,sm,sizeof(obj->u1));
}
else if(obj->AddressType==Student_u2_FieldNumber)
{
DeserializeField((uint8_t *)&obj->u2,sm,sizeof(obj->u2));
}
else if(obj->AddressType==Student_u3_FieldNumber)
{
DeserializeField((uint8_t *)&obj->u3,sm,sizeof(UInt64));
}
else if(obj->AddressType==Student_u4_FieldNumber)
{
AddResult_Deserialize(sm,&obj->u4);

}
DeserializeField((uint8_t *)&obj->uend1,sm,sizeof(obj->uend1));
DeserializeField((uint8_t *)&obj->uend2,sm,sizeof(obj->uend2));
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


void DateTime_t_Serialize(SerializationManager &sm,DateTime_t *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->Year,sizeof(obj->Year));
sm.Index+=sizeof(obj->Year);

Memcpy(&sm.Buf[sm.Index],&obj->Month,sizeof(obj->Month));
sm.Index+=sizeof(obj->Month);

Memcpy(&sm.Buf[sm.Index],&obj->Day,sizeof(obj->Day));
sm.Index+=sizeof(obj->Day);

Memcpy(&sm.Buf[sm.Index],&obj->Hour,sizeof(obj->Hour));
sm.Index+=sizeof(obj->Hour);

Memcpy(&sm.Buf[sm.Index],&obj->Min,sizeof(obj->Min));
sm.Index+=sizeof(obj->Min);

Memcpy(&sm.Buf[sm.Index],&obj->Sec,sizeof(obj->Sec));
sm.Index+=sizeof(obj->Sec);

Memcpy(&sm.Buf[sm.Index],&obj->Sex,sizeof(UInt64));
sm.Index+=sizeof(UInt64);

Memcpy(&sm.Buf[sm.Index],&obj->DateStringLen,sizeof(obj->DateStringLen));
sm.Index+=sizeof(obj->DateStringLen);

for(UInt8 DateString_index=0;DateString_index<obj->DateStringLen;DateString_index++)
{
Memcpy(&sm.Buf[sm.Index],&obj->DateString[DateString_index],sizeof(UInt8));
sm.Index+=sizeof(UInt8);

}

Student_Serialize(sm,&obj->David);

}


void DateTime_t_Deserialize(SerializationManager &sm,DateTime_t *obj)
{
DeserializeField((uint8_t *)&obj->Year,sm,sizeof(obj->Year));
DeserializeField((uint8_t *)&obj->Month,sm,sizeof(obj->Month));
DeserializeField((uint8_t *)&obj->Day,sm,sizeof(obj->Day));
DeserializeField((uint8_t *)&obj->Hour,sm,sizeof(obj->Hour));
DeserializeField((uint8_t *)&obj->Min,sm,sizeof(obj->Min));
DeserializeField((uint8_t *)&obj->Sec,sm,sizeof(obj->Sec));
DeserializeField((uint8_t *)&obj->Sex,sm,sizeof(UInt64));
DeserializeField((uint8_t *)&obj->DateStringLen,sm,sizeof(obj->DateStringLen));
obj->DateString=(UInt8 *)Malloc(sizeof(UInt8)*obj->DateStringLen);
Memset(obj->DateString,0,sizeof(UInt8)*obj->DateStringLen);
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

Free(obj->DateString);

Student_FreeData(&obj->David);

}


void DateTimeChange_Parameter_Serialize(SerializationManager &sm,DateTimeChange_Parameter *obj)
{
for(uint8_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Serialize(sm,&obj->now[now_index]);

}

}


void DateTimeChange_Parameter_Deserialize(SerializationManager &sm,DateTimeChange_Parameter *obj)
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


void TestDelegate_Parameter_Serialize(SerializationManager &sm,TestDelegate_Parameter *obj)
{
for(uint8_t now_index=0;now_index<1;now_index++)
{
DateTime_t_Serialize(sm,&obj->now[now_index]);

}

}


void TestDelegate_Parameter_Deserialize(SerializationManager &sm,TestDelegate_Parameter *obj)
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


void Inter_Add_Parameter_Serialize(SerializationManager &sm,Inter_Add_Parameter *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->a,sizeof(obj->a));
sm.Index+=sizeof(obj->a);

Memcpy(&sm.Buf[sm.Index],&obj->b,sizeof(obj->b));
sm.Index+=sizeof(obj->b);

Memcpy(&sm.Buf[sm.Index],&obj->dataLen,sizeof(obj->dataLen));
sm.Index+=sizeof(obj->dataLen);

for(Int32 data_index=0;data_index<obj->dataLen;data_index++)
{
Memcpy(&sm.Buf[sm.Index],&obj->data[data_index],sizeof(UInt8));
sm.Index+=sizeof(UInt8);

}

}


void Inter_Add_Parameter_Deserialize(SerializationManager &sm,Inter_Add_Parameter *obj)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(obj->a));
DeserializeField((uint8_t *)&obj->b,sm,sizeof(obj->b));
DeserializeField((uint8_t *)&obj->dataLen,sm,sizeof(obj->dataLen));
obj->data=(UInt8 *)Malloc(sizeof(UInt8)*obj->dataLen);
Memset(obj->data,0,sizeof(UInt8)*obj->dataLen);
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

Free(obj->data);

}


void Inter_Add_Return_Serialize(SerializationManager &sm,Inter_Add_Return *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->State,sizeof(UInt8));
sm.Index+=sizeof(UInt8);

AddResult_Serialize(sm,&obj->ReturnValue);

}


void Inter_Add_Return_Deserialize(SerializationManager &sm,Inter_Add_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
AddResult_Deserialize(sm,&obj->ReturnValue);

}


void Inter_Add_Return_FreeData(Inter_Add_Return *obj)
{
AddResult_FreeData(&obj->ReturnValue);

}


void Inter_NoArg_Parameter_Serialize(SerializationManager &sm,Inter_NoArg_Parameter *obj)
{
}


void Inter_NoArg_Parameter_Deserialize(SerializationManager &sm,Inter_NoArg_Parameter *obj)
{
}


void Inter_NoArg_Parameter_FreeData(Inter_NoArg_Parameter *obj)
{
}


void Inter_NoArg_Return_Serialize(SerializationManager &sm,Inter_NoArg_Return *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->State,sizeof(UInt8));
sm.Index+=sizeof(UInt8);

Memcpy(&sm.Buf[sm.Index],&obj->ReturnValue,sizeof(obj->ReturnValue));
sm.Index+=sizeof(obj->ReturnValue);

}


void Inter_NoArg_Return_Deserialize(SerializationManager &sm,Inter_NoArg_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
DeserializeField((uint8_t *)&obj->ReturnValue,sm,sizeof(obj->ReturnValue));
}


void Inter_NoArg_Return_FreeData(Inter_NoArg_Return *obj)
{
}


void Inter_NoReturn_Parameter_Serialize(SerializationManager &sm,Inter_NoReturn_Parameter *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->a,sizeof(obj->a));
sm.Index+=sizeof(obj->a);

}


void Inter_NoReturn_Parameter_Deserialize(SerializationManager &sm,Inter_NoReturn_Parameter *obj)
{
DeserializeField((uint8_t *)&obj->a,sm,sizeof(obj->a));
}


void Inter_NoReturn_Parameter_FreeData(Inter_NoReturn_Parameter *obj)
{
}


void Inter_NoReturn_Return_Serialize(SerializationManager &sm,Inter_NoReturn_Return *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->State,sizeof(UInt8));
sm.Index+=sizeof(UInt8);

}


void Inter_NoReturn_Return_Deserialize(SerializationManager &sm,Inter_NoReturn_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
}


void Inter_NoReturn_Return_FreeData(Inter_NoReturn_Return *obj)
{
}


void Inter_NoArgAndReturn_Parameter_Serialize(SerializationManager &sm,Inter_NoArgAndReturn_Parameter *obj)
{
}


void Inter_NoArgAndReturn_Parameter_Deserialize(SerializationManager &sm,Inter_NoArgAndReturn_Parameter *obj)
{
}


void Inter_NoArgAndReturn_Parameter_FreeData(Inter_NoArgAndReturn_Parameter *obj)
{
}


void Inter_NoArgAndReturn_Return_Serialize(SerializationManager &sm,Inter_NoArgAndReturn_Return *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->State,sizeof(UInt8));
sm.Index+=sizeof(UInt8);

}


void Inter_NoArgAndReturn_Return_Deserialize(SerializationManager &sm,Inter_NoArgAndReturn_Return *obj)
{
DeserializeField((uint8_t *)&obj->State,sm,sizeof(UInt8));
}


void Inter_NoArgAndReturn_Return_FreeData(Inter_NoArgAndReturn_Return *obj)
{
}


