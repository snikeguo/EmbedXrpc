#include "../EmbedLibrary.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#ifdef __cplusplus
extern "C" {
#endif
El_Thread_t El_CreateThread(const char *threadName, uint8_t priority, void (*Thread)(void *), void *Arg)
{
	TaskHandle_t ServiceThread = nullptr;
	xTaskCreate(Thread, threadName, 512, Arg, priority, &ServiceThread);
	vTaskSuspend(ServiceThread);
	return ServiceThread;
}

El_Mutex_t El_CreateMutex(const char *mutexName)
{
	SemaphoreHandle_t  mutex = xSemaphoreCreateMutex();
	return mutex;
}
El_Queue_t El_CreateQueue(const char *queueName,
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
El_Timer_t El_CreateTimer(const char *timerName, uint32_t timeout, void (*timercb)(void *arg), void *Arg)
{
	FreeRtosTimer_t *frt = (FreeRtosTimer_t *)pvPortMalloc(sizeof(FreeRtosTimer_t));
	TimerHandle_t timer = xTimerCreate(timerName, timeout, pdTRUE, frt, FreeRtosTimerCallBack);
	frt->timer = timer;
	frt->arg = Arg;
	frt->timercb = timercb;
	return timer;
}
El_Semaphore_t El_CreateSemaphore(const char *SemaphoreName)
{
	//auto *sem = rt_sem_create(SemaphoreName,0,RT_IPC_FLAG_FIFO);
	QueueHandle_t sem = xSemaphoreCreateCounting(10, 0);
	return sem;
}
void El_DeleteThread(El_Thread_t thread)
{
}
void El_DeleteMutex(El_Mutex_t mutex)
{
}
void El_DeleteQueue(El_Queue_t queue)
{
}
void El_DeleteSemaphore(El_Semaphore_t sem)
{
}
void El_DeleteTimer(El_Timer_t timer)
{
	TimerHandle_t xtimer = (TimerHandle_t)timer;
	xTimerStop(xtimer,0);
	FreeRtosTimer_t *frt = (FreeRtosTimer_t *)pvTimerGetTimerID(xtimer);
	vPortFree(frt);
}
void El_ThreadStart(El_Thread_t thread)
{
	auto x = static_cast<TaskHandle_t>(thread);
	vTaskResume(x);
}
void El_TimerStart(El_Timer_t timer, uint16_t interval)
{
	TimerHandle_t xtimer = (TimerHandle_t)timer;
	xTimerStart(xtimer, 0);
}
void El_TimerReset(El_Timer_t timer)
{
	TimerHandle_t xtimer = (TimerHandle_t)timer;
	xTimerReset(xtimer, 0);
}
void El_TimerStop(El_Timer_t timer)
{
	TimerHandle_t xtimer = (TimerHandle_t)timer;
	xTimerStop(xtimer, 0);
}
Bool El_TakeSemaphore(El_Semaphore_t sem, uint32_t timeout)
{
	auto frsem = static_cast<QueueHandle_t>(sem);
	return xSemaphoreTake(frsem, timeout) == pdTRUE ? True : False;
}
void El_ReleaseSemaphore(El_Semaphore_t sem)
{
	auto frsem = static_cast<QueueHandle_t>(sem);
	xSemaphoreGive(frsem);
}
void El_ResetSemaphore(El_Semaphore_t sem)
{
	auto frsem = static_cast<QueueHandle_t>(sem);
	xQueueReset(frsem); //��ն���
}
Bool El_TakeMutex(El_Mutex_t mutex, uint32_t timeout)
{
	auto m = static_cast<SemaphoreHandle_t >(mutex);
	auto r = xSemaphoreTake(m, timeout);
	if (r == pdTRUE)
	{
		return True;
	}
	else
	{
		return False;
	}
}
Bool El_ReleaseMutex(El_Mutex_t mutex)
{
	auto m = static_cast<SemaphoreHandle_t >(mutex);
	auto r = xSemaphoreGive(m);
	if (r == pdTRUE)
	{
		return True;
	}
	else
	{
		return False;
	}
}

QueueState El_ReceiveQueue(El_Queue_t queue, void *item, uint32_t itemlen, uint32_t timeout)
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
QueueState El_SendQueue(El_Queue_t queue, void *item, uint32_t itemlen)
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
void El_ResetQueue(El_Queue_t queue)
{
	auto q = static_cast<QueueHandle_t>(queue);
	xQueueReset(q);
}
uint32_t El_QueueSpacesAvailable(El_Queue_t queue)
{
	auto q = static_cast<QueueHandle_t>(queue);
	return uxQueueSpacesAvailable(q);
}
//uint32_t MallocCount = 0;
void * El_Malloc(uint32_t size)
{
	auto x = pvPortMalloc(size);
	//configASSERT(x != NULL);
	//EmbedSerializationShowMessage("EmbedXrpcPort", "Malloc %x\n", x);
	//MallocCount++;
	return x;
}
//uint32_t FreeCount = 0;
void El_Free(void *ptr)
{
	//FreeCount++;
	//EmbedSerializationShowMessage("EmbedXrpcPort", "Free %x\n", ptr);
	vPortFree(ptr);
}
void El_Memcpy(void *d, const void *s, uint32_t size)
{
	memcpy(d, s, size);
}
void El_Memset(void* d, int v, uint32_t size)
{
	memset(d, v, size);
}
#ifdef __cplusplus
}
#endif
