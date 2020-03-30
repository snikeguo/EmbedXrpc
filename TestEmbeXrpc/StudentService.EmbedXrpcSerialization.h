#ifndef StudentService_EmbedXrpcSerialization_H
#define StudentService_EmbedXrpcSerialization_H

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
extern ObjectType GetValue_Request_Type;

#define GetValue_Request_ServiceId 16
extern Uint8Field GetValue_Response_Field_State;
extern Uint16Field GetValue_Response_Field_ReturnValue;
extern ObjectType GetValue_Response_Type;

#define GetValue_Response_ServiceId 17
extern Uint16Field SetValue_Request_Field_v;
extern ObjectType SetValue_Request_Type;

#define SetValue_Request_ServiceId 18
#define SetValue_Response_ServiceId 19
extern MessageMap RequestMessages[];

#endif
