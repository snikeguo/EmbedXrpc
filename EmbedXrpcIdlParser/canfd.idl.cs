//css_import global.idl.cs
using System;
using EmbedXrpcIdlParser;


[FileName("canfd.idl.cs")]
public delegate void CanFdReceived(
                                        byte RxLen,
                                        [MaxCount(IsFixed = true, MaxCount = 7, LenFieldName = "RxLen")]
                                        CANFD_RxPacket[] rx);

[FileName("canfd.idl.cs")]
public delegate void CanFdSendErrorReport(byte errorLen,
                                        [MaxCount(IsFixed = true, MaxCount = 100, LenFieldName = "errorLen")]
                                        Can_ErrorResult[] rx);                                          

[FileName("canfd.idl.cs")]                                        
public interface ISendCanFd
{
    void CanFdSend([MaxCount(IsFixed = true)]
    CANFD_TxPacket[] tx);
}

[FileName("canfd.idl.cs")]
public class CanFdOptionProcess:IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "usb2can.canfd";
        option.CSharpNameSpace = "Usb2Can";
        option.UserIncludes.Add("Usb2Can");
        option.UserIncludes.Add("Usb2Can.EmbedXrpcSerialization");
        option.UserNamespace.Add("Usb2Can");
        return option;
    }
}
