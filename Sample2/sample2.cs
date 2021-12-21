using System;
using EmbedXrpcIdlParser;

[FileName("Sample2.cs")]
struct GetSumResult
{
    bool IsSuccess;
    int Value;
}


[FileName("Sample2.cs")]
[Role(RoleType.Client)]
delegate GetSumResult GetSum(int a, int b);

[FileName("Sample2.cs")]
public class OptionProcess : IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "Sample2";
        option.CSharpNameSpace = "Sample2";
        return option;
    }
}
