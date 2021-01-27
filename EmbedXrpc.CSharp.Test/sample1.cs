using System;
using EmbedXrpcIdlParser;

[FileName("sample1.cs")]
struct Student
{
    [FieldNumber(1)]
    int a;
    [FieldNumber(2)]
    int b;

    [FieldNumber(3)]
    [UnionTargetType]
    int AddressType;

    [FieldNumber(4)]
    [UnionField]
    byte u1;


    [FieldNumber(5)]
    [UnionField]
    UInt64 u2;

    [FieldNumber(6)]
    [UnionField]
    Sex u3;

    [FieldNumber(7)]
    [UnionField]
    AddResult u4;


    [FieldNumber(100)]
    byte uend1;
    [FieldNumber(101)]
    byte uend2;
}
[FileName("sample1.cs")]
enum Sex : ulong
{
    Man = 2,
    WoMan = 9,
}
[FileName("sample1.cs")]
struct DateTime_t
{
    [FieldNumber(10086)]
    int Year;
    [FieldNumber(2)]
    int Month;
    [FieldNumber(3)]
    int Day;
    [FieldNumber(4)]
    int Hour;
    [FieldNumber(5)]
    int Min;
    [FieldNumber(6)]
    int Sec;

    [FieldNumber(8)]
    Sex Sex;

    [FieldNumber(7)]
    Student David;

}
[FileName("sample1.cs")]
delegate void DateTimeChange(DateTime_t[] now);

[FileName("sample1.cs")]
[ExternalParameter(true)]
delegate void TestDelegate(DateTime_t[] now);
[FileName("sample1.cs")]
struct AddResult
{
    [FieldNumber(1)]
    int Sum;
    [FieldNumber(2)]
    Int32 dataLen;
    [FieldNumber(3)]
    [MaxCount(LenFieldName = "dataLen", IsFixed = false)]
    byte[] data;

}
[FileName("sample1.cs")]
interface Inter
{
    [ExternalParameter(true)]
    AddResult Add(Int32 a, Int32 b, Int32 dataLen,
        [MaxCount(LenFieldName="dataLen",IsFixed=false)]
        byte[]data);

    bool NoArg();
    void NoReturn(int a);
    void NoArgAndReturn();
}

[FileName("sample1.cs")]
public class OptionProcess : IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "Sample1.gen";
        option.CSharpNameSpace = "Sample1";
        return option;
    }
}
