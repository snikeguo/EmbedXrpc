// EmbedXrpcSerialization.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "EmbedXrpcSerialization.h"

DefineBaseValueInstance(Uint8);
DefineBaseValueInstance(Int8);
DefineBaseValueInstance(Uint16);
DefineBaseValueInstance(Int16);
DefineBaseValueInstance(Uint32);
DefineBaseValueInstance(Int32);
DefineBaseValueInstance(Uint64);
DefineBaseValueInstance(Int64);
DefineBaseValueInstance(Float);
DefineBaseValueInstance(Double);


struct Result
{
	uint8_t Value;
	uint8_t Ret;
	int16_t ArgLen;
	int32_t* Args;
};
struct Student
{
	uint8_t Age;
	uint8_t StudentIdLen;
	uint8_t StudentId[16];
	uint8_t ResultLen;
	Result* Result;
};

Uint8Field Result_Value_Field("Result.Value",offsetof(Result, Value));
Uint8Field Result_Ret_Field("Result.Ret", offsetof(Result, Ret));
Int16Field Result_ArgLen_Field("Result.ArgLen", offsetof(Result, ArgLen));
ArrayField Result_Args_Field("Result.Args", false, &Int32TypeInstance, sizeof(int32_t), offsetof(Result, Args), &Result_ArgLen_Field);
IField* ResultDesc[] =
{
	(&Result_Value_Field),
    (&Result_Ret_Field),
	&Result_ArgLen_Field,
    &Result_Args_Field
};
ObjectType Result_Type(sizeof(ResultDesc)/sizeof(IField), ResultDesc);


Uint8Field Student_Age_Field("Student.Age", offsetof(Student, Age));
Uint8Field Student_StudentIdLen_Field("Student.StudentIdLen", offsetof(Student, StudentIdLen));

ArrayField Student_StudentId_Field("Student.StudentId",true, &Uint8TypeInstance, sizeof(uint8_t), offsetof(Student, StudentId), &Student_StudentIdLen_Field);

Uint8Field Student_ResultLen_Field("ResultLen", offsetof(Student, ResultLen));
ArrayField Student_Results_Field("Results", false, &Result_Type, sizeof(Result), offsetof(Student, Result), &Student_ResultLen_Field);

IField* StudentDesc[] =
{
	(&Student_Age_Field),
	(&Student_StudentIdLen_Field),
	(&Student_StudentId_Field),
    (&Student_ResultLen_Field),
    (&Student_Results_Field),
};
ObjectType StudentType(sizeof(StudentDesc) / sizeof(IField), StudentDesc);

Student x;
Student y;
Result xr[3];
int32_t xrArgs0[16] = { 9,8,7,6,5,4,3,2};
int32_t xrArgs1[16] = { 1,2,3,4,5,6,7,8 };
int32_t xrArgs2[16] = { 6,2,3,4,5,6,7,8 };
uint8_t buf[1024];
int main()
{
    SerializationManager manager;
    
    memset(buf, 0, 1024);
    manager.Buf = buf;//(uint8_t *)malloc(1024);
    manager.BufferLen = 1024;
    manager.Index = 0;
    /*x.Age = 2;
    x.StudentIdLen = 3;
    x.StudentId[0] = 4;
    x.StudentId[1] = 5;
    x.StudentId[2] = 6;*/
    x.ResultLen = 3;
    x.Result = xr;


    x.Result[0].Value = 7;
    x.Result[0].Ret = 10;
    x.Result[0].ArgLen = 8;
    x.Result[0].Args = xrArgs0;


    x.Result[1].Value = 9;
    x.Result[1].Ret = 10;
	x.Result[1].ArgLen = 10;
	x.Result[1].Args = xrArgs1;

	x.Result[2].Value = 9;
	x.Result[2].Ret = 10;
	x.Result[2].ArgLen = 10;
	x.Result[2].Args = xrArgs2;

    StudentType.Serialize(manager, 9, &x);
    manager.Reset();
    StudentType.Deserialize(manager, &y);
    StudentType.Free(&y);
    std::cout << "Hello World!\n";
    int a;
    std::cin >> a;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件