#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"
#include "rtthread.h"

EmbedXrpc_Thread_t EmbedXrpc_CreateThread(const char *threadName,uint8_t priority, void (*Thread)(void *), void *Arg)
{
	auto ServiceThread = rt_thread_create(threadName, Thread, Arg, 2048, priority, 10);
	return ServiceThread;
}

EmbedXrpc_Mutex_t EmbedXrpc_CreateMutex(const char *mutexName)
{
	auto mutex = rt_mutex_create(mutexName, RT_IPC_FLAG_FIFO);
	return mutex;
}
EmbedXrpc_Queue_t EmbedXrpc_CreateQueue(const char *queueName,
												uint32_t queueItemSize,
												uint32_t maxItemLen)
{
	auto queue = rt_mq_create(queueName, queueItemSize, maxItemLen, RT_IPC_FLAG_FIFO);
	return queue;
}

EmbedXrpc_Timer_t EmbedXrpc_CreateTimer(const char *timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg)
{
	auto timer = rt_timer_create(timerName, timercb, Arg, timeout, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
	return timer;
}
EmbedXrpc_Semaphore_t  EmbedXrpc_CreateSemaphore(const char* SemaphoreName)
{
	auto *sem = rt_sem_create(SemaphoreName,0,RT_IPC_FLAG_FIFO);
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

}
void EmbedXrpc_ThreadStart(EmbedXrpc_Thread_t thread)
{
	auto x = static_cast<rt_thread_t>(thread);
	rt_thread_startup(x);
}
void EmbedXrpc_TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
{
	auto t = static_cast<rt_timer_t>(timer);
	rt_tick_t inter = interval;
	rt_timer_control(t, RT_TIMER_CTRL_SET_TIME, &inter);
	rt_timer_start(t);
}
void EmbedXrpc_TimerReset(EmbedXrpc_Timer_t timer)
{
	//auto t = static_cast<rt_timer_t>(timer);
}
void EmbedXrpc_TimerStop(EmbedXrpc_Timer_t timer)
{
	auto t = static_cast<rt_timer_t>(timer);
	rt_timer_stop(t);
}
bool EmbedXrpc_TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
{
	auto rttsem=static_cast<rt_sem_t>(sem);
	return rt_sem_take(rttsem,timeout)==RT_EOK?true:false;
}
void EmbedXrpc_ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
{
	auto rttsem=static_cast<rt_sem_t>(sem);
	rt_sem_release(rttsem);
}
void EmbedXrpc_ResetSemaphore(EmbedXrpc_Semaphore_t sem)
{
	auto rttsem=static_cast<rt_sem_t>(sem);
	rt_sem_control(rttsem,RT_IPC_CMD_RESET,0);
}
bool EmbedXrpc_TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout)
{
	auto m = static_cast<rt_mutex_t>(mutex);
	auto r = rt_mutex_take(m, timeout);
	if (r == RT_EOK)
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
	auto m = static_cast<rt_mutex_t>(mutex);
	auto r = rt_mutex_release(m);
	if (r == RT_EOK)
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
	auto q = static_cast<rt_mq_t>(queue);
	auto r = rt_mq_recv(q, item, itemlen, timeout);
	if (r == RT_EOK)
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
	auto q = static_cast<rt_mq_t>(queue);
	auto r = rt_mq_send(q, item, itemlen);
	//RT_ASSERT(r==RT_EOK);
	if (r == RT_EOK)
	{
		return QueueState_OK;
	}
	else if (r == RT_EFULL)
	{
		return QueueState_Full;
	}
	else
	{
		EmbedSerializationShowMessage("EmbedXrpcPort","EmbedXrpc_SendQueue r:%d\n", r);
		//while(true);
	}
	return QueueState_Full;
}
void EmbedXrpc_ResetQueue(EmbedXrpc_Queue_t queue)
{
	auto q = static_cast<rt_mq_t>(queue);
	rt_mq_control(q, RT_IPC_CMD_RESET, nullptr);
}
uint32_t EmbedXrpc_QueueSpacesAvailable(EmbedXrpc_Queue_t queue)
{
	auto q = static_cast<rt_mq_t>(queue);

	register rt_ubase_t temp;

	rt_uint16_t ava = 0;

	temp = rt_hw_interrupt_disable();
	ava = q->max_msgs - q->entry;
	rt_hw_interrupt_enable(temp);

	return ava;
}
uint32_t MallocCount=0;
void * EmbedXrpc_Malloc(uint32_t size)
{
	auto x = rt_malloc(size);
	RT_ASSERT(x != RT_NULL);
	EmbedSerializationShowMessage("EmbedXrpcPort", "Malloc %x\n", x);
	MallocCount++;
	return x;
}
void EmbedXrpc_Free(void *ptr)
{
	EmbedSerializationShowMessage("EmbedXrpcPort", "Free %x\n", ptr);
	rt_free(ptr);
}
void Memcpy(void *d, const void *s, uint32_t size)
{
	rt_memcpy(d, s, size);
}
void EmbedXrpc_Memset(void* d, int v, uint32_t size)
{
	rt_memset(d, v, size);
}
