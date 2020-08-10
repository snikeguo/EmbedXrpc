#pragma  warning disable 0169
using System;
using EmbedXrpcIdlParser;

[FileName("idltest1.cs")]
public struct Achievement
{
    [FieldNumber(7)]
    byte Value;
}
[FileName("idltest1.cs")]
public struct Student
{
    [FieldNumber(4)]
    Achievement ChineseAchievement;
    [FieldNumber(1)]
    int A;
    [FieldNumber(2)]
    int B;
    [FieldNumber(3)]
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
