
#include <stdlib.h>
#include <string.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
 * all the API functions to use the MPU wrappers.  That should only be done when
 * task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#if ( configUSE_CO_ROUTINES == 1 )
#include "croutine.h"
#endif

 /* Lint e9021, e961 and e750 are suppressed as a MISRA exception justified
  * because the MPU ports require MPU_WRAPPERS_INCLUDED_FROM_API_FILE to be defined
  * for the header files above, but not in this file, in order to generate the
  * correct privileged Vs unprivileged linkage and placement. */
#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE /*lint !e961 !e750 !e9021. */


  /* Constants used with the cRxLock and cTxLock structure members. */
#define queueUNLOCKED             ( ( int8_t ) -1 )
#define queueLOCKED_UNMODIFIED    ( ( int8_t ) 0 )
#define queueINT8_MAX             ( ( int8_t ) 127 )

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#define uxQueueType               pcHead
#define queueQUEUE_IS_MUTEX       NULL

typedef struct QueuePointers
{
    int8_t* pcTail;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t* pcReadFrom; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} QueuePointers_t;

typedef struct SemaphoreData
{
    TaskHandle_t xMutexHolder;        /*< The handle of the task that holds the mutex. */
    UBaseType_t uxRecursiveCallCount; /*< Maintains a count of the number of times a recursive mutex has been recursively 'taken' when the structure is used as a mutex. */
} SemaphoreData_t;

/* Semaphores do not actually store or copy data, so have an item size of
 * zero. */
#define queueSEMAPHORE_QUEUE_ITEM_LENGTH    ( ( UBaseType_t ) 0 )
#define queueMUTEX_GIVE_BLOCK_TIME          ( ( TickType_t ) 0U )

#if ( configUSE_PREEMPTION == 0 )

 /* If the cooperative scheduler is being used then a yield should not be
  * performed just because a higher priority task has been woken. */
#define queueYIELD_IF_USING_PREEMPTION()
#else
#define queueYIELD_IF_USING_PREEMPTION()    portYIELD_WITHIN_API()
#endif

 /*
  * Definition of the queue used by the scheduler.
  * Items are queued by copy, not reference.  See the following link for the
  * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
  */
typedef struct QueueDefinition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t* pcHead;           /*< Points to the beginning of the queue storage area. */
    int8_t* pcWriteTo;        /*< Points to the free next place in the storage area. */

    union
    {
        QueuePointers_t xQueue;     /*< Data required exclusively when this structure is used as a queue. */
        SemaphoreData_t xSemaphore; /*< Data required exclusively when this structure is used as a semaphore. */
    } u;

    List_t xTasksWaitingToSend;             /*< List of tasks that are blocked waiting to post onto this queue.  Stored in priority order. */
    List_t xTasksWaitingToReceive;          /*< List of tasks that are blocked waiting to read from this queue.  Stored in priority order. */

    volatile UBaseType_t uxMessagesWaiting; /*< The number of items currently in the queue. */
    UBaseType_t uxLength;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t uxItemSize;                 /*< The size of each items that the queue will hold. */

    volatile int8_t cRxLock;                /*< Stores the number of items received from the queue (removed from the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */
    volatile int8_t cTxLock;                /*< Stores the number of items transmitted to the queue (added to the queue) while the queue was locked.  Set to queueUNLOCKED when the queue is not locked. */

#if ( ( configSUPPORT_STATIC_ALLOCATION == 1 ) && ( configSUPPORT_DYNAMIC_ALLOCATION == 1 ) )
    uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */
#endif

#if ( configUSE_QUEUE_SETS == 1 )
    struct QueueDefinition* pxQueueSetContainer;
#endif

#if ( configUSE_TRACE_FACILITY == 1 )
    UBaseType_t uxQueueNumber;
    uint8_t ucQueueType;
#endif
} xQUEUE;

/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
typedef xQUEUE Queue_t;

/*-----------------------------------------------------------*/

/*
 * The queue registry is just a means for kernel aware debuggers to locate
 * queue structures.  It has no other purpose so is an optional component.
 */
#if ( configQUEUE_REGISTRY_SIZE > 0 )

 /* The type stored within the queue registry array.  This allows a name
  * to be assigned to each queue making kernel aware debugging a little
  * more user friendly. */
