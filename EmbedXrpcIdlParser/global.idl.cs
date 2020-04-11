using System;
using EmbedXrpcIdlParser;
[FileName("global.idl.cs")]
enum CAN_RTR:byte
{
    CAN_FRAME_DATA=0,
    CAN_FRAME_REMOTE=1,
}
[FileName("global.idl.cs")]
enum CAN_IDE:byte
{
    CAN_IDE_STD=0,
    CAN_IDE_EXT=1,
}
[FileName("global.idl.cs")]
enum CAN_FormatFilter:byte
{
    MatchBaseFormat=0,
    MatchExtendedFormat=1,
    MatchBaseAndExtendedFormat=2,
};
[FileName("global.idl.cs")]
enum CAN_FrameType:byte
{
    UnknownFrame=0,
    DataFrame=1,
    ErrorFrame=2,
    RemoteRequestFrame=3,
    InvalidFrame=4,
};
[FileName("global.idl.cs")]
enum CAN_RunMode:byte
{
    CAN_RunMode_Normal=0,
    CAN_RunMode_LoopBack=1,
    CAN_RunMode_Silent=2,
    CAN_RunMode_SilentLoopBack=3,
}
[FileName("global.idl.cs")]
public struct CAN_RxPacket
{
    UInt32 Identifier;

    Byte DataLen;

    [MaxCount(IsFixed = true, MaxCount = 8, LenFieldName = "DataLen")]
    Byte []Data;

    CAN_RTR Rtr;

    CAN_IDE Ide;

    UInt64 TimeStamp;
}
[FileName("global.idl.cs")]
public struct CANFD_RxPacket
{
    UInt32 Identifier;

    Byte DataLen;

    [MaxCount(IsFixed = true, MaxCount = 64, LenFieldName = "DataLen")]
    Byte []Data;
    
    CAN_RTR CanRtr;
    CAN_IDE CanIde;
    Byte Brs;
    Byte Esi;
    UInt64 TimeStamp;
};
[FileName("global.idl.cs")]
public struct CAN_TxPacket
{
    UInt32 Identifier;

    Byte DataLen;

    [MaxCount(IsFixed = true, MaxCount = 8, LenFieldName = "DataLen")]
    Byte []Data;
    CAN_RTR CanRtr;
    CAN_IDE CanIde;
    //Byte ChannelIndex;
    UInt32 Key;
};
[FileName("global.idl.cs")]
public struct CANFD_TxPacket
{
    UInt32 Identifier;
    Byte DataLen;
    [MaxCount(IsFixed = true, MaxCount = 64, LenFieldName = "DataLen")]
    Byte []Data;
    
    CAN_RTR CanRtr;
    CAN_IDE CanIde;
    Byte Brs;
    Byte Esi;
    //Byte ChannelIndex;
    UInt32 Key;
};
[FileName("global.idl.cs")]
public struct Can_ErrorResult
{
    UInt32 Key;
    UInt32 ErrorCode;
};//MCU-->PC  Key所对应的CAN帧是失败的。
[FileName("global.idl.cs")]
public struct CAN_Config
{
    Byte             IsEnableCanFd;
    Byte             IsEnableFilter;
    Byte             IsEnableChannel;
    CAN_RunMode             RunMode;
    UInt32                BaudRate;
    CAN_FormatFilter             Format;
    UInt32            FrameId;
    UInt32            FrameIdMask;
    CAN_FrameType             Type;
};
[FileName("global.idl.cs")]
public class GlobalOptionProcess:IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "Usb2Can.Global";
        option.CSharpNameSpace = "Usb2Can";
        return option;
    }
}
