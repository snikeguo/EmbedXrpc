#include"StudentService.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

const UInt8Field Achievement_Field_Value(7,"Achievement.Value",offsetof(Achievement,Value),false);
const IField* AchievementDesc []=
{
&Achievement_Field_Value,
};
const ObjectType Achievement_Type(sizeof(AchievementDesc)/sizeof(IField*),AchievementDesc);

const ObjectField Student_Field_ChineseAchievement(4,"Student.ChineseAchievement",sizeof(AchievementDesc)/sizeof(IField*),AchievementDesc,offsetof(Student,ChineseAchievement));
const Int32Field Student_Field_A(1,"Student.A",offsetof(Student,A), false);
const Int32Field Student_Field_B(2,"Student.B",offsetof(Student,B), false);
const Int32Field Student_Field_C(3,"Student.C",offsetof(Student,C), false);
const IField* StudentDesc []=
{
&Student_Field_ChineseAchievement,
&Student_Field_A,
&Student_Field_B,
&Student_Field_C,
};
const ObjectType Student_Type(sizeof(StudentDesc)/sizeof(IField*),StudentDesc);

