using EmbedXrpc;
using StudentService;
using System;
using System.Collections.Generic;
using System.Linq;
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

    class Program
    {
        static void Main(string[] args)
        {
            client = new Client(500000, clientSend, Assembly.GetExecutingAssembly());
            client.Start();
            server = new Server(500000, serverSend, Assembly.GetExecutingAssembly());
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
            server.ReceivedMessage(sid, (UInt32)dataLen, 0, data);
        }
        public static void serverSend(UInt32 sid, int dataLen, byte[] data)
        {
            client.ReceivedMessage(sid, (UInt32)dataLen, 0, data);
        }
    }
}
