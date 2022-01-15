using System;
using EmbedXrpcIdlParser;

[FileName("Sample3.cs")]
struct Student
{
    byte a;
    byte b;
    byte c;
    byte d;

    int e;

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
