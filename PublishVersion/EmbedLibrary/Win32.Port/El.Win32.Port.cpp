#include  "../EmbedLibrary.h"

#include <thread>
#include <mutex>

#include "BlockQueue.h"
#include "windows.h"
using Semaphore = BlockingQueue<int>;
extern "C"
{

	osThreadId_t osThreadNew(osThreadFunc_t 	func,void* argument,const osThreadAttr_t* attr)
	{
		std::thread* ServiceThread = new std::thread(func, argument);
		ServiceThread->detach();
		return  ServiceThread;
	}

	osMutexId_t osMutexNew(const osMutexAttr_t* attr)
	{
		//QMutex* mutex = new QMutex();
		std::timed_mutex* mutex = new std::timed_mutex();
		return  mutex;
	}
	osMessageQueueId_t 	osMessageQueueNew(uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t* attr)
	{
		NoGenericBlockingQueue* q = new NoGenericBlockingQueue(msg_size);
		return q;
	}
	osSemaphoreId_t 	osSemaphoreNew(uint32_t max_count, uint32_t initial_count, const osSemaphoreAttr_t* attr)
	{
		Semaphore* sem = new Semaphore();
		return sem;
	}
	class Win32Timer //¼òµ¥µÄtimer
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
	osTimerId_t osTimerNew(osTimerFunc_t 	func,osTimerType_t 	type,void* argument,const osTimerAttr_t* attr)
	{
		Win32Timer* timer = new Win32Timer(attr->name, 0, func, argument);
		return timer;
	}
	osStatus_t osThreadTerminate(osThreadId_t 	thread_id)
	{
		//auto qtThread = static_cast<QThread*>(thread);
		auto qtThread = static_cast<std::thread*>(thread_id);
		delete qtThread;
		return osOK;
	}

	osStatus_t osMutexDelete(osMutexId_t 	mutex_id)
	{
		auto qtMutex = static_cast<std::mutex*>(mutex_id);
		delete qtMutex;
		return osOK;
	}
	osStatus_t osMessageQueueDelete(osMessageQueueId_t 	mq_id)
	{
		auto qtQueue = static_cast<NoGenericBlockingQueue*>(mq_id);
		qtQueue->Reset();
		delete qtQueue;
		return osOK;
	}
	osStatus_t osSemaphoreDelete(osSemaphoreId_t 	semaphore_id)
	{
		Semaphore* qtsem = static_cast<Semaphore*>(semaphore_id);
		qtsem->Reset();
		delete qtsem;
		return osOK;
	}
	osStatus_t osTimerDelete(osTimerId_t 	timer_id)
	{
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer_id);
		win32timer->Stop();
		delete win32timer;
		return osOK;
	}
	osStatus_t osThreadResume(osThreadId_t 	thread_id)
	{
		std::thread* x = static_cast<std::thread*>(thread_id);
		x->detach();
		return osOK;
	}
	osStatus_t 	osTimerStart(osTimerId_t timer_id, uint32_t ticks)
	{
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer_id);
		win32timer->timerout = ticks;
		win32timer->Start();
		return osOK;
	}
	
	osStatus_t 	osTimerStop(osTimerId_t timer_id)
	{
		Win32Timer* win32timer = static_cast<Win32Timer*>(timer_id);
		win32timer->Stop();
		return osOK;
	}

	osStatus_t 	osSemaphoreAcquire(osSemaphoreId_t semaphore_id, uint32_t timeout)
	{
		Semaphore* qtsem = static_cast<Semaphore*>(semaphore_id);
		int recItem = 0;
		return qtsem->Receive(recItem, timeout)==Ok?osOK:osError;
		//assert(r == true);

	}
	osStatus_t 	osSemaphoreRelease(osSemaphoreId_t semaphore_id)
	{
		Semaphore* qtsem = static_cast<Semaphore*>(semaphore_id);
		int recItem = 0;
		qtsem->Send(recItem);
		return osOK;
	}
	

	osStatus_t 	osMutexAcquire(osMutexId_t mutex_id, uint32_t timeout)
	{
		std::timed_mutex* m = static_cast<std::timed_mutex*>(mutex_id);
		std::chrono::milliseconds to(timeout);
		return m->try_lock_for(to)==true?osOK:osError;
	}
	osStatus_t 	osMutexRelease(osMutexId_t mutex_id)
	{
		std::timed_mutex* m = static_cast<std::timed_mutex*>(mutex_id);
		m->unlock();
		return osOK;
	}

	osStatus_t 	osMessageQueueGet(osMessageQueueId_t mq_id, void* msg_ptr, uint8_t* msg_prio, uint32_t timeout)
	{
		NoGenericBlockingQueue* q = static_cast<NoGenericBlockingQueue*>(mq_id);
		auto r = q->Receive(msg_ptr, timeout);
		if (r == QueueStatus::Ok)
		{
			return osOK;
		}
		else
		{
			return osErrorTimeout;
		}

	}
	osStatus_t 	osMessageQueuePut(osMessageQueueId_t mq_id, const void* msg_ptr, uint8_t msg_prio, uint32_t timeout)
	{
		NoGenericBlockingQueue* q = static_cast<NoGenericBlockingQueue*>(mq_id);
		q->Send((void *)msg_ptr);
		return osOK;
	}
	osStatus_t 	osMessageQueueReset(osMessageQueueId_t mq_id)
	{
		NoGenericBlockingQueue* q = static_cast<NoGenericBlockingQueue*>(mq_id);
		q->Reset();
		return osOK;
	}
	uint32_t osMessageQueueGetSpace(osMessageQueueId_t mq_id)
	{
		return -1;
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