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

    public class EmbedXrpcObject   
    {
        public UInt32 TimeOut { get; set; }
        public Mutex ObjectMutex { get; private set; } = new Mutex();
        
        public Win32Queue<EmbeXrpcRawData> MessageQueueOfRequestServiceHandle { get; private set; } = new Win32Queue<EmbeXrpcRawData>();
        private List<ServiceDescribe> AllServices { get; set; } = new List<ServiceDescribe>();

        public Send Send;
        private Assembly Assembly;

        //server
        private Timer SuspendTimer;
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
        private Win32Queue<EmbeXrpcRawData> ServiceQueueHandle = new Win32Queue<EmbeXrpcRawData>();
        
        //public bool IsEnableMataDataEncode { get; set; }
        public int BufferLen { get; private set; }
        public byte[] RequestBuffer { get; private set; }
        public byte[] ResponseBuffer { get; private set; }
        public EmbedXrpcObject(UInt32 timeout, Send send, Assembly assembly,int maxBufferLen)
        {
            //IsEnableMataDataEncode = isEnableMataDataEncode;
            BufferLen = maxBufferLen;
            RequestBuffer= new byte[maxBufferLen];
            ResponseBuffer = new byte[maxBufferLen];
            var types = assembly.GetTypes();
            Assembly = assembly;
            foreach (var oldtype in types)
            {
                var responseServiceInfoAttr = oldtype.GetCustomAttribute<ResponseServiceInfoAttribute>();
                if (responseServiceInfoAttr != null)
                {
                    ServiceDescribe service = new ServiceDescribe();
                    service.Name = responseServiceInfoAttr.Name;
                    service.Service = (IService)assembly.CreateInstance(oldtype.FullName);
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
        public UInt32 GetBufferCrc(byte[]buf,int start,int len)
        {
            CrcCalculate crcer=new CrcCalculate();
            crcer.Init();
            crcer.Compute(buf, start, len);
            crcer.Finish();
            return crcer.CurrentValue;
        }
        private void SuspendTimerCallback(object s)
        {
            EmbedXrpcObject server = s as EmbedXrpcObject;
            byte[] sendBytes = new byte[12];
            sendBytes[0] = (byte)(EmbedXrpcCommon.EmbedXrpcSuspendSid >> 0 & 0xff);
            sendBytes[1] = (byte)(EmbedXrpcCommon.EmbedXrpcSuspendSid >> 8 & 0xff);
            sendBytes[2] = (byte)(server.TimeOut >> 0 & 0xff);
            sendBytes[3] = (byte)((server.TimeOut >> 8 & 0xff) & (0x3F));
            sendBytes[3] |= ((byte)(ReceiveType.Response)) << 6;

            sendBytes[4] = (byte)(0);
            sendBytes[5] = (byte)(0);
            sendBytes[6] = (byte)(0);
            sendBytes[7] = (byte)(0);

            UInt32 bufCrc = GetBufferCrc(null, 8, 0);
            sendBytes[8] = (byte)(bufCrc >> 0 & 0xff);
            sendBytes[9] = (byte)(bufCrc >> 8 & 0xff);
            sendBytes[10] = (byte)(bufCrc >> 16 & 0xff);
            sendBytes[11] = (byte)(bufCrc >> 32 & 0xff);
            Send(sendBytes.Length, 0, sendBytes);
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
        public RequestResponseState Wait<T>(UInt32 sid,T response) where T: IEmbedXrpcSerialization
        {
            EmbeXrpcRawData recData;
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
                SerializationManager sm = new SerializationManager(Assembly, recData.Data,0);
                response.Deserialize(sm);
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
            UInt32 dataLen = validdataLen - 12;

            UInt32 wantedDataLen = (UInt32)(alldata[4 + offset] << 0 | alldata[5 + offset] << 8 | alldata[6 + offset] << 16 | alldata[7 + offset] << 24);
            UInt32 wantedBufCrc = (UInt32)(alldata[8 + offset] << 0 | alldata[9 + offset] << 8 | alldata[10 + offset] << 16 | alldata[11 + offset] << 24);

            if(wantedDataLen!=dataLen)
            {
                goto sqs;
            }
            UInt32 calcrc = GetBufferCrc(alldata, (int)(12+offset), (int)dataLen);
            if(wantedBufCrc!=calcrc)
            {
                goto sqs;
            }

            EmbeXrpcRawData raw = new EmbeXrpcRawData();
            ReceiveType rt = (ReceiveType)(alldata[3 + offset] >> 6);
            if (rt== ReceiveType.Response)
            {
                raw.Sid = serviceId;
                if (dataLen > 0)
                {
                    raw.Data = new byte[dataLen];
                    Array.Copy(alldata, offset + 12, raw.Data, 0, dataLen);
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
                Array.Copy(alldata, offset + 12, raw.Data, 0, dataLen);
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
            EmbeXrpcRawData recData;
            ServiceInvokeParameter serviceInvokeParameter =new ServiceInvokeParameter();
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
                                SerializationManager rsm = new SerializationManager(Assembly, recData.Data,0);
                                SerializationManager sendsm = new SerializationManager(Assembly, ResponseBuffer,12);

                                //Console.WriteLine($"get client timeout value{recData.TargetTimeOut}");
                                //SuspendTimer.Change(recData.TargetTimeOut / 2, recData.TargetTimeOut / 2);.
                                serviceInvokeParameter.RpcObject = this;
                                serviceInvokeParameter.TargetTimeOut = recData.TargetTimeOut;
                                var exec_task= AllServices[i].Service.Invoke(serviceInvokeParameter,
                                    rsm,
                                    sendsm);
                                exec_task.Wait();
                                SuspendTimer.Change(Timeout.Infinite, Timeout.Infinite);//手动关闭 怕用户忘了

                                //lock (ObjectMutex)
                                {
                                    int responseObject_dataLen = sendsm.Index - 12;
                                    if (responseObject_dataLen > 0)//
                                    {
                                        ResponseBuffer[0] = (byte)(recData.Sid >> 0 & 0xff);
                                        ResponseBuffer[1] = (byte)(recData.Sid >> 8 & 0xff);
                                        ResponseBuffer[2] = (byte)(TimeOut >> 0 & 0xff);
                                        ResponseBuffer[3] = (byte)((TimeOut >> 8 & 0xff) & (0x3F));
                                        ResponseBuffer[3] |= ((byte)(ReceiveType.Response)) << 6;

                                        ResponseBuffer[4]= (byte)(responseObject_dataLen >> 0 & 0xff);
                                        ResponseBuffer[5] = (byte)(responseObject_dataLen >> 8 & 0xff);
                                        ResponseBuffer[6] = (byte)(responseObject_dataLen >> 16 & 0xff);
                                        ResponseBuffer[7] = (byte)(responseObject_dataLen >> 24 & 0xff);

                                        UInt32 bufcrc = GetBufferCrc(sendsm.Buf, 12, responseObject_dataLen);

                                        ResponseBuffer[8] = (byte)(bufcrc >> 0 & 0xff);
                                        ResponseBuffer[9] = (byte)(bufcrc >> 8 & 0xff);
                                        ResponseBuffer[10] = (byte)(bufcrc >> 16 & 0xff);
                                        ResponseBuffer[11] = (byte)(bufcrc >> 24 & 0xff);

                                        Send(sendsm.Index, 0, ResponseBuffer);
                                    }
                                }
                                break;
                            }
                        }
                        if (isFindService == false)
                        {
                            byte[] sendBytes = new byte[12];
                            sendBytes[0] = (byte)(EmbedXrpcCommon.EmbedXrpcUnsupportedSid >> 0 & 0xff);
                            sendBytes[1] = (byte)(EmbedXrpcCommon.EmbedXrpcUnsupportedSid >> 8 & 0xff);
                            sendBytes[2] = (byte)(this.TimeOut >> 0 & 0xff);
                            sendBytes[3] = (byte)((this.TimeOut >> 8 & 0xff) & (0x3F));
                            sendBytes[3] |= ((byte)(ReceiveType.Response)) << 6;

                            sendBytes[4] = (byte)(0 >> 0 & 0xff);
                            sendBytes[5] = (byte)(0 >> 8 & 0xff);
                            sendBytes[6] = (byte)(0 >> 16 & 0xff);
                            sendBytes[7] = (byte)(0 >> 24 & 0xff);

                            UInt32 bufcrc = GetBufferCrc(new byte[0], 0, 0);

                            sendBytes[8] = (byte)(bufcrc >> 0 & 0xff);
                            sendBytes[9] = (byte)(bufcrc >> 8 & 0xff);
                            sendBytes[10] = (byte)(bufcrc >> 16 & 0xff);
                            sendBytes[11] = (byte)(bufcrc >> 24 & 0xff);


                            Send(sendBytes.Length, 0, sendBytes);
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
