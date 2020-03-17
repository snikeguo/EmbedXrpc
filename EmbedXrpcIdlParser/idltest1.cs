using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using EmbedXrpcIdlParser;

enum Sex_t:byte
{
    Man,
    Woman
}
struct Result_t
{
    [FieldIndex(0)]
    int Value;

    [FieldIndex(1)]
    byte NameLen;

    [FieldIndex(2)]
    [MaxCount(IsFixed=true, MaxCount =32,LenFieldIndex =1)]
    byte[] Name;

    [FieldIndex(3)]
    sbyte Arg1;
}
struct  Student_t
{
    [FieldIndex(0)]
    UInt16 ResultsLen2;

    [FieldIndex(1)]
    [MaxCount(IsFixed = true, MaxCount = 32, LenFieldIndex = 0)]
    Result_t[] Results;


    [FieldIndex(2)]
    int Age;

    [FieldIndex(3)]
    byte NameLen;

    [FieldIndex(4)]
    [MaxCount(IsFixed = false, MaxCount = 64, LenFieldIndex = 3)]
    byte[] Name;

    [FieldIndex(5)]
    byte StudentIdLen;

    [FieldIndex(6)]
    [MaxCount(IsFixed = true, MaxCount = 100, LenFieldIndex = 5)]
    byte[] StudentId;

    [FieldIndex(7)]
    Sex_t Sex;
}
struct DateTime_t
{
    [FieldIndex(0)]
    int Year;
    [FieldIndex(1)]
    int Month;
    [FieldIndex(2)]
    int Day;
    [FieldIndex(3)]
    int Hour;
    [FieldIndex(4)]
    int Min;
    [FieldIndex(5)]
    int Sec;
}
struct StudentArray_t
{
    [FieldIndex(0)]
    byte StudentIdLen;

    [FieldIndex(1)]
    [MaxCount(IsFixed = true, MaxCount = 64, LenFieldIndex = 0)]
    Student_t[] Students;
}
delegate void BroadcastDataTime([FieldIndex(0)]DateTime_t t);
interface IMyInterface
{
        Student_t GetStudentInfoFormStudentId(
        [FieldIndex(0)]
        byte StudentIdLen,

        [FieldIndex(1)]
        [MaxCount(IsFixed = true, MaxCount = 100,LenFieldIndex = 1)]
        byte[] StudentId,

        [FieldIndex(1)]
        int arg2,

        [FieldIndex(2)]
        int arg3
        );

    StudentArray_t GetStudentsInfoFormAge(
        [FieldIndex(0)]int Age
        );//客户端从服务器获取数据：根据姓名筛选出那个学生
}

[GenerationOptionParameter(OutPutFileName="StudentService")]
public struct GenerationOption
{

}
