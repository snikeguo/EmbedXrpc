#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"
#include "Win32EmbedXrpcPort.h"
#include <thread>
#include <mutex>

#include "Semaphore.h"



EmbedXrpc_Thread_t Win32EmbedXrpcPort::CreateThread(const char* threadName, uint8_t priority, void(*Thread)(void*), void* Arg)
{
	std::thread * ServiceThread = new std::thread(Thread, Arg);
	return  ServiceThread;
}

EmbedXrpc_Mutex_t Win32EmbedXrpcPort::CreateMutex(const char* mutexName)
{
	//QMutex* mutex = new QMutex();
	std::timed_mutex* mutex = new std::timed_mutex();
	return  mutex;
}
EmbedXrpc_Queue_t Win32EmbedXrpcPort::CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen)
{
	//这里创建队列，由于我只实现了C++泛型的队列，而底层RTOS一般要求提供的是queueItemSize，所以这里硬编码直接创建EmbeXrpcRawData;
	BlockingQueue<EmbeXrpcRawData> *q = new BlockingQueue<EmbeXrpcRawData>();
	return q;
}
EmbedXrpc_Semaphore_t  Win32EmbedXrpcPort::CreateSemaphore(const char* SemaphoreName)
{
	Semaphore*sem = new Semaphore(std::string(SemaphoreName));
	return sem;
}

EmbedXrpc_Timer_t Win32EmbedXrpcPort::CreateTimer(const char* timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg)
{
	return nullptr;
}
void Win32EmbedXrpcPort::DeleteThread(EmbedXrpc_Thread_t thread)
{
	//auto qtThread = static_cast<QThread*>(thread);
	auto qtThread = static_cast<std::thread*>(thread);
	delete qtThread;
}
void Win32EmbedXrpcPort::DeleteMutex(EmbedXrpc_Mutex_t mutex)
{
	auto qtMutex = static_cast<std::mutex*>(mutex);
	delete qtMutex;
}
void Win32EmbedXrpcPort::DeleteQueue(EmbedXrpc_Queue_t queue)
{
	auto qtQueue = static_cast<BlockingQueue<EmbeXrpcRawData>*>(queue);
	qtQueue->Reset();
	delete qtQueue;
}
void Win32EmbedXrpcPort::DeleteSemaphore(EmbedXrpc_Semaphore_t sem)
{
	Semaphore* qtsem = static_cast<Semaphore*>(sem);
	while (qtsem->wait()== true);
	delete sem;
}
void Win32EmbedXrpcPort::DeleteTimer(EmbedXrpc_Timer_t timer) 
{
	
}
void Win32EmbedXrpcPort::ThreadStart(EmbedXrpc_Thread_t thread)
{
	std::thread* x = static_cast<std::thread*>(thread);
	x->detach();
}
void  Win32EmbedXrpcPort::TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
{
		
}
void Win32EmbedXrpcPort::TimerReset(EmbedXrpc_Timer_t timer)
{
	
}
void Win32EmbedXrpcPort::TimerStop(EmbedXrpc_Timer_t timer)
{

}

bool Win32EmbedXrpcPort::TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
{
	Semaphore* qtsem = static_cast<Semaphore*>(sem);
	auto r= qtsem->wait(timeout);
	//assert(r == true);
	return r;

}
void Win32EmbedXrpcPort::ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
{
	Semaphore* qtsem = static_cast<Semaphore*>(sem);
	qtsem->release();
}
void Win32EmbedXrpcPort::ResetSemaphore(EmbedXrpc_Semaphore_t sem)
{
	Semaphore* qtsem = static_cast<Semaphore*>(sem);
	while (qtsem->wait() == true);
}

bool Win32EmbedXrpcPort::TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout)
{
	std::timed_mutex*m = static_cast<std::timed_mutex*>(mutex);
	std::chrono::milliseconds to(timeout);
	return m->try_lock_for(to);
}
bool Win32EmbedXrpcPort::ReleaseMutex(EmbedXrpc_Mutex_t mutex)
{
	std::timed_mutex* m = static_cast<std::timed_mutex*>(mutex);
	m->unlock();
	return true;
}

QueueState Win32EmbedXrpcPort::ReceiveQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemlen, uint32_t timeout)
{
	BlockingQueue<EmbeXrpcRawData>* q= static_cast<BlockingQueue<EmbeXrpcRawData>*>(queue);
	auto r=q->Receive(*(EmbeXrpcRawData *)item, timeout);
	if (r== QueueStatus::Ok)
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
	q->Send((*data));
	return QueueState_OK;
}
void Win32EmbedXrpcPort::ResetQueue(EmbedXrpc_Queue_t queue)
{
	BlockingQueue<EmbeXrpcRawData>* q = static_cast<BlockingQueue<EmbeXrpcRawData>*>(queue);
	q->Reset();
}
void* Win32EmbedXrpcPort::Malloc(uint32_t size)
{
	auto x=malloc(size);
	//printf("malloc ptr:0x%8x,size:%4d\n", x, size);
	return x;
}
void Win32EmbedXrpcPort::Free(void* ptr)
{
	free(ptr);
	//printf("free ptr:0x%8x\n", ptr);
}
void Win32EmbedXrpcPort::Memcpy(void* d, const void* s, uint32_t size)
{
	::memcpy(d, s, size);
}