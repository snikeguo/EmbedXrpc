using System;
using System.Collections.Generic;
using System.Linq;
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
        private Send send;
        public Server(UInt32 timeout, Send send, RequestMessageMap[] maps)
        {
            TimeOut = timeout;
            this.send = send;
            MessageMaps = maps;

            ServiceThreadHandle = new Thread(ServiceThread);
            ServiceThreadHandle.IsBackground = true;
        }
        void ReceivedMessage(UInt32 serviceId, UInt32 dataLen, UInt32 offset, byte[] data)
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
                                    send(recData.Sid, (UInt32)sendsm.Index, sendsm.Data.ToArray());
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
