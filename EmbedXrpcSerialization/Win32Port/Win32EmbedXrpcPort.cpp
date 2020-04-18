#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"
#include "Win32EmbedXrpcPort.h"


//这是一个demo 所以不用考虑内存的问题，直接new出来不用管即可。 因为一般地	Server是单片机，创建后一直在跑。
EmbedXrpc_Thread_t Win32EmbedXrpcPort::CreateThread(const char* threadName, uint8_t priority, void(*Thread)(void*), void* Arg)
{
	auto ServiceThread = QThread::create(Thread, Arg);
	return  ServiceThread;
}

EmbedXrpc_Mutex_t Win32EmbedXrpcPort::CreateMutex(const char* mutexName)
{
	QMutex* mutex = new QMutex();
	return  mutex;
}
EmbedXrpc_Queue_t Win32EmbedXrpcPort::CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen)
{
	//这里创建队列，由于我只实现了C++泛型的队列，而底层RTOS一般要求提供的是queueItemSize，所以这里硬编码直接创建EmbeXrpcRawData;
	BlockingQueue<EmbeXrpcRawData> *queue = new BlockingQueue<EmbeXrpcRawData>();
	return queue;
}


EmbedXrpc_Timer_t Win32EmbedXrpcPort::CreateTimer(const char* timerName, uint32_t timeout, void* Arg, void (*timercb)(void* arg))
{
	MyTimer* timer = new MyTimer(timeout,Arg,timercb);
	return timer;
}
void Win32EmbedXrpcPort::ThreadStart(EmbedXrpc_Thread_t thread)
{
	QThread* x = static_cast<QThread*>(thread);
	x->start();
}
void  Win32EmbedXrpcPort::TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
{
	MyTimer* t = static_cast<MyTimer*>(timer);
	t->Start(interval);
	
}
void Win32EmbedXrpcPort::TimerReset(EmbedXrpc_Timer_t timer)
{
	MyTimer* t = static_cast<MyTimer*>(timer);
	t->Reset();
}
void Win32EmbedXrpcPort::TimerStop(EmbedXrpc_Timer_t timer)
{
	MyTimer* t = static_cast<MyTimer*>(timer);
	t->Stop();
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
	QMutex*m = static_cast<QMutex*>(mutex);
	return m->tryLock(timeout);
}
bool Win32EmbedXrpcPort::ReleaseMutex(EmbedXrpc_Mutex_t mutex)
{
	QMutex* m = static_cast<QMutex*>(mutex);
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