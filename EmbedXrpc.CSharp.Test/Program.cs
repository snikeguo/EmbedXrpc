﻿using EmbedXrpc;
using Sample1;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace Sample1
{
    public partial class DateTimeChangeClientImpl<DTL> : IDelegate<DTL>
    {
        public void DateTimeChange(DTL request_UserDataOfTransportLayer,  DateTime_t[] now)
        {
            var t = now[0];
            Console.WriteLine($"{t.Year}-{t.Month}-{t.Day}  {t.Hour}:{t.Min}:{t.Sec}");
        }
    }
    public partial class TestDelegateClientImpl<DTL> : IDelegate<DTL>
    {
        public void TestDelegate(DTL request_UserDataOfTransportLayer, DateTime_t[] now)
        {

        }
    }
    public partial class Inter_AddService<DTL> : IService<DTL>
    {
        public void Add(DTL request_UserDataOfTransportLayer, ref DTL response_UserDataOfTransportLayer, EmbedXrpcObject<DTL> rpcObject, UInt16 targetTimeOut, Int32 a, Int32 b, Int32 dataLen, Byte[] data)
        {
            rpcObject.Start_SuspendTimer(targetTimeOut / 2);
            Response.ReturnValue.Sum = a + b;
            Response.ReturnValue.dataLen = 0;
            Thread.Sleep(5000);
            rpcObject.Stop_SuspendTimer();
        }
    }
    public partial class Inter_NoArgService<DTL> : IService<DTL>
    {
        public void NoArg(DTL request_UserDataOfTransportLayer, ref DTL response_UserDataOfTransportLayer, EmbedXrpcObject<DTL> rpcObject, UInt16 targetTimeOut)
        {

        }
    }
    public partial class Inter_NoReturnService<DTL> : IService<DTL>
    {
        public void NoReturn(DTL request_UserDataOfTransportLayer, ref DTL response_UserDataOfTransportLayer, EmbedXrpcObject<DTL> rpcObject, UInt16 targetTimeOut, int a)
        {

        }
    }
    public partial class Inter_NoArgAndReturnService<DTL> : IService<DTL>
    {
        public void NoArgAndReturn(DTL request_UserDataOfTransportLayer, ref DTL response_UserDataOfTransportLayer, EmbedXrpcObject<DTL> rpcObject, UInt16 targetTimeOut)
        {

        }
    }
}
namespace EmbedXrpc
{

    struct Win32UserDataOfTransportLayer
    {
        public string Ip { get; set; }
        public int Port { get; set; }
    }
    class Program
    {
        static void Main(string[] args)
        {
            client = new EmbedXrpcObject<Win32UserDataOfTransportLayer>(100, clientSend, Assembly.GetExecutingAssembly());
            client.Start();
            server = new EmbedXrpcObject<Win32UserDataOfTransportLayer>(2000, serverSend, Assembly.GetExecutingAssembly());
            server.Start();
            Task.Run(() =>
            {
                Inter_Add<Win32UserDataOfTransportLayer> inter_Add = new Inter_Add<Win32UserDataOfTransportLayer>(client);
                while (true)
                {
                    Thread.Sleep(1000);
                    var reAdd = inter_Add.Invoke(new Win32UserDataOfTransportLayer() {  Ip="123",Port=11},1,2,0,null);
                    Debug.Assert(reAdd.State == RequestResponseState.ResponseState_Ok);
                    Console.WriteLine($"{1}+{2}={reAdd.ReturnValue.Sum}");
                    /*var reNoArg = inter.NoArg();
                    if (reNoArg.State == RequestResponseState.RequestState_Failed)
                    {
                        throw new Exception("send failed!");
                    }
                    var reNoReturn = inter.NoReturn(3);
                    if (reNoReturn.State == RequestResponseState.RequestState_Failed)
                    {
                        throw new Exception("send failed!");
                    }
                    var reNoArgAndReturn = inter.NoArgAndReturn();
                    if (reNoArgAndReturn.State == RequestResponseState.RequestState_Failed)
                    {
                        throw new Exception("send failed!");
                    }*/
                    
                }
            });

            Task.Run(() =>
            {
                DateTimeChangeDelegate<Win32UserDataOfTransportLayer> broadcastDataTimeDelegate = new DateTimeChangeDelegate<Win32UserDataOfTransportLayer>(server);
                while (true)
                {
                    Thread.Sleep(1000);
                    broadcastDataTimeDelegate.Invoke(new Win32UserDataOfTransportLayer() { Ip = "123", Port = 11 }, new DateTime_t[1] {
                    new DateTime_t()
                    {
                        Year = DateTime.Now.Year,
                        Month = DateTime.Now.Month,
                        Day = DateTime.Now.Day,
                        Hour = DateTime.Now.Hour,
                        Min = DateTime.Now.Minute,
                        Sec = DateTime.Now.Second,
                        David={ AddressType=Student.u2_FieldNumber,u2=0x778899AA , uend1=1,uend2=0x88},
                    }}); ;
                    
                }
            });

            while (true)
            {

                Thread.Sleep(1000);
            }
        }

        static EmbedXrpcObject<Win32UserDataOfTransportLayer> client;
        static EmbedXrpcObject<Win32UserDataOfTransportLayer> server;
        public static bool clientSend(Win32UserDataOfTransportLayer win32UserDataOfTransportLayer,int dataLen, int offset, byte[] data)
        {
            //Console.WriteLine($"clientSend {sid}");
            Win32UserDataOfTransportLayer ClientAddressInfo = new Win32UserDataOfTransportLayer() { Port = 1, Ip = "192.168.1.1" };
            server.ReceivedMessage((UInt32)dataLen, (UInt32)offset,data, ClientAddressInfo);
            return true;
        }
        public static bool serverSend(Win32UserDataOfTransportLayer win32UserDataOfTransportLayer, int dataLen, int offset, byte[] data)
        {
            //Console.WriteLine($"serverSend {sid}");
            Win32UserDataOfTransportLayer ServerAddressInfo = new Win32UserDataOfTransportLayer() { Port = 2, Ip = "127.0.0.1" };
            client.ReceivedMessage((UInt32)dataLen, (UInt32)offset, data, ServerAddressInfo);
            return true;
        }
    }

}
