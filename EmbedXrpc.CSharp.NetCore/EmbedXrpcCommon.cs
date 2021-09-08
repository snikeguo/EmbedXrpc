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
        Request=0x1,
        Response=0x2,
        Delegate=0x3,
    };
    public interface IDelegate<DTL> where DTL : struct
    {
        UInt16 GetSid();
        void Invoke(DTL userDataOfTransportLayer, SerializationManager recManager);
    }
    public static class EmbedXrpcCommon
    {
        public static readonly UInt16 EmbedXrpcSuspendSid = 0x1;
    }

    public interface IService<DTL> where DTL : struct
    {
        UInt16 GetSid();
        void Invoke(DTL request_UserDataOfTransportLayer,
         ref DTL response_UserDataOfTransportLayer,
         EmbedXrpcObject<DTL> rpcObject,
         UInt16 targetTimeOut, SerializationManager recManager, SerializationManager sendManager);
    }
    public interface IRequestService<DTL> where DTL : struct
    {
        UInt16 GetSid();
    }
    public class RequestDescribe<DTL> where DTL : struct
    {
        public string Name { get; set; }
        public IService<DTL> Service { get; set; }
    }
    public class DelegateDescribe<DTL> where DTL : struct
    {
        public string Name { get; set; }
        public IDelegate<DTL> Delegate { get; set; }
    };
    public class ResponseDescribe
    {
        public string Name { get; set; }
        public UInt16 Sid { get; set; }//有可能是Response/Delegate
    };
    public delegate bool Send<DTL>(DTL userDataOfTransportLayer,  int dataLen, int offset ,byte[] data);
    public struct EmbeXrpcRawData<DTL> where DTL : struct
    {
        public UInt16 Sid { get; set; }
        public UInt16 TargetTimeOut { get; set; }
        public byte[] Data { get; set; }
        public UInt32 DataLen { get; set; }
        public DTL UserDataOfTransportLayer { get; set; }
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
    public sealed class ResponseServiceInfoAttribute : Attribute
    {
        public ResponseServiceInfoAttribute()
        {
           
        }
        public string Name { get; set; }
        public UInt16 ServiceId { get; set; }
    }

    [System.AttributeUsage(AttributeTargets.Class, Inherited = false, AllowMultiple = true)]
    public sealed class RequestServiceInfoAttribute : Attribute
    {
       
        public RequestServiceInfoAttribute()
        {
            
        }
        // This is a named argument
        public string Name { get; set; }
        public UInt16 ServiceId { get; set; }
    }
    
}
