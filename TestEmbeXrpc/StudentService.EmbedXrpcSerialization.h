#ifndef StudentService_EmbedXrpcSerialization_H
#define StudentService_EmbedXrpcSerialization_H
//EmbedXrpc Code gen .By snikeguo.e-mail:snikeguo@foxmail.com
//关于snikeguo作者:92年生人,热爱技术,底层功底扎实.深入理解C语言底层到汇编层，单片机从外设/裸机到OS均有涉猎
//上位机专注于C# 界面库熟悉WPF.服务器专注于dotnet core
//行业经验：汽车电子.独自完成UDS(包括FBL)协议栈  GBT27930-2015(SAE J1939)协议栈等
//熟悉BMS业务逻辑,有一套自己稳定出货的BMS软件/硬件/上位机/服务器/USB转CAN&CANFD
//TITLE:高级嵌入式软件工程师.软件架构师
//微信:snikeguo.有好的职位推荐请加
#include"EmbedSerialization.h"
#ifndef offsetof
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
#endif
extern Int32Field Result_t_Value;
extern Uint8Field Result_t_NameLen;
extern ArrayField Result_t_Name;
extern Int8Field Result_t_Arg1;
extern ObjectType Result_t_Type;



extern Uint16Field Student_t_ResultsLen;
extern ArrayField Student_t_Results;
extern Int32Field Student_t_Age;
extern ArrayField Student_t_Name;
extern Uint8Field Student_t_StudentIdLen;
extern ArrayField Student_t_StudentId;
extern Uint8Field Student_t_Sex;
extern ObjectType Student_t_Type;



extern Int32Field DateTime_t_Year;
extern Int32Field DateTime_t_Month;
extern Int32Field DateTime_t_Day;
extern Int32Field DateTime_t_Hour;
extern Int32Field DateTime_t_Min;
extern Int32Field DateTime_t_Sec;
extern ObjectType DateTime_t_Type;



extern Uint8Field StudentArray_t_StudentIdLen;
extern ArrayField StudentArray_t_Students;
extern ObjectType StudentArray_t_Type;



extern ObjectField BroadcastDataTimeStruct_t;
extern ObjectType BroadcastDataTimeStruct_Type;



extern Uint8Field GetStudentInfoFormStudentId_Request_StudentIdLen;
extern ArrayField GetStudentInfoFormStudentId_Request_StudentId;
extern Int32Field GetStudentInfoFormStudentId_Request_arg2;
extern Int32Field GetStudentInfoFormStudentId_Request_arg3;
extern ObjectType GetStudentInfoFormStudentId_Request_Type;



extern ObjectField GetStudentInfoFormStudentId_Response_ReturnValue;
extern ObjectType GetStudentInfoFormStudentId_Response_Type;



extern ObjectType GetStudentsInfoFormAge_Request_Type;



extern ObjectField GetStudentsInfoFormAge_Response_ReturnValue;
extern ObjectType GetStudentsInfoFormAge_Response_Type;



extern ObjectType Test_Request_Type;




#endif
