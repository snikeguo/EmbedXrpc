#ifndef Sample1_EmbedXrpcSerialization_H
#define Sample1_EmbedXrpcSerialization_H
#include"Sample1.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

#ifndef offsetof
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
#endif
void AddResult_Serialize(SerializationManager *sm,AddResult *obj);

void AddResult_Deserialize(SerializationManager *sm,AddResult *obj);

void AddResult_FreeData(AddResult *obj);

void Student_Serialize(SerializationManager *sm,Student *obj);

void Student_Deserialize(SerializationManager *sm,Student *obj);

void Student_FreeData(Student *obj);

void DateTime_t_Serialize(SerializationManager *sm,DateTime_t *obj);

void DateTime_t_Deserialize(SerializationManager *sm,DateTime_t *obj);

void DateTime_t_FreeData(DateTime_t *obj);

void DateTimeChange_Parameter_Serialize(SerializationManager *sm,DateTimeChange_Parameter *obj);

void DateTimeChange_Parameter_Deserialize(SerializationManager *sm,DateTimeChange_Parameter *obj);

void DateTimeChange_Parameter_FreeData(DateTimeChange_Parameter *obj);

#define DateTimeChange_ServiceId 16   //0x10
void TestDelegate_Parameter_Serialize(SerializationManager *sm,TestDelegate_Parameter *obj);

void TestDelegate_Parameter_Deserialize(SerializationManager *sm,TestDelegate_Parameter *obj);

void TestDelegate_Parameter_FreeData(TestDelegate_Parameter *obj);

#define TestDelegate_ServiceId 17   //0x11
void Inter_Add_Parameter_Serialize(SerializationManager *sm,Inter_Add_Parameter *obj);

void Inter_Add_Parameter_Deserialize(SerializationManager *sm,Inter_Add_Parameter *obj);

void Inter_Add_Parameter_FreeData(Inter_Add_Parameter *obj);

#define Inter_Add_ServiceId 18   //0x12
void Inter_Add_Return_Serialize(SerializationManager *sm,Inter_Add_Return *obj);

void Inter_Add_Return_Deserialize(SerializationManager *sm,Inter_Add_Return *obj);

void Inter_Add_Return_FreeData(Inter_Add_Return *obj);

void Inter_NoArg_Parameter_Serialize(SerializationManager *sm,Inter_NoArg_Parameter *obj);

void Inter_NoArg_Parameter_Deserialize(SerializationManager *sm,Inter_NoArg_Parameter *obj);

void Inter_NoArg_Parameter_FreeData(Inter_NoArg_Parameter *obj);

#define Inter_NoArg_ServiceId 19   //0x13
void Inter_NoArg_Return_Serialize(SerializationManager *sm,Inter_NoArg_Return *obj);

void Inter_NoArg_Return_Deserialize(SerializationManager *sm,Inter_NoArg_Return *obj);

void Inter_NoArg_Return_FreeData(Inter_NoArg_Return *obj);

void Inter_NoReturn_Parameter_Serialize(SerializationManager *sm,Inter_NoReturn_Parameter *obj);

void Inter_NoReturn_Parameter_Deserialize(SerializationManager *sm,Inter_NoReturn_Parameter *obj);

void Inter_NoReturn_Parameter_FreeData(Inter_NoReturn_Parameter *obj);

#define Inter_NoReturn_ServiceId 20   //0x14
void Inter_NoReturn_Return_Serialize(SerializationManager *sm,Inter_NoReturn_Return *obj);

void Inter_NoReturn_Return_Deserialize(SerializationManager *sm,Inter_NoReturn_Return *obj);

void Inter_NoReturn_Return_FreeData(Inter_NoReturn_Return *obj);

void Inter_NoArgAndReturn_Parameter_Serialize(SerializationManager *sm,Inter_NoArgAndReturn_Parameter *obj);

void Inter_NoArgAndReturn_Parameter_Deserialize(SerializationManager *sm,Inter_NoArgAndReturn_Parameter *obj);

void Inter_NoArgAndReturn_Parameter_FreeData(Inter_NoArgAndReturn_Parameter *obj);

#define Inter_NoArgAndReturn_ServiceId 21   //0x15
void Inter_NoArgAndReturn_Return_Serialize(SerializationManager *sm,Inter_NoArgAndReturn_Return *obj);

void Inter_NoArgAndReturn_Return_Deserialize(SerializationManager *sm,Inter_NoArgAndReturn_Return *obj);

void Inter_NoArgAndReturn_Return_FreeData(Inter_NoArgAndReturn_Return *obj);


#endif
