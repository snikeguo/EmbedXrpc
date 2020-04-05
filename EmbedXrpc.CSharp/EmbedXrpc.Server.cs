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
        UInt32 TimeOut;
        private object BufMutex = new object();
        private Thread ServiceThreadHandle;
        private Win32Queue<EmbeXrpcRawData> RequestQueueHandle=new Win32Queue<EmbeXrpcRawData>(10);
        private RequestMessageMap[] MessageMaps;
        public Send Send;
        public Server(UInt32 timeout, Send send, Assembly assembly)
        {
            var types = assembly.GetTypes();

            List<RequestMessageMap> maps = new List<RequestMessageMap>();
            foreach (var type in types)
            {             
                var serviceInfoAttr = type.GetCustomAttribute<ServiceInfoAttribute>();
                if (serviceInfoAttr != null)
                {
                    RequestMessageMap map = new RequestMessageMap();
                    map.Name = serviceInfoAttr.Name;
                    map.Service = (IService)Assembly.GetExecutingAssembly().CreateInstance(type.FullName);
                    maps.Add(map);
                }
            }

            TimeOut = timeout;
            Send = send;
            MessageMaps = maps.ToArray();

            ServiceThreadHandle = new Thread(ServiceThread);
            ServiceThreadHandle.IsBackground = true;
        }
        public Server(UInt32 timeout, Send send, RequestMessageMap[] maps)
        {
            TimeOut = timeout;
            Send = send;
            MessageMaps = maps;

            ServiceThreadHandle = new Thread(ServiceThread);
            ServiceThreadHandle.IsBackground = true;
        }
        public void ReceivedMessage(UInt32 serviceId, UInt32 dataLen, UInt32 offset, byte[] data)
        {
            EmbeXrpcRawData raw=new EmbeXrpcRawData();
            raw.Sid = serviceId;
            raw.Data = new byte[dataLen];
            Array.Copy(data, offset, raw.Data, 0, dataLen);
            raw.DataLen = dataLen;
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
                            lock (BufMutex)
                            {
                                sendsm.Reset();
                                sendsm.Data = new List<byte>();
                                MessageMaps[i].Service.Invoke(rsm, sendsm);
                                if (sendsm.Index > 0)//
                                    Send(recData.Sid, sendsm.Index, sendsm.Data.ToArray());
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
