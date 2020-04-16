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
    public class Server
    {
        public UInt32 TimeOut { get; set; }
        public object SendMutex { get; private set; } = new object();
        private Thread ServiceThreadHandle;
        private Win32Queue<EmbeXrpcRawData> RequestQueueHandle=new Win32Queue<EmbeXrpcRawData>(10);
        private RequestMessageMap[] MessageMaps;
        public Send Send;
        private Assembly Assembly;
        private Timer SuspendTimer;
        public Server(UInt32 timeout, Send send, Assembly assembly)
        {
            var types = assembly.GetTypes();
            Assembly = assembly;
            List<RequestMessageMap> maps = new List<RequestMessageMap>();
            foreach (var type in types)
            {             
                var serviceInfoAttr = type.GetCustomAttribute<ServiceInfoAttribute>();
                if (serviceInfoAttr != null)
                {
                    RequestMessageMap map = new RequestMessageMap();
                    map.Name = serviceInfoAttr.Name;
                    map.Service = (IService)assembly.CreateInstance(type.FullName);
                    maps.Add(map);
                }
            }

            TimeOut = timeout;
            Send = send;
            MessageMaps = maps.ToArray();

            ServiceThreadHandle = new Thread(ServiceThread);
            ServiceThreadHandle.IsBackground = true;

            SuspendTimer = new Timer(SuspendTimerCallback, this, Timeout.Infinite, Timeout.Infinite);
        }
        private void SuspendTimerCallback(object s)
        {
            Server server = s as Server;
            lock (SendMutex)
            {
                byte[] sendBytes = new byte[4];
                sendBytes[0] = (byte)(EmbedXrpcCommon.EmbedXrpcSuspendSid >> 0 & 0xff);
                sendBytes[1] = (byte)(EmbedXrpcCommon.EmbedXrpcSuspendSid >> 8 & 0xff);
                sendBytes[2] = (byte)(server.TimeOut >> 0 & 0xff);
                sendBytes[3] = (byte)(server.TimeOut >> 8 & 0xff);
                Send(sendBytes.Length, 0, sendBytes);
            }
        }
        public void ReceivedMessage(UInt32 validdataLen, UInt32 offset, byte[] alldata)
        {
            if (alldata.Length < offset + validdataLen)
            {
                return;
            }
            UInt16 serviceId = (UInt16)(alldata[0 + offset] << 0 | alldata[1 + offset] << 8);
            UInt16 targettimeout = (UInt16)(alldata[2 + offset] << 0 | alldata[3 + offset] << 8);
            UInt32 dataLen = validdataLen - 4;
            EmbeXrpcRawData raw=new EmbeXrpcRawData();
            raw.Sid = serviceId;
            raw.Data = new byte[dataLen];
            Array.Copy(alldata, offset+4, raw.Data, 0, dataLen);
            raw.DataLen = dataLen;
            raw.TargetTimeOut = targettimeout;
            RequestQueueHandle.Send(raw);

        }
        public void Start()
        {
            ServiceThreadHandle.Start();
        }
        public void Stop()
        {
            ServiceThreadHandle.Abort();
        }
        private void ServiceThread()
        {
            EmbeXrpcRawData recData;
            UInt32 i = 0;
            try
            {
                while(true)
                {
                    if (RequestQueueHandle.Receive(out recData, Win32BinarySignal.MAX_Delay) != QueueStatus.OK)
                    {
                        continue;
                    }
                    for (i = 0; i < MessageMaps.Length; i++)
                    {
                        if (MessageMaps[i].Service.GetSid() == recData.Sid)
                        {
                            SerializationManager rsm = new SerializationManager();
                            SerializationManager sendsm = new SerializationManager();
                            rsm.Reset();
                            rsm.Data = new List<byte>(recData.Data);

                            sendsm.Reset();
                            sendsm.Data = new List<byte>();

                            //Console.WriteLine($"get client timeout value{recData.TargetTimeOut}");
                            SuspendTimer.Change(recData.TargetTimeOut / 2, recData.TargetTimeOut / 2);
                            MessageMaps[i].Service.Invoke(rsm, sendsm);
                            SuspendTimer.Change(Timeout.Infinite, Timeout.Infinite);

                            lock (SendMutex)
                            {
                                if (sendsm.Index > 0)//
                                {
                                    byte[] sendBytes = new byte[sendsm.Index + 4];
                                    sendBytes[0] = (byte)(recData.Sid>>0 & 0xff);
                                    sendBytes[1] = (byte)(recData.Sid >> 8 & 0xff);
                                    sendBytes[2] = (byte)(this.TimeOut>>0 & 0xff);
                                    sendBytes[3] = (byte)(this.TimeOut >> 8 & 0xff);
                                    Array.Copy(sendsm.Data.ToArray(), 0, sendBytes, 4, sendsm.Index);
                                    Send(sendBytes.Length,0, sendBytes);
                                }
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                Console.WriteLine($"{e.Message}");
            }
        }
    }
}
