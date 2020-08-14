using System;
using System.Collections.Generic;
using System.Linq;
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
            SerializationManager sm = new SerializationManager();
            sm.IsEnableMataData = true;
            sm.Serialize(s, 0);
            for (int i = 0; i < sm.Index; i++)
            {
                Console.Write("0x{0:X},", sm.Data[i]);
                if((i+1)%10==0)
                {
                    Console.Write("\n");
                }
            }
            Console.ReadLine();
        }
    }
}
