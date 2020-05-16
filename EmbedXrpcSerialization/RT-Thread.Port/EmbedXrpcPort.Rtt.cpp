#include "EmbedXrpcPort.Rtt.h"
#include "EmbedSerialization.h"
#include "EmbedXrpcCommon.h"
#include "rtthread.h"

EmbedXrpc_Thread_t RttEmbedXrpcPort::CreateThread(const char *threadName,uint8_t priority, void (*Thread)(void *), void *Arg)
{
	auto ServiceThread = rt_thread_create(threadName, Thread, Arg, 2048, priority, 10);
	return ServiceThread;
}

EmbedXrpc_Mutex_t RttEmbedXrpcPort::CreateMutex(const char *mutexName)
{
	auto mutex = rt_mutex_create(mutexName, RT_IPC_FLAG_FIFO);
	return mutex;
}
EmbedXrpc_Queue_t RttEmbedXrpcPort::CreateQueue(const char *queueName,
												uint32_t queueItemSize,
												uint32_t maxItemLen)
{
	auto queue = rt_mq_create(queueName, queueItemSize, maxItemLen, RT_IPC_FLAG_FIFO);
	return queue;
}

EmbedXrpc_Timer_t RttEmbedXrpcPort::CreateTimer(const char *timerName, uint32_t timeout, void *Arg, void (*timercb)(void *arg))
{
	auto timer = rt_timer_create(timerName, timercb, Arg, timeout, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
	return timer;
}
EmbedXrpc_Semaphore_t  RttEmbedXrpcPort::CreateSemaphore(const char* SemaphoreName)
{
	auto *sem = rt_sem_create(SemaphoreName,0,RT_IPC_FLAG_FIFO);
	return sem;
}
void RttEmbedXrpcPort::DeleteThread(EmbedXrpc_Thread_t thread)
{
	
}
void RttEmbedXrpcPort::DeleteMutex(EmbedXrpc_Mutex_t mutex)
{
	
}
void RttEmbedXrpcPort::DeleteQueue(EmbedXrpc_Queue_t queue)
{

}
void RttEmbedXrpcPort::DeleteSemaphore(EmbedXrpc_Semaphore_t sem)
{
	
}
void RttEmbedXrpcPort::DeleteTimer(EmbedXrpc_Timer_t timer) 
{

}
void RttEmbedXrpcPort::ThreadStart(EmbedXrpc_Thread_t thread)
{
	auto x = static_cast<rt_thread_t>(thread);
	rt_thread_startup(x);
}
void RttEmbedXrpcPort::TimerStart(EmbedXrpc_Timer_t timer, uint16_t interval)
{
	auto t = static_cast<rt_timer_t>(timer);
	rt_tick_t inter = interval;
	rt_timer_control(t, RT_TIMER_CTRL_SET_TIME, &inter);
	rt_timer_start(t);
}
void RttEmbedXrpcPort::TimerReset(EmbedXrpc_Timer_t timer)
{
	//auto t = static_cast<rt_timer_t>(timer);
}
void RttEmbedXrpcPort::TimerStop(EmbedXrpc_Timer_t timer)
{
	auto t = static_cast<rt_timer_t>(timer);
	rt_timer_stop(t);
}
bool RttEmbedXrpcPort::TakeSemaphore(EmbedXrpc_Semaphore_t sem, uint32_t timeout)
{
	auto rttsem=static_cast<rt_sem_t>(sem);
	return rt_sem_take(rttsem,timeout)==RT_EOK?true:false;
}
void RttEmbedXrpcPort::ReleaseSemaphore(EmbedXrpc_Semaphore_t sem)
{
	auto rttsem=static_cast<rt_sem_t>(sem);
	rt_sem_release(rttsem);
}
void RttEmbedXrpcPort::ResetSemaphore(EmbedXrpc_Semaphore_t sem)
{
	auto rttsem=static_cast<rt_sem_t>(sem);
	rt_sem_control(rttsem,RT_IPC_CMD_RESET,0);
}
bool RttEmbedXrpcPort::TakeMutex(EmbedXrpc_Mutex_t mutex, uint32_t timeout)
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
bool RttEmbedXrpcPort::ReleaseMutex(EmbedXrpc_Mutex_t mutex)
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

QueueState RttEmbedXrpcPort::ReceiveQueue(EmbedXrpc_Queue_t queue, void *item, uint32_t itemlen, uint32_t timeout)
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
QueueState RttEmbedXrpcPort::SendQueue(EmbedXrpc_Queue_t queue, void *item, uint32_t itemlen)
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
		XrpcDebug("RttEmbedXrpcPort::SendQueue r:%d\n", r);
		//while(true);
	}
	return QueueState_Full;
}
void RttEmbedXrpcPort::ResetQueue(EmbedXrpc_Queue_t queue)
{
	auto q = static_cast<rt_mq_t>(queue);
	rt_mq_control(q, RT_IPC_CMD_RESET, nullptr);
}
uint32_t MallocCount=0;
void *RttEmbedXrpcPort::Malloc(uint32_t size)
{
	auto x = rt_malloc(size);
	RT_ASSERT(x != RT_NULL);
	XrpcDebug("RttEmbedXrpcPort::Malloc %x\n", x);
	MallocCount++;
	return x;
}
void RttEmbedXrpcPort::Free(void *ptr)
{
	XrpcDebug("RttEmbedXrpcPort::Free %x\n", ptr);
	rt_free(ptr);
}
void RttEmbedXrpcPort::Memcpy(void *d, const void *s, uint32_t size)
{
	rt_memcpy(d, s, size);
}