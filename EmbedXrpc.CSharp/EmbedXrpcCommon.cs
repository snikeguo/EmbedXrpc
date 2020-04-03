using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpc
{
    public enum ResponseState
    {
        Ok,
        Timeout,
        SidError,
    }
    public enum ReceiveType
    {
        Request,
        Response,
        Delegate,
    };
    public abstract class IDelegate
    {
        public abstract UInt32 GetSid();
        public abstract void Invoke(SerializationManager recManager);
    }


    public abstract class IService
    {
        public abstract UInt32 GetSid();
        public abstract void Invoke(SerializationManager recManager, SerializationManager sendManager);
    }
    public class RequestMessageMap
    {
        public string Name { get; set; }
        public IService Service { get; set; }
    }
    public class ResponseDelegateMessageMap
    {
        public string Name { get; set; }
        public UInt32 Sid { get; set; }//有可能是Response/Delegate
        public ReceiveType ReceiveType { get; set; }
        public IDelegate Delegate { get; set; }
    };
    public delegate void Send(UInt32 sid, UInt32 dataLen, byte[] data);
    public struct EmbeXrpcRawData
    {
        public UInt32 Sid { get; set; }
        public byte[] Data { get; set; }
        public UInt32 DataLen { get; set; }
    }
}
