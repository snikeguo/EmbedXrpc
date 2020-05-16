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
EmbedXrpc_Semaphore_t  Win32EmbedXrpcPort::CreateSemaphore(const char* SemaphoreName)
{
	QSemaphore *sem = new QSemaphore();
	return sem;
}

EmbedXrpc_Timer_t Win32EmbedXrpcPort::CreateTimer(const char* timerName, uint32_t timeout, void* Arg, void (*timercb)(void* arg))
{
	MyTimer* timer = new MyTimer(timeout,Arg,timercb);
	return timer;
}
void Win32EmbedXrpcPort::DeleteThread(EmbedXrpc_Thread_t thread)
{
	auto qtThread = static_cast<QThread*>(thread);
	while (qtThread->isRunning() == true);
	delete qtThread;
}
void Win32EmbedXrpcPort::DeleteMutex(EmbedXrpc_Mutex_t mutex)
{
	auto qtMutex = static_cast<QMutex*>(mutex);
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
	QSemaphore* qtsem = static_cast<QSemaphore*>(sem);
	while (qtsem->tryAcquire() == true);
	delete sem;
}
void Win32EmbedXrpcPort::DeleteTimer(EmbedXrpc_Timer_t timer) 
{
	auto myTimer = static_cast<MyTimer*>(timer);
	myTimer->Stop();
	delete myTimer;
}
void Win32EmbedXrpcPort::ThreadStart(EmbedXrpc_Thread_t thread)
{
	QThread* x = static_cast<QThread*>(thread);
	x->start();
}
void  Win32EmbedXrpcPort::TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
{
	MyTimer* t = static_cast<MyTimer*>(timer);
	t->Start(interval);//在其他线程操作Qt定时器 会报警告  没啥好办法
	
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

bool Win32EmbedXrpcPort::TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
{
	QSemaphore* qtsem = static_cast<QSemaphore*>(sem);
	return qtsem->tryAcquire(1, timeout);
}
void Win32EmbedXrpcPort::ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
{
	QSemaphore* qtsem = static_cast<QSemaphore*>(sem);
	qtsem->release(1);
}
void Win32EmbedXrpcPort::ResetSemaphore(EmbedXrpc_Semaphore_t sem)
{
	QSemaphore* qtsem = static_cast<QSemaphore*>(sem);
	while (qtsem->tryAcquire(1) == true);
}
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
	printf("malloc ptr:0x%8x,size:%4d\n", x, size);
	return x;
}
void Win32EmbedXrpcPort::Free(void* ptr)
{
	free(ptr);
	printf("free ptr:0x%8x\n", ptr);
}
void Win32EmbedXrpcPort::Memcpy(void* d, const void* s, uint32_t size)
{
	::memcpy(d, s, size);
}