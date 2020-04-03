using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpc
{
    public class Sub2Class
    {
        public Int32 C { get; set; } = 0x12345678;
    }
    public class SubClass
    {
        public byte A { get; set; } = 5;
        public byte B { get; set; } = 8;

        public Int16 Sub2Len { get; set; } = 1;

        [ArrayProperty(LenFieldName = "Sub2Len")]
        public Sub2Class[] Sub2 { get; set; }
    }
    public class Student
    {
        public byte Age { get; set; } = 1;
        public byte Value { get; set; } = 2;

        public Int16 SubLen { get; set; } = 6;
        [ArrayProperty(LenFieldName = "SubLen")]
        public SubClass[] Sub { get; set; }
    }
    class Program
    {
        static void Main(string[] args)
        {
            SerializationManager sm = new SerializationManager();
            Student s = new Student();
            s.Sub = new SubClass[1] { new SubClass() {  A=0x33, Sub2Len=1,Sub2=new Sub2Class[] { new Sub2Class()} }};
            s.SubLen = 1;
            s.Age = 0x99;
            Serialization.Serialize(sm, s);

            SerializationManager rsm = new SerializationManager();
            rsm.Reset();
            rsm.Data = new List<byte>(sm.Data);
            var a=Serialization.Deserialize<Student>(rsm);
            Console.ReadLine();
        }
        static Client client;
        static Server server;
        public static void clientSend(UInt32 sid,UInt32 dataLen,UInt32 offset,byte[]data)
        {

        }
        public static void serverSend(UInt32 sid, UInt32 dataLen, UInt32 offset, byte[] data)
        {

        }
    }
}
