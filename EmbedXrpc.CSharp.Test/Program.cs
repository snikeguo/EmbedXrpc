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
    public partial class BroadcastDataTimeClientImpl : IDelegate
    {
        public void BroadcastDataTime(DateTime_t t)
        {
            Console.WriteLine($"{t.Year}-{t.Month}-{t.Day}  {t.Hour}:{t.Min}:{t.Sec}");
        }
    }
    public partial class GetStudentInfoFormStudentIdService : IService
    {
        public void GetStudentInfoFormStudentId(Byte StudentIdLen, Byte[] StudentId, Int32 arg2, Int32 arg3)
        {

        }
    }
    public partial class GetStudentsInfoFormAgeService : IService
    {
        public void GetStudentsInfoFormAge()
        {
            Thread.Sleep(10000);
            Response.ReturnValue = new StudentArray_t();
            Response.ReturnValue.StudentIdLen = 1;
            var stu = new Student_t() { Age = 0x99, ResultsLen = 0, StudentIdLen = 0 };
            //stu.Name = new byte[1];
            //stu.Results = new Result_t[0];
            stu.StudentId = Encoding.ASCII.GetBytes("0123456789");
            stu.StudentIdLen = (byte)stu.StudentId.Length;
            stu.Sex = Sex_t.HHHH;
            Response.ReturnValue.Students = new Student_t[1] { stu };
        }
    }
    public partial class TestService : IService
    {
        public void Test()
        {

        }
    }
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
            server = new Server(1000, serverSend, Assembly.GetExecutingAssembly());
            server.Start();

            Task.Run(() =>
            {
                IMyInterfaceClientImpl inter = new IMyInterfaceClientImpl(client);
                while (true)
                {
                    var re = inter.GetStudentsInfoFormAge();
                    Console.WriteLine(Encoding.ASCII.GetString(re.ReturnValue.Students[0].StudentId));
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
        }
        static Client client;
        static Server server;
        public static void clientSend(UInt32 sid, int dataLen, byte[] data)
        {
            //Console.WriteLine($"clientSend {sid}");
            server.ReceivedMessage(sid, (UInt32)dataLen, 0, data);
        }
        public static void serverSend(UInt32 sid, int dataLen, byte[] data)
        {
            //Console.WriteLine($"serverSend {sid}");
            client.ReceivedMessage(sid, (UInt32)dataLen, 0, data);
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
                client = new Client(1000, clientSend, Assembly.GetExecutingAssembly());
                client.Start();
                TcpClient.GetStream().BeginRead(recBuff, 0, recBuff.Length, RecCallback, null);
                Task.Run(() =>
                {
                    IMyInterfaceClientImpl inter = new IMyInterfaceClientImpl(client);
                    while (true)
                    {
                        
                        try
                        {
                            var re = inter.GetStudentsInfoFormAge();
                            Console.WriteLine(Encoding.ASCII.GetString(re.ReturnValue.Students[0].StudentId));
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
                UInt32 sid = (UInt32)(recBuff[2] << 0 | recBuff[3] << 8 | recBuff[4] << 16 | recBuff[5] << 24);
                client.ReceivedMessage(sid, (UInt32)(len - 6), 6, recBuff);
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
        public static void clientSend(UInt32 sid, int dataLen, byte[] data)
        {
            var stream = TcpClient.GetStream();
            byte[] buffer = new byte[4 + dataLen + 2];
            buffer[0] = 0xff;
            buffer[1] = 0xff;
            buffer[2] = (byte)(sid&0xff);
            buffer[3] = (byte)(sid>>8 & 0xff);
            buffer[4] = (byte)(sid>>16 & 0xff);
            buffer[5] = (byte)(sid>>24 & 0xff);
            Array.Copy(data, 0, buffer, 6, dataLen);
            stream.Write(buffer);
        }
        
    }
#endif
}
