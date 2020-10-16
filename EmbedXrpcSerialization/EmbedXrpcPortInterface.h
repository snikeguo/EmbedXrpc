#ifndef EmbedXrpc_PortInterface_H
#define EmbedXrpc_PortInterface_H
#include "EmbedSerialization.Port.h"
#include "EmbedXrpc.Port.h"


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
	virtual EmbedXrpc_Thread_t CreateThread(const char* threadName,uint8_t priority,void (*Thread)(void *),void *Arg)=0;
	//virtual EmbedXrpc_Mutex_t CreateSemaphore(const char* semaphoreName)=0;
	virtual EmbedXrpc_Mutex_t CreateMutex(const char* mutexName)=0;
	virtual EmbedXrpc_Queue_t CreateQueue(const char* queueName,uint32_t queueItemSize,uint32_t maxItemLen) = 0;
	virtual EmbedXrpc_Semaphore_t CreateSemaphore(const char* SemaphoreName) = 0;
	virtual EmbedXrpc_Timer_t CreateTimer(const char* timerName, uint32_t timeout, void (*timercb)(void* arg),void *Arg)=0;

	virtual void DeleteThread(EmbedXrpc_Thread_t thread) = 0;
	virtual void DeleteMutex(EmbedXrpc_Mutex_t) = 0;
	virtual void DeleteQueue(EmbedXrpc_Queue_t) = 0;
	virtual void DeleteSemaphore(EmbedXrpc_Semaphore_t) = 0;
	virtual void DeleteTimer(EmbedXrpc_Timer_t) = 0;

	virtual void ThreadStart(EmbedXrpc_Thread_t thread) = 0;

	virtual void TimerStart(EmbedXrpc_Timer_t timer,uint16_t interval) = 0;
	virtual void TimerReset(EmbedXrpc_Timer_t timer) = 0;
	virtual void TimerStop(EmbedXrpc_Timer_t timer) = 0;
	virtual bool TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout) = 0;
	virtual void ReleaseSemaphore(EmbedXrpc_Semaphore_t sem) = 0;
	virtual void ResetSemaphore(EmbedXrpc_Semaphore_t sem) = 0;
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
