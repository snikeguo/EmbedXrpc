using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpc
{
    public enum RequestResponseState:Byte
    {
        RequestState_Ok = 1,
        RequestState_Failed = 2,
        ResponseState_Ok = 3,
        ResponseState_Timeout = 4,
        ResponseState_SidError = 5,
        ResponseState_UnsupportedSid = 6,
    }
    public enum ReceiveType
    {
        Request,
        Response,
        Delegate,
    };
    public abstract class IDelegate
    {
        public abstract UInt16 GetSid();
        public abstract void Invoke(SerializationManager recManager);
    }
    public static class EmbedXrpcCommon
    {
        public static readonly UInt16 EmbedXrpcSuspendSid = 0x1;
    }

    public abstract class IService
    {
        public abstract UInt16 GetSid();
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
        public UInt16 Sid { get; set; }//有可能是Response/Delegate
        public ReceiveType ReceiveType { get; set; }
        public IDelegate Delegate { get; set; }
    };
    public delegate bool Send(int dataLen, int offset ,byte[] data);
    public struct EmbeXrpcRawData
    {
        public UInt16 Sid { get; set; }
        public UInt16 TargetTimeOut { get; set; }
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
        public UInt16 ServiceId { get; set; }
    }
}