typedef struct QUEUE_REGISTRY_ITEM
{
    const char* pcQueueName; /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    QueueHandle_t xHandle;
} xQueueRegistryItem;

/* The old xQueueRegistryItem name is maintained above then typedefed to the
 * new xQueueRegistryItem name below to enable the use of older kernel aware
 * debuggers. */
typedef xQueueRegistryItem QueueRegistryItem_t;

/* The queue registry is simply an array of QueueRegistryItem_t structures.
 * The pcQueueName member of a structure being NULL is indicative of the
 * array position being vacant. */
PRIVILEGED_DATA QueueRegistryItem_t xQueueRegistry[configQUEUE_REGISTRY_SIZE];

#endif /* configQUEUE_REGISTRY_SIZE */

/*
 * Uses a critical section to determine if there is any data in a queue.
 *
 * @return pdTRUE if the queue contains no items, otherwise pdFALSE.
 */
static BaseType_t prvIsQueueEmpty(const Queue_t *pxQueue) ;

/*
 * Uses a critical section to determine if there is any space in a queue.
 *
 * @return pdTRUE if there is no space, otherwise pdFALSE;
 */
static BaseType_t prvIsQueueFull(const Queue_t *pxQueue) ;

/*
 * Copies an item into the queue, either at the front of the queue or the
 * back of the queue.
 */
static BaseType_t prvCopyDataToQueue(Queue_t *const pxQueue,
                                     const void *pvItemToQueue,
                                     const BaseType_t xPosition) ;

/*
 * Copies an item out of a queue.
 */
static void prvCopyDataFromQueue(Queue_t *const pxQueue,
                                 void *const pvBuffer) ;

/*
 * Called after a Queue_t structure has been allocated either statically or
 * dynamically to fill in the structure's members.
 */
static void prvInitialiseNewQueue(const UBaseType_t uxQueueLength,
                                  const UBaseType_t uxItemSize,
                                  uint8_t *pucQueueStorage,
                                  const uint8_t ucQueueType,
                                  Queue_t *pxNewQueue);

BaseType_t xQueueGenericReset(QueueHandle_t xQueue)
{
    Queue_t *const pxQueue = xQueue;

    configASSERT(pxQueue);

    taskENTER_CRITICAL();
    {
        pxQueue->u.xQueue.pcTail = pxQueue->pcHead + (pxQueue->uxLength * pxQueue->uxItemSize); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
        pxQueue->uxMessagesWaiting = (UBaseType_t)0U;
        pxQueue->pcWriteTo = pxQueue->pcHead;
        pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead + ((pxQueue->uxLength - 1U) * pxQueue->uxItemSize); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
    }
    taskEXIT_CRITICAL();

    /* A value is returned for calling semantic consistency with previous
     * versions. */
    return pdPASS;
}
/*-----------------------------------------------------------*/

QueueHandle_t xQueueGenericCreateStatic(const UBaseType_t uxQueueLength,
                                        const UBaseType_t uxItemSize,
                                        uint8_t *pucQueueStorage,
                                        StaticQueue_t *pxStaticQueue,
                                        const uint8_t ucQueueType)
{
    Queue_t *pxNewQueue;

    configASSERT(uxQueueLength > (UBaseType_t)0);

    /* The StaticQueue_t structure and the queue storage area must be
     * supplied. */
    configASSERT(pxStaticQueue != NULL);

    /* A queue storage area should be provided if the item size is not 0, and
     * should not be provided if the item size is 0. */
    configASSERT(!((pucQueueStorage != NULL) && (uxItemSize == 0)));
    configASSERT(!((pucQueueStorage == NULL) && (uxItemSize != 0)));

#if (configASSERT_DEFINED == 1)
    {
        /* Sanity check that the size of the structure used to declare a
         * variable of type StaticQueue_t or StaticSemaphore_t equals the size of
         * the real queue and semaphore structures. */
        volatile size_t xSize = sizeof(StaticQueue_t);
        configASSERT(xSize == sizeof(Queue_t));
        (void)xSize; /* Keeps lint quiet when configASSERT() is not defined. */
    }
#endif /* configASSERT_DEFINED */

    /* The address of a statically allocated queue was passed in, use it.
     * The address of a statically allocated storage area was also passed in
     * but is already set. */
    pxNewQueue = (Queue_t *)pxStaticQueue; /*lint !e740 !e9087 Unusual cast is ok as the structures are designed to have the same alignment, and the size is checked by an assert. */

    if (pxNewQueue != NULL)
    {
        pxNewQueue->ucStaticallyAllocated = pdTRUE;
        prvInitialiseNewQueue(uxQueueLength, uxItemSize, pucQueueStorage, ucQueueType, pxNewQueue);
    }
    else
    {
        
    }

    return pxNewQueue;
}

