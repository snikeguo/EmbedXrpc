#pragma once
#ifndef Win32EmbedXrpcPort_H
#define  Win32EmbedXrpcPort_H
#include "EmbedXrpc.Port.h"
class Win32EmbedXrpcPort :public IEmbeXrpcPort
{
public:
	EmbeXrpc_Thread_t CreateThread(const char* threadName,void(*Thread)(void*)) ;
	EmbeXrpc_Mutex_t CreateSemaphore(const char* semaphoreName) ;
	EmbeXrpc_Mutex_t CreateMutex(const char* mutexName) ;
	EmbeXrpc_Queue_t CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen) ;

	void ThreadStart(EmbeXrpc_Thread_t thread) ;

	bool TakeSemaphore(EmbeXrpc_Semaphore_t sem, uint32_t timeout) ;
	bool ReleaseSemaphore(EmbeXrpc_Semaphore_t sem) ;

	bool TakeMutex(EmbeXrpc_Semaphore_t sem, uint32_t timeout) ;
	bool ReleaseMutex(EmbeXrpc_Semaphore_t sem) ;

	QueueState ReceiveQueue(EmbeXrpc_Semaphore_t sem, void* item, uint32_t itemlen, uint32_t timeout) ;
	QueueState SendQueue(EmbeXrpc_Semaphore_t sem, void* item, uint32_t itemlen) ;

	void* Malloc(uint32_t size) ;
	void Free(void* ptr) ;
	void Memcpy(void* d, const void* s, uint32_t size) ;
};
#endif

