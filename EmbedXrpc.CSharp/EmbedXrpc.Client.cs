using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using David.Common;
namespace EmbedXrpc
{

    public class Client
    {
        public UInt32 TimeOut { get; set; }
        private object BufMutex = new object();
        private Win32Queue<EmbeXrpcRawData> DelegateMessageQueueHandle = new Win32Queue<EmbeXrpcRawData>(10);
        private Win32Queue<EmbeXrpcRawData> ResponseMessageQueueHandle = new Win32Queue<EmbeXrpcRawData>(10);
        private ResponseDelegateMessageMap[] MessageMaps { get; set; }

        private Send send;
        public Client(UInt32 timeout,Send send, ResponseDelegateMessageMap[] maps)
        {
            TimeOut = timeout;
            this.send = send;
            MessageMaps = maps;

            ServiceThreadHandle = new Thread(ServiceThread);
            ServiceThreadHandle.IsBackground = true;

        }
        private Thread ServiceThreadHandle;
        public void Start()
        {
            ServiceThreadHandle.Start();
        }
        public void Stop()
        {
            ServiceThreadHandle.Abort();
        }
        ResponseState Wait<T>(UInt32 sid)
        {
            EmbeXrpcRawData recData;
            T response = default(T);
            Type res_t =typeof(T);
            if (ResponseMessageQueueHandle.Receive(out recData, TimeOut)!= QueueStatus.OK)
            {
                return ResponseState.Timeout;
            }
            if (sid != recData.Sid)
            {
                return ResponseState.SidError;
            }

            SerializationManager rsm=new SerializationManager();
            rsm.Reset();
            rsm.Data = new List<byte>(recData.Data);
            response = (T)Serialization.Deserialize(res_t, rsm);
            return ResponseState.Ok;
        }
        public void ReceivedMessage(UInt32 serviceId, UInt32 dataLen, UInt32 offset,byte[] data)
        {
            EmbeXrpcRawData raw = new EmbeXrpcRawData();

            for (int i = 0; i < MessageMaps.Length; i++)
            {
                var iter = MessageMaps[i];
                if (iter.Sid == serviceId)
                {
                    raw.Sid = serviceId;
                    raw.Data = new byte[dataLen];
                    Array.Copy(data, offset, raw.Data, 0, dataLen);
                    raw.DataLen = dataLen;
                    if (iter.ReceiveType == ReceiveType.Response)
                    {
                        ResponseMessageQueueHandle.Send(raw);
                    }
                    else if (iter.ReceiveType == ReceiveType.Delegate)
                    {
                        DelegateMessageQueueHandle.Send(raw);
                    }
                }
            }
        }
        private void ServiceThread()
        {
            EmbeXrpcRawData recData;
            try
            {
                while (true)
                {
                    if(DelegateMessageQueueHandle.Receive(out recData, Win32BinarySignal.MAX_Delay)!= QueueStatus.OK)
                    {
                        continue;
                    }
                    for (int i = 0; i < MessageMaps.Length; i++)
                    {
                        if ((MessageMaps[i].ReceiveType ==  ReceiveType.Delegate) && (MessageMaps[i].Delegate.GetSid() == recData.Sid))
                        {
                            SerializationManager rsm=new SerializationManager();
                            rsm.Reset();
                            rsm.Data = new List<byte>(recData.Data);
                            MessageMaps[i].Delegate.Invoke(rsm);
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
