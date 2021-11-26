﻿using System;
using System.Collections.Generic;
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
        public Win32Queue<EmbeXrpcRawData<DTL>> DelegateMessageQueueHandle { get; private set; } = new Win32Queue<EmbeXrpcRawData<DTL>>();
        public Win32Queue<EmbeXrpcRawData<DTL>> ResponseMessageQueueHandle { get; private set; } = new Win32Queue<EmbeXrpcRawData<DTL>>();
        private List<DelegateDescribe<DTL>> Delegates { get; set; } = new List<DelegateDescribe<DTL>>();
        private List<ResponseDescribe> Responses { get; set; } = new List<ResponseDescribe>();
        private List<RequestDescribe<DTL>> Requests { get; set; } = new List<RequestDescribe<DTL>>();

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

        private Thread RequestServiceThreadHandle;
        private Thread DelegateServiceThreadHandle;
        private Win32Queue<EmbeXrpcRawData<DTL>> RequestQueueHandle = new Win32Queue<EmbeXrpcRawData<DTL>>();
        
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
                    ResponseDescribe response = new ResponseDescribe();
                    response.Name = requestInfoAttr.Name;
                    response.Sid = requestInfoAttr.ServiceId;
                    Responses.Add(response);
                }

                var DelAttr = oldtype.GetCustomAttribute<DelegateInfoAttribute>();
                if(DelAttr!=null)
                {
                    var dtlType = typeof(DTL);
                    var genericType = oldtype.MakeGenericType(dtlType);
                    DelegateDescribe<DTL> delegateDescribe = new DelegateDescribe<DTL>();
                    delegateDescribe.Name = DelAttr.Name;
                    var v = assembly.CreateInstance(genericType.FullName);
                    delegateDescribe.Delegate = (IDelegate<DTL>)assembly.CreateInstance(genericType.FullName);
                    Delegates.Add(delegateDescribe);
                }

                var responseServiceInfoAttr = oldtype.GetCustomAttribute<ResponseServiceInfoAttribute>();
                if (responseServiceInfoAttr != null)
                {
                    var dtlType = typeof(DTL);
                    var genericType = oldtype.MakeGenericType(dtlType);
                    RequestDescribe<DTL> request = new RequestDescribe<DTL>();
                    request.Name = responseServiceInfoAttr.Name;
                    request.Service = (IService<DTL>)assembly.CreateInstance(genericType.FullName);
                    Requests.Add(request);
                }
            }

            TimeOut = timeout;
            Send = send;

            DelegateServiceThreadHandle = new Thread(DelegateServiceThread);
            DelegateServiceThreadHandle.IsBackground = true;


            RequestServiceThreadHandle = new Thread(RequestServiceThread);
            RequestServiceThreadHandle.IsBackground = true;

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
            EmbeXrpcRawData<DTL> recData;
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
                SerializationManager sm = new SerializationManager(Assembly, new List<byte>(recData.Data));
                response = (T)sm.Deserialize(res_t);
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

                raw.Sid = serviceId;
                if (dataLen > 0)
                {
                    raw.Data = new byte[dataLen];
                    Array.Copy(alldata, offset + 4, raw.Data, 0, dataLen);
                }
                raw.DataLen = dataLen;
                raw.TargetTimeOut = targettimeout;
                queueStatus = ResponseMessageQueueHandle.Send(raw);
                goto sqs;
            }
            else if (rt == ReceiveType.Delegate)
            {
                raw.Sid = serviceId;
                if (dataLen > 0)
                {
                    raw.Data = new byte[dataLen];
                    Array.Copy(alldata, offset + 4, raw.Data, 0, dataLen);
                }
                raw.DataLen = dataLen;
                raw.TargetTimeOut = targettimeout;
                queueStatus = DelegateMessageQueueHandle.Send(raw);
                goto sqs;
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
        private void DelegateServiceThread()
        {
            EmbeXrpcRawData<DTL> recData;
            //try
            {
                while (true)
                {
                    if(DelegateMessageQueueHandle.Receive(out recData, Win32BinarySignal.MAX_Delay)!= QueueStatus.OK)
                    {
                        continue;
                    }
                    for (int i = 0; i < Delegates.Count; i++)
                    {
                        if (Delegates[i].Delegate.GetSid() == recData.Sid)
                        {
                            SerializationManager rsm = new SerializationManager(Assembly,  new List<byte>(recData.Data));
                            //Console.WriteLine($"get server timeout value{recData.TargetTimeOut}");
                            Delegates[i].Delegate.Invoke(recData.UserDataOfTransportLayer,rsm);
                            break;
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
            EmbeXrpcRawData<DTL> recData;
            ServiceInvokeParameter < DTL > serviceInvokeParameter =new ServiceInvokeParameter<DTL>();
            //try
            {
                while (true)
                {
                    if (RequestQueueHandle.Receive(out recData, Win32BinarySignal.MAX_Delay) != QueueStatus.OK)
                    {
                        continue;
                    }
                    for (int i = 0; i < Requests.Count; i++)
                    {
                        if (Requests[i].Service.GetSid() == recData.Sid)
                        {
                            SerializationManager rsm = new SerializationManager(Assembly, new List<byte>(recData.Data));
                            SerializationManager sendsm = new SerializationManager(Assembly,new List<byte>());

                            //Console.WriteLine($"get client timeout value{recData.TargetTimeOut}");
                            //SuspendTimer.Change(recData.TargetTimeOut / 2, recData.TargetTimeOut / 2);.
                            serviceInvokeParameter.request_UserDataOfTransportLayer = recData.UserDataOfTransportLayer;
                            serviceInvokeParameter.response_UserDataOfTransportLayer = recData.UserDataOfTransportLayer;
                            serviceInvokeParameter.rpcObject = this;
                            serviceInvokeParameter.targetTimeOut = recData.TargetTimeOut;
                            Requests[i].Service.Invoke(ref serviceInvokeParameter,
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
                                    sendBytes[3] = (byte)((this.TimeOut >> 8 & 0xff)&(0x3F));
                                    sendBytes[3] |= ((byte)(ReceiveType.Response)) << 6;
                                    Array.Copy(sendsm.Data.ToArray(), 0, sendBytes, 4, sendsm.Index);
                                    Send(serviceInvokeParameter.response_UserDataOfTransportLayer, sendBytes.Length, 0, sendBytes);
                                }
                            }
                            break;
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