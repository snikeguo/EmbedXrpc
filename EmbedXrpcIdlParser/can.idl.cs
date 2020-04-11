//css_import global.idl.cs
using System;
using EmbedXrpcIdlParser;


[FileName("can.idl.cs")]
public delegate void CanFrameReceived([MaxCount(IsFixed = true)]
                                        CAN_RxPacket[] rx);

[FileName("can.idl.cs")]                                        
public interface ISendCan
{
    void SendCanFrame([MaxCount(IsFixed = true)]
    CAN_TxPacket[] tx);
}

[FileName("can.idl.cs")]
public class CanOptionProcess:IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "testcan";
        option.CSharpNameSpace = "Usb2Can";
        option.UserIncludes.Add("Usb2Can.Global");
        option.UserIncludes.Add("Usb2Can.Global.EmbedXrpcSerialization");
        option.UserNamespace.Add("Usb2Can");
        return option;
    }
}
