#pragma  warning disable 0169
using System;
using EmbedXrpcIdlParser;

[FileName("idltest1.cs")]
public struct Test
{
    [FieldNumber(10)]
    byte IntPtrsLen;

    [FieldNumber(11)]
    [MaxCount(LenFieldName = "IntPtrsLen", MaxCount = 2, IsFixed = false)]
    int[] IntPtr;
}
[FileName("idltest1.cs")]
public struct Achievement
{

    [FieldNumber(10)]
    byte Arlen;

    [FieldNumber(8)]
    [MaxCount(LenFieldName="Arlen", MaxCount=2, IsFixed=false)]
    Test[] Ar;

}
[FileName("idltest1.cs")]
public struct Student
{
    [FieldNumber(4)]
    Achievement ChineseAchievement; 
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
