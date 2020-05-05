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
extern const Int32Field Result_t_Field_Value;
extern const UInt8Field Result_t_Field_NameLen;
extern const ArrayField Result_t_Field_Name;
extern const Int8Field Result_t_Field_Arg1;
extern const IField* Result_tDesc [4];
extern const ObjectType Result_t_Type;

extern const UInt16Field Student_t_Field_ResultsLen;
extern const ArrayField Student_t_Field_Results;
extern const Int32Field Student_t_Field_Age;
extern const ArrayField Student_t_Field_Name;
extern const UInt8Field Student_t_Field_StudentIdLen;
extern const ArrayField Student_t_Field_StudentId;
extern const UInt32Field Student_t_Field_Sex;
extern const IField* Student_tDesc [7];
extern const ObjectType Student_t_Type;

extern const Int32Field DateTime_t_Field_Year;
extern const Int32Field DateTime_t_Field_Month;
extern const Int32Field DateTime_t_Field_Day;
extern const Int32Field DateTime_t_Field_Hour;
extern const Int32Field DateTime_t_Field_Min;
extern const Int32Field DateTime_t_Field_Sec;
extern const IField* DateTime_tDesc [6];
extern const ObjectType DateTime_t_Type;

extern const UInt8Field StudentArray_t_Field_StudentIdLen;
extern const ArrayField StudentArray_t_Field_Students;
extern const IField* StudentArray_tDesc [2];
extern const ObjectType StudentArray_t_Type;

extern const ObjectField BroadcastDataTimeStruct_Field_t;
extern const IField* BroadcastDataTimeStructDesc [1];
extern const ObjectType BroadcastDataTimeStruct_Type;

#define BroadcastDataTime_ServiceId 16
extern const UInt8Field IMyInterface_GetStudentInfoFormStudentId_Request_Field_StudentIdLen;
extern const ArrayField IMyInterface_GetStudentInfoFormStudentId_Request_Field_StudentId;
extern const Int32Field IMyInterface_GetStudentInfoFormStudentId_Request_Field_arg2;
extern const Int32Field IMyInterface_GetStudentInfoFormStudentId_Request_Field_arg3;
extern const IField* IMyInterface_GetStudentInfoFormStudentId_RequestDesc [4];
extern const ObjectType IMyInterface_GetStudentInfoFormStudentId_Request_Type;

#define IMyInterface_GetStudentInfoFormStudentId_ServiceId 17
extern const UInt8Field IMyInterface_GetStudentInfoFormStudentId_Response_Field_State;
extern const ObjectField IMyInterface_GetStudentInfoFormStudentId_Response_Field_ReturnValue;
extern const IField* IMyInterface_GetStudentInfoFormStudentId_ResponseDesc [2];
extern const ObjectType IMyInterface_GetStudentInfoFormStudentId_Response_Type;

extern const ObjectType IMyInterface_GetStudentsInfoFormAge_Request_Type;

#define IMyInterface_GetStudentsInfoFormAge_ServiceId 18
extern const UInt8Field IMyInterface_GetStudentsInfoFormAge_Response_Field_State;
extern const ObjectField IMyInterface_GetStudentsInfoFormAge_Response_Field_ReturnValue;
extern const IField* IMyInterface_GetStudentsInfoFormAge_ResponseDesc [2];
extern const ObjectType IMyInterface_GetStudentsInfoFormAge_Response_Type;

extern const ArrayField IMyInterface_Test_Request_Field_noLen;
extern const IField* IMyInterface_Test_RequestDesc [1];
extern const ObjectType IMyInterface_Test_Request_Type;

#define IMyInterface_Test_ServiceId 19
extern const UInt8Field IMyInterface_Test_Response_Field_State;
extern const UInt8Field IMyInterface_Test_Response_Field_ReturnValue;
extern const IField* IMyInterface_Test_ResponseDesc [2];
extern const ObjectType IMyInterface_Test_Response_Type;


#endif
