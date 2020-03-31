#pragma once
#ifndef EmbedXrpc_Port_H
#define EmbedXrpc_Port_H
#include <cstdint>

typedef void* EmbeXrpc_Semaphore_t;
typedef void* EmbeXrpc_Mutex_t;
typedef void* EmbeXrpc_Thread_t;
typedef void* EmbeXrpc_Queue_t;
#define WAIT_FOREVER	0xFFFFFFFF

#define  XrpcDebug	//printf

enum QueueState
{
	QueueState_Empty,
	QueueState_Full,
	QueueState_OK,
};
class IEmbeXrpcPort
{
public:
	virtual EmbeXrpc_Thread_t CreateThread(const char* threadName,void (*Thread)(void *),void *Arg)=0;
	//virtual EmbeXrpc_Mutex_t CreateSemaphore(const char* semaphoreName)=0;
	virtual EmbeXrpc_Mutex_t CreateMutex(const char* mutexName)=0;
	virtual EmbeXrpc_Queue_t CreateQueue(const char* queueName,uint32_t queueItemSize,uint32_t maxItemLen) = 0;

	virtual void ThreadStart(EmbeXrpc_Thread_t thread) = 0;

	//virtual bool TakeSemaphore(EmbeXrpc_Semaphore_t sem, uint32_t timeout) = 0;
	//virtual bool ReleaseSemaphore(EmbeXrpc_Semaphore_t sem) = 0;

	virtual bool TakeMutex(EmbeXrpc_Mutex_t mutex, uint32_t timeout) = 0;
	virtual bool ReleaseMutex(EmbeXrpc_Mutex_t mutex) = 0;

	//virtual bool TakeMutex(EmbeXrpc_Semaphore_t sem, uint32_t timeout) = 0;
	//virtual bool ReleaseMutex(EmbeXrpc_Semaphore_t sem) = 0;

	virtual QueueState ReceiveQueue(EmbeXrpc_Queue_t queue, void * item,uint32_t itemlen,uint32_t timeout) = 0;
	virtual QueueState SendQueue(EmbeXrpc_Queue_t queue, void* item, uint32_t itemlen) = 0;

	virtual void* Malloc(uint32_t size) = 0;
	virtual void Free(void* ptr) = 0;
	virtual void Memcpy(void* d, const void* s, uint32_t size)=0;

};

#endif
