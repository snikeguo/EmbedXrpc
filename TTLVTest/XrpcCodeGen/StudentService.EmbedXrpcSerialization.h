#ifndef StudentService_EmbedXrpcSerialization_H
#define StudentService_EmbedXrpcSerialization_H
#include"StudentService.h"

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
extern const UInt8Field Test_Field_IntPtrsLen;
extern const ArrayField Test_Field_IntPtr;
extern const IField* TestDesc [2];
extern const ObjectType Test_Type;

extern const UInt8Field Achievement_Field_Arlen;
extern const ArrayField Achievement_Field_Ar;
extern const IField* AchievementDesc [2];
extern const ObjectType Achievement_Type;

extern const ObjectField Student_Field_ChineseAchievement;
extern const IField* StudentDesc [1];
extern const ObjectType Student_Type;


#endif
