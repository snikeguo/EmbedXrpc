#include  "EmbedLibrary.h"
#if EmbedXrpc_UsingOs == 1
#include <thread>
#endif
#include "EmbedXrpcCommon.h"
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
		CRITICAL_SECTION* cs = new CRITICAL_SECTION;
		InitializeCriticalSection(cs);
		return  cs;
#else
		return nullptr;
#endif
	}
	El_Queue_t El_CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen)
	{
		return xQueueCreate(maxItemLen, queueItemSize);
	}
	El_Semaphore_t  El_CreateSemaphore(const char* SemaphoreName)
	{
#if EmbedXrpc_UsingOs == 1
		//这个函数会多次执行
		//由于代码设计使然，SemaphoreName 每次执行的值一样
		//HANDLE sem = CreateEventA(NULL, FALSE, FALSE, SemaphoreName);//有问题
		HANDLE sem = CreateEventA(NULL, FALSE, FALSE, NULL);//没问题
		return sem;
#else
		return nullptr;
#endif
	}
#if EmbedXrpc_UsingOs == 1
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
#endif
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
		CRITICAL_SECTION* cs = (CRITICAL_SECTION *)mutex;
		DeleteCriticalSection(cs);
		delete cs;
#else
		return;
#endif
	}
	void El_DeleteQueue(El_Queue_t queue)
	{
		QueueHandle_t q = (QueueHandle_t)queue;
		vQueueDelete(q);
	}
	void El_DeleteSemaphore(El_Semaphore_t sem)
	{
#if EmbedXrpc_UsingOs == 1
		HANDLE handle = (HANDLE)sem;
		CloseHandle(handle);
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

	

	bool El_TakeMutex(El_Mutex_t mutex, uint32_t timeout, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		CRITICAL_SECTION* cs = (CRITICAL_SECTION*)mutex;
		EnterCriticalSection(cs);
		return true;
#else
		return true;
#endif
	}
	bool El_ReleaseMutex(El_Mutex_t mutex, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		CRITICAL_SECTION* cs = (CRITICAL_SECTION*)mutex;
		LeaveCriticalSection(cs);
		return true;
#else
		return true;
#endif
	}
	QueueState El_TakeSemaphore(El_Semaphore_t sem, uint32_t timeout, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		HANDLE handle = (HANDLE)sem;
		
		DWORD r= WaitForSingleObject(sem, timeout);
		if (r == WAIT_OBJECT_0)
		{
			return QueueState::QueueState_OK;
		}
		else if (r == WAIT_TIMEOUT)
		{
			return QueueState::QueueState_Timeout;
		}
		else
		{
			El_Debug("[El_TakeSemaphore]:%d,", r);
			return QueueState::QueueState_Timeout;
		}
#else
		return QueueState::QueueState_OK;
#endif
	}
	void El_ReleaseSemaphore(El_Semaphore_t sem, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		HANDLE handle = (HANDLE)sem;
		SetEvent(handle);
#else

#endif
	}
	void El_ResetSemaphore(El_Semaphore_t sem, int isIsr)
	{
#if EmbedXrpc_UsingOs == 1
		HANDLE handle = (HANDLE)sem;
		ResetEvent(handle);
#else

#endif
	}

	QueueState El_ReceiveQueue(El_Queue_t queue, void* item, uint32_t itemSize, uint32_t timeout, int isIsr)
	{
		El_Assert(timeout == 0);
		QueueHandle_t q = (QueueHandle_t)queue;
		BaseType_t bt = xQueueReceive(q, item, timeout);
		return bt == pdPASS ? QueueState_OK : QueueState_Empty;
	}
	QueueState El_SendQueue(El_Queue_t queue, void* item, uint32_t itemSize,int isIsr)
	{
		QueueHandle_t q = (QueueHandle_t)queue;
		BaseType_t bt = xQueueSendToBack(q, item);
		return bt == pdPASS ? QueueState_OK : QueueState_Full;
	}
	void El_ResetQueue(El_Queue_t queue, int isIsr)
	{
		QueueHandle_t q = (QueueHandle_t)queue;
		xQueueReset(q);
	}
	uint32_t El_QueueSpacesAvailable(El_Queue_t queue, int isIsr)
	{
	    QueueHandle_t q = (QueueHandle_t)queue;
	    return uxQueueSpacesAvailable(q);
	}
	int32_t allsize = 0;
	void* El_Malloc(uint32_t size)
	{
#if EmbedXrpc_UsingOs == 1
		auto x = malloc(size);
		allsize += size;
		//printf("	memory malloc!allsize:%4d\n",allsize);
		return x;
#else
		auto x = pvPortMalloc(size);
		return x;
#endif
	}
	void El_Free(void* ptr)
	{
#if EmbedXrpc_UsingOs == 1
		size_t sz= _msize(ptr);
		free(ptr);
		allsize -= sz;
#else
		vPortFree(ptr);
#endif
		//printf("	memory free!allsize:%d\n", allsize);
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