using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using David.Common;
namespace EmbedXrpc
{

    public class EmbedXrpcObject
    {
        public UInt32 TimeOut { get; set; }
        public object ObjectMutex { get; private set; } = new object();
        public Win32Queue<EmbeXrpcRawData> DelegateMessageQueueHandle { get; private set; } = new Win32Queue<EmbeXrpcRawData>();
        public Win32Queue<EmbeXrpcRawData> ResponseMessageQueueHandle { get; private set; } = new Win32Queue<EmbeXrpcRawData>();
        private ResponseDelegateMessageMap[] ResponseDelegateMessageMaps { get; set; }

        public Send Send;
        private Assembly Assembly;

        //server
        private Timer SuspendTimer;
        private Thread RequestServiceThreadHandle;
        private Thread DelegateServiceThreadHandle;
        private Win32Queue<EmbeXrpcRawData> RequestQueueHandle = new Win32Queue<EmbeXrpcRawData>();
        private RequestMessageMap[] RequestMessageMaps;
        public bool IsEnableMataDataEncode { get; set; }
        public EmbedXrpcObject(UInt32 timeout, Send send, Assembly assembly,bool isEnableMataDataEncode)
        {
            IsEnableMataDataEncode = isEnableMataDataEncode;
            var types = assembly.GetTypes();
            Assembly = assembly;
            List<ResponseDelegateMessageMap> rdMaps = new List<ResponseDelegateMessageMap>();
            List<RequestMessageMap> rMaps = new List<RequestMessageMap>();
            foreach (var type in types)
            {
                var requestInfoAttr = type.GetCustomAttribute<RequestServiceInfoAttribute>();
                if (requestInfoAttr != null)
                {
                    ResponseDelegateMessageMap map = new ResponseDelegateMessageMap();
                    map.Name = requestInfoAttr.Name;
                    map.ReceiveType = ReceiveType.Response;
                    map.Delegate = null;
                    map.Sid = requestInfoAttr.ServiceId;
                    rdMaps.Add(map);
                }

                var DelAttr = type.GetCustomAttribute<DelegateInfoAttribute>();
                if(DelAttr!=null)
                {
                    ResponseDelegateMessageMap map = new ResponseDelegateMessageMap();
                    map.Name = DelAttr.Name;
                    map.ReceiveType = ReceiveType.Delegate;
                    var v = assembly.CreateInstance(type.FullName);
                    map.Delegate = (IDelegate)assembly.CreateInstance(type.FullName);
                    map.Sid = map.Delegate.GetSid();
                    rdMaps.Add(map);
                }

                var responseServiceInfoAttr = type.GetCustomAttribute<ResponseServiceInfoAttribute>();
                if (responseServiceInfoAttr != null)
                {
                    RequestMessageMap map = new RequestMessageMap();
                    map.Name = responseServiceInfoAttr.Name;
                    map.Service = (IService)assembly.CreateInstance(type.FullName);
                    rMaps.Add(map);
                }
            }

            TimeOut = timeout;
            Send = send;

            ResponseDelegateMessageMaps = rdMaps.ToArray();
            DelegateServiceThreadHandle = new Thread(DelegateServiceThread);
            DelegateServiceThreadHandle.IsBackground = true;


            RequestMessageMaps = rMaps.ToArray();
            RequestServiceThreadHandle = new Thread(RequestServiceThread);
            RequestServiceThreadHandle.IsBackground = true;

            SuspendTimer = new Timer(SuspendTimerCallback, this, Timeout.Infinite, Timeout.Infinite);
        }
        private void SuspendTimerCallback(object s)
        {
            EmbedXrpcObject server = s as EmbedXrpcObject;
            byte[] sendBytes = new byte[4];
            sendBytes[0] = (byte)(EmbedXrpcCommon.EmbedXrpcSuspendSid >> 0 & 0xff);
            sendBytes[1] = (byte)(EmbedXrpcCommon.EmbedXrpcSuspendSid >> 8 & 0xff);
            sendBytes[2] = (byte)(server.TimeOut >> 0 & 0xff);
            sendBytes[3] = (byte)((server.TimeOut >> 8 & 0xff) & (0x3F));
            sendBytes[3] |= ((byte)(ReceiveType.Response)) << 6;
            Send(sendBytes.Length, 0, sendBytes);
        }

        
        public void Start()
        {
            DelegateServiceThreadHandle.Start();
            RequestServiceThreadHandle.Start();
        }
        public void Stop()
        {
            DelegateServiceThreadHandle.Abort();
            RequestServiceThreadHandle.Abort();
        }
        public RequestResponseState Wait<T>(UInt32 sid,out T response)
        {
            EmbeXrpcRawData recData;
            response = default(T);
            Type res_t =typeof(T);
            RequestResponseState ret = RequestResponseState.ResponseState_Ok;
            while (true)
            {
                if (ResponseMessageQueueHandle.Receive(out recData, TimeOut) != QueueStatus.OK)
                {
                    return RequestResponseState.ResponseState_Timeout;
                }
                if(recData.Sid== EmbedXrpcCommon.EmbedXrpcSuspendSid)
                {
                    Console.WriteLine("{0}:sid== EmbedXrpcCommon.EmbedXrpcSuspendSid. this.timeout is:{1}", DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"),TimeOut);
                    continue;
                }
                if (sid != recData.Sid)
                {
                    ret= RequestResponseState.ResponseState_SidError;
                }
                break;
            }
            if(ret == RequestResponseState.ResponseState_Ok)
            {
                SerializationManager sm = new SerializationManager(Assembly, IsEnableMataDataEncode, new List<byte>(recData.Data));
                response = (T)sm.Deserialize(res_t);
            }
            return ret;
        }
        public void ReceivedMessage(UInt32 validdataLen, UInt32 offset,byte[] alldata)
        {
            QueueStatus queueStatus = QueueStatus.Empty;
            if (alldata.Length < offset + validdataLen)
            {
                goto sqs;
            }
            UInt16 serviceId = (UInt16)(alldata[0 + offset] << 0 | alldata[1 + offset] << 8);
            UInt16 targettimeout = (UInt16)((alldata[2 + offset] << 0) | ((alldata[3 + offset])&0x3F)<<8);
            UInt32 dataLen = validdataLen - 4;
            EmbeXrpcRawData raw = new EmbeXrpcRawData();
            ReceiveType rt = (ReceiveType)(alldata[3 + offset] >> 6);
            if (rt== ReceiveType.Delegate|| rt== ReceiveType.Response)
            {
                if (serviceId == EmbedXrpcCommon.EmbedXrpcSuspendSid)
                {
                    raw.Sid = serviceId;
                    if (dataLen > 0)
                    {
                        raw.Data = new byte[dataLen];
                        Array.Copy(alldata, offset + 4, raw.Data, 0, dataLen);
                    }

                    raw.DataLen = dataLen;
                    raw.TargetTimeOut = targettimeout;
                    queueStatus =ResponseMessageQueueHandle.Send(raw);
                    goto sqs;
                }

                for (int i = 0; i < ResponseDelegateMessageMaps.Length; i++)
                {
                    var iter = ResponseDelegateMessageMaps[i];
                    if (iter.Sid == serviceId)
                    {
                        raw.Sid = serviceId;
                        if (dataLen > 0)
                        {
                            raw.Data = new byte[dataLen];
                            Array.Copy(alldata, offset + 4, raw.Data, 0, dataLen);
                        }
                        raw.DataLen = dataLen;
                        raw.TargetTimeOut = targettimeout;
                        if (iter.ReceiveType == ReceiveType.Response)
                        {
                            queueStatus=ResponseMessageQueueHandle.Send(raw);
                        }
                        else if (iter.ReceiveType == ReceiveType.Delegate)
                        {
                            queueStatus=DelegateMessageQueueHandle.Send(raw);
                        }
                        goto sqs;
                    }
                }
            }
            else if(rt == ReceiveType.Request)
            {
                raw.Sid = serviceId;
                raw.Data = new byte[dataLen];
                Array.Copy(alldata, offset + 4, raw.Data, 0, dataLen);
                raw.DataLen = dataLen;
                raw.TargetTimeOut = targettimeout;
                queueStatus=RequestQueueHandle.Send(raw);
            }
            sqs:
            if(queueStatus!= QueueStatus.OK)
            {
                throw new Exception("queueStatus!= QueueStatus.OK");
            }

        }
        private void DelegateServiceThread()
        {
            EmbeXrpcRawData recData;
            //try
            {
                while (true)
                {
                    if(DelegateMessageQueueHandle.Receive(out recData, Win32BinarySignal.MAX_Delay)!= QueueStatus.OK)
                    {
                        continue;
                    }
                    for (int i = 0; i < ResponseDelegateMessageMaps.Length; i++)
                    {
                        if ((ResponseDelegateMessageMaps[i].ReceiveType ==  ReceiveType.Delegate) && (ResponseDelegateMessageMaps[i].Delegate.GetSid() == recData.Sid))
                        {
                            SerializationManager rsm = new SerializationManager(Assembly, IsEnableMataDataEncode, new List<byte>(recData.Data));
                            //Console.WriteLine($"get server timeout value{recData.TargetTimeOut}");
                            ResponseDelegateMessageMaps[i].Delegate.Invoke(rsm);
                        }
                    }
                }
            }
            //catch (Exception e)
            {
            //    Console.WriteLine($"{e.Message}");
            }
        }
        private void RequestServiceThread()
        {
            EmbeXrpcRawData recData;
            UInt32 i = 0;
            //try
            {
                while (true)
                {
                    if (RequestQueueHandle.Receive(out recData, Win32BinarySignal.MAX_Delay) != QueueStatus.OK)
                    {
                        continue;
                    }
                    for (i = 0; i < RequestMessageMaps.Length; i++)
                    {
                        if (RequestMessageMaps[i].Service.GetSid() == recData.Sid)
                        {
                            SerializationManager rsm = new SerializationManager(Assembly,IsEnableMataDataEncode, new List<byte>(recData.Data));
                            SerializationManager sendsm = new SerializationManager(Assembly,IsEnableMataDataEncode,new List<byte>());

                            //Console.WriteLine($"get client timeout value{recData.TargetTimeOut}");
                            SuspendTimer.Change(recData.TargetTimeOut / 2, recData.TargetTimeOut / 2);
                            RequestMessageMaps[i].Service.Invoke(rsm, sendsm);
                            SuspendTimer.Change(Timeout.Infinite, Timeout.Infinite);

                            //lock (ObjectMutex)
                            {
                                if (sendsm.Index > 0)//
                                {
                                    byte[] sendBytes = new byte[sendsm.Index + 4];
                                    sendBytes[0] = (byte)(recData.Sid >> 0 & 0xff);
                                    sendBytes[1] = (byte)(recData.Sid >> 8 & 0xff);
                                    sendBytes[2] = (byte)(this.TimeOut >> 0 & 0xff);
                                    sendBytes[3] = (byte)((this.TimeOut >> 8 & 0xff)&(0x3F));
                                    sendBytes[3] |= ((byte)(ReceiveType.Response)) << 6;
                                    Array.Copy(sendsm.Data.ToArray(), 0, sendBytes, 4, sendsm.Index);
                                    Send(sendBytes.Length, 0, sendBytes);
                                }
                            }
                        }
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
