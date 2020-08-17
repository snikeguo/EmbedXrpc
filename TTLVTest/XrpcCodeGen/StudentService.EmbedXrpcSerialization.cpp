#include"StudentService.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

const UInt8Field Test_Field_IntPtrsLen(10,"Test.IntPtrsLen",offsetof(Test,IntPtrsLen),true);
const ArrayField Test_Field_IntPtr(11,"Test.IntPtr",false,&Int32TypeInstance,sizeof(Int32),offsetof(Test,IntPtr),&Test_Field_IntPtrsLen);
const IField* TestDesc []=
{
&Test_Field_IntPtrsLen,
&Test_Field_IntPtr,
};
const ObjectType Test_Type(sizeof(TestDesc)/sizeof(IField*),TestDesc);

const UInt8Field Achievement_Field_Arlen(10,"Achievement.Arlen",offsetof(Achievement,Arlen),true);
const ArrayField Achievement_Field_Ar(8,"Achievement.Ar",false,&Test_Type,sizeof(Test),offsetof(Achievement,Ar),&Achievement_Field_Arlen);
const IField* AchievementDesc []=
{
&Achievement_Field_Arlen,
&Achievement_Field_Ar,
};
const ObjectType Achievement_Type(sizeof(AchievementDesc)/sizeof(IField*),AchievementDesc);

const ObjectField Student_Field_ChineseAchievement(4,"Student.ChineseAchievement",sizeof(AchievementDesc)/sizeof(IField*),AchievementDesc,offsetof(Student,ChineseAchievement));
const IField* StudentDesc []=
{
&Student_Field_ChineseAchievement,
};
const ObjectType Student_Type(sizeof(StudentDesc)/sizeof(IField*),StudentDesc);

const Int32Field DelegateTestStruct_Field_index(1,"DelegateTestStruct.index",offsetof(DelegateTestStruct,index),false);
const ObjectField DelegateTestStruct_Field_student(2,"DelegateTestStruct.student",sizeof(StudentDesc)/sizeof(IField*),StudentDesc,offsetof(DelegateTestStruct,student));
const IField* DelegateTestStructDesc []=
{
&DelegateTestStruct_Field_index,
&DelegateTestStruct_Field_student,
};
const ObjectType DelegateTestStruct_Type(sizeof(DelegateTestStructDesc)/sizeof(IField*),DelegateTestStructDesc);

const Int32Field MyInerface_GetStudent_Request_Field_index(1,"MyInerface_GetStudent_Request.index",offsetof(MyInerface_GetStudent_Request,index),false);
const IField* MyInerface_GetStudent_RequestDesc []=
{
&MyInerface_GetStudent_Request_Field_index,
};
const ObjectType MyInerface_GetStudent_Request_Type(sizeof(MyInerface_GetStudent_RequestDesc)/sizeof(IField*),MyInerface_GetStudent_RequestDesc);

const UInt8Field MyInerface_GetStudent_RequestResponseContent_Field_State(1,"MyInerface_GetStudent_RequestResponseContent.State",offsetof(MyInerface_GetStudent_RequestResponseContent,State),false);
const ObjectField MyInerface_GetStudent_RequestResponseContent_Field_ReturnValue(2,"MyInerface_GetStudent_RequestResponseContent.ReturnValue",sizeof(StudentDesc)/sizeof(IField*),StudentDesc,offsetof(MyInerface_GetStudent_RequestResponseContent,ReturnValue));
const IField* MyInerface_GetStudent_RequestResponseContentDesc []=
{
&MyInerface_GetStudent_RequestResponseContent_Field_State,
&MyInerface_GetStudent_RequestResponseContent_Field_ReturnValue,
};
const ObjectType MyInerface_GetStudent_RequestResponseContent_Type(sizeof(MyInerface_GetStudent_RequestResponseContentDesc)/sizeof(IField*),MyInerface_GetStudent_RequestResponseContentDesc);

