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
    public partial class DateTimeChange_Service<DTL> : IService<DTL>
    {
        public async Task DateTimeChange(ServiceInvokeParameter<DTL> serviceInvokeParameter, DateTime_t[] now)
        {
            var t = now[0];
            Console.WriteLine($"{t.Year}-{t.Month}-{t.Day}  {t.Hour}:{t.Min}:{t.Sec}");
        }
    }
    public partial class Add_Service<DTL> : IService<DTL>
    {
        public async Task Add(ServiceInvokeParameter<DTL> serviceInvokeParameter, Int32 a, Int32 b, Int32 dataLen, Byte[] data)
        {
            serviceInvokeParameter.RpcObject.Start_SuspendTimer(serviceInvokeParameter.TargetTimeOut / 2);
            Response.ReturnValue = (Int64)a + b;
            Thread.Sleep(1000);
            serviceInvokeParameter.RpcObject.Stop_SuspendTimer();
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
            Student2 stu = new Student2();
            stu.u8 = 0xff;
            stu.s8 = -1;
            stu.u16 = 0xffff;
            stu.i16 = -1;
            stu.u32 = UInt32.MaxValue;
            stu.i32 = -1;
            stu.u64 = UInt64.MaxValue;
            stu.i64 = Int64.MaxValue;
            stu.f = float.MaxValue;
            stu.d = double.MaxValue;

            byte[]x=new byte[128];
            SerializationManager sm = new SerializationManager(Assembly.GetExecutingAssembly(),x,0);
            stu.Serialize(sm);

            var x2 = new byte[]
            {
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
0xff,0xff,0x7f,0x7f,0xff,0xff,0xff,0xff,0xff,0xff,
0xef,0x7f,
            };
            sm.Buf = x2;

            sm.Index = 0;
            Student2 dest = new Student2();
            dest.Deserialize(sm);


            client = new EmbedXrpcObject<Win32UserDataOfTransportLayer>(100, clientSend, Assembly.GetExecutingAssembly(),4096);
            client.Start();
            server = new EmbedXrpcObject<Win32UserDataOfTransportLayer>(2000, serverSend, Assembly.GetExecutingAssembly(), 4096);
            server.Start();
            Task.Run(async () =>
            {
                Add_Requester<Win32UserDataOfTransportLayer> inter_Add = new Add_Requester<Win32UserDataOfTransportLayer>(client);
                while (true)
                {
                    Random random = new Random();
                    int a = random.Next();
                    Thread.Sleep(1000);
                    int b = random.Next();
                    var reAdd =await inter_Add.Invoke(new Win32UserDataOfTransportLayer() {  Ip="123",Port=11},a,b,0,null);
                    if(reAdd.State != RequestResponseState.ResponseState_Ok)
                    {
                        Console.WriteLine("request failed! error code:{0}", reAdd.State);
                    }
                    else
                    {
                        Console.WriteLine($"{a}+{b}={reAdd.ReturnValue}");
                        Debug.Assert((Int64)a + b == reAdd.ReturnValue);
                    }
                    
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
                Thread.Sleep(-1);
                DateTimeChange_Requester<Win32UserDataOfTransportLayer> broadcastDataTime = new DateTimeChange_Requester<Win32UserDataOfTransportLayer>(server);
                while (true)
                {
                    Thread.Sleep(1000);
                    broadcastDataTime.Invoke(new Win32UserDataOfTransportLayer() { Ip = "123", Port = 11 }, new DateTime_t[1] {
                    new DateTime_t()
                    {
                        Year = DateTime.Now.Year,
                        Month = DateTime.Now.Month,
                        Day = DateTime.Now.Day,
                        Hour = DateTime.Now.Hour,
                        Min = DateTime.Now.Minute,
                        Sec = DateTime.Now.Second,
                        David={ AddressType=Student.Student_u2_FieldNumber,u2=0x778899AA , uend1=1,uend2=0x88},
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
        public static async Task<bool> clientSend(Win32UserDataOfTransportLayer win32UserDataOfTransportLayer,int dataLen, int offset, byte[] data)
        {
            //Console.WriteLine($"clientSend {sid}");
            Win32UserDataOfTransportLayer ClientAddressInfo = new Win32UserDataOfTransportLayer() { Port = 1, Ip = "192.168.1.1" };
            server.ReceivedMessage((UInt32)dataLen, (UInt32)offset,data, ClientAddressInfo);
            return true;
        }
        public static async Task<bool> serverSend(Win32UserDataOfTransportLayer win32UserDataOfTransportLayer, int dataLen, int offset, byte[] data)
        {
            //Console.WriteLine($"serverSend {sid}");
            Win32UserDataOfTransportLayer ServerAddressInfo = new Win32UserDataOfTransportLayer() { Port = 2, Ip = "127.0.0.1" };
            client.ReceivedMessage((UInt32)dataLen, (UInt32)offset, data, ServerAddressInfo);
            return true;
        }
    }

}
