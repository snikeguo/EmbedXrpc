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
extern const Int32Field DateTime_t_Field_Year;
extern const Int32Field DateTime_t_Field_Month;
extern const Int32Field DateTime_t_Field_Day;
extern const Int32Field DateTime_t_Field_Hour;
extern const Int32Field DateTime_t_Field_Min;
extern const Int32Field DateTime_t_Field_Sec;
extern const IField* DateTime_tDesc [6];
extern const ObjectType DateTime_t_Type;

extern const Int32Field AddResult_Field_Sum;
extern const Int32Field AddResult_Field_dataLen;
extern const ArrayField AddResult_Field_data;
extern const IField* AddResultDesc [3];
extern const ObjectType AddResult_Type;

extern const ArrayField DateTimeChange_Parameter_Field_now;
extern const IField* DateTimeChange_ParameterDesc [1];
extern const ObjectType DateTimeChange_Parameter_Type;

#define DateTimeChange_ServiceId 16
extern const Int32Field Inter_Add_Parameter_Field_a;
extern const Int32Field Inter_Add_Parameter_Field_b;
extern const Int32Field Inter_Add_Parameter_Field_dataLen;
extern const ArrayField Inter_Add_Parameter_Field_data;
extern const IField* Inter_Add_ParameterDesc [4];
extern const ObjectType Inter_Add_Parameter_Type;

#define Inter_Add_ServiceId 17
extern const UInt8Field Inter_Add_Return_Field_State;
extern const ObjectField Inter_Add_Return_Field_ReturnValue;
extern const IField* Inter_Add_ReturnDesc [2];
extern const ObjectType Inter_Add_Return_Type;

extern const ObjectType Inter_NoArg_Parameter_Type;

#define Inter_NoArg_ServiceId 18
extern const UInt8Field Inter_NoArg_Return_Field_State;
extern const UInt8Field Inter_NoArg_Return_Field_ReturnValue;
extern const IField* Inter_NoArg_ReturnDesc [2];
extern const ObjectType Inter_NoArg_Return_Type;

extern const Int32Field Inter_NoReturn_Parameter_Field_a;
extern const IField* Inter_NoReturn_ParameterDesc [1];
extern const ObjectType Inter_NoReturn_Parameter_Type;

#define Inter_NoReturn_ServiceId 19
extern const UInt8Field Inter_NoReturn_Return_Field_State;
extern const IField* Inter_NoReturn_ReturnDesc [1];
extern const ObjectType Inter_NoReturn_Return_Type;

extern const ObjectType Inter_NoArgAndReturn_Parameter_Type;

#define Inter_NoArgAndReturn_ServiceId 20
extern const UInt8Field Inter_NoArgAndReturn_Return_Field_State;
extern const IField* Inter_NoArgAndReturn_ReturnDesc [1];
extern const ObjectType Inter_NoArgAndReturn_Return_Type;


#endif
