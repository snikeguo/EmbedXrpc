#include "Win32EmbedXrpcPort.h"

EmbeXrpc_Thread_t Win32EmbedXrpcPort::CreateThread(const char* threadName, void(*Thread)(void*))
{
	return  nullptr;
}
EmbeXrpc_Mutex_t Win32EmbedXrpcPort::CreateSemaphore(const char* semaphoreName)
{
	return  nullptr;
}
EmbeXrpc_Mutex_t Win32EmbedXrpcPort::CreateMutex(const char* mutexName)
{
	return  nullptr;
}
EmbeXrpc_Queue_t Win32EmbedXrpcPort::CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen)
{
	return nullptr;
}

void Win32EmbedXrpcPort::ThreadStart(EmbeXrpc_Thread_t thread)
{

}

bool Win32EmbedXrpcPort::TakeSemaphore(EmbeXrpc_Semaphore_t sem, uint32_t timeout)
{
	return false;
}
bool Win32EmbedXrpcPort::ReleaseSemaphore(EmbeXrpc_Semaphore_t sem)
{
	return false;
}

bool Win32EmbedXrpcPort::TakeMutex(EmbeXrpc_Semaphore_t sem, uint32_t timeout)
{
	return false;
}
bool Win32EmbedXrpcPort::ReleaseMutex(EmbeXrpc_Semaphore_t sem)
{
	return false;
}

QueueState Win32EmbedXrpcPort::ReceiveQueue(EmbeXrpc_Semaphore_t sem, void* item, uint32_t itemlen, uint32_t timeout)
{
	return QueueState_Empty;
}
QueueState Win32EmbedXrpcPort::SendQueue(EmbeXrpc_Semaphore_t sem, void* item, uint32_t itemlen)
{
	return QueueState_Full;
}

void* Win32EmbedXrpcPort::Malloc(uint32_t size)
{
	return nullptr;
}
void Win32EmbedXrpcPort::Free(void* ptr)
{

}
void Win32EmbedXrpcPort::Memcpy(void* d, const void* s, uint32_t size)
{

}