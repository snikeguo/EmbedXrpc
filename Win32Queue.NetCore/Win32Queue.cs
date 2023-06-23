using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Collections.Concurrent;
using System.Threading;

namespace David.Common
{
    public enum QueueStatus
    {
        OK,
        Full,
        Empty,
    }
    public class Win32Queue<T>
    {
        public static UInt32 Zero = 0;
        public static UInt32 MAX_Delay = 0xFFFFFFFF;
        public Win32Queue()
        {
            //queue = new ConcurrentQueue<T>();
            queue = new BlockingCollection<T>(new ConcurrentQueue<T>(),int.MaxValue);
        }
        public void Reset()
        {
            while (queue.TryTake(out _)) ;
        }
        public int Count { get { return queue.Count; } }
        private BlockingCollection<T> queue = null;
        public QueueStatus Receive(out T r, UInt32 delayMillisecond)
        {
            r = default(T);
            if (queue.TryTake(out r, (int)delayMillisecond) == true)
            {
                return QueueStatus.OK;                  
            }  
            else
            {
                return QueueStatus.Empty;
            }
        }
        public QueueStatus Send(T msg)
        {
            if (queue.Count > queue.BoundedCapacity)
                return QueueStatus.Full;
            queue.TryAdd(msg,0);
            return QueueStatus.OK;
        }
    }
}

