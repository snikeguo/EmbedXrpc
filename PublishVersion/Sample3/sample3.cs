using System;
using EmbedXrpcIdlParser;

[FileName("Sample3.cs")]
struct Student
{
    byte u8;
    sbyte s8;
    UInt16 u16;
    Int16 i16;
    UInt32 u32;
    Int32 i32;
    UInt64 u64;
    Int64 i64;
    float f;
    double d;
}

[FileName("Sample3.cs")]
public class OptionProcess : IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "Sample3";
        option.CSharpNameSpace = "Sample3";
        return option;
    }
}
