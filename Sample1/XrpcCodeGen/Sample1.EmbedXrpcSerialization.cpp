#include"Sample1.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

void Student_Serialize(SerializationManager &sm,Student *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->a,sizeof(obj->a));
sm.Index+=sizeof(obj->a);
Memcpy(&sm.Buf[sm.Index],&obj->b,sizeof(obj->b));
sm.Index+=sizeof(obj->b);
}

void Student_Deserialize(SerializationManager &sm,Student *obj)
{
Memcpy(&obj->a,&sm.Buf[sm.Index],sizeof(obj->a));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->a)));
sm.Index+=sizeof(obj->a);
Memcpy(&obj->b,&sm.Buf[sm.Index],sizeof(obj->b));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->b)));
sm.Index+=sizeof(obj->b);
}

void Student_FreeData(Student *obj)
{
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
Student_Serialize(sm,&obj->David);
}

void DateTime_t_Deserialize(SerializationManager &sm,DateTime_t *obj)
{
Memcpy(&obj->Year,&sm.Buf[sm.Index],sizeof(obj->Year));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->Year)));
sm.Index+=sizeof(obj->Year);
Memcpy(&obj->Month,&sm.Buf[sm.Index],sizeof(obj->Month));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->Month)));
sm.Index+=sizeof(obj->Month);
Memcpy(&obj->Day,&sm.Buf[sm.Index],sizeof(obj->Day));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->Day)));
sm.Index+=sizeof(obj->Day);
Memcpy(&obj->Hour,&sm.Buf[sm.Index],sizeof(obj->Hour));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->Hour)));
sm.Index+=sizeof(obj->Hour);
Memcpy(&obj->Min,&sm.Buf[sm.Index],sizeof(obj->Min));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->Min)));
sm.Index+=sizeof(obj->Min);
Memcpy(&obj->Sec,&sm.Buf[sm.Index],sizeof(obj->Sec));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->Sec)));
sm.Index+=sizeof(obj->Sec);
Memcpy(&obj->Sex,&sm.Buf[sm.Index],sizeof(UInt64));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(UInt64)));
sm.Index+=sizeof(UInt64);
Student_Deserialize(sm,&obj->David);
}

void DateTime_t_FreeData(DateTime_t *obj)
{
Student_FreeData(&obj->David);
}

void AddResult_Serialize(SerializationManager &sm,AddResult *obj)
{
Memcpy(&sm.Buf[sm.Index],&obj->Sum,sizeof(obj->Sum));
sm.Index+=sizeof(obj->Sum);
Memcpy(&sm.Buf[sm.Index],&obj->dataLen,sizeof(obj->dataLen));
sm.Index+=sizeof(obj->dataLen);
for(Int32 data_index=0;data_index<obj->dataLen;data_index++)
{
Memcpy(&sm.Buf[sm.Index],&obj->data[data_index],sizeof(UInt8));
sm.Index+=sizeof(UInt8);
}
}

void AddResult_Deserialize(SerializationManager &sm,AddResult *obj)
{
Memcpy(&obj->Sum,&sm.Buf[sm.Index],sizeof(obj->Sum));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->Sum)));
sm.Index+=sizeof(obj->Sum);
Memcpy(&obj->dataLen,&sm.Buf[sm.Index],sizeof(obj->dataLen));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->dataLen)));
sm.Index+=sizeof(obj->dataLen);
obj->data=(UInt8 *)Malloc(sizeof(UInt8)*obj->dataLen);
for(Int32 data_index=0;data_index<obj->dataLen;data_index++)
{
Memcpy(&obj->data[data_index],&sm.Buf[sm.Index],sizeof(UInt8));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(UInt8)));
sm.Index+=sizeof(UInt8);
}
}

void AddResult_FreeData(AddResult *obj)
{
for(Int32 data_index=0;data_index<obj->dataLen;data_index++)
{
}
Free(obj->data);
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
Memcpy(&obj->a,&sm.Buf[sm.Index],sizeof(obj->a));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->a)));
sm.Index+=sizeof(obj->a);
Memcpy(&obj->b,&sm.Buf[sm.Index],sizeof(obj->b));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->b)));
sm.Index+=sizeof(obj->b);
Memcpy(&obj->dataLen,&sm.Buf[sm.Index],sizeof(obj->dataLen));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->dataLen)));
sm.Index+=sizeof(obj->dataLen);
obj->data=(UInt8 *)Malloc(sizeof(UInt8)*obj->dataLen);
for(Int32 data_index=0;data_index<obj->dataLen;data_index++)
{
Memcpy(&obj->data[data_index],&sm.Buf[sm.Index],sizeof(UInt8));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(UInt8)));
sm.Index+=sizeof(UInt8);
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
Memcpy(&obj->State,&sm.Buf[sm.Index],sizeof(UInt8));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(UInt8)));
sm.Index+=sizeof(UInt8);
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
Memcpy(&obj->State,&sm.Buf[sm.Index],sizeof(UInt8));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(UInt8)));
sm.Index+=sizeof(UInt8);
Memcpy(&obj->ReturnValue,&sm.Buf[sm.Index],sizeof(obj->ReturnValue));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->ReturnValue)));
sm.Index+=sizeof(obj->ReturnValue);
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
Memcpy(&obj->a,&sm.Buf[sm.Index],sizeof(obj->a));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(obj->a)));
sm.Index+=sizeof(obj->a);
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
Memcpy(&obj->State,&sm.Buf[sm.Index],sizeof(UInt8));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(UInt8)));
sm.Index+=sizeof(UInt8);
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
Memcpy(&obj->State,&sm.Buf[sm.Index],sizeof(UInt8));
SerializationManagerAppendDataSum(sm,GetSum(&sm.Buf[sm.Index],sizeof(UInt8)));
sm.Index+=sizeof(UInt8);
}

void Inter_NoArgAndReturn_Return_FreeData(Inter_NoArgAndReturn_Return *obj)
{
}

