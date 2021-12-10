#ifndef El_PortInterface_H
#define El_PortInterface_H
#include "stdint.h"
#include "assert.h"
#include "string.h"
//#include "console.h"
#ifdef __cplusplus
extern "C" {
#endif
	typedef enum _QueueState
	{
		QueueState_Empty,
		QueueState_Full,
		QueueState_OK,
		QueueState_Timeout,
	}QueueState;

	typedef  uint8_t UInt8;
	typedef  int8_t Int8;
	typedef  uint16_t UInt16;
	typedef  int16_t Int16;
	typedef  uint32_t UInt32;
	typedef  int32_t Int32;
	typedef  uint64_t UInt64;
	typedef  int64_t Int64;
	typedef  float Float;
	typedef  double Double;

#ifndef Bool
#define Bool uint8_t 
#endif

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif
#define El_WAIT_FOREVER	0xFFFFFFFF

#define Windows 1
#define FreeRtos	2
#define SupportedOs	Windows

#if SupportedOs==Windows
	typedef void* El_Semaphore_t;
	typedef void* El_Mutex_t;
	typedef void* El_Thread_t;
	typedef void* El_Queue_t;
	typedef void* El_Semaphore_t;
	typedef void* El_Timer_t;
	#define El_Assert assert
#define El_Debug	printf
#define El_Sprintf     sprintf//??????sprintf
#define El_Delay(x)    Sleep(x)
#elif SupportedOs==FreeRtos
	#include "FreeRTOS.h"
	#include "task.h"
	#include "semphr.h"
	#include "timers.h"
	typedef SemaphoreHandle_t  El_Mutex_t;
	typedef TimerHandle_t El_Timer_t;
	typedef SemaphoreHandle_t  El_Semaphore_t;
	typedef TaskHandle_t El_Thread_t;
	typedef QueueHandle_t El_Queue_t;
#define El_Debug	rt_kprintf
#define El_Assert configASSERT
#define El_Sprintf     rt_sprintf//2?¨°a¨º1¨®?sprintf
#define El_Delay(x)    vTaskDelay(x)
#endif
#define El_Strncpy strncpy
#define El_Strncmp    strncmp
#define El_Strlen      strlen
#define El_Strcat		strcat

#define CallFunction(Function,...) do{if (Function != NULL){Function(__VA_ARGS__);}}while(0); //using C99 Mode.
	void* El_Malloc(uint32_t size);
	void El_Free(void* ptr);
	void El_Memcpy(void* d, const void* s, uint32_t size);
	void El_Memset(void* d, int v, uint32_t size);
	
	

	El_Thread_t El_CreateThread(const char* threadName, uint8_t priority, void (*Thread)(void*), void* Arg, uint16_t stack_size);
	El_Mutex_t El_CreateSemaphore(const char* semaphoreName);
	El_Mutex_t El_CreateMutex(const char* mutexName);
	El_Queue_t El_CreateQueue(const char* queueName, uint32_t queueItemSize, uint32_t maxItemLen);
	El_Timer_t El_CreateTimer(const char* timerName, uint32_t timeout, void (*timercb)(void* arg), void* Arg);

	void El_DeleteThread(El_Thread_t thread);
	void El_DeleteMutex(El_Mutex_t);
	void El_DeleteQueue(El_Queue_t);
	void El_DeleteSemaphore(El_Semaphore_t);
	void El_DeleteTimer(El_Timer_t);

	void El_ThreadStart(El_Thread_t thread);

	void El_TimerStart(El_Timer_t timer, uint16_t interval);
	void El_TimerReset(El_Timer_t timer);
	void El_TimerStop(El_Timer_t timer);

	Bool El_TakeSemaphore(El_Semaphore_t sem, uint32_t timeout);
	void El_ReleaseSemaphore(El_Semaphore_t sem);
	void El_ResetSemaphore(El_Semaphore_t sem);

	Bool El_TakeMutex(El_Mutex_t mutex, uint32_t timeout);
	Bool El_ReleaseMutex(El_Mutex_t mutex);



	QueueState El_ReceiveQueue(El_Queue_t queue, void* item, uint32_t itemSize, uint32_t timeout);
	QueueState El_SendQueue(El_Queue_t queue, void* item, uint32_t itemSize);
	void El_ResetQueue(El_Queue_t queue);
	uint32_t El_QueueSpacesAvailable(El_Queue_t queue);
	void* El_Malloc(uint32_t size);
	void El_Free(void* ptr);
	void El_Memcpy(void* d, const void* s, uint32_t size);
	void El_Memset(void* d, int v, uint32_t size);
#ifdef __cplusplus
}
#endif

#endif
