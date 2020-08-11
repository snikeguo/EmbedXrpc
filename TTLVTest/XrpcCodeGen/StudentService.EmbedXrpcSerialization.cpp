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

