#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
EmbedXrpc_Thread_t EmbedXrpc_CreateThread(const char *threadName, uint8_t priority, void (*Thread)(void *), void *Arg)
{
	TaskHandle_t ServiceThread = nullptr;
	xTaskCreate(Thread, threadName, 512, Arg, priority, &ServiceThread);
	vTaskSuspend(ServiceThread);
	return ServiceThread;
}

EmbedXrpc_Mutex_t EmbedXrpc_CreateMutex(const char *mutexName)
{
	SemaphoreHandle_t  mutex = xSemaphoreCreateMutex();
	return mutex;
}
EmbedXrpc_Queue_t EmbedXrpc_CreateQueue(const char *queueName,
										uint32_t queueItemSize,
										uint32_t maxItemLen)
{

	QueueHandle_t queue = xQueueCreate(maxItemLen, queueItemSize);
	return queue;
}
struct FreeRtosTimer_t
{
	void *arg;
	TimerHandle_t timer;
	void (*timercb)(void *arg);
};

static void FreeRtosTimerCallBack(TimerHandle_t xTimer)
{
	FreeRtosTimer_t *frt = (FreeRtosTimer_t *)pvTimerGetTimerID(xTimer);
	frt->timercb(frt->arg);
}
EmbedXrpc_Timer_t EmbedXrpc_CreateTimer(const char *timerName, uint32_t timeout, void (*timercb)(void *arg), void *Arg)
{
	FreeRtosTimer_t *frt = (FreeRtosTimer_t *)pvPortMalloc(sizeof(FreeRtosTimer_t));
	TimerHandle_t timer = xTimerCreate(timerName, timeout, pdTRUE, frt, FreeRtosTimerCallBack);
	frt->timer = timer;
	frt->arg = Arg;
	frt->timercb = timercb;
	return timer;
}
EmbedXrpc_Semaphore_t EmbedXrpc_CreateSemaphore(const char *SemaphoreName)
{
	//auto *sem = rt_sem_create(SemaphoreName,0,RT_IPC_FLAG_FIFO);
	QueueHandle_t sem = xSemaphoreCreateCounting(10, 0);
	return sem;
}
void EmbedXrpc_DeleteThread(EmbedXrpc_Thread_t thread)
{
}
void EmbedXrpc_DeleteMutex(EmbedXrpc_Mutex_t mutex)
{
}
void EmbedXrpc_DeleteQueue(EmbedXrpc_Queue_t queue)
{
}
void EmbedXrpc_DeleteSemaphore(EmbedXrpc_Semaphore_t sem)
{
}
void EmbedXrpc_DeleteTimer(EmbedXrpc_Timer_t timer)
{
	TimerHandle_t xtimer = (TimerHandle_t)timer;
	xTimerStop(xtimer,0);
	FreeRtosTimer_t *frt = (FreeRtosTimer_t *)pvTimerGetTimerID(xtimer);
	vPortFree(frt);
}
void EmbedXrpc_ThreadStart(EmbedXrpc_Thread_t thread)
{
	auto x = static_cast<TaskHandle_t>(thread);
	vTaskResume(x);
}
void EmbedXrpc_TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
{
	TimerHandle_t xtimer = (TimerHandle_t)timer;
	xTimerStart(xtimer, 0);
}
void EmbedXrpc_TimerReset(EmbedXrpc_Timer_t timer)
{
	TimerHandle_t xtimer = (TimerHandle_t)timer;
	xTimerReset(xtimer, 0);
}
void EmbedXrpc_TimerStop(EmbedXrpc_Timer_t timer)
{
	TimerHandle_t xtimer = (TimerHandle_t)timer;
	xTimerStop(xtimer, 0);
}
bool EmbedXrpc_TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
{
	auto frsem = static_cast<QueueHandle_t>(sem);
	return xSemaphoreTake(frsem, timeout) == pdTRUE ? true : false;
}
void EmbedXrpc_ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
{
	auto frsem = static_cast<QueueHandle_t>(sem);
	xSemaphoreGive(frsem);
}
void EmbedXrpc_ResetSemaphore(EmbedXrpc_Semaphore_t sem)
{
	auto frsem = static_cast<QueueHandle_t>(sem);
	xQueueReset(frsem); //Çå¿Õ¶ÓÁÐ
}
bool EmbedXrpc_TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout)
{
	auto m = static_cast<SemaphoreHandle_t >(mutex);
	auto r = xSemaphoreTake(m, timeout);
	if (r == pdTRUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool EmbedXrpc_ReleaseMutex(EmbedXrpc_Mutex_t mutex)
{
	auto m = static_cast<SemaphoreHandle_t >(mutex);
	auto r = xSemaphoreGive(m);
	if (r == pdTRUE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

QueueState EmbedXrpc_ReceiveQueue(EmbedXrpc_Queue_t queue, void *item, uint32_t itemlen, uint32_t timeout)
{
	auto q = static_cast<QueueHandle_t>(queue);
	auto r = xQueueReceive(q, item, timeout);
	if (r == pdTRUE)
	{
		return QueueState_OK;
	}
	else
	{
		return QueueState_Timeout;
	}
}
QueueState EmbedXrpc_SendQueue(EmbedXrpc_Queue_t queue, void *item, uint32_t itemlen)
{
	auto q = static_cast<QueueHandle_t>(queue);
	auto r = xQueueSend(q, item, 0);
	//configASSERT(r == pdPASS);
	if (r == pdPASS)
	{
		return QueueState_OK;
	}
	else //(r == errQUEUE_FULL)
	{
		return QueueState_Full;
	}
	return QueueState_Full;
}
void EmbedXrpc_ResetQueue(EmbedXrpc_Queue_t queue)
{
	auto q = static_cast<QueueHandle_t>(queue);
	xQueueReset(q);
}
uint32_t EmbedXrpc_QueueSpacesAvailable(EmbedXrpc_Queue_t queue)
{
	auto q = static_cast<QueueHandle_t>(queue);
	return uxQueueSpacesAvailable(q);
}
uint32_t MallocCount = 0;
void * EmbedXrpc_Malloc(uint32_t size)
{
	auto x = pvPortMalloc(size);
	//configASSERT(x != NULL);
	EmbedSerializationShowMessage("EmbedXrpcPort", "Malloc %x\n", x);
	MallocCount++;
	return x;
}
uint32_t FreeCount = 0;
void EmbedXrpc_Free(void *ptr)
{
	FreeCount++;
	EmbedSerializationShowMessage("EmbedXrpcPort", "Free %x\n", ptr);
	vPortFree(ptr);
}
void EmbedXrpc_Memcpy(void *d, const void *s, uint32_t size)
{
	memcpy(d, s, size);
}
void EmbedXrpc_Memset(void* d, int v, uint32_t size)
{
	memset(d, v, size);
}
