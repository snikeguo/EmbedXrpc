using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpc
{
    public enum ResponseState:UInt32
    {
        Ok=1,
        Timeout=2,
        SidError=3,
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
    public static class EmbedXrpcCommon
    {
        public static readonly UInt32 EmbedXrpcSuspendSid = 0x1;
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
    public delegate void Send(UInt32 sid, int dataLen, byte[] data);
    public struct EmbeXrpcRawData
    {
        public UInt32 Sid { get; set; }
        public byte[] Data { get; set; }
        public UInt32 DataLen { get; set; }
    }
    [AttributeUsage(AttributeTargets.Class, Inherited = false, AllowMultiple = true)]
    public sealed class DelegateInfoAttribute : Attribute
    {
        public DelegateInfoAttribute()
        {
        }
        public string Name { get; set; }
    }

    [System.AttributeUsage(AttributeTargets.Class, Inherited = false, AllowMultiple = true)]
    public sealed class ServiceInfoAttribute : Attribute
    {
        public ServiceInfoAttribute()
        {
           
        }
        public string Name { get; set; }
    }

    [System.AttributeUsage(AttributeTargets.Class, Inherited = false, AllowMultiple = true)]
    public sealed class ResponseInfoAttribute : Attribute
    {
       
        public ResponseInfoAttribute()
        {
            
        }
        // This is a named argument
        public string Name { get; set; }
        public uint ServiceId { get; set; }
    }
}
