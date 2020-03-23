// EmbedXrpcSerialization.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "EmbedXrpcSerialization.h"
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
struct Result
{
    uint8_t Value;
    uint8_t Ret;
};
struct Student
{
    uint8_t Age;
    uint8_t StudentIdLen;
    uint8_t StudentId[16];
    uint8_t ResultLen;
    Result Result[2];
};


Uint8Field Result_Value_Field("Value",offsetof(Result, Value));
Uint8Field Result_Ret_Field("Ret", offsetof(Result, Ret));
IField* ResultDesc[] =
{
	(&Result_Value_Field),
    (&Result_Ret_Field),
};
ObjectField Result_Field("Struct:Result", 2, ResultDesc, offsetof(Student, Result));


Uint8Field Student_Age_Field("Age", offsetof(Student, Age));
Uint8Field Student_StudentIdLen_Field("StudentIdLen", offsetof(Student, StudentIdLen));
Uint8Type u8type;
ArrayField Student_StudentId_Field("StudentId", &u8type, sizeof(uint8_t), offsetof(Student, StudentId), &Student_StudentIdLen_Field);

Uint8Field Student_ResultLen_Field("ResultLen", offsetof(Student, ResultLen));
ArrayField Student_Results_Field("Results", &Result_Field, sizeof(Result), offsetof(Student, Result), &Student_ResultLen_Field);

IField* StudentDesc[] =
{
	(&Student_Age_Field),
	(&Student_StudentIdLen_Field),
	(&Student_StudentId_Field),
	(&Result_Field),
    (&Student_ResultLen_Field),
    (&Student_Results_Field),
};
ObjectField StudentField("Student", 6, StudentDesc, 0);

Student x;
Student y;
int main()
{
    SerializationManager manager;
    uint8_t buf[1024];
    memset(buf, 0, 1024);
    manager.Buf = buf;//(uint8_t *)malloc(1024);
    manager.BufferLen = 1024;
    manager.Index = 0;
    x.Age = 2;
    x.StudentIdLen = 3;
    x.StudentId[0] = 4;
    x.StudentId[1] = 5;
    x.StudentId[2] = 6;
    x.ResultLen = 2;
    x.Result[0].Value = 7;
    x.Result[0].Ret = 8;
	x.Result[1].Value = 9;
	x.Result[1].Ret = 10;
    StudentField.Serialize(manager, 9, &x);
    manager.Reset();
    StudentField.Deserialize(manager, &y);
    std::cout << "Hello World!\n";
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