using System;
using EmbedXrpcIdlParser;
[FileName("sample1.cs")]
struct DateTime_t
{
    int Year;

    int Month;

    int Day;

    int Hour;

    int Min;

    int Sec;
}
[FileName("sample1.cs")]
delegate void DateTimeChange(DateTime_t[] now);

[FileName("sample1.cs")]
interface Inter
{
    Int32 Add(Byte a, Byte b);

    bool NoArg();
    void NoReturn();
    void NoArgAndReturn();
}

[FileName("sample1.cs")]
public class OptionProcess:IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "Sample1";
        return option;
    }
}
