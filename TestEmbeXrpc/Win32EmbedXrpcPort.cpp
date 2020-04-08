#include "Win32EmbedXrpcPort.h"
#include <thread>  
#include <mutex>
#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"
#include "BlockingQueue.h"
#include <iostream>

EmbedXrpc_Thread_t Win32EmbedXrpcPort::CreateThread(const char* threadName, void(*Thread)(void*), void* Arg)
{
	std::thread *x = new std::thread(Thread,Arg);
	return  x;
}

EmbedXrpc_Mutex_t Win32EmbedXrpcPort::CreateMutex(const char* mutexName)
{
	std::mutex* mutex = new std::mutex();
	return  mutex;
}
EmbedXrpc_Queue_t Win32EmbedXrpcPort::CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen)
{
	//这里创建队列，由于我只实现了C++泛型的队列，而底层RTOS一般要求提供的是queueItemSize，所以这里硬编码直接创建EmbeXrpcRawData;
	BlockingQueue<EmbeXrpcRawData> *queue = new BlockingQueue<EmbeXrpcRawData>();

	return queue;
}
#include <windows.h>  
struct wintimer
{
	uint32_t counter = 0;
	uint32_t timeout;
	void *Arg;
	HANDLE hThread;
	void (*timercb)(void* arg);
	bool isEnable;
};

//线程  
DWORD CALLBACK ThreadProc(PVOID pvoid)
{
	//强制系统为线程简历消息队列  
	wintimer* par = (wintimer *)pvoid;
	while (true)
	{
		Sleep(1);
		if (par->isEnable == true)
		{
			par->counter++;
			if (par->counter >= par->timeout)
			{
				par->timercb(par->Arg);
				par->counter = 0;
			}
		}
		
	}
	return 0;
}
EmbedXrpc_Timer_t Win32EmbedXrpcPort::CreateTimer(const char* timerName, uint32_t timeout, void* Arg, void (*timercb)(void* arg))
{
	DWORD dwThreadId;
	wintimer* wintimerpar = new wintimer;
	wintimerpar->timeout = timeout;
	wintimerpar->Arg = Arg;
	wintimerpar->timercb = timercb;
	wintimerpar->isEnable = false;
	HANDLE hThread = ::CreateThread(NULL, 0, ThreadProc, wintimerpar, 0, &dwThreadId);
	wintimerpar->hThread = hThread;
	
	return wintimerpar;
}
void Win32EmbedXrpcPort::ThreadStart(EmbedXrpc_Thread_t thread)
{
	std::thread* x = static_cast<std::thread*>(thread);
	//x->join();

}
void  Win32EmbedXrpcPort::TimerStart(EmbedXrpc_Timer_t timer)
{
	wintimer* x = static_cast<wintimer*>(timer);
	x->counter = 0;
	x->isEnable = true;
	
}
void Win32EmbedXrpcPort::TimerReset(EmbedXrpc_Timer_t timer)
{
	wintimer* x = static_cast<wintimer*>(timer);
	x->counter = 0;
}
void Win32EmbedXrpcPort::TimerStop(EmbedXrpc_Timer_t timer)
{
	wintimer* x = static_cast<wintimer*>(timer);
	x->isEnable = false;
	x->counter = 0;
}
#if 0
EmbedXrpc_Mutex_t Win32EmbedXrpcPort::CreateSemaphore(const char* semaphoreName)
{
	return  nullptr;
}
bool Win32EmbedXrpcPort::TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
{
	return false;
}
bool Win32EmbedXrpcPort::ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
{
	return false;
}
#endif
bool Win32EmbedXrpcPort::TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout)
{
	return true;
}
bool Win32EmbedXrpcPort::ReleaseMutex(EmbedXrpc_Mutex_t mutex)
{
	return true;
}

QueueState Win32EmbedXrpcPort::ReceiveQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemlen, uint32_t timeout)
{
	BlockingQueue<EmbeXrpcRawData>* q= static_cast<BlockingQueue<EmbeXrpcRawData>*>(queue);
	auto r=q->take(timeout, (EmbeXrpcRawData *)item);
	if (r== true)
	{
		return QueueState_OK;
	}
	else
	{
		return QueueState_Timeout;
	}
	
}
QueueState Win32EmbedXrpcPort::SendQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemlen)
{
	BlockingQueue<EmbeXrpcRawData>* q = static_cast<BlockingQueue<EmbeXrpcRawData>*>(queue);
	EmbeXrpcRawData* data = static_cast<EmbeXrpcRawData*> (item);
	q->put(*data);
	return QueueState_OK;
}
void Win32EmbedXrpcPort::ResetQueue(EmbedXrpc_Queue_t queue)
{
	BlockingQueue<EmbeXrpcRawData>* q = static_cast<BlockingQueue<EmbeXrpcRawData>*>(queue);
	//reset;
}
void* Win32EmbedXrpcPort::Malloc(uint32_t size)
{
	auto x=malloc(size);
	//std::cout << "malloc ptr:	"<<std::hex<<x<<"	size:	" << size << std::endl;
	return x;
}
void Win32EmbedXrpcPort::Free(void* ptr)
{
	//std::cout << "free ptr:	" << std::hex << ptr  << std::endl;
}
void Win32EmbedXrpcPort::Memcpy(void* d, const void* s, uint32_t size)
{
	::memcpy(d, s, size);
}