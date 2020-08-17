using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using EmbedXrpc;
namespace SerializationManagerTest
{
    class Program
    {
        static void Main(string[] args)
        {
            SerializationManager sm = new SerializationManager(Assembly.GetExecutingAssembly(),true,new List<byte>());
            Console.ReadLine();
        }
    }
}
/*
 *
 0x01,0x02,0x03,0x00,0x00,0x00,0x04,0x00,0x00,0x00,
 *
 0xB,0x4B,0x8A,0x1,0x1,0x2,0xBA,0x1,0x2,0x51,
0x3,0x0,0x0,0x0,0x4,0x0,0x0,0x0,0x7F,0x7F,
0x7F,
 * 
SerializeKey FieldNumber:0,Type:TYPE_OBJECT
SerializeKey FieldNumber:4,Type:TYPE_OBJECT
SerializeKey FieldNumber:8,Type:TYPE_ARRAY
SerializeLen:1
SerializeArrayElementFlag:0x2
SerializeKey FieldNumber:11,Type:TYPE_ARRAY
SerializeLen:2
SerializeArrayElementFlag:0x51
SerializeValue:
3,0,0,0,
SerializeValue:
4,0,0,0,
SerializeEnd
SerializeEnd
SerializeEnd
send:
0x0b,0x4b,0x8a,0x01,0x01,0x02,0xba,0x01,0x02,0x51,
0x03,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x7f,0x7f,
0x7f,Deserialize:Student.ChineseAchievement
Deserialize:Achievement.Ar
malloc:arrayfield:Achievement.Ar,malloc :0x7a7770
Deserialize:Test.IntPtr
malloc:arrayfield:Test.IntPtr,malloc :0x7a77a8
free :0x7a77a8
free :0x7a7770
 */
