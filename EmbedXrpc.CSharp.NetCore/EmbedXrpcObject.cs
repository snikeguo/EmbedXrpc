using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using David.Common;
namespace EmbedXrpc
{

    public class EmbedXrpcObject<DTL> where DTL:struct  
    {
        public UInt32 TimeOut { get; set; }
        public object ObjectMutex { get; private set; } = new object();
        
        public Win32Queue<EmbeXrpcRawData<DTL>> MessageQueueOfRequestServiceHandle { get; private set; } = new Win32Queue<EmbeXrpcRawData<DTL>>();
        private List<RequestServiceDescribe> AllRequestServices { get; set; } = new List<RequestServiceDescribe>();
        private List<ServiceDescribe<DTL>> AllServices { get; set; } = new List<ServiceDescribe<DTL>>();

        public Send<DTL> Send;
        private Assembly Assembly;

        //server
        private Timer SuspendTimer;
        public DTL UserDataOfTransportLayerOfSuspendTimerUsed { get; set; }
        public void Start_SuspendTimer(int period)
        {
            SuspendTimer.Change(period, period);
        }
        public void Stop_SuspendTimer()
        {
            SuspendTimer.Change(Timeout.Infinite, Timeout.Infinite);
        }

        private Thread ServiceThreadHandle;
        private CancellationTokenSource ServiceThreadCancellationTokenSource;
        private Win32Queue<EmbeXrpcRawData<DTL>> ServiceQueueHandle = new Win32Queue<EmbeXrpcRawData<DTL>>();
        
