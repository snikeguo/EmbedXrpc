using System;
using EmbedXrpcIdlParser;
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
}

[FileName("sample1.cs")]
delegate void DateTimeChange(DateTime_t[] now);

[FileName("sample1.cs")]
interface Inter
{
    Int32 Add(Int32 a, Int32 b);

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
        option.OutPutFileName = "Sample1";
        option.CSharpNameSpace = "Sample1";
        return option;
    }
}