QueueHandle_t xQueueGenericCreate(const UBaseType_t uxQueueLength,
                                  const UBaseType_t uxItemSize,
                                  const uint8_t ucQueueType)
{
    Queue_t *pxNewQueue;
    size_t xQueueSizeInBytes;
    uint8_t *pucQueueStorage;

    configASSERT(uxQueueLength > (UBaseType_t)0);

    /* Allocate enough space to hold the maximum number of items that
     * can be in the queue at any time.  It is valid for uxItemSize to be
     * zero in the case the queue is used as a semaphore. */
    xQueueSizeInBytes = (size_t)(uxQueueLength * uxItemSize); /*lint !e961 MISRA exception as the casts are only redundant for some ports. */

    /* Check for multiplication overflow. */
    configASSERT((uxItemSize == 0) || (uxQueueLength == (xQueueSizeInBytes / uxItemSize)));

    /* Check for addition overflow. */
    configASSERT((sizeof(Queue_t) + xQueueSizeInBytes) > xQueueSizeInBytes);

    /* Allocate the queue and storage area.  Justification for MISRA
     * deviation as follows:  pvPortMalloc() always ensures returned memory
     * blocks are aligned per the requirements of the MCU stack.  In this case
     * pvPortMalloc() must return a pointer that is guaranteed to meet the
     * alignment requirements of the Queue_t structure - which in this case
     * is an int8_t *.  Therefore, whenever the stack alignment requirements
     * are greater than or equal to the pointer to char requirements the cast
     * is safe.  In other cases alignment requirements are not strict (one or
     * two bytes). */
    pxNewQueue = (Queue_t *)El_Malloc(sizeof(Queue_t) + xQueueSizeInBytes); /*lint !e9087 !e9079 see comment above. */

    if (pxNewQueue != NULL)
    {
        /* Jump past the queue structure to find the location of the queue
         * storage area. */
        pucQueueStorage = (uint8_t *)pxNewQueue;
        pucQueueStorage += sizeof(Queue_t); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
        pxNewQueue->ucStaticallyAllocated = pdFALSE;
        prvInitialiseNewQueue(uxQueueLength, uxItemSize, pucQueueStorage, ucQueueType, pxNewQueue);
    }
    return pxNewQueue;
}

/*-----------------------------------------------------------*/

static void prvInitialiseNewQueue(const UBaseType_t uxQueueLength,
                                  const UBaseType_t uxItemSize,
                                  uint8_t *pucQueueStorage,
                                  const uint8_t ucQueueType,
                                  Queue_t *pxNewQueue)
{
    /* Remove compiler warnings about unused parameters should
     * configUSE_TRACE_FACILITY not be set to 1. */
    (void)ucQueueType;

    if (uxItemSize == (UBaseType_t)0)
    {
        /* No RAM was allocated for the queue storage area, but PC head cannot
         * be set to NULL because NULL is used as a key to say the queue is used as
         * a mutex.  Therefore just set pcHead to point to the queue as a benign
         * value that is known to be within the memory map. */
        pxNewQueue->pcHead = (int8_t *)pxNewQueue;
    }
    else
    {
        /* Set the head to the start of the queue storage area. */
        pxNewQueue->pcHead = (int8_t *)pucQueueStorage;
    }

    /* Initialise the queue members as described where the queue type is
     * defined. */
    pxNewQueue->uxLength = uxQueueLength;
    pxNewQueue->uxItemSize = uxItemSize;
    (void)xQueueGenericReset(pxNewQueue);
}

