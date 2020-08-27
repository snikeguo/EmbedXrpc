using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
namespace David.Common
{
    /*public enum SignalStatus
    {
        OK,
        Full,
        Empty,
    }*/
    public class Win32BinarySignal:Win32Queue<int>
    {
        public Win32BinarySignal():base()
        {          
        }
        public QueueStatus Send()
        {
            return Send(1);
        }
        public QueueStatus Receive(UInt32 delay)
        {
            int i = 0;
            return  Receive(out i, delay);            
        }
    }
}
