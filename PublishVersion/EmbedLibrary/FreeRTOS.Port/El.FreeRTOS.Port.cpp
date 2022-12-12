#include "EmbedLibrary.h"
#include "cmsis_os2.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#ifdef __cplusplus
extern "C" {
#endif
osThreadId_t osThreadNew (osThreadFunc_t func, void *argument, const osThreadAttr_t *attr)
{
	TaskHandle_t ServiceThread = nullptr;
	xTaskCreate(func, attr->name, attr->stack_size/4, argument, attr->priority, &ServiceThread);
	vTaskSuspend(ServiceThread);
	return ServiceThread;
}

osMutexId_t osMutexNew (const osMutexAttr_t *attr)
{
	SemaphoreHandle_t  mutex = xSemaphoreCreateMutex();
	return mutex;
}
osMessageQueueId_t osMessageQueueNew (uint32_t msg_count, uint32_t msg_size, const osMessageQueueAttr_t *attr)
{

	QueueHandle_t queue = xQueueCreate(msg_count==0?1:msg_count, msg_size);
	return queue;
}
struct FreeRtosTimer_t
{
	void *arg;
	TickType_t ticks;
	TimerHandle_t FreeRtosTimerHandle;
	void (*timercb)(void *arg);
};

static void FreeRtosTimerCallBack(TimerHandle_t xTimer)
{
	FreeRtosTimer_t *frt = (FreeRtosTimer_t *)pvTimerGetTimerID(xTimer);
	frt->timercb(frt->arg);
}
osTimerId_t osTimerNew (osTimerFunc_t func, osTimerType_t type, void *argument, const osTimerAttr_t *attr)
{
	FreeRtosTimer_t *frt = (FreeRtosTimer_t *)pvPortMalloc(sizeof(FreeRtosTimer_t));
	TimerHandle_t timer = xTimerCreate(attr->name, 0, type==osTimerPeriodic?pdTRUE:pdFALSE, frt, FreeRtosTimerCallBack);
	frt->FreeRtosTimerHandle = timer;
	frt->ticks=0;
	frt->arg = argument;
	frt->timercb = func;
	return frt;
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
osStatus_t osTimerStart(osTimerId_t timer_id, uint32_t ticks)
{
	FreeRtosTimer_t * xtimer = (FreeRtosTimer_t *)timer_id;
	xTimerChangePeriod(xtimer->FreeRtosTimerHandle,ticks,0);
	xTimerStart(xtimer->FreeRtosTimerHandle, 0);
}
osStatus_t osTimerStop (osTimerId_t timer_id)
{
  FreeRtosTimer_t * xtimer = (FreeRtosTimer_t *)timer_id;
	xTimerStop(xtimer->FreeRtosTimerHandle, 0);
}

osStatus_t osMutexAcquire (osMutexId_t mutex_id, uint32_t timeout)
{
	auto m = static_cast<SemaphoreHandle_t >(mutex_id);
	auto r = xSemaphoreTake(m, timeout);
	if (r == pdTRUE)
	{
		return osOK;
	}
	else
	{
		return osErrorTimeout;
	}
}
osStatus_t osMutexRelease (osMutexId_t mutex_id)
{
	auto m = static_cast<SemaphoreHandle_t >(mutex_id);
	auto r = xSemaphoreGive(m);
	if (r == pdTRUE)
	{
		return osOK;
	}
	else
	{
		return osError;
	}
}

osStatus_t osMessageQueueGet (osMessageQueueId_t mq_id, void *msg_ptr, uint8_t *msg_prio, uint32_t timeout)
{
	auto q = static_cast<QueueHandle_t>(mq_id);
	auto r = xQueueReceive(q, msg_ptr, timeout);
	if (r == pdTRUE)
	{
		return osOK;
	}
	else
	{
		return osErrorTimeout;
	}
}
osStatus_t osMessageQueuePut (osMessageQueueId_t mq_id, const void *msg_ptr, uint8_t msg_prio, uint32_t timeout)
{
	auto q = static_cast<QueueHandle_t>(mq_id);
	auto r = xQueueSend(q, msg_ptr, 0);
	//configASSERT(r == pdPASS);
	if (r == pdPASS)
	{
		return osOK;
	}
	else
	{
		return osErrorNoMemory;
	}
}
void El_ResetQueue(El_Queue_t queue)
{
	auto q = static_cast<QueueHandle_t>(queue);
	xQueueReset(q);
}
uint32_t osMessageQueueGetSpace (osMessageQueueId_t mq_id)
{
	auto q = static_cast<QueueHandle_t>(mq_id);
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
