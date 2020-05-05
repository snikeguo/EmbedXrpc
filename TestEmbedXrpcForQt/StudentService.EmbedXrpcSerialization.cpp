#include"StudentService.h"
#include"StudentService.EmbedXrpcSerialization.h"

//auto code gen ! DO NOT modify this file!
//自动代码生成,请不要修改本文件!

const Int32Field BitsTest_Field_Field1("BitsTest.Field1",offsetof(BitsTest,Field1));
const Int32Field BitsTest_Field_Field2("BitsTest.Field2",offsetof(BitsTest,Field2));
const IField* BitsTestDesc []=
{
&BitsTest_Field_Field1,
&BitsTest_Field_Field2,
};
const ObjectType BitsTest_Type(sizeof(BitsTestDesc)/sizeof(IField*),BitsTestDesc);

const UInt64Field StudentBitTest_Field_bt("StudentBitTest.bt",offsetof(StudentBitTest,bt));
const IField* StudentBitTestDesc []=
{
&StudentBitTest_Field_bt,
};
const ObjectType StudentBitTest_Type(sizeof(StudentBitTestDesc)/sizeof(IField*),StudentBitTestDesc);

