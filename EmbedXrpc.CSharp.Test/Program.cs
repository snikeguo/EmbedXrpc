using EmbedXrpc;
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
    public partial class DateTimeChangeClientImpl : IDelegate
    {
        public void DateTimeChange(DateTime_t[] now)
        {
            var t = now[0];
            Console.WriteLine($"{t.Year}-{t.Month}-{t.Day}  {t.Hour}:{t.Min}:{t.Sec}");
        }
    }
    public partial class Inter_AddService : IService
    {
        public void Add(Int32 a, Int32 b, Int32 dataLen, Byte[] data)
        {
            Response.ReturnValue.Sum = a + b;
            Response.ReturnValue.dataLen = 0;
        }
    }
    public partial class Inter_NoArgService : IService
    {
        public void NoArg()
        {

        }
    }
    public partial class Inter_NoReturnService : IService
    {
        public void NoReturn(int a)
        {

        }
    }
    public partial class Inter_NoArgAndReturnService : IService
    {
        public void NoArgAndReturn()
        {

        }
    }
}
namespace EmbedXrpc
{

    class Program
    {
        static void Main(string[] args)
        {
            client = new EmbedXrpcObject(1000, clientSend, Assembly.GetExecutingAssembly(),true);
            client.Start();
            server = new EmbedXrpcObject(2000, serverSend, Assembly.GetExecutingAssembly(),true);
            server.Start();
            Task.Run(() =>
            {
                Inter_Add inter_Add = new Inter_Add(client);
                while (true)
                {
                    Thread.Sleep(-1);
                    var reAdd = inter_Add.Invoke(1,2,0,null);
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
                DateTimeChangeDelegate broadcastDataTimeDelegate = new DateTimeChangeDelegate(server);
                while (true)
                {
                    Thread.Sleep(100);
                    broadcastDataTimeDelegate.Invoke(new DateTime_t[1] {
                    new DateTime_t()
                    {
                        Year = DateTime.Now.Year,
                        Month = DateTime.Now.Month,
                        Day = DateTime.Now.Day,
                        Hour = DateTime.Now.Hour,
                        Min = DateTime.Now.Minute,
                        Sec = DateTime.Now.Second,
                        David=null,
                    }});
                    
                }
            });

            while (true)
            {

                Thread.Sleep(1000);
            }
        }

        static EmbedXrpcObject client;
        static EmbedXrpcObject server;
        public static bool clientSend(int dataLen, int offset, byte[] data)
        {
            //Console.WriteLine($"clientSend {sid}");
            server.ReceivedMessage((UInt32)dataLen, (UInt32)offset,data);
            return true;
        }
        public static bool serverSend(int dataLen, int offset, byte[] data)
        {
            //Console.WriteLine($"serverSend {sid}");
            client.ReceivedMessage((UInt32)dataLen, (UInt32)offset, data);
            return true;
        }
    }

}