BaseType_t xQueueGenericSend(QueueHandle_t xQueue,
                             const void *const pvItemToQueue,
                             const BaseType_t xCopyPosition)
{
    Queue_t *const pxQueue = xQueue;

    configASSERT(pxQueue);
    configASSERT(!((pvItemToQueue == NULL) && (pxQueue->uxItemSize != (UBaseType_t)0U)));
    configASSERT(!((xCopyPosition == queueOVERWRITE) && (pxQueue->uxLength != 1)));

    /*lint -save -e904 This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    for (;;)
    {
        taskENTER_CRITICAL();
        {
            /* Is there room on the queue now?  The running task must be the
             * highest priority task wanting to access the queue.  If the head item
             * in the queue is to be overwritten then it does not matter if the
             * queue is full. */
            if ((pxQueue->uxMessagesWaiting < pxQueue->uxLength) || (xCopyPosition == queueOVERWRITE))
            {
                prvCopyDataToQueue(pxQueue, pvItemToQueue, xCopyPosition);
                taskEXIT_CRITICAL();
                return pdPASS;
            }
            else
            {
                return errQUEUE_FULL;
            }
        }
        taskEXIT_CRITICAL();
    } /*lint -restore */
}

/*-----------------------------------------------------------*/

BaseType_t xQueueReceive(QueueHandle_t xQueue,
                         void *const pvBuffer)
{
    Queue_t *const pxQueue = xQueue;

    /* Check the pointer is not NULL. */
    configASSERT((pxQueue));

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT(!(((pvBuffer) == NULL) && ((pxQueue)->uxItemSize != (UBaseType_t)0U)));

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    for (;;)
    {
        taskENTER_CRITICAL();
        {
            const UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            if (uxMessagesWaiting > (UBaseType_t)0)
            {
                /* Data available, remove one item. */
                prvCopyDataFromQueue(pxQueue, pvBuffer);
                pxQueue->uxMessagesWaiting = uxMessagesWaiting - (UBaseType_t)1;
                taskEXIT_CRITICAL();
                return pdPASS;
            }
            else
            {
                taskEXIT_CRITICAL();
                return errQUEUE_EMPTY;
            }
        }
        taskEXIT_CRITICAL();
    } /*lint -restore */
}

/*-----------------------------------------------------------*/

BaseType_t xQueuePeek(QueueHandle_t xQueue,
                      void *const pvBuffer)
{
    int8_t *pcOriginalReadPosition;
    Queue_t *const pxQueue = xQueue;

    /* Check the pointer is not NULL. */
    configASSERT((pxQueue));

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer. */
    configASSERT(!(((pvBuffer) == NULL) && ((pxQueue)->uxItemSize != (UBaseType_t)0U)));

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    for (;;)
    {
        taskENTER_CRITICAL();
        {
            const UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

            /* Is there data in the queue now?  To be running the calling task
             * must be the highest priority task wanting to access the queue. */
            if (uxMessagesWaiting > (UBaseType_t)0)
            {
                /* Remember the read position so it can be reset after the data
                 * is read from the queue as this function is only peeking the
                 * data, not removing it. */
                pcOriginalReadPosition = pxQueue->u.xQueue.pcReadFrom;
                prvCopyDataFromQueue(pxQueue, pvBuffer);
                /* The data is not being removed, so reset the read pointer. */
                pxQueue->u.xQueue.pcReadFrom = pcOriginalReadPosition;
                taskEXIT_CRITICAL();
                return pdPASS;
            }
            else
            {
                taskEXIT_CRITICAL();
                return errQUEUE_EMPTY;
            }
        }
        taskEXIT_CRITICAL();
    } /*lint -restore */
}

UBaseType_t uxQueueMessagesWaiting(const QueueHandle_t xQueue)
{
    UBaseType_t uxReturn;

    configASSERT(xQueue);

    taskENTER_CRITICAL();
    {
        uxReturn = ((Queue_t *)xQueue)->uxMessagesWaiting;
    }
    taskEXIT_CRITICAL();

    return uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t uxQueueSpacesAvailable(const QueueHandle_t xQueue)
{
    UBaseType_t uxReturn;
    Queue_t *const pxQueue = xQueue;

    configASSERT(pxQueue);

    taskENTER_CRITICAL();
    {
        uxReturn = pxQueue->uxLength - pxQueue->uxMessagesWaiting;
    }
    taskEXIT_CRITICAL();

    return uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */

/*-----------------------------------------------------------*/

void vQueueDelete(QueueHandle_t xQueue)
{
    Queue_t *const pxQueue = xQueue;

    configASSERT(pxQueue);
    if(pxQueue->ucStaticallyAllocated==pdFALSE)
    {
        El_Free(pxQueue);
    }
}

/*-----------------------------------------------------------*/

static BaseType_t prvCopyDataToQueue(Queue_t *const pxQueue,
                                     const void *pvItemToQueue,
                                     const BaseType_t xPosition)
{
    BaseType_t xReturn = pdFALSE;
    UBaseType_t uxMessagesWaiting;

    /* This function is called from a critical section. */

    uxMessagesWaiting = pxQueue->uxMessagesWaiting;

    if (pxQueue->uxItemSize == (UBaseType_t)0)
    {
    }
    else if (xPosition == queueSEND_TO_BACK)
    {
        (void)memcpy((void *)pxQueue->pcWriteTo, pvItemToQueue, (size_t)pxQueue->uxItemSize); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports, plus previous logic ensures a null pointer can only be passed to memcpy() if the copy size is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
        pxQueue->pcWriteTo += pxQueue->uxItemSize;                                            /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        if (pxQueue->pcWriteTo >= pxQueue->u.xQueue.pcTail) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            pxQueue->pcWriteTo = pxQueue->pcHead;
        }
        else
        {
            
        }
    }
    else
    {
        (void)memcpy((void *)pxQueue->u.xQueue.pcReadFrom, pvItemToQueue, (size_t)pxQueue->uxItemSize); /*lint !e961 !e9087 !e418 MISRA exception as the casts are only redundant for some ports.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes.  Assert checks null pointer only used when length is 0. */
        pxQueue->u.xQueue.pcReadFrom -= pxQueue->uxItemSize;

        if (pxQueue->u.xQueue.pcReadFrom < pxQueue->pcHead) /*lint !e946 MISRA exception justified as comparison of pointers is the cleanest solution. */
        {
            pxQueue->u.xQueue.pcReadFrom = (pxQueue->u.xQueue.pcTail - pxQueue->uxItemSize);
        }
        else
        {
            
        }

        if (xPosition == queueOVERWRITE)
        {
            if (uxMessagesWaiting > (UBaseType_t)0)
            {
                /* An item is not being added but overwritten, so subtract
                 * one from the recorded number of items in the queue so when
                 * one is added again below the number of recorded items remains
                 * correct. */
                --uxMessagesWaiting;
            }
            else
            {
                
            }
        }
        else
        {
            
        }
    }

    pxQueue->uxMessagesWaiting = uxMessagesWaiting + (UBaseType_t)1;

    return xReturn;
}
/*-----------------------------------------------------------*/

static void prvCopyDataFromQueue(Queue_t *const pxQueue,
                                 void *const pvBuffer)
{
    if (pxQueue->uxItemSize != (UBaseType_t)0)
    {
        pxQueue->u.xQueue.pcReadFrom += pxQueue->uxItemSize; /*lint !e9016 Pointer arithmetic on char types ok, especially in this use case where it is the clearest way of conveying intent. */

        if (pxQueue->u.xQueue.pcReadFrom >= pxQueue->u.xQueue.pcTail) /*lint !e946 MISRA exception justified as use of the relational operator is the cleanest solutions. */
        {
            pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead;
        }
        else
        {
            
        }

        (void)memcpy((void *)pvBuffer, (void *)pxQueue->u.xQueue.pcReadFrom, (size_t)pxQueue->uxItemSize); /*lint !e961 !e418 !e9087 MISRA exception as the casts are only redundant for some ports.  Also previous logic ensures a null pointer can only be passed to memcpy() when the count is 0.  Cast to void required by function signature and safe as no alignment requirement and copy length specified in bytes. */
    }
}

static BaseType_t prvIsQueueEmpty(const Queue_t *pxQueue)
{
    BaseType_t xReturn;

    taskENTER_CRITICAL();
    {
        if (pxQueue->uxMessagesWaiting == (UBaseType_t)0)
        {
            xReturn = pdTRUE;
        }
        else
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL();

    return xReturn;
}

/*-----------------------------------------------------------*/

static BaseType_t prvIsQueueFull(const Queue_t *pxQueue)
{
    BaseType_t xReturn;

    taskENTER_CRITICAL();
    {
        if (pxQueue->uxMessagesWaiting == pxQueue->uxLength)
        {
            xReturn = pdTRUE;
        }
        else
        {
            xReturn = pdFALSE;
        }
    }
    taskEXIT_CRITICAL();

    return xReturn;
}
