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
struct ReceiveItemInfo
{
    uint32_t DataLen ;
    uint32_t CheckSum ;
    uint16_t Sid ;
    uint16_t TargetTimeout ;
#if EmbedXrpc_UseRingBufferWhenReceiving==0
    uint8_t* Data;
#endif
    UserDataOfTransportLayer_t UserDataOfTransportLayer;
};

EmbedXrpc_Thread_t EmbedXrpc_CreateThread(const char* threadName, uint8_t priority, void (*Thread)(void*), void* Arg) ;
EmbedXrpc_Mutex_t EmbedXrpc_CreateSemaphore(const char* semaphoreName);
EmbedXrpc_Mutex_t EmbedXrpc_CreateMutex(const char* mutexName) ;
EmbedXrpc_Queue_t EmbedXrpc_CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen) ;
EmbedXrpc_Timer_t EmbedXrpc_CreateTimer(const char* timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg) ;

void EmbedXrpc_DeleteThread(EmbedXrpc_Thread_t thread) ;
void EmbedXrpc_DeleteMutex(EmbedXrpc_Mutex_t) ;
void EmbedXrpc_DeleteQueue(EmbedXrpc_Queue_t) ;
void EmbedXrpc_DeleteSemaphore(EmbedXrpc_Semaphore_t) ;
void EmbedXrpc_DeleteTimer(EmbedXrpc_Timer_t) ;

void EmbedXrpc_ThreadStart(EmbedXrpc_Thread_t thread) ;

void EmbedXrpc_TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval) ;
void EmbedXrpc_TimerReset(EmbedXrpc_Timer_t timer) ;
void EmbedXrpc_TimerStop(EmbedXrpc_Timer_t timer) ;

bool EmbedXrpc_TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout) ;
void EmbedXrpc_ReleaseSemaphore(EmbedXrpc_Semaphore_t sem) ;
void EmbedXrpc_ResetSemaphore(EmbedXrpc_Semaphore_t sem) ;

bool EmbedXrpc_TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout) ;
bool EmbedXrpc_ReleaseMutex(EmbedXrpc_Mutex_t mutex) ;



QueueState EmbedXrpc_ReceiveQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemSize, uint32_t timeout) ;
QueueState EmbedXrpc_SendQueue(EmbedXrpc_Queue_t queue, void* item, uint32_t itemSize) ;
void EmbedXrpc_ResetQueue(EmbedXrpc_Queue_t queue) ;
uint32_t EmbedXrpc_QueueSpacesAvailable(EmbedXrpc_Queue_t queue);
void* Malloc(uint32_t size) ;
void Free(void* ptr) ;
void Memcpy(void* d, const void* s, uint32_t size) ;
#endif
