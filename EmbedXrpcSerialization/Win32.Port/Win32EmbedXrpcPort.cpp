#include  "../EmbedXrpcPortInterface.h"

#include <thread>
#include <mutex>

#include "BlockQueue.h"
#include "windows.h"
using Semaphore = BlockingQueue<int>;
extern "C"
{

	EmbedXrpc_Thread_t EmbedXrpc_CreateThread(const char* threadName, uint8_t priority, void(*Thread)(void*), void* Arg)
	{
		std::thread* ServiceThread = new std::thread(Thread, Arg);
		return  ServiceThread;
	}

	EmbedXrpc_Mutex_t EmbedXrpc_CreateMutex(const char* mutexName)
	{
		//QMutex* mutex = new QMutex();
		std::timed_mutex* mutex = new std::timed_mutex();
		return  mutex;
	}
	EmbedXrpc_Queue_t EmbedXrpc_CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen)
	{
		//这里创建队列，由于我只实现了C++泛型的队列，而底层RTOS一般要求提供的是queueItemSize，所以这里硬编码直接创建EmbeXrpcRawData;
		NoGenericBlockingQueue* q = new NoGenericBlockingQueue(queueItemSize);
		return q;
	}
	EmbedXrpc_Semaphore_t  EmbedXrpc_CreateSemaphore(const char* SemaphoreName)
	{
		Semaphore* sem = new Semaphore();
		return sem;
	}
	class Win32Timer //简单的timer
	{
	public:
		Win32Timer(const char* timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg)
		{
			name = timerName;
			this->timerout = timeout;
			this->timercb = timercb;
			this->arg = Arg;
			timerThread = new std::thread(TimerThread, this);
			timerThread->detach();
		}
		static void TimerThread(void* arg)
		{
			Win32Timer* This = static_cast<Win32Timer*> (arg);
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				if (This->RunFlag)
				{
					This->count += 10;
					if (This->count >= This->timerout)
					{
						This->count = 0;
						This->timercb(This->arg);
					}
				}
				if (This->ExitFlag == true)
				{
					break;
				}
			}
			This->ExitedFlag = true;
		}
		void Start()
		{
			RunFlag = true;

		}
		void Stop()
		{
			RunFlag = false;
			count = 0;
		}
		~Win32Timer()
		{
			ExitFlag = true;
			while (ExitedFlag == false);
		}
		std::thread* timerThread;
		volatile bool RunFlag;
		volatile bool ExitFlag = false;
		volatile bool ExitedFlag = false;
		const char* name;
		uint32_t count = 0;
		uint32_t timerout;
		void (*timercb)(void* arg);
		void* arg;
	};
	EmbedXrpc_Timer_t EmbedXrpc_CreateTimer(const char* timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg)
	{
		Win32Timer* timer = new Win32Timer(timerName, timeout, timercb, Arg);
		return timer;
	}
	void EmbedXrpc_DeleteThread(EmbedXrpc_Thread_t thread)
	{
		//auto qtThread = static_cast<QThread*>(thread);
		auto qtThread = static_cast<std::thread*>(thread);
		delete qtThread;
	}
	void EmbedXrpc_DeleteMutex(EmbedXrpc_Mutex_t mutex)
	{
		auto qtMutex = static_cast<std::mutex*>(mutex);
		delete qtMutex;
	}
	void EmbedXrpc_DeleteQueue(EmbedXrpc_Queue_t queue)
	{
		auto qtQueue = static_cast<NoGenericBlockingQueue*>(queue);
		qtQueue->Reset();
		delete qtQueue;
	}
	void EmbedXrpc_DeleteSemaphore(EmbedXrpc_Semaphore_t sem)
	{
		Semaphore* qtsem = static_cast<Semaphore*>(sem);
		qtsem->Reset();
		delete sem;
	}
	void EmbedXrpc_DeleteTimer(EmbedXrpc_Timer_t timer)
	{
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer);
		win32timer->Stop();
		delete win32timer;
	}
	void EmbedXrpc_ThreadStart(EmbedXrpc_Thread_t thread)
	{
		std::thread* x = static_cast<std::thread*>(thread);
		x->detach();
	}
	void  EmbedXrpc_TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
	{
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer);
		win32timer->timerout = interval;
		win32timer->Start();
	}
	void EmbedXrpc_TimerReset(EmbedXrpc_Timer_t timer)
	{
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer);
		win32timer->Stop();
	}
	void EmbedXrpc_TimerStop(EmbedXrpc_Timer_t timer)
	{
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer);
		win32timer->Stop();
	}

	Bool EmbedXrpc_TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
	{
		Semaphore* qtsem = static_cast<Semaphore*>(sem);
		int recItem = 0;
		auto r = qtsem->Receive(recItem, timeout);
		//assert(r == true);
		return r;

	}
	void EmbedXrpc_ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
	{
		Semaphore* qtsem = static_cast<Semaphore*>(sem);
		int recItem = 0;
		qtsem->Send(recItem);
	}
	void EmbedXrpc_ResetSemaphore(EmbedXrpc_Semaphore_t sem)
	{
		Semaphore* qtsem = static_cast<Semaphore*>(sem);
		qtsem->Reset();
	}

	Bool EmbedXrpc_TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout)
	{
		std::timed_mutex* m = static_cast<std::timed_mutex*>(mutex);
		std::chrono::milliseconds to(timeout);
		return m->try_lock_for(to);
	}
	Bool EmbedXrpc_ReleaseMutex(EmbedXrpc_Mutex_t mutex)
	{
		std::timed_mutex* m = static_cast<std::timed_mutex*>(mutex);
		m->unlock();
		return true;
	}

	QueueState EmbedXrpc_ReceiveQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemSize, uint32_t timeout)
	{
		NoGenericBlockingQueue* q = static_cast<NoGenericBlockingQueue*>(queue);
		auto r = q->Receive(item, timeout);
		if (r == QueueStatus::Ok)
		{
			return QueueState_OK;
		}
		else
		{
			return QueueState_Timeout;
		}

	}
	QueueState EmbedXrpc_SendQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemSize)
	{
		NoGenericBlockingQueue* q = static_cast<NoGenericBlockingQueue*>(queue);
		q->Send(item);
		return QueueState_OK;
	}
	void EmbedXrpc_ResetQueue(EmbedXrpc_Queue_t queue)
	{
		NoGenericBlockingQueue* q = static_cast<NoGenericBlockingQueue*>(queue);
		q->Reset();
	}
	uint32_t EmbedXrpc_QueueSpacesAvailable(EmbedXrpc_Queue_t queue)
	{
		return -1;
	}
	void* EmbedXrpc_Malloc(uint32_t size)
	{
		auto x = malloc(size);
		printf("malloc ptr:0x%8x,size:%4d\n", x, size);
		return x;
	}
	void EmbedXrpc_Free(void* ptr)
	{
		free(ptr);
		printf("free ptr:0x%8x\n", ptr);
	}
	void EmbedXrpc_Memcpy(void* d, const void* s, uint32_t size)
	{
		::memcpy(d, s, size);
	}
	void EmbedXrpc_Memset(void* d, int v, uint32_t size)
	{
		::memset(d, v, size);
	}
}