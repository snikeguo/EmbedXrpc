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
extern Int32Field Result_t_Field_Value;
extern Uint8Field Result_t_Field_NameLen;
extern ArrayField Result_t_Field_Name;
extern Int8Field Result_t_Field_Arg1;
extern ObjectType Result_t_Type;

extern Uint16Field Student_t_Field_ResultsLen;
extern ArrayField Student_t_Field_Results;
extern Int32Field Student_t_Field_Age;
extern ArrayField Student_t_Field_Name;
extern Uint8Field Student_t_Field_StudentIdLen;
extern ArrayField Student_t_Field_StudentId;
extern Uint8Field Student_t_Field_Sex;
extern ObjectType Student_t_Type;

extern Int32Field DateTime_t_Field_Year;
extern Int32Field DateTime_t_Field_Month;
extern Int32Field DateTime_t_Field_Day;
extern Int32Field DateTime_t_Field_Hour;
extern Int32Field DateTime_t_Field_Min;
extern Int32Field DateTime_t_Field_Sec;
extern ObjectType DateTime_t_Type;

extern Uint8Field StudentArray_t_Field_StudentIdLen;
extern ArrayField StudentArray_t_Field_Students;
extern ObjectType StudentArray_t_Type;

extern ObjectField BroadcastDataTimeStruct_Field_t;
extern ObjectType BroadcastDataTimeStruct_Type;

#define BroadcastDataTimeStruct_ServiceId 16
extern Uint8Field GetStudentInfoFormStudentId_Request_Field_StudentIdLen;
extern ArrayField GetStudentInfoFormStudentId_Request_Field_StudentId;
extern Int32Field GetStudentInfoFormStudentId_Request_Field_arg2;
extern Int32Field GetStudentInfoFormStudentId_Request_Field_arg3;
extern ObjectType GetStudentInfoFormStudentId_Request_Type;

#define GetStudentInfoFormStudentId_ServiceId 17
extern Uint8Field GetStudentInfoFormStudentId_Response_Field_State;
extern ObjectField GetStudentInfoFormStudentId_Response_Field_ReturnValue;
extern ObjectType GetStudentInfoFormStudentId_Response_Type;

extern ObjectType GetStudentsInfoFormAge_Request_Type;

#define GetStudentsInfoFormAge_ServiceId 18
extern Uint8Field GetStudentsInfoFormAge_Response_Field_State;
extern ObjectField GetStudentsInfoFormAge_Response_Field_ReturnValue;
extern ObjectType GetStudentsInfoFormAge_Response_Type;

extern ObjectType Test_Request_Type;

#define Test_ServiceId 19

#endif
