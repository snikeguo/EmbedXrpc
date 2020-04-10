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

    public class Client
    {
        public UInt32 TimeOut { get; set; }
        public object ObjectMutex { get; private set; } =new object();
        public Win32Queue<EmbeXrpcRawData> DelegateMessageQueueHandle { get; private set; } = new Win32Queue<EmbeXrpcRawData>(10);
        public Win32Queue<EmbeXrpcRawData> ResponseMessageQueueHandle { get; private set; } = new Win32Queue<EmbeXrpcRawData>(10);
        private ResponseDelegateMessageMap[] MessageMaps { get; set; }

        public Send Send;
        private Assembly Assembly;
        public Client(UInt32 timeout, Send send, Assembly assembly)
        {
            var types = assembly.GetTypes();
            Assembly = assembly;
            List<ResponseDelegateMessageMap>  maps=new List<ResponseDelegateMessageMap>();
            foreach (var type in types)
            {
                var ResponseInfoAttrs = type.GetCustomAttributes<ResponseInfoAttribute>();
                if (ResponseInfoAttrs != null)
                {
                    foreach (var resInfoAttr in ResponseInfoAttrs)
                    {
                        ResponseDelegateMessageMap map = new ResponseDelegateMessageMap();
                        map.Name = resInfoAttr.Name;
                        map.ReceiveType = ReceiveType.Response;
                        map.Delegate = null;
                        map.Sid = resInfoAttr.ServiceId;
                        
                        maps.Add(map);
                    }
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
                    maps.Add(map);
                }
            }

            TimeOut = timeout;
            this.Send = send;
            MessageMaps = maps.ToArray();

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
        public ResponseState Wait<T>(UInt32 sid,out T response)
        {
            EmbeXrpcRawData recData;
            response = default(T);
            Type res_t =typeof(T);
            ResponseState ret = ResponseState.Ok;
            while (true)
            {
                if (ResponseMessageQueueHandle.Receive(out recData, TimeOut) != QueueStatus.OK)
                {
                    return ResponseState.Timeout;
                }
                if(recData.Sid== EmbedXrpcCommon.EmbedXrpcSuspendSid)
                {
                    //Console.WriteLine("sid== EmbedXrpcCommon.EmbedXrpcSuspendSid{0}",DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff"));
                    continue;
                }
                if (sid != recData.Sid)
                {
                    ret=ResponseState.SidError;
                }
                break;
            }
            if(ret == ResponseState.Ok)
            {
                SerializationManager rsm = new SerializationManager();
                rsm.Reset();
                rsm.Data = new List<byte>(recData.Data);
                Serialization serialization = new Serialization(Assembly);
                response = (T)serialization.Deserialize(res_t, rsm);
            }
            return ret;
        }
        public void ReceivedMessage(UInt32 serviceId, UInt32 dataLen, UInt32 offset,byte[] data)
        {
            EmbeXrpcRawData raw = new EmbeXrpcRawData();

            if (serviceId == EmbedXrpcCommon.EmbedXrpcSuspendSid)
            {
                raw.Sid = serviceId;
                if(dataLen>0)
                {
                    raw.Data = new byte[dataLen];
                    Array.Copy(data, offset, raw.Data, 0, dataLen);
                }
                
                raw.DataLen = dataLen;
                ResponseMessageQueueHandle.Send(raw);
                return;
            }

            for (int i = 0; i < MessageMaps.Length; i++)
            {
                var iter = MessageMaps[i];
                if (iter.Sid == serviceId)
                {
                    raw.Sid = serviceId;
                    if (dataLen > 0)
                    {
                        raw.Data = new byte[dataLen];
                        Array.Copy(data, offset, raw.Data, 0, dataLen);
                    }
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