#include "EmbedXrpcPort.FreeRTOS.h"
#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
EmbedXrpc_Thread_t FreeRtosEmbedXrpcPort::CreateThread(const char *threadName,uint8_t priority, void (*Thread)(void *), void *Arg)
{	
	TaskHandle_t ServiceThread = nullptr;
	xTaskCreate(Thread, threadName, 2048, Arg, priority, &ServiceThread);
	vTaskSuspend(ServiceThread);
	return ServiceThread;
}

EmbedXrpc_Mutex_t FreeRtosEmbedXrpcPort::CreateMutex(const char *mutexName)
{
	xSemaphoreCreateMutex();
	QueueHandle_t mutex = xSemaphoreCreateMutex();
	return mutex;
}
EmbedXrpc_Queue_t FreeRtosEmbedXrpcPort::CreateQueue(const char *queueName,
												uint32_t queueItemSize,
												uint32_t maxItemLen)
{
	
	QueueHandle_t queue = xQueueCreate(maxItemLen, queueItemSize);;
	return queue;
}
struct FreeRtosTimer_t
{
	void *arg;
	TimerHandle_t timer;
	void (*timercb)(void* arg);
};

static void FreeRtosTimerCallBack(TimerHandle_t xTimer)
{
	FreeRtosTimer_t * frt=(FreeRtosTimer_t *)pvTimerGetTimerID(xTimer);
	frt->timercb(frt->arg);
}
EmbedXrpc_Timer_t FreeRtosEmbedXrpcPort::CreateTimer(const char *timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg)
{
	FreeRtosTimer_t * frt=(FreeRtosTimer_t *)pvPortMalloc(sizeof(FreeRtosTimer_t));
	TimerHandle_t timer = xTimerCreate(timerName, timeout, pdTRUE, frt, FreeRtosTimerCallBack);
	frt->timer=timer;
	frt->arg=Arg;
	frt->timercb=timercb;
	return timer;
}
EmbedXrpc_Semaphore_t  FreeRtosEmbedXrpcPort::CreateSemaphore(const char* SemaphoreName)
{
	//auto *sem = rt_sem_create(SemaphoreName,0,RT_IPC_FLAG_FIFO);
	QueueHandle_t sem = xSemaphoreCreateCounting(10, 0);
	return sem;
}
void FreeRtosEmbedXrpcPort::DeleteThread(EmbedXrpc_Thread_t thread)
{
	
}
void FreeRtosEmbedXrpcPort::DeleteMutex(EmbedXrpc_Mutex_t mutex)
{
	
}
void FreeRtosEmbedXrpcPort::DeleteQueue(EmbedXrpc_Queue_t queue)
{

}
void FreeRtosEmbedXrpcPort::DeleteSemaphore(EmbedXrpc_Semaphore_t sem)
{
	
}
void FreeRtosEmbedXrpcPort::DeleteTimer(EmbedXrpc_Timer_t timer) 
{
	TimerHandle_t  xtimer=(TimerHandle_t )timer;
	xTimerStop(xtimer,0);
	FreeRtosTimer_t * frt=(FreeRtosTimer_t *)pvTimerGetTimerID(xtimer);
	vPortFree(frt);
}
void FreeRtosEmbedXrpcPort::ThreadStart(EmbedXrpc_Thread_t thread)
{
	auto x = static_cast<TaskHandle_t>(thread);
	vTaskResume(x);
}
void FreeRtosEmbedXrpcPort::TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
{
	TimerHandle_t  xtimer = (TimerHandle_t)timer;
	xTimerStart(xtimer,0);
}
void FreeRtosEmbedXrpcPort::TimerReset(EmbedXrpc_Timer_t timer)
{
	TimerHandle_t  xtimer = (TimerHandle_t)timer;
	xTimerReset(xtimer, 0);
}
void FreeRtosEmbedXrpcPort::TimerStop(EmbedXrpc_Timer_t timer)
{
	TimerHandle_t  xtimer = (TimerHandle_t)timer;
	xTimerStop(xtimer,0);
}
bool FreeRtosEmbedXrpcPort::TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
{
	auto frsem=static_cast<QueueHandle_t>(sem);
	return xSemaphoreTake(frsem,timeout)== pdTRUE ?true:false;
}
void FreeRtosEmbedXrpcPort::ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
{
	auto frsem = static_cast<QueueHandle_t>(sem);
	xSemaphoreGive(frsem);
}
void FreeRtosEmbedXrpcPort::ResetSemaphore(EmbedXrpc_Semaphore_t sem)
{
	auto frsem = static_cast<QueueHandle_t>(sem);
	xQueueReset(frsem);//Çå¿Õ¶ÓÁÐ
}
bool FreeRtosEmbedXrpcPort::TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout)
{
	auto m = static_cast<QueueHandle_t>(mutex);
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
bool FreeRtosEmbedXrpcPort::ReleaseMutex(EmbedXrpc_Mutex_t mutex)
{
	auto m = static_cast<QueueHandle_t>(mutex);
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

QueueState FreeRtosEmbedXrpcPort::ReceiveQueue(EmbedXrpc_Queue_t queue, void *item, uint32_t itemlen, uint32_t timeout)
{
	auto q = static_cast<QueueHandle_t>(queue);
	auto r = xQueueReceive(q, item,timeout);
	if (r == pdTRUE)
	{
		return QueueState_OK;
	}
	else
	{
		return QueueState_Timeout;
	}
}
QueueState FreeRtosEmbedXrpcPort::SendQueue(EmbedXrpc_Queue_t queue, void *item, uint32_t itemlen)
{
	auto q = static_cast<QueueHandle_t>(queue);
	auto r = xQueueSend(q, item, 0);
	//configASSERT(r==RT_EOK);
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
void FreeRtosEmbedXrpcPort::ResetQueue(EmbedXrpc_Queue_t queue)
{
	auto q = static_cast<QueueHandle_t>(queue);
	xQueueReset(q);
}
uint32_t MallocCount=0;
void *FreeRtosEmbedXrpcPort::Malloc(uint32_t size)
{
	auto x = pvPortMalloc(size);
	configASSERT(x != NULL);
	EmbedSerializationShowMessage("EmbedXrpcPort","FreeRtosEmbedXrpcPort::Malloc %x\n", x);
	MallocCount++;
	return x;
}
void FreeRtosEmbedXrpcPort::Free(void *ptr)
{
	EmbedSerializationShowMessage("EmbedXrpcPort","FreeRtosEmbedXrpcPort::Free %x\n", ptr);
	vPortFree(ptr);
}
void FreeRtosEmbedXrpcPort::Memcpy(void *d, const void *s, uint32_t size)
{
	memcpy(d, s, size);
}