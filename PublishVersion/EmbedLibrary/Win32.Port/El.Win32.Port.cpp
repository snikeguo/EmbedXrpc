#include  "../EmbedLibrary.h"

#include <thread>
#include <mutex>

#include "BlockQueue.h"
#include "windows.h"
#include "EmbedXrpcCommon.h"
#if EmbedXrpc_UsingOs == 0
#include "noos_queue.h"
#endif
using Semaphore = BlockingQueue<int>;
extern "C"
{

	El_Thread_t El_CreateThread(const char* threadName, uint8_t priority, void(*Thread)(void*), void* Arg, uint16_t stack_size)
	{
#if EmbedXrpc_UsingOs == 1
		std::thread* ServiceThread = new std::thread(Thread, Arg);
		return  ServiceThread;
#else
		return nullptr;
#endif
	}

	El_Mutex_t El_CreateMutex(const char* mutexName)
	{
		//QMutex* mutex = new QMutex();
#if EmbedXrpc_UsingOs == 1
		std::timed_mutex* mutex = new std::timed_mutex();
		return  mutex;
#else
		return nullptr;
#endif
	}
	El_Queue_t El_CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen)
	{
		//这里创建队列，由于我只实现了C++泛型的队列，而底层RTOS一般要求提供的是queueItemSize，所以这里硬编码直接创建EmbeXrpcRawData;
#if EmbedXrpc_UsingOs == 1
		NoGenericBlockingQueue* q = new NoGenericBlockingQueue(queueItemSize);
		return q;
#else
		QueueHandle_t q = xQueueCreate(maxItemLen, queueItemSize);
		return q;
#endif
	}
	El_Semaphore_t  El_CreateSemaphore(const char* SemaphoreName)
	{
#if EmbedXrpc_UsingOs == 1
		Semaphore* sem = new Semaphore();
		return sem;
#else
		return nullptr;
#endif
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
	El_Timer_t El_CreateTimer(const char* timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg)
	{
#if EmbedXrpc_UsingOs == 1
		Win32Timer* timer = new Win32Timer(timerName, timeout, timercb, Arg);
		return timer;
#else
		return nullptr;
#endif
	}
	void El_DeleteThread(El_Thread_t thread)
	{
		//auto qtThread = static_cast<QThread*>(thread);
#if EmbedXrpc_UsingOs == 1
		auto qtThread = static_cast<std::thread*>(thread);
		delete qtThread;
#else
		return ;
#endif
	}
	void El_DeleteMutex(El_Mutex_t mutex)
	{
#if EmbedXrpc_UsingOs == 1
		auto qtMutex = static_cast<std::mutex*>(mutex);
		delete qtMutex;
#else
		return;
#endif
	}
	void El_DeleteQueue(El_Queue_t queue)
	{
#if EmbedXrpc_UsingOs == 1
		auto qtQueue = static_cast<NoGenericBlockingQueue*>(queue);
		qtQueue->Reset();
		delete qtQueue;
#else
		QueueHandle_t q = (QueueHandle_t)queue;
		vQueueDelete(q);
#endif
	}
	void El_DeleteSemaphore(El_Semaphore_t sem)
	{
#if EmbedXrpc_UsingOs == 1
		Semaphore* qtsem = static_cast<Semaphore*>(sem);
		qtsem->Reset();
		delete sem;
#else
		
#endif
	}
	void El_DeleteTimer(El_Timer_t timer)
	{
#if EmbedXrpc_UsingOs == 1
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer);
		win32timer->Stop();
		delete win32timer;
#else
		return;
#endif
	}
	void El_ThreadStart(El_Thread_t thread, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		std::thread* x = static_cast<std::thread*>(thread);
		x->detach();
#else
		return;
#endif
	}
	void  El_TimerStart(El_Timer_t timer, uint16_t interval, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer);
		win32timer->timerout = interval;
		win32timer->Start();
#else
		return;
#endif
	}
	void El_TimerReset(El_Timer_t timer, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer);
		win32timer->Stop();
#else
		return;
#endif
	}
	void El_TimerStop(El_Timer_t timer, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer);
		win32timer->Stop();
#else
		return;
#endif
	}

	

	Bool El_TakeMutex(El_Mutex_t mutex, uint32_t timeout, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		std::timed_mutex* m = static_cast<std::timed_mutex*>(mutex);
		std::chrono::milliseconds to(timeout);
		return m->try_lock_for(to);
#else
		return true;
#endif
	}
	Bool El_ReleaseMutex(El_Mutex_t mutex, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		std::timed_mutex* m = static_cast<std::timed_mutex*>(mutex);
		m->unlock();
		return true;
#else
		return true;
#endif
	}

	QueueState El_ReceiveQueue(El_Queue_t queue, void* item, uint32_t itemSize, uint32_t timeout, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
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
#else
		El_Assert(timeout == 0);
		QueueHandle_t q = (QueueHandle_t)queue;
		BaseType_t bt = xQueueReceive(q, item);
		return bt == pdPASS ? QueueState_OK : QueueState_Empty;
#endif
	}
	QueueState El_SendQueue(El_Queue_t queue, void* item, uint32_t itemSize,int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		NoGenericBlockingQueue* q = static_cast<NoGenericBlockingQueue*>(queue);
		q->Send(item);
		return QueueState_OK;
#else
		QueueHandle_t q = (QueueHandle_t)queue;
		BaseType_t bt = xQueueSendToBack(q, item);
		return bt == pdPASS ? QueueState_OK : QueueState_Full;
#endif
	}
	void El_ResetQueue(El_Queue_t queue, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		NoGenericBlockingQueue* q = static_cast<NoGenericBlockingQueue*>(queue);
		q->Reset();
#else
		QueueHandle_t q = (QueueHandle_t)queue;
		xQueueReset(q);
#endif
	}
	uint32_t El_QueueSpacesAvailable(El_Queue_t queue, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		return -1;
#else
		QueueHandle_t q = (QueueHandle_t)queue;
		return uxQueueSpacesAvailable(q);
#endif
	}
	int32_t allsize = 0;
	void* El_Malloc(uint32_t size)
	{
		auto x = malloc(size);
		allsize += size;
		printf("	memory malloc!allsize:%4d\n",allsize);
		return x;
	}
	void El_Free(void* ptr)
	{
		size_t sz= _msize(ptr);
		free(ptr);
		allsize -= sz;
		printf("	memory free!allsize:%d\n", allsize);
	}
	void El_Memcpy(void* d, const void* s, uint32_t size)
	{
		::memcpy(d, s, size);
	}
	void El_Memset(void* d, int v, uint32_t size)
	{
		::memset(d, v, size);
	}
}
uint32_t El_GetTick(int isIsr)
{
	return GetTickCount();
}