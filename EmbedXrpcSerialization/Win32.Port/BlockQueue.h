#ifndef BLOCKQUEUE_H
#define BLOCKQUEUE_H
#include <mutex>
#include <queue>
#include <windows.h>
#include <iostream>
#include <cassert>
enum QueueStatus
{
	Full,
	Empty,
	Ok,
};
template<class T>
class BlockingQueue
{
public:
	BlockingQueue()
	{
		mutex = CreateMutex(NULL, false, NULL);
		Win32Semaphore = CreateSemaphore(NULL, 0, LONG_MAX, NULL);
	}
	~BlockingQueue() 
	{
		CloseHandle(Win32Semaphore);
		CloseHandle(mutex);
	}
	void Send(T& msg) ;
	QueueStatus Receive(T& msg, unsigned long timeOut) ;
	void Reset() ;
	int Size() ;
private:
	volatile HANDLE Win32Semaphore;
	volatile HANDLE			mutex;
	 std::queue<T> 	queue;
	 volatile int count = 0;
	void Lock() 
	{
		WaitForSingleObject(mutex, INFINITE);
	}
	void Unlock() 
	{
		ReleaseMutex(mutex);
	}
};
template<class  T>
void BlockingQueue<T>::Send(T& msg) 
{
	//VXLOG("before Send/size:%d\n", Size());
	Lock();
	queue.push(msg);
	count++;
	Unlock();
	auto rs=ReleaseSemaphore(Win32Semaphore, 1, nullptr);	
	//VXLOG("after Send/size:%d\n", Size());
	assert(rs != 0);
	
}

template<class T>															//timeOut最大等待延迟时间
QueueStatus BlockingQueue<T>::Receive(T& msg, unsigned long timeOut) 
{
	//VXLOG("before InternalWait/size:%d,timeout:%d,issub:%d\n", Size(), timeOut,isSub);
	QueueStatus s = QueueStatus::Empty;
	auto r = WaitForSingleObject(Win32Semaphore, timeOut);
	Lock();//锁起来要再次判断一次count
	if (r == WAIT_OBJECT_0)
	{
		if (count == 0)
		{
			DebugBreak();
			assert(count > 0);
		}
		if (count > 0)
		{
			s = QueueStatus::Ok;
			msg = queue.front();
			queue.pop();
			count--;
		}
		else
		{
			s= QueueStatus::Empty;
		}
		
		//VXLOG("Pop:count:%d\n", count);
	}
	else
	{
		s = QueueStatus::Empty;
	}
	Unlock();
	//VXLOG("after InternalWait/size:%d,timeout:%d,issub:%d\n", Size(), timeOut, isSub);
	return s;
}
template<class T>
void BlockingQueue<T>::Reset() 
{
	Lock();
	std::queue<T> empty;
	swap(empty, queue);
	while (WaitForSingleObject(Win32Semaphore, 0) == WAIT_OBJECT_0);
	count = 0;
	Unlock();
}
template<class T>
int BlockingQueue<T>::Size() 
{
	int size = 0;
	Lock();
	size=count;
	Unlock();
	return size;
}

class NoGenericBlockingQueue
{
public:
	NoGenericBlockingQueue(unsigned long itemSize)
		:genericQueue(), itemSize(itemSize) 
	{

	}
	void Send(void* msg) 
	{
		NoGenericStruct ngs;
		ngs.Item = malloc(itemSize);
		//printf("malloc %x\n", ngs.Item);
		memcpy(ngs.Item, msg, itemSize);
		genericQueue.Send(ngs);
	}
	QueueStatus Receive(void* msg, unsigned long timeOut) 
	{
		NoGenericStruct ngs;
		auto recsult=genericQueue.Receive(ngs, timeOut);
		if (recsult == QueueStatus::Ok)
		{
			memcpy(msg, ngs.Item, itemSize);
			free(ngs.Item);
			//printf("free %x\n", ngs.Item);
		}
		return recsult;
	}
	void Reset() 
	{
		while (true)
		{
			NoGenericStruct ngs;
			auto recsult = genericQueue.Receive(ngs, 0);
			if (recsult == QueueStatus::Ok)
			{
				free(ngs.Item);
			}
			else
			{
				break;
			}
		}
	}
	int Size() 
	{
		return genericQueue.Size();
	}
private:
	struct NoGenericStruct
	{
		void* Item;
	};
	 BlockingQueue<NoGenericStruct> genericQueue;
	unsigned long itemSize;
};
#endif