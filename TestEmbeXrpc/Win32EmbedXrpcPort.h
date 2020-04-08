#pragma once
#ifndef Win32EmbedXrpcPort_H
#define  Win32EmbedXrpcPort_H
#include "EmbedXrpc.Port.h"
class Win32EmbedXrpcPort :public IEmbeXrpcPort
{
public:
	EmbedXrpc_Thread_t CreateThread(const char* threadName,void(*Thread)(void*), void* Arg) ;
	//EmbedXrpc_Mutex_t CreateSemaphore(const char* semaphoreName) ;
	EmbedXrpc_Mutex_t CreateMutex(const char* mutexName) ;
	EmbedXrpc_Queue_t CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen) ;
	EmbedXrpc_Timer_t CreateTimer(const char* timerName, uint32_t timeout, void* Arg, void (*timercb)(void* arg));
	void ThreadStart(EmbedXrpc_Thread_t thread) ;
	void TimerStart(EmbedXrpc_Timer_t timer);
	void TimerReset(EmbedXrpc_Timer_t timer);
	void TimerStop(EmbedXrpc_Timer_t timer);
	//bool TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout) ;
	//bool ReleaseSemaphore(EmbedXrpc_Semaphore_t sem) ;

	bool TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout) ;
	bool ReleaseMutex(EmbedXrpc_Mutex_t mutex) ;

	QueueState ReceiveQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemlen, uint32_t timeout) ;
	QueueState SendQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemlen) ;
	void ResetQueue(EmbedXrpc_Queue_t queue) ;

	void* Malloc(uint32_t size) ;
	void Free(void* ptr) ;
	void Memcpy(void* d, const void* s, uint32_t size) ;
};
#endif

