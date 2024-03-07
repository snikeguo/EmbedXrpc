#ifndef El_PortInterface_H
#define El_PortInterface_H
#include "stdint.h"
#include "assert.h"
#include "string.h"
#include "stdio.h"
#include "stdbool.h"
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

	#include "windows.h"
	#include "assert.h"
	typedef void* El_Semaphore_t;
	typedef void* El_Mutex_t;
	typedef void* El_Thread_t;
	typedef void* El_Queue_t;
	typedef void* El_Semaphore_t;
	typedef void* El_Timer_t;
	#define El_Assert assert
#define El_Debug	printf
#define El_Delay(x)    Sleep(x)
#define ThreadBeginHook()
#define ThreadExitHook()	
#define taskENTER_CRITICAL()    
#define taskEXIT_CRITICAL()

//为了移植FreeRTOS自带的IPC机制 所定义的宏
#define PRIVILEGED_FUNCTION
#define configSUPPORT_DYNAMIC_ALLOCATION	1
#define configSUPPORT_STATIC_ALLOCATION	1
#define configMESSAGE_BUFFER_LENGTH_TYPE size_t

#define configASSERT_DEFINED	1 //校验静态和动态结构体是否一致的宏，建议打开

#define configMIN( a, b )    ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )

#define traceSTREAM_BUFFER_CREATE	
#define traceSTREAM_BUFFER_CREATE_FAILED
#define traceSTREAM_BUFFER_CREATE_STATIC_FAILED
#define traceSTREAM_BUFFER_DELETE
#define traceSTREAM_BUFFER_RESET
#define traceBLOCKING_ON_STREAM_BUFFER_SEND
#define traceSTREAM_BUFFER_SEND
#define traceSTREAM_BUFFER_SEND_FAILED
#define traceBLOCKING_ON_STREAM_BUFFER_RECEIVE
#define traceSTREAM_BUFFER_RECEIVE
#define traceSTREAM_BUFFER_RECEIVE_FAILED
#define traceSTREAM_BUFFER_RECEIVE_FROM_ISR
#define traceSTREAM_BUFFER_SEND_FROM_ISR

#define sbRECEIVE_COMPLETED
#define sbRECEIVE_COMPLETED_FROM_ISR

#define mtCOVERAGE_TEST_MARKER()



#define pdTRUE	1
#define	pdPASS	1
#define pdFALSE 0
#define pdFAIL	0
#define errQUEUE_EMPTY ((BaseType_t)0)
#define errQUEUE_FULL ((BaseType_t)0)

#define configASSERT	assert

	typedef int32_t BaseType_t;
	typedef uint32_t TickType_t;
	typedef uint32_t UBaseType_t;

	void* pvPortMalloc(size_t size);
	void vPortFree(void*);

#include "message_buffer.h"

#elif SupportedOs==FreeRtos
	#include "FreeRTOS.h"
	#include "task.h"
	#include "semphr.h"
	#include "timers.h"
	#include "message_buffer.h"
#define El_Debug	printf
#define ThreadBeginHook()
#define ThreadExitHook()	vTaskDelete(xTaskGetCurrentTaskHandle());
#endif
#define El_Strncpy strncpy
#define El_Strncmp    strncmp
#define El_Strlen      strlen
#define El_Strcat		strcat

#define CallFunction(Function,...) do{if (Function != NULL){Function(__VA_ARGS__);}}while(0); //using C99 Mode.

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

	void El_ThreadStart(El_Thread_t thread,int isIsr);

	void El_TimerStart(El_Timer_t timer, uint16_t interval, int isIsr);
	void El_TimerReset(El_Timer_t timer, int isIsr);
	void El_TimerStop(El_Timer_t timer, int isIsr);


	bool El_TakeMutex(El_Mutex_t mutex, uint32_t timeout, int isIsr);
	bool El_ReleaseMutex(El_Mutex_t mutex, int isIsr);

	QueueState El_TakeSemaphore(El_Semaphore_t sem, uint32_t timeout,int isIsr);
	void El_ReleaseSemaphore(El_Semaphore_t sem,int isIsr);
	void El_ResetSemaphore(El_Semaphore_t sem,int isIsr);

	QueueState El_ReceiveQueue(El_Queue_t queue, void* item, uint32_t itemSize, uint32_t timeout, int isIsr);
	QueueState El_SendQueue(El_Queue_t queue, void* item, uint32_t itemSize,int isIsr);
	void El_ResetQueue(El_Queue_t queue, int isIsr);
	uint32_t El_QueueSpacesAvailable(El_Queue_t queue, int isIsr);

	void* El_Malloc(uint32_t size);
	void El_Free(void* ptr);
	void El_Memcpy(void* d, const void* s, uint32_t size);
	void El_Memset(void* d, int v, uint32_t size);
	uint32_t El_GetTick(int isIsr);
#ifdef __cplusplus
}
#endif

#endif
