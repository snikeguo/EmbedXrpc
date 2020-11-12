#ifndef Sample1_EmbedXrpcSerialization_H
#define Sample1_EmbedXrpcSerialization_H
#include"Sample1.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

//EmbedXrpc Code gen .By snikeguo.e-mail:snikeguo@foxmail.com
//关于snikeguo作者:92年生人,热爱技术,底层功底扎实.深入理解C语言底层到汇编层，单片机从外设/裸机到OS均有涉猎
//上位机专注于C# 界面库熟悉WPF.服务器专注于dotnet core
//行业经验：汽车电子.独自完成UDS(包括FBL)协议栈  GBT27930-2015(SAE J1939)协议栈等
//熟悉BMS业务逻辑,有一套自己稳定出货的BMS软件/硬件/上位机/服务器/USB转CAN&CANFD
//TITLE:高级嵌入式软件工程师.软件架构师
//微信:snikeguo.有好的职位推荐请加
#ifndef offsetof
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
#endif
extern const Int32Field Student_Field_a;
extern const Int32Field Student_Field_b;
extern const IField* Student_Type_Field_Desc [2];
extern const ObjectType Student_TypeInstance;

#define Student_Serialize(sm,objptr)    sm.Serialize(&Student_TypeInstance,objptr,0)
#define Student_Deserialize(sm,objptr)    sm.Deserialize(&Student_TypeInstance,objptr)
#define Student_FreeData(objptr)    SerializationManager::FreeData(&Student_TypeInstance,objptr)
extern const Int32Field DateTime_t_Field_Year;
extern const Int32Field DateTime_t_Field_Month;
extern const Int32Field DateTime_t_Field_Day;
extern const Int32Field DateTime_t_Field_Hour;
extern const Int32Field DateTime_t_Field_Min;
extern const Int32Field DateTime_t_Field_Sec;
extern const UInt64Field DateTime_t_Field_Sex;
extern const UInt8Field DateTime_t_Field_DateStringLen;
extern const ArrayType UInt8_Array_TypeInstance;
extern const ArrayField DateTime_t_Field_DateString;
extern const ObjectField DateTime_t_Field_David;
extern const IField* DateTime_t_Type_Field_Desc [10];
extern const ObjectType DateTime_t_TypeInstance;

#define DateTime_t_Serialize(sm,objptr)    sm.Serialize(&DateTime_t_TypeInstance,objptr,0)
#define DateTime_t_Deserialize(sm,objptr)    sm.Deserialize(&DateTime_t_TypeInstance,objptr)
#define DateTime_t_FreeData(objptr)    SerializationManager::FreeData(&DateTime_t_TypeInstance,objptr)
extern const Int32Field AddResult_Field_Sum;
extern const Int32Field AddResult_Field_dataLen;
extern const ArrayField AddResult_Field_data;
extern const IField* AddResult_Type_Field_Desc [3];
extern const ObjectType AddResult_TypeInstance;

#define AddResult_Serialize(sm,objptr)    sm.Serialize(&AddResult_TypeInstance,objptr,0)
#define AddResult_Deserialize(sm,objptr)    sm.Deserialize(&AddResult_TypeInstance,objptr)
#define AddResult_FreeData(objptr)    SerializationManager::FreeData(&AddResult_TypeInstance,objptr)
extern const ArrayType DateTime_t_Array_TypeInstance;
extern const ArrayField DateTimeChange_Parameter_Field_now;
extern const IField* DateTimeChange_Parameter_Type_Field_Desc [1];
extern const ObjectType DateTimeChange_Parameter_TypeInstance;

#define DateTimeChange_Parameter_Serialize(sm,objptr)    sm.Serialize(&DateTimeChange_Parameter_TypeInstance,objptr,0)
#define DateTimeChange_Parameter_Deserialize(sm,objptr)    sm.Deserialize(&DateTimeChange_Parameter_TypeInstance,objptr)
#define DateTimeChange_Parameter_FreeData(objptr)    SerializationManager::FreeData(&DateTimeChange_Parameter_TypeInstance,objptr)
#define DateTimeChange_ServiceId 16   //0x10
extern const Int32Field Inter_Add_Parameter_Field_a;
extern const Int32Field Inter_Add_Parameter_Field_b;
extern const Int32Field Inter_Add_Parameter_Field_dataLen;
extern const ArrayField Inter_Add_Parameter_Field_data;
extern const IField* Inter_Add_Parameter_Type_Field_Desc [4];
extern const ObjectType Inter_Add_Parameter_TypeInstance;

#define Inter_Add_Parameter_Serialize(sm,objptr)    sm.Serialize(&Inter_Add_Parameter_TypeInstance,objptr,0)
#define Inter_Add_Parameter_Deserialize(sm,objptr)    sm.Deserialize(&Inter_Add_Parameter_TypeInstance,objptr)
#define Inter_Add_Parameter_FreeData(objptr)    SerializationManager::FreeData(&Inter_Add_Parameter_TypeInstance,objptr)
#define Inter_Add_ServiceId 17   //0x11
extern const UInt8Field Inter_Add_Return_Field_State;
extern const ObjectField Inter_Add_Return_Field_ReturnValue;
extern const IField* Inter_Add_Return_Type_Field_Desc [2];
extern const ObjectType Inter_Add_Return_TypeInstance;

