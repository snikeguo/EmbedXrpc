#include "EmbedSerialization.h"
//#include "EmbedXrpcCommon.h"
#include <thread>
#include <mutex>

#include "Semaphore.h"
#include "BlockQueue.h"


EmbedXrpc_Thread_t EmbedXrpc_CreateThread(const char* threadName, uint8_t priority, void(*Thread)(void*), void* Arg)
{
	std::thread * ServiceThread = new std::thread(Thread, Arg);
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
	NoGenericBlockingQueue *q = new NoGenericBlockingQueue(queueItemSize);
	return q;
}
EmbedXrpc_Semaphore_t  EmbedXrpc_CreateSemaphore(const char* SemaphoreName)
{
	Semaphore*sem = new Semaphore(std::string(SemaphoreName));
	return sem;
}

EmbedXrpc_Timer_t EmbedXrpc_CreateTimer(const char* timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg)
{
	return nullptr;
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
	while (qtsem->wait()== true);
	delete sem;
}
void EmbedXrpc_DeleteTimer(EmbedXrpc_Timer_t timer) 
{
	
}
void EmbedXrpc_ThreadStart(EmbedXrpc_Thread_t thread)
{
	std::thread* x = static_cast<std::thread*>(thread);
	x->detach();
}
void  EmbedXrpc_TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
{
		
}
void EmbedXrpc_TimerReset(EmbedXrpc_Timer_t timer)
{
	
}
void EmbedXrpc_TimerStop(EmbedXrpc_Timer_t timer)
{

}

bool EmbedXrpc_TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
{
	Semaphore* qtsem = static_cast<Semaphore*>(sem);
	auto r= qtsem->wait(timeout);
	//assert(r == true);
	return r;

}
void EmbedXrpc_ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
{
	Semaphore* qtsem = static_cast<Semaphore*>(sem);
	qtsem->release();
}
void EmbedXrpc_ResetSemaphore(EmbedXrpc_Semaphore_t sem)
{
	Semaphore* qtsem = static_cast<Semaphore*>(sem);
	while (qtsem->wait() == true);
}

bool EmbedXrpc_TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout)
{
	std::timed_mutex*m = static_cast<std::timed_mutex*>(mutex);
	std::chrono::milliseconds to(timeout);
	return m->try_lock_for(to);
}
bool EmbedXrpc_ReleaseMutex(EmbedXrpc_Mutex_t mutex)
{
	std::timed_mutex* m = static_cast<std::timed_mutex*>(mutex);
	m->unlock();
	return true;
}

QueueState EmbedXrpc_ReceiveQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemSize, uint32_t timeout)
{
	NoGenericBlockingQueue* q= static_cast<NoGenericBlockingQueue*>(queue);
	auto r=q->Receive(item, timeout);
	if (r== QueueStatus::Ok)
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
void* Malloc(uint32_t size)
{
	auto x=malloc(size);
	printf("malloc ptr:0x%8x,size:%4d\n", x, size);
	return x;
}
void Free(void* ptr)
{
	free(ptr);
	printf("free ptr:0x%8x\n", ptr);
}
void Memcpy(void* d, const void* s, uint32_t size)
{
	::memcpy(d, s, size);
}