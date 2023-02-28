using System;
using EmbedXrpcIdlParser;

[FileName("sample1.cs")]
struct AddResult
{
    bool boolTest;
    [FieldNumber(1)]
    [BitField(11)]
    uint Sum;

    [BitField(19)]
    uint Sum2;

    [BitField(2)]
    uint Sum3;

    [FieldNumber(2)]
    Int32 dataLen;

    [FieldNumber(3)]
    [MaxCount(LenFieldName = "dataLen", IsFixed = false)]
    [NoSerialization]
    byte[] data;


    [FieldNumber(1)]
    [BitField(11)]
    UInt64 Sum4;

    [BitField(19)]
    UInt64 Sum5;

    [BitField(2)]
    
    UInt64 Sum6;

    
    UInt32 Sum7;
    [NoSerialization]
    IntPtr PtrTest;
}

[FileName("sample1.cs")]
struct Student
{
    [FieldNumber(1)]
    [MacroControl("MyMacro", "TRUE")]
    int a;
    [FieldNumber(2)]
    int b;

    [FieldNumber(3)]
    [UnionTargetType]
    int AddressType;

    [FieldNumber(4)]
    [UnionField]
    [MacroControl("MyMacro", "TRUE")]
    byte u1;


    [FieldNumber(5)]
    [UnionField]
    [MacroControl("MyMacro", "TRUE")]
    UInt64 u2;

    [FieldNumber(6)]
    [UnionField]
    [MacroControl("MyMacro", "TRUE")]
    Sex u3;

    [FieldNumber(7)]
    [UnionField]
    [MacroControl("MyMacro", "TRUE")]
    AddResult u4;


    [FieldNumber(100)]
    [MacroControl("MyMacro", "TRUE")]
    byte uend1;
    //[FieldNumber(101)]
    [MacroControl("MyMacro", "TRUE")]
    byte uend2;
}
[FileName("sample1.cs")]
enum Sex: ulong
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

    [FieldNumber(8)]
    Sex Sex;

    byte DateStringLen;

    [MaxCount(LenFieldName = "DateStringLen", IsFixed = false)]
    [MacroControl("MyMacro", "TRUE")]
    byte[] DateString;

    [FieldNumber(7)]
    Student David;
}

[FileName("sample1.cs")]
//[Role(RoleType.Server)]
delegate void DateTimeChange(DateTime_t[] now);

[FileName("sample1.cs")]
[ExternalParameter(true)]
//[Role(RoleType.Server)]
delegate void Test2(DateTime_t[] now);

[FileName("sample1.cs")]
[ExternalParameter(true)]
//[Role(RoleType.Client)]
delegate AddResult Add(Int32 a, Int32 b, Int32 dataLen,
        [MaxCount(LenFieldName="dataLen",IsFixed=false)]
        byte[]data);

[FileName("sample1.cs")]
//[Role(RoleType.Client)]
delegate bool NoArg();

[FileName("sample1.cs")]
//[Role(RoleType.Client)]
delegate void NoReturn(int a);

[FileName("sample1.cs")]
//[Role(RoleType.Client)]
delegate void NoArgAndReturn();


[FileName("sample1.cs")]
public class OptionProcess : IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "Sample1";
        option.UserIncludes.Add("MyInclude.h");
        option.CSharpNameSpace = "Sample1";
        option.UserNote = "这是一个注释!";
        return option;
    }
}
