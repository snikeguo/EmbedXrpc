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
            stu.Name = new byte[1];
            stu.Results = new Result_t[0];
            stu.StudentId = new short[0];
            stu.Sex = Sex_t.Woman;
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
            client = new Client(5000, clientSend, Assembly.GetExecutingAssembly());
            client.Start();
            server = new Server(5000, serverSend, Assembly.GetExecutingAssembly());
            server.Start();
            IMyInterfaceClientImpl inter = new IMyInterfaceClientImpl(client);
            var re = inter.GetStudentsInfoFormAge();
            while(true)
            {
                Thread.Sleep(100);
            }
        }
        static Client client;
        static Server server;
        public static void clientSend(UInt32 sid,int dataLen,byte[]data)
        {
            server.ReceivedMessage(sid, (UInt32)dataLen, 0, data);
        }
        public static void serverSend(UInt32 sid, int dataLen, byte[] data)
        {
            client.ReceivedMessage(sid, (UInt32)dataLen, 0, data);
        }
    }
}
