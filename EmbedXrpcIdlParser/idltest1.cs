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
    
    int Value;


    byte NameLen;


    [MaxCount(IsFixed=true, MaxCount =32,LenFieldName = "NameLen")]
    byte[] Name;


    sbyte Arg1;
}

[FileName("idltest1.cs")]
struct  Student_t
{

    UInt16 ResultsLen;


    [MaxCount(IsFixed = true, MaxCount = 32, LenFieldName = "ResultsLen")]
    Result_t[] Results;



    int Age;


    //byte NameLen;


    [MaxCount(IsFixed = false, MaxCount = 64)]
    byte[] Name;


    byte StudentIdLen;


    [MaxCount(IsFixed = true, MaxCount = 100, LenFieldName = "StudentIdLen")]
    byte[] StudentId;


    Sex_t Sex;

}
[FileName("idltest1.cs")]
struct DateTime_t
{

    int Year;

    int Month;

    int Day;

    int Hour;

    int Min;

    int Sec;
}
[FileName("idltest1.cs")]
struct StudentArray_t
{

    byte StudentIdLen;


    [MaxCount(IsFixed = true, MaxCount = 2, LenFieldName = "StudentIdLen")]
    Student_t[] Students;
}

[FileName("idltest1.cs")]
[ServiceId(99)]
delegate void BroadcastDataTime(DateTime_t t);
[FileName("idltest1.cs")]
interface IMyInterface
{
        [ServiceId(100)]
        Student_t GetStudentInfoFormStudentId(

        byte StudentIdLen,


        [MaxCount(IsFixed = true, MaxCount = 100, LenFieldName = "StudentIdLen")]
        byte[] StudentId,


        int arg2,


        int arg3
        );

    StudentArray_t GetStudentsInfoFormAge(
        //[FieldIndex(0)]int Age
        );//客户端从服务器获取数据：根据姓名筛选出那个学生

    bool Test(byte[]noLen);
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
