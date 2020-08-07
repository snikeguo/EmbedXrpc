#pragma  warning disable 0169
using System;
using EmbedXrpcIdlParser;

[FileName("idltest1.cs")]
public struct Student
{
    [FieldNumber(0)]
    int A;
    [FieldNumber(1)]
    int B;
    [FieldNumber(2)]
    int C;
}
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