#define Inter_Add_Return_Serialize(sm,objptr)    sm.Serialize(&Inter_Add_Return_TypeInstance,objptr,0)
#define Inter_Add_Return_Deserialize(sm,objptr)    sm.Deserialize(&Inter_Add_Return_TypeInstance,objptr)
#define Inter_Add_Return_FreeData(objptr)    SerializationManager::FreeData(&Inter_Add_Return_TypeInstance,objptr)
extern const ObjectType Inter_NoArg_Parameter_TypeInstance;

#define Inter_NoArg_Parameter_Serialize(sm,objptr)    sm.Serialize(&Inter_NoArg_Parameter_TypeInstance,objptr,0)
#define Inter_NoArg_Parameter_Deserialize(sm,objptr)    sm.Deserialize(&Inter_NoArg_Parameter_TypeInstance,objptr)
#define Inter_NoArg_Parameter_FreeData(objptr)    SerializationManager::FreeData(&Inter_NoArg_Parameter_TypeInstance,objptr)
#define Inter_NoArg_ServiceId 18   //0x12
extern const UInt8Field Inter_NoArg_Return_Field_State;
extern const UInt8Field Inter_NoArg_Return_Field_ReturnValue;
extern const IField* Inter_NoArg_Return_Type_Field_Desc [2];
extern const ObjectType Inter_NoArg_Return_TypeInstance;

#define Inter_NoArg_Return_Serialize(sm,objptr)    sm.Serialize(&Inter_NoArg_Return_TypeInstance,objptr,0)
#define Inter_NoArg_Return_Deserialize(sm,objptr)    sm.Deserialize(&Inter_NoArg_Return_TypeInstance,objptr)
#define Inter_NoArg_Return_FreeData(objptr)    SerializationManager::FreeData(&Inter_NoArg_Return_TypeInstance,objptr)
extern const Int32Field Inter_NoReturn_Parameter_Field_a;
extern const IField* Inter_NoReturn_Parameter_Type_Field_Desc [1];
extern const ObjectType Inter_NoReturn_Parameter_TypeInstance;

#define Inter_NoReturn_Parameter_Serialize(sm,objptr)    sm.Serialize(&Inter_NoReturn_Parameter_TypeInstance,objptr,0)
#define Inter_NoReturn_Parameter_Deserialize(sm,objptr)    sm.Deserialize(&Inter_NoReturn_Parameter_TypeInstance,objptr)
#define Inter_NoReturn_Parameter_FreeData(objptr)    SerializationManager::FreeData(&Inter_NoReturn_Parameter_TypeInstance,objptr)
#define Inter_NoReturn_ServiceId 19   //0x13
extern const UInt8Field Inter_NoReturn_Return_Field_State;
extern const IField* Inter_NoReturn_Return_Type_Field_Desc [1];
extern const ObjectType Inter_NoReturn_Return_TypeInstance;

#define Inter_NoReturn_Return_Serialize(sm,objptr)    sm.Serialize(&Inter_NoReturn_Return_TypeInstance,objptr,0)
#define Inter_NoReturn_Return_Deserialize(sm,objptr)    sm.Deserialize(&Inter_NoReturn_Return_TypeInstance,objptr)
#define Inter_NoReturn_Return_FreeData(objptr)    SerializationManager::FreeData(&Inter_NoReturn_Return_TypeInstance,objptr)
extern const ObjectType Inter_NoArgAndReturn_Parameter_TypeInstance;

#define Inter_NoArgAndReturn_Parameter_Serialize(sm,objptr)    sm.Serialize(&Inter_NoArgAndReturn_Parameter_TypeInstance,objptr,0)
#define Inter_NoArgAndReturn_Parameter_Deserialize(sm,objptr)    sm.Deserialize(&Inter_NoArgAndReturn_Parameter_TypeInstance,objptr)
#define Inter_NoArgAndReturn_Parameter_FreeData(objptr)    SerializationManager::FreeData(&Inter_NoArgAndReturn_Parameter_TypeInstance,objptr)
#define Inter_NoArgAndReturn_ServiceId 20   //0x14
extern const UInt8Field Inter_NoArgAndReturn_Return_Field_State;
extern const IField* Inter_NoArgAndReturn_Return_Type_Field_Desc [1];
extern const ObjectType Inter_NoArgAndReturn_Return_TypeInstance;

#define Inter_NoArgAndReturn_Return_Serialize(sm,objptr)    sm.Serialize(&Inter_NoArgAndReturn_Return_TypeInstance,objptr,0)
#define Inter_NoArgAndReturn_Return_Deserialize(sm,objptr)    sm.Deserialize(&Inter_NoArgAndReturn_Return_TypeInstance,objptr)
#define Inter_NoArgAndReturn_Return_FreeData(objptr)    SerializationManager::FreeData(&Inter_NoArgAndReturn_Return_TypeInstance,objptr)

#endif
