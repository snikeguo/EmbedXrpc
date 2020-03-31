#include "Win32EmbedXrpcPort.h"
#include <thread>  
#include <mutex>
#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"
#include "BlockingQueue.h"
#include <iostream>
EmbeXrpc_Thread_t Win32EmbedXrpcPort::CreateThread(const char* threadName, void(*Thread)(void*), void* Arg)
{
	std::thread *x = new std::thread(Thread,Arg);
	return  x;
}

EmbeXrpc_Mutex_t Win32EmbedXrpcPort::CreateMutex(const char* mutexName)
{
	std::mutex* mutex = new std::mutex();
	return  mutex;
}
EmbeXrpc_Queue_t Win32EmbedXrpcPort::CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen)
{
	//这里创建队列，由于我只实现了C++泛型的队列，而底层RTOS一般要求提供的是queueItemSize，所以这里硬编码直接创建EmbeXrpcRawData;
	BlockingQueue<EmbeXrpcRawData> *queue = new BlockingQueue<EmbeXrpcRawData>();

	return queue;
}

void Win32EmbedXrpcPort::ThreadStart(EmbeXrpc_Thread_t thread)
{
	std::thread* x = static_cast<std::thread*>(thread);
	//x->join();

}
#if 0
EmbeXrpc_Mutex_t Win32EmbedXrpcPort::CreateSemaphore(const char* semaphoreName)
{
	return  nullptr;
}
bool Win32EmbedXrpcPort::TakeSemaphore(EmbeXrpc_Semaphore_t sem, uint32_t timeout)
{
	return false;
}
bool Win32EmbedXrpcPort::ReleaseSemaphore(EmbeXrpc_Semaphore_t sem)
{
	return false;
}
#endif
bool Win32EmbedXrpcPort::TakeMutex(EmbeXrpc_Mutex_t mutex, uint32_t timeout)
{
	return true;
}
bool Win32EmbedXrpcPort::ReleaseMutex(EmbeXrpc_Mutex_t mutex)
{
	return true;
}

QueueState Win32EmbedXrpcPort::ReceiveQueue(EmbeXrpc_Queue_t queue, void* item, uint32_t itemlen, uint32_t timeout)
{
	BlockingQueue<EmbeXrpcRawData>* q= static_cast<BlockingQueue<EmbeXrpcRawData>*>(queue);
	auto r=q->take(timeout, (EmbeXrpcRawData *)item);
	if (r== true)
	{
		return QueueState_OK;
	}
	else
	{
		return QueueState_Empty;
	}
	
}
QueueState Win32EmbedXrpcPort::SendQueue(EmbeXrpc_Queue_t queue, void* item, uint32_t itemlen)
{
	BlockingQueue<EmbeXrpcRawData>* q = static_cast<BlockingQueue<EmbeXrpcRawData>*>(queue);
	EmbeXrpcRawData* data = static_cast<EmbeXrpcRawData*> (item);
	q->put(*data);
	return QueueState_OK;
}

void* Win32EmbedXrpcPort::Malloc(uint32_t size)
{
	auto x=malloc(size);
	std::cout << "malloc ptr:	"<<std::hex<<x<<"	size:	" << size << std::endl;
	return x;
}
void Win32EmbedXrpcPort::Free(void* ptr)
{
	std::cout << "free ptr:	" << std::hex << ptr  << std::endl;
}
void Win32EmbedXrpcPort::Memcpy(void* d, const void* s, uint32_t size)
{
	::memcpy(d, s, size);
}