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
struct DynamicStudentArray
{
    [FieldNumber(2)]
    Int32 dataLen;

    [MaxCount(LenFieldName = "dataLen", IsFixed = false)]
    Student[] FalseFixedStudent;

    [MaxCount(LenFieldName = "dataLen", MaxCount = 15, IsFixed = true)]
    Student[] TrueFixedStudent;

    [MaxCount(LenFieldName = "dataLen", IsFixed = false)]
    [NoSerialization]
    Student[] NoSerializationStudent;
}
[FileName("Sample3.cs")]
struct MixedStudentArray
{
    DynamicStudentArray DynamicStudent;
    Student OneStudent;
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
