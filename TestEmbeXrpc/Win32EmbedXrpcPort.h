#pragma once
#ifndef Win32EmbedXrpcPort_H
#define  Win32EmbedXrpcPort_H
#include "EmbedXrpc.Port.h"
class Win32EmbedXrpcPort :public IEmbeXrpcPort
{
public:
	EmbeXrpc_Thread_t CreateThread(const char* threadName,void(*Thread)(void*), void* Arg) ;
	//EmbeXrpc_Mutex_t CreateSemaphore(const char* semaphoreName) ;
	EmbeXrpc_Mutex_t CreateMutex(const char* mutexName) ;
	EmbeXrpc_Queue_t CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen) ;

	void ThreadStart(EmbeXrpc_Thread_t thread) ;

	//bool TakeSemaphore(EmbeXrpc_Semaphore_t sem, uint32_t timeout) ;
	//bool ReleaseSemaphore(EmbeXrpc_Semaphore_t sem) ;

	bool TakeMutex(EmbeXrpc_Mutex_t mutex, uint32_t timeout) ;
	bool ReleaseMutex(EmbeXrpc_Mutex_t mutex) ;

	QueueState ReceiveQueue(EmbeXrpc_Queue_t queue, void* item, uint32_t itemlen, uint32_t timeout) ;
	QueueState SendQueue(EmbeXrpc_Queue_t queue, void* item, uint32_t itemlen) ;

	void* Malloc(uint32_t size) ;
	void Free(void* ptr) ;
	void Memcpy(void* d, const void* s, uint32_t size) ;
};
#endif

