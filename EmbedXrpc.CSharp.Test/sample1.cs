using System;
using EmbedXrpcIdlParser;

[FileName("sample1.cs")]
struct Student
{
    [FieldNumber(1)]
    int a;
    [FieldNumber(2)]
    int b;
}
[FileName("sample1.cs")]
enum Sex: byte
{
    Man=2,
    WoMan=9,
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
    [FieldNumber(7)]
    Student David;
    [FieldNumber(8)]
    Sex Sex;
}
[FileName("sample1.cs")]
delegate void DateTimeChange(DateTime_t[] now);

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
