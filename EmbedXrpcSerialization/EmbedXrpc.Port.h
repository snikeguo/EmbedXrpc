#pragma once
#ifndef EmbedXrpc_Port_H
#define EmbedXrpc_Port_H
#include "EmbedSerialization.Port.h"
typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Mutex_t;
typedef void* EmbedXrpc_Thread_t;
typedef void* EmbedXrpc_Queue_t;
typedef void* EmbedXrpc_Timer_t;
#define EmbedXrpc_WAIT_FOREVER	0xFFFFFFFF
#ifdef WIN32
#include <cstdio>
#define  XrpcDebug	printf
#else
#define  XrpcDebug	rt_kprintf
#endif
enum QueueState
{
	QueueState_Empty,
	QueueState_Full,
	QueueState_OK,
	QueueState_Timeout,
};
class IEmbeXrpcPort
{
public:
	virtual EmbedXrpc_Thread_t CreateThread(const char* threadName,void (*Thread)(void *),void *Arg)=0;
	//virtual EmbedXrpc_Mutex_t CreateSemaphore(const char* semaphoreName)=0;
	virtual EmbedXrpc_Mutex_t CreateMutex(const char* mutexName)=0;
	virtual EmbedXrpc_Queue_t CreateQueue(const char* queueName,uint32_t queueItemSize,uint32_t maxItemLen) = 0;
	virtual EmbedXrpc_Timer_t CreateTimer(const char* timerName, uint32_t timeout,void *Arg,void (*timercb)(void *arg))=0;

	virtual void ThreadStart(EmbedXrpc_Thread_t thread) = 0;

	virtual void TimerStart(EmbedXrpc_Timer_t timer,uint16_t interval) = 0;
	virtual void TimerReset(EmbedXrpc_Timer_t timer) = 0;
	virtual void TimerStop(EmbedXrpc_Timer_t timer) = 0;
	//virtual bool TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout) = 0;
	//virtual bool ReleaseSemaphore(EmbedXrpc_Semaphore_t sem) = 0;

	virtual bool TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout) = 0;
	virtual bool ReleaseMutex(EmbedXrpc_Mutex_t mutex) = 0;

	//virtual bool TakeMutex(EmbedXrpc_Semaphore_t sem, uint32_t timeout) = 0;
	//virtual bool ReleaseMutex(EmbedXrpc_Semaphore_t sem) = 0;

	virtual QueueState ReceiveQueue(EmbedXrpc_Queue_t queue, void * item,uint32_t itemlen,uint32_t timeout) = 0;
	virtual QueueState SendQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemlen) = 0;
	virtual void ResetQueue(EmbedXrpc_Queue_t queue) = 0;

	virtual void* Malloc(uint32_t size) = 0;
	virtual void Free(void* ptr) = 0;
	virtual void Memcpy(void* d, const void* s, uint32_t size)=0;

};

#endif
