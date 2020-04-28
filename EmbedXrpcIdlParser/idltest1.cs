#pragma  warning disable 0169
using System;
using EmbedXrpcIdlParser;
#if true
[FileName("idltest1.cs")]
enum Sex_t:uint
{
    Man,
    Woman,
    HHHH,
}
[FileName("idltest1.cs")]
struct Result_t
{
    ////[FieldIndex(0)]
    int Value;

    //[FieldIndex(1)]
    byte NameLen;

    //[FieldIndex(2)]
    [MaxCount(IsFixed=true, MaxCount =32,LenFieldName = "NameLen")]
    byte[] Name;

    //[FieldIndex(3)]
    sbyte Arg1;
}
[FileName("idltest1.cs")]
[Bits(BitsType.UInt64)]
struct BitsTest
{
    [BitsFieldLength(10)]
    int Field1;

    [BitsFieldLength(20)]
    int Field2;
}
[FileName("idltest1.cs")]
struct  Student_t
{
    //[FieldIndex(0)]
    UInt16 ResultsLen;

    //[FieldIndex(1)]
    [MaxCount(IsFixed = true, MaxCount = 32, LenFieldName = "ResultsLen")]
    Result_t[] Results;


    //[FieldIndex(2)]
    int Age;

    //[FieldIndex(3)]
    //byte NameLen;

    //[FieldIndex(4)]
    [MaxCount(IsFixed = false, MaxCount = 64)]
    byte[] Name;

    //[FieldIndex(5)]
    byte StudentIdLen;

    //[FieldIndex(6)]
    [MaxCount(IsFixed = true, MaxCount = 100, LenFieldName = "StudentIdLen")]
    byte[] StudentId;

    //[FieldIndex(7)]
    Sex_t Sex;

    BitsTest Bt;
}
[FileName("idltest1.cs")]
struct DateTime_t
{
    //[FieldIndex(0)]
    int Year;
    //[FieldIndex(1)]
    int Month;
    //[FieldIndex(2)]
    int Day;
    //[FieldIndex(3)]
    int Hour;
    //[FieldIndex(4)]
    int Min;
    //[FieldIndex(5)]
    int Sec;
}
[FileName("idltest1.cs")]
struct StudentArray_t
{
    //[FieldIndex(0)]
    byte StudentIdLen;

    //[FieldIndex(1)]
    [MaxCount(IsFixed = true, MaxCount = 2, LenFieldName = "StudentIdLen")]
    Student_t[] Students;
}

[FileName("idltest1.cs")]
delegate void BroadcastDataTime(DateTime_t t);
[FileName("idltest1.cs")]
interface IMyInterface
{
        Student_t GetStudentInfoFormStudentId(
        //[FieldIndex(0)]
        byte StudentIdLen,

        //[FieldIndex(1)]
        [MaxCount(IsFixed = true, MaxCount = 100, LenFieldName = "StudentIdLen")]
        byte[] StudentId,

        //[FieldIndex(1)]
        int arg2,

        //[FieldIndex(2)]
        int arg3
        );

    StudentArray_t GetStudentsInfoFormAge(
        //[FieldIndex(0)]int Age
        );//客户端从服务器获取数据：根据姓名筛选出那个学生

    bool Test(byte[]noLen);
}
#else
[FileName("idltest1.cs")]
interface IMyInterface
{
    UInt16 GetValue();
    void SetValue(UInt16 v);
}
#endif
[FileName("idltest1.cs")]
public class OptionProcess:IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "StudentService";
        option.CSharpNameSpace = "StudentService";
        return option;
    }
}
