using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpc
{
    public enum RequestResponseState : Byte
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
        Request = 0x1,
        Response = 0x2,
    };
    public static class EmbedXrpcCommon
    {
        public static readonly UInt16 EmbedXrpcSuspendSid = 0x1;
        public static readonly UInt16 EmbedXrpcUnsupportedSid = 0x2;
    }

    public class ServiceInvokeParameter<DTL> 
    {
        public DTL Request_UserDataOfTransportLayer;
        public DTL Response_UserDataOfTransportLayer;
        public EmbedXrpcObject<DTL> RpcObject;
        public UInt16 TargetTimeOut;
    };


    public interface IService<DTL> 
    {
        UInt16 GetSid();
        Task Invoke(ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager);
    }
    public interface IRequestService<DTL> 
    {
        UInt16 GetSid();
    }
    public class ServiceDescribe<DTL> 
    {
        public string Name { get; set; }
        public IService<DTL> Service { get; set; }
    }
    /*public class RequestServiceDescribe
    {
        public string Name { get; set; }
        public UInt16 Sid { get; set; }//有可能是Response
    };*/
    public delegate Task<bool> Send<DTL>(DTL userDataOfTransportLayer, int dataLen, int offset, byte[] data);
    public struct EmbeXrpcRawData<DTL> 
    {
        public UInt16 Sid { get; set; }
        public UInt16 TargetTimeOut { get; set; }
        public byte[] Data { get; set; }
        public UInt32 DataLen { get; set; }
        public DTL UserDataOfTransportLayer { get; set; }
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
}
