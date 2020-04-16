#pragma once
#ifndef Win32EmbedXrpcPort_H
#define  Win32EmbedXrpcPort_H
#include "EmbedXrpc.Port.h"
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QTimer>
#include "BlockQueue.h"
class MyTimer : public QObject
{
	Q_OBJECT
public:
	MyTimer(uint32_t timeout, void* arg, void (*cb)(void* par), QObject* parent = nullptr):Timer(parent),Arg(arg),Cb(cb),Timeout(timeout)
	{
		connect(&Timer, &QTimer::timeout, this, [=] {Cb(Arg);});
	}
	void Start(uint16_t interval)
	{
		Timer.start(interval);
	}
	void Stop()
	{
		Timer.stop();
	}
	void Reset()
	{
		
	}
private:
	QTimer Timer;
	void *Arg;
	void (*Cb)(void* par);
	uint32_t Timeout;
};
class Win32EmbedXrpcPort :public QObject, public  IEmbeXrpcPort
{
	Q_OBJECT
public:
	EmbedXrpc_Thread_t CreateThread(const char* threadName,void(*Thread)(void*), void* Arg) ;
	//EmbedXrpc_Mutex_t CreateSemaphore(const char* semaphoreName) ;
	EmbedXrpc_Mutex_t CreateMutex(const char* mutexName) ;
	EmbedXrpc_Queue_t CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen) ;
	EmbedXrpc_Timer_t CreateTimer(const char* timerName, uint32_t timeout, void* Arg, void (*timercb)(void* arg));
	void ThreadStart(EmbedXrpc_Thread_t thread) ;
	void TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval);
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

