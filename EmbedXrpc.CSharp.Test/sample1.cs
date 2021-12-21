using System;
using EmbedXrpcIdlParser;

[FileName("sample1.cs")]
struct AddResult
{

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
}


[FileName("sample1.cs")]
struct Student
{


    [NoSerialization]
    [FieldNumber(1)]
    int a;
    [FieldNumber(2)]
    int b;

    [FieldNumber(3)]
    [UnionTargetType]
    int AddressType;

    [FieldNumber(4)]
    [UnionField]
    byte u1;


    [FieldNumber(5)]
    [UnionField]
    UInt64 u2;

    [FieldNumber(6)]
    [UnionField]
    Sex u3;

    [FieldNumber(7)]
    [UnionField]
    AddResult u4;


    [FieldNumber(8)]
    [BitField(11)]
    uint Sum;

    [FieldNumber(9)]
    [BitField(19)]
    uint Sum2;

    [FieldNumber(10)]
    [BitField(2)]
    uint Sum3;

    [FieldNumber(100)]
    byte uend1;
    [FieldNumber(101)]
    byte uend2;

    [FieldNumber(11)]
    [BitField(11)]
    UInt64 Sum4;

    [BitField(19)]
    [FieldNumber(12)]
    UInt64 Sum5;

    [BitField(2)]
    [FieldNumber(13)]
    UInt64 Sum6;

    [FieldNumber(14)]
    UInt32 Sum7;

}
[FileName("sample1.cs")]
enum Sex : ulong
{
    Man = 2,
    WoMan = 9,
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

    [FieldNumber(7)]
    Student David;

}
[FileName("sample1.cs")]
[Role(RoleType.Server)]
delegate void DateTimeChange(DateTime_t[] now);


[FileName("sample1.cs")]
[ExternalParameter(true)]
[Role(RoleType.Client)]
delegate AddResult Add(Int32 a, Int32 b, Int32 dataLen,
        [MaxCount(LenFieldName="dataLen",IsFixed=false)]
        byte[]data);

[FileName("sample1.cs")]
public class OptionProcess : IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "Sample1.gen";
        option.CSharpNameSpace = "Sample1";
        return option;
    }
}