        //public bool IsEnableMataDataEncode { get; set; }
        public EmbedXrpcObject(UInt32 timeout, Send<DTL> send, Assembly assembly)
        {
            //IsEnableMataDataEncode = isEnableMataDataEncode;
            var types = assembly.GetTypes();
            Assembly = assembly;
            foreach (var oldtype in types)
            {
                var requestInfoAttr = oldtype.GetCustomAttribute<RequestServiceInfoAttribute>();
                if (requestInfoAttr != null)
                {
                    RequestServiceDescribe request = new RequestServiceDescribe();
                    request.Name = requestInfoAttr.Name;
                    request.Sid = requestInfoAttr.ServiceId;
                    AllRequestServices.Add(request);
                }


                var responseServiceInfoAttr = oldtype.GetCustomAttribute<ResponseServiceInfoAttribute>();
                if (responseServiceInfoAttr != null)
                {
                    var dtlType = typeof(DTL);
                    var genericType = oldtype.MakeGenericType(dtlType);
                    ServiceDescribe<DTL> service = new ServiceDescribe<DTL>();
                    service.Name = responseServiceInfoAttr.Name;
                    service.Service = (IService<DTL>)assembly.CreateInstance(genericType.FullName);
                    AllServices.Add(service);
                }
            }

            TimeOut = timeout;
            Send = send;

            ServiceThreadHandle = new Thread(ServiceThread);
            ServiceThreadHandle.IsBackground = true;
            ServiceThreadCancellationTokenSource = new CancellationTokenSource();

            SuspendTimer = new Timer(SuspendTimerCallback, this, Timeout.Infinite, Timeout.Infinite);
        }
        private void SuspendTimerCallback(object s)
        {
            EmbedXrpcObject<DTL> server = s as EmbedXrpcObject<DTL>;
            byte[] sendBytes = new byte[4];
            sendBytes[0] = (byte)(EmbedXrpcCommon.EmbedXrpcSuspendSid >> 0 & 0xff);
            sendBytes[1] = (byte)(EmbedXrpcCommon.EmbedXrpcSuspendSid >> 8 & 0xff);
            sendBytes[2] = (byte)(server.TimeOut >> 0 & 0xff);
            sendBytes[3] = (byte)((server.TimeOut >> 8 & 0xff) & (0x3F));
            sendBytes[3] |= ((byte)(ReceiveType.Response)) << 6;
            Send(UserDataOfTransportLayerOfSuspendTimerUsed,sendBytes.Length, 0, sendBytes);
        }

        
        public void Start()
        {
            ServiceThreadHandle.Start();
        }
        public void Stop()
        {
            ServiceThreadCancellationTokenSource.Cancel();
            //ServiceThreadHandle.Abort();
        }
        public RequestResponseState Wait<T>(UInt32 sid,ref T response) where T: IEmbedXrpcSerialization
        {
            EmbeXrpcRawData<DTL> recData;
            //response = default(T);
            Type res_t =typeof(T);
            RequestResponseState ret = RequestResponseState.ResponseState_Ok;
            while (true)
            {
                if (MessageQueueOfRequestServiceHandle.Receive(out recData, TimeOut) != QueueStatus.OK)
                {
                    return RequestResponseState.ResponseState_Timeout;
                }
                if(recData.Sid== EmbedXrpcCommon.EmbedXrpcSuspendSid)
                {
                    Debug.WriteLine("{0}:sid== EmbedXrpcCommon.EmbedXrpcSuspendSid. this.timeout is:{1}", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"),TimeOut);
                    continue;
                }
                else if(recData.Sid== EmbedXrpcCommon.EmbedXrpcUnsupportedSid)
                {
                    Debug.WriteLine("{0}:sid== EmbedXrpcCommon.EmbedXrpcUnsupportedSid. this.timeout is:{1}", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"),TimeOut);
                    ret= RequestResponseState.ResponseState_UnsupportedSid;
                }
                else if (sid != recData.Sid)
                {
                    ret= RequestResponseState.ResponseState_SidError;
                }
                break;
            }
            if(ret == RequestResponseState.ResponseState_Ok)
            {
                SerializationManager sm = new SerializationManager(Assembly, new List<byte>(recData.Data));
                response.Deserialize(sm);
            }
            return ret;
        }
        public void ReceivedMessage(UInt32 validdataLen, UInt32 offset,byte[] alldata,DTL userDataOfTransportLayer)
        {
            QueueStatus queueStatus = QueueStatus.Empty;
            if (alldata.Length < offset + validdataLen)
            {
                goto sqs;
            }
            UInt16 serviceId = (UInt16)(alldata[0 + offset] << 0 | alldata[1 + offset] << 8);
            UInt16 targettimeout = (UInt16)((alldata[2 + offset] << 0) | ((alldata[3 + offset])&0x3F)<<8);
            UInt32 dataLen = validdataLen - 4;
            EmbeXrpcRawData<DTL> raw = new EmbeXrpcRawData<DTL>();
            raw.UserDataOfTransportLayer = userDataOfTransportLayer;
            ReceiveType rt = (ReceiveType)(alldata[3 + offset] >> 6);
            if (rt== ReceiveType.Response)
            {
                raw.Sid = serviceId;
                if (dataLen > 0)
                {
                    raw.Data = new byte[dataLen];
                    Array.Copy(alldata, offset + 4, raw.Data, 0, dataLen);
                }
                raw.DataLen = dataLen;
                raw.TargetTimeOut = targettimeout;
                queueStatus = MessageQueueOfRequestServiceHandle.Send(raw);
                goto sqs;
            }
            else if(rt == ReceiveType.Request)
            {
                raw.Sid = serviceId;
                raw.Data = new byte[dataLen];
                Array.Copy(alldata, offset + 4, raw.Data, 0, dataLen);
                raw.DataLen = dataLen;
                raw.TargetTimeOut = targettimeout;
                queueStatus=ServiceQueueHandle.Send(raw);
            }
            else
            {
                throw new NotSupportedException();
            }
            sqs:
            if(queueStatus!= QueueStatus.OK)
            {
                throw new Exception("queueStatus!= QueueStatus.OK");
            }

        }
        private void ServiceThread()
        {
            EmbeXrpcRawData<DTL> recData;
            ServiceInvokeParameter < DTL > serviceInvokeParameter =new ServiceInvokeParameter<DTL>();
            bool isFindService=false;
            //try
            {
                while (true)
                {
                    if (ServiceQueueHandle.Receive(out recData, 10) == QueueStatus.OK)
                    {
                        isFindService = false;
                        for (int i = 0; i < AllServices.Count; i++)
                        {
                            if (AllServices[i].Service.GetSid() == recData.Sid)
                            {
                                isFindService = true;
                                SerializationManager rsm = new SerializationManager(Assembly, new List<byte>(recData.Data));
                                SerializationManager sendsm = new SerializationManager(Assembly, new List<byte>());

                                //Console.WriteLine($"get client timeout value{recData.TargetTimeOut}");
                                //SuspendTimer.Change(recData.TargetTimeOut / 2, recData.TargetTimeOut / 2);.
                                serviceInvokeParameter.Request_UserDataOfTransportLayer = recData.UserDataOfTransportLayer;
                                serviceInvokeParameter.Response_UserDataOfTransportLayer = recData.UserDataOfTransportLayer;
                                serviceInvokeParameter.RpcObject = this;
                                serviceInvokeParameter.TargetTimeOut = recData.TargetTimeOut;
                                AllServices[i].Service.Invoke(ref serviceInvokeParameter,
                                    rsm,
                                    sendsm);
                                SuspendTimer.Change(Timeout.Infinite, Timeout.Infinite);//手动关闭 怕用户忘了

                                //lock (ObjectMutex)
                                {
                                    if (sendsm.Index > 0)//
                                    {
                                        byte[] sendBytes = new byte[sendsm.Index + 4];
                                        sendBytes[0] = (byte)(recData.Sid >> 0 & 0xff);
                                        sendBytes[1] = (byte)(recData.Sid >> 8 & 0xff);
                                        sendBytes[2] = (byte)(this.TimeOut >> 0 & 0xff);
                                        sendBytes[3] = (byte)((this.TimeOut >> 8 & 0xff) & (0x3F));
                                        sendBytes[3] |= ((byte)(ReceiveType.Response)) << 6;
                                        Array.Copy(sendsm.Buf.ToArray(), 0, sendBytes, 4, sendsm.Index);
                                        Send(serviceInvokeParameter.Response_UserDataOfTransportLayer, sendBytes.Length, 0, sendBytes);
                                    }
                                }
                                break;
                            }
                        }
                        if (isFindService == false)
                        {
                            byte[] sendBytes = new byte[4];
                            sendBytes[0] = (byte)(EmbedXrpcCommon.EmbedXrpcUnsupportedSid >> 0 & 0xff);
                            sendBytes[1] = (byte)(EmbedXrpcCommon.EmbedXrpcUnsupportedSid >> 8 & 0xff);
                            sendBytes[2] = (byte)(this.TimeOut >> 0 & 0xff);
                            sendBytes[3] = (byte)((this.TimeOut >> 8 & 0xff) & (0x3F));
                            sendBytes[3] |= ((byte)(ReceiveType.Response)) << 6;
                            Send(serviceInvokeParameter.Response_UserDataOfTransportLayer, sendBytes.Length, 0, sendBytes);
                        }
                    }
                    if(ServiceThreadCancellationTokenSource.IsCancellationRequested==true)
                    {
                        break;
                    }
                }
            }
            //catch (Exception e)
            {
            //    Console.WriteLine($"{e.Message}");
            }
        }
    }
}
