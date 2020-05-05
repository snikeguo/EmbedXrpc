using EmbedXrpc;
using StudentService;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


namespace StudentService
{
#if false
    public partial class BroadcastDataTimeClientImpl : IDelegate
    {
        public void BroadcastDataTime(DateTime_t t)
        {
            Console.WriteLine($"{t.Year}-{t.Month}-{t.Day}  {t.Hour}:{t.Min}:{t.Sec}");
        }
    }
    public partial class IMyInterface_GetStudentInfoFormStudentIdService : IService
    {
        public void GetStudentInfoFormStudentId(Byte StudentIdLen, Byte[] StudentId, Int32 arg2, Int32 arg3)
        {

        }
    }
    public partial class IMyInterface_GetStudentsInfoFormAgeService : IService
    {
        public void GetStudentsInfoFormAge()
        {
            //Thread.Sleep(10000);
            Response.ReturnValue = new StudentArray_t();
            Response.ReturnValue.StudentIdLen = 1;
            var stu = new Student_t() { Age = 2, ResultsLen = 0, StudentIdLen = 0 };
            //stu.Name = new byte[1];
            //stu.Results = new Result_t[0];
            stu.StudentId = Encoding.ASCII.GetBytes("0123456789");
            stu.StudentIdLen = (byte)stu.StudentId.Length;
            stu.Sex = Sex_t.HHHH;
            Response.ReturnValue.Students = new Student_t[1] { stu };
        }
    }
    public partial class IMyInterface_TestService : IService
    {
        public void Test(Byte[] noLen)
        {
            this.Response.ReturnValue = true;
        }
    }
#endif
}

namespace EmbedXrpc
{
#if local
    class Program
    {
        static void Main(string[] args)
        {
            client = new Client(1000, clientSend, Assembly.GetExecutingAssembly());
            client.Start();
            server = new Server(2000, serverSend, Assembly.GetExecutingAssembly());
            server.Start();
            Serialization serialization = new Serialization(Assembly.GetExecutingAssembly());
            SerializationManager sm = new SerializationManager();
            StudentBitTest studentBitTest = new StudentBitTest();
            studentBitTest.bt.Field1 = 1;
            studentBitTest.bt.Field2 = 2;
            serialization.Serialize(sm, studentBitTest);
#if false
            Task.Run(() =>
            {
                IMyInterfaceClientImpl inter = new IMyInterfaceClientImpl(client);
                while (true)
                {
                    var re = inter.GetStudentsInfoFormAge();
                    Console.WriteLine(Encoding.ASCII.GetString(re.ReturnValue.Students[0].StudentId));
                    var x=inter.Test(new byte[1] { 22});
                    Thread.Sleep(100);
                }
            });

            Task.Run(() =>
            {
                BroadcastDataTimeDelegate broadcastDataTimeDelegate = new BroadcastDataTimeDelegate(server);
                while (true)
                {
                    broadcastDataTimeDelegate.Invoke(new DateTime_t()
                    {
                        Year = DateTime.Now.Year,
                        Month = DateTime.Now.Month,
                        Day = DateTime.Now.Day,
                        Hour = DateTime.Now.Hour,
                        Min = DateTime.Now.Minute,
                        Sec = DateTime.Now.Second
                    });
                    Thread.Sleep(100);
                }
            });

            while (true)
            {

                Thread.Sleep(1000);
            }
#endif
        }

        static Client client;
        static Server server;
        public static void clientSend(int dataLen, int offset, byte[] data)
        {
            //Console.WriteLine($"clientSend {sid}");
            server.ReceivedMessage((UInt32)dataLen, (UInt32)offset,data);
        }
        public static void serverSend(int dataLen, int offset, byte[] data)
        {
            //Console.WriteLine($"serverSend {sid}");
            client.ReceivedMessage((UInt32)dataLen, (UInt32)offset, data);
        }
    }
#else
        class Program
    {
        static void Main(string[] args)
        {
            TcpListener = new TcpListener(IPAddress.Any, 5567);
            TcpListener.Start();
            Task.Run(() =>
            {
                TcpClient = TcpListener.AcceptTcpClient();
                client = new Client(6666, clientSend, Assembly.GetExecutingAssembly());
                client.Start();
                TcpClient.GetStream().BeginRead(recBuff, 0, recBuff.Length, RecCallback, null);
                Task.Run(() =>
                {
                    IMyInterfaceClientImpl inter = new IMyInterfaceClientImpl(client);
                    while (true)
                    {
                        
                        try
                        {
                            //var re = inter.GetStudentsInfoFormAge();
                            //Console.WriteLine(Encoding.ASCII.GetString(re.ReturnValue.Students[0].StudentId));
                            var testResult = inter.Test();
                            Console.WriteLine("testResult:{0}", testResult.ReturnValue);
                        }
                        catch 
                        {

                            
                        }
                       
                        Thread.Sleep(1000);
                    }
                });
            });
            

            

            while (true)
            {

                Thread.Sleep(1000);
            }
        }
        public static void RecCallback(IAsyncResult ar)
        {
            try
            {
                var stream = TcpClient.GetStream();
                var len = stream.EndRead(ar);
                if (recBuff[0] != 0xff && recBuff[1] != 0xff)
                {
                    return;
                }
                client.ReceivedMessage((UInt32)(len - 2), 2, recBuff);
                TcpClient.GetStream().BeginRead(recBuff, 0, recBuff.Length, RecCallback, null);
            }
            catch (Exception e)
            {

                Console.WriteLine(e.Message);
            }
            
        }
        static Client client;
        static TcpListener TcpListener;
        static TcpClient TcpClient;
        static byte[] recBuff = new byte[10240];
        public static void clientSend(int dataLen, int offset,byte[] data)
        {
            var stream = TcpClient.GetStream();
            byte[] buffer = new byte[dataLen + 2];
            buffer[0] = 0xff;
            buffer[1] = 0xff;
            Array.Copy(data, offset, buffer, 2, dataLen);
            stream.Write(buffer);
        }
        
    }
#endif
}
