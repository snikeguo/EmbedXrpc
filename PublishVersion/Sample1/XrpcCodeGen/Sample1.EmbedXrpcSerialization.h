#ifndef Sample1_EmbedXrpcSerialization_H
#define Sample1_EmbedXrpcSerialization_H
#include"Sample1.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef offsetof
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
#endif
void AddResult_Serialize(SerializationManager *sm,AddResult *obj);

void AddResult_Deserialize(SerializationManager *sm,AddResult *obj);

//! void AddResult_FreeData(AddResult *obj);

void Student_Serialize(SerializationManager *sm,Student *obj);

void Student_Deserialize(SerializationManager *sm,Student *obj);

//! void Student_FreeData(Student *obj);

void DateTime_t_Serialize(SerializationManager *sm,DateTime_t *obj);

void DateTime_t_Deserialize(SerializationManager *sm,DateTime_t *obj);

 void DateTime_t_FreeData(DateTime_t *obj);

void DateTimeChange_Return_Serialize(SerializationManager *sm,DateTimeChange_Return *obj);

void DateTimeChange_Return_Deserialize(SerializationManager *sm,DateTimeChange_Return *obj);

//! void DateTimeChange_Return_FreeData(DateTimeChange_Return *obj);

void DateTimeChange_Parameter_Serialize(SerializationManager *sm,DateTimeChange_Parameter *obj);

void DateTimeChange_Parameter_Deserialize(SerializationManager *sm,DateTimeChange_Parameter *obj);

 void DateTimeChange_Parameter_FreeData(DateTimeChange_Parameter *obj);

#define DateTimeChange_ServiceId 16   //0x10
void Test2_Return_Serialize(SerializationManager *sm,Test2_Return *obj);

void Test2_Return_Deserialize(SerializationManager *sm,Test2_Return *obj);

//! void Test2_Return_FreeData(Test2_Return *obj);

void Test2_Parameter_Serialize(SerializationManager *sm,Test2_Parameter *obj);

void Test2_Parameter_Deserialize(SerializationManager *sm,Test2_Parameter *obj);

 void Test2_Parameter_FreeData(Test2_Parameter *obj);

#define Test2_ServiceId 17   //0x11
void Add_Return_Serialize(SerializationManager *sm,Add_Return *obj);

void Add_Return_Deserialize(SerializationManager *sm,Add_Return *obj);

//! void Add_Return_FreeData(Add_Return *obj);

void Add_Parameter_Serialize(SerializationManager *sm,Add_Parameter *obj);

void Add_Parameter_Deserialize(SerializationManager *sm,Add_Parameter *obj);

 void Add_Parameter_FreeData(Add_Parameter *obj);

#define Add_ServiceId 18   //0x12
void NoArg_Return_Serialize(SerializationManager *sm,NoArg_Return *obj);

void NoArg_Return_Deserialize(SerializationManager *sm,NoArg_Return *obj);

//! void NoArg_Return_FreeData(NoArg_Return *obj);

void NoArg_Parameter_Serialize(SerializationManager *sm,NoArg_Parameter *obj);

void NoArg_Parameter_Deserialize(SerializationManager *sm,NoArg_Parameter *obj);

//! void NoArg_Parameter_FreeData(NoArg_Parameter *obj);

#define NoArg_ServiceId 19   //0x13
void NoReturn_Return_Serialize(SerializationManager *sm,NoReturn_Return *obj);

void NoReturn_Return_Deserialize(SerializationManager *sm,NoReturn_Return *obj);

//! void NoReturn_Return_FreeData(NoReturn_Return *obj);

void NoReturn_Parameter_Serialize(SerializationManager *sm,NoReturn_Parameter *obj);

void NoReturn_Parameter_Deserialize(SerializationManager *sm,NoReturn_Parameter *obj);

//! void NoReturn_Parameter_FreeData(NoReturn_Parameter *obj);

#define NoReturn_ServiceId 20   //0x14
void NoArgAndReturn_Return_Serialize(SerializationManager *sm,NoArgAndReturn_Return *obj);

void NoArgAndReturn_Return_Deserialize(SerializationManager *sm,NoArgAndReturn_Return *obj);

//! void NoArgAndReturn_Return_FreeData(NoArgAndReturn_Return *obj);

void NoArgAndReturn_Parameter_Serialize(SerializationManager *sm,NoArgAndReturn_Parameter *obj);

void NoArgAndReturn_Parameter_Deserialize(SerializationManager *sm,NoArgAndReturn_Parameter *obj);

//! void NoArgAndReturn_Parameter_FreeData(NoArgAndReturn_Parameter *obj);

#define NoArgAndReturn_ServiceId 21   //0x15
#ifdef  __cplusplus
}
#endif


#endif
