using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;
using EmbedXrpc;
using StudentService;
namespace SerializationManagerTest
{
    class Program
    {
        static void Main(string[] args)
        {
            Student s = new Student();
            s.ChineseAchievement.Arlen = 1;
            s.ChineseAchievement.Ar = new Test[] { new Test()};
            s.ChineseAchievement.Ar[0].IntPtrsLen = 2;
            s.ChineseAchievement.Ar[0].IntPtr = new int[2];
            s.ChineseAchievement.Ar[0].IntPtr[0] = 3;
            s.ChineseAchievement.Ar[0].IntPtr[1] = 4;
            SerializationManager sm = new SerializationManager(Assembly.GetExecutingAssembly());
            sm.IsEnableMataData = false;
            sm.Serialize(s, 0);
            for (int i = 0; i < sm.Index; i++)
            {
                Console.Write("0x{0:X},", sm.Data[i]);
                if((i+1)%10==0)
                {
                    Console.Write("\n");
                }
            }
            sm.Index = 0;
            var r=sm.Deserialize(typeof(Student));
            Console.ReadLine();
        }
    }
}
/*
 *
 0x01,0x02,0x03,0x00,0x00,0x00,0x04,0x00,0x00,0x00,
 *
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
