
#include "queue_noos_and_win32.h"


/* The old xQUEUE name is maintained above then typedefed to the new Queue_t
 * name below to enable the use of older kernel aware debuggers. */
typedef xQUEUE Queue_t;

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

    El_TakeMutex(xQueue->DataMutex, -1, 0); //taskENTER_CRITICAL(xQueue);
    {
        pxQueue->u.xQueue.pcTail = pxQueue->pcHead + (pxQueue->uxLength * pxQueue->uxItemSize); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
        pxQueue->uxMessagesWaiting = (UBaseType_t)0U;
        pxQueue->pcWriteTo = pxQueue->pcHead;
        pxQueue->u.xQueue.pcReadFrom = pxQueue->pcHead + ((pxQueue->uxLength - 1U) * pxQueue->uxItemSize); /*lint !e9016 Pointer arithmetic allowed on char types, especially when it assists conveying intent. */
        El_ResetSemaphore(pxQueue->NotifySemaphore, 0);//复位信号量 
    }
    El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);

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
        pxNewQueue->DataMutex = El_CreateMutex("Queue.DataMutex");
        pxNewQueue->NotifySemaphore = El_CreateSemaphore("Queue.NotifySemaphore");
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
        pxNewQueue->DataMutex = El_CreateMutex("Queue.DataMutex");
        pxNewQueue->NotifySemaphore = El_CreateSemaphore("Queue.NotifySemaphore");
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
    El_TakeMutex(xQueue->DataMutex, -1, 0); //taskENTER_CRITICAL(xQueue);
    {
        /* Is there room on the queue now?  The running task must be the
         * highest priority task wanting to access the queue.  If the head item
         * in the queue is to be overwritten then it does not matter if the
         * queue is full. */
        if ((pxQueue->uxMessagesWaiting < pxQueue->uxLength) || (xCopyPosition == queueOVERWRITE))
        {
            prvCopyDataToQueue(pxQueue, pvItemToQueue, xCopyPosition);
            El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
            El_ReleaseSemaphore(xQueue->NotifySemaphore, 0);//通知任务
            return pdPASS;
        }
        else
        {
            El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
            return errQUEUE_FULL;
        }
    }
    El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
    return errQUEUE_FULL;
}

/*-----------------------------------------------------------*/

BaseType_t xQueueReceive(QueueHandle_t xQueue,
                         void *const pvBuffer, TickType_t xTicksToWait)
{
    Queue_t *const pxQueue = xQueue;
    QueueState qs = QueueState_Empty;
    /* Check the pointer is not NULL. */
    configASSERT((pxQueue));

    /* The buffer into which data is received can only be NULL if the data size
     * is zero (so no data is copied into the buffer). */
    configASSERT(!(((pvBuffer) == NULL) && ((pxQueue)->uxItemSize != (UBaseType_t)0U)));

    /*lint -save -e904  This function relaxes the coding standard somewhat to
     * allow return statements within the function itself.  This is done in the
     * interest of execution time efficiency. */
    if (uxQueueMessagesWaiting(xQueue) == 0)
    {
        qs = El_TakeSemaphore(xQueue->NotifySemaphore, xTicksToWait, 0);
    }
    El_TakeMutex(xQueue->DataMutex, -1, 0); //taskENTER_CRITICAL(xQueue);
    {
        const UBaseType_t uxMessagesWaiting = pxQueue->uxMessagesWaiting;

        /* Is there data in the queue now?  To be running the calling task
         * must be the highest priority task wanting to access the queue. */
        if (uxMessagesWaiting > (UBaseType_t)0)
        {
            /* Data available, remove one item. */
            prvCopyDataFromQueue(pxQueue, pvBuffer);
            pxQueue->uxMessagesWaiting = uxMessagesWaiting - (UBaseType_t)1;
            El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
            return pdPASS;
        }
        else
        {
            El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
            return errQUEUE_EMPTY;
        }
    }
    El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
    return errQUEUE_EMPTY;
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
    El_TakeMutex(xQueue->DataMutex, -1, 0); //taskENTER_CRITICAL(xQueue);
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
            El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
            return pdPASS;
        }
        else
        {
            El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
            return errQUEUE_EMPTY;
        }
    }
    El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);
    return errQUEUE_EMPTY;
}

UBaseType_t uxQueueMessagesWaiting(const QueueHandle_t xQueue)
{
    UBaseType_t uxReturn;

    configASSERT(xQueue);

    El_TakeMutex(xQueue->DataMutex, -1, 0); //taskENTER_CRITICAL(xQueue);
    {
        uxReturn = ((Queue_t *)xQueue)->uxMessagesWaiting;
    }
    El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);

    return uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */
/*-----------------------------------------------------------*/

UBaseType_t uxQueueSpacesAvailable(const QueueHandle_t xQueue)
{
    UBaseType_t uxReturn;
    Queue_t *const pxQueue = xQueue;

    configASSERT(pxQueue);

    El_TakeMutex(xQueue->DataMutex, -1, 0); //taskENTER_CRITICAL(xQueue);
    {
        uxReturn = pxQueue->uxLength - pxQueue->uxMessagesWaiting;
    }
    El_ReleaseMutex(xQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);

    return uxReturn;
} /*lint !e818 Pointer cannot be declared const as xQueue is a typedef not pointer. */

/*-----------------------------------------------------------*/

void vQueueDelete(QueueHandle_t xQueue)
{
    Queue_t *const pxQueue = xQueue;

    configASSERT(pxQueue);
    El_DeleteMutex(pxQueue->DataMutex);
    pxQueue->DataMutex = NULL;
    El_DeleteSemaphore(pxQueue->NotifySemaphore);
    pxQueue->NotifySemaphore = NULL;
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

    El_TakeMutex(pxQueue->DataMutex, -1, 0); //taskENTER_CRITICAL(xQueue);
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
    El_ReleaseMutex(pxQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);

    return xReturn;
}

/*-----------------------------------------------------------*/

static BaseType_t prvIsQueueFull(const Queue_t *pxQueue)
{
    BaseType_t xReturn;

    El_TakeMutex(pxQueue->DataMutex, -1, 0); //taskENTER_CRITICAL(xQueue);
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
    El_ReleaseMutex(pxQueue->DataMutex, 0);//taskEXIT_CRITICAL(xQueue);

    return xReturn;
}
