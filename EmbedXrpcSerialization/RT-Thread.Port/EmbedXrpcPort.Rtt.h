#ifndef EmbedXrpcPort_RTT_H
#define EmbedXrpcPort_RTT_H
#include "EmbedXrpc.Port.h"

class RttEmbedXrpcPort :public  IEmbeXrpcPort
{
public:
	EmbedXrpc_Thread_t CreateThread(const char* threadName,uint8_t priority,void(*Thread)(void*), void* Arg) ;
	//EmbedXrpc_Mutex_t CreateSemaphore(const char* semaphoreName) ;
	EmbedXrpc_Mutex_t CreateMutex(const char* mutexName) ;
	EmbedXrpc_Queue_t CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen) ;
	EmbedXrpc_Semaphore_t CreateSemaphore(const char* SemaphoreName);
	EmbedXrpc_Timer_t CreateTimer(const char* timerName, uint32_t timeout, void* Arg, void (*timercb)(void* arg));
	void DeleteThread(EmbedXrpc_Thread_t thread);
	void DeleteMutex(EmbedXrpc_Mutex_t mutex);
	void DeleteQueue(EmbedXrpc_Queue_t queue);
	void DeleteSemaphore(EmbedXrpc_Semaphore_t sem);
	void DeleteTimer(EmbedXrpc_Timer_t timer);
	void ThreadStart(EmbedXrpc_Thread_t thread) ;
	void TimerStart(EmbedXrpc_Timer_t timer,uint16_t interval);
	void TimerReset(EmbedXrpc_Timer_t timer);
	void TimerStop(EmbedXrpc_Timer_t timer);
	bool TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout) ;
	void ReleaseSemaphore(EmbedXrpc_Semaphore_t sem) ;
	void ResetSemaphore(EmbedXrpc_Semaphore_t sem);

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