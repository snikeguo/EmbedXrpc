

#ifndef NOOS_QUEUE_H
#define NOOS_QUEUE_H

#include <stdlib.h>
#include <string.h>
#include "stdint.h"
#include "assert.h"
#include "EmbedLibrary.h"

#ifndef configASSERT
#define configASSERT    assert
#endif

typedef long BaseType_t;
typedef unsigned long UBaseType_t;
typedef unsigned TickType_t;
#define pdFALSE ((BaseType_t)0)
#define pdTRUE ((BaseType_t)1)

#define pdPASS (pdTRUE)
#define pdFAIL (pdFALSE)
#define errQUEUE_EMPTY ((BaseType_t)0)
#define errQUEUE_FULL ((BaseType_t)0)

#define configSUPPORT_STATIC_ALLOCATION 1
#define configSUPPORT_DYNAMIC_ALLOCATION 1
#define configASSERT_DEFINED    1

/* When the Queue_t structure is used to represent a base queue its pcHead and
 * pcTail members are used as pointers into the queue storage area.  When the
 * Queue_t structure is used to represent a mutex pcHead and pcTail pointers are
 * not necessary, and the pcHead pointer is set to NULL to indicate that the
 * structure instead holds a pointer to the mutex holder (if any).  Map alternative
 * names to the pcHead and structure member to ensure the readability of the code
 * is maintained.  The QueuePointers_t and SemaphoreData_t types are used to form
 * a union as their usage is mutually exclusive dependent on what the queue is
 * being used for. */
#define uxQueueType pcHead

typedef struct QueuePointers
{
    int8_t* pcTail;     /*< Points to the byte at the end of the queue storage area.  Once more byte is allocated than necessary to store the queue items, this is used as a marker. */
    int8_t* pcReadFrom; /*< Points to the last place that a queued item was read from when the structure is used as a queue. */
} QueuePointers_t;
/*
 * Definition of the queue used by the scheduler.
 * Items are queued by copy, not reference.  See the following link for the
 * rationale: https://www.FreeRTOS.org/Embedded-RTOS-Queues.html
 */
typedef struct QueueDefinition /* The old naming convention is used to prevent breaking kernel aware debuggers. */
{
    int8_t* pcHead;    /*< Points to the beginning of the queue storage area. */
    int8_t* pcWriteTo; /*< Points to the free next place in the storage area. */

    union
    {
        QueuePointers_t xQueue; /*< Data required exclusively when this structure is used as a queue. */
    } u;

    volatile UBaseType_t uxMessagesWaiting; /*< The number of items currently in the queue. */
    UBaseType_t uxLength;                   /*< The length of the queue defined as the number of items it will hold, not the number of bytes. */
    UBaseType_t uxItemSize;                 /*< The size of each items that the queue will hold. */

    uint8_t ucStaticallyAllocated; /*< Set to pdTRUE if the memory used by the queue was statically allocated to ensure no attempt is made to free the memory. */

} xQUEUE;

typedef struct xSTATIC_QUEUE
{
    void* pvDummy1[2];
    union
    {
        //void* pvDummy2;
        //UBaseType_t uxDummy2;
        QueuePointers_t pvDummy2;
    } u;
    UBaseType_t uxDummy4[3];
    uint8_t ucDummy5[1];
} StaticQueue_t;

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
    void* pvPortMalloc(size_t size);
    void vPortFree(void* ptr);
/**
 * Type by which queues are referenced.  For example, a call to xQueueCreate()
 * returns an QueueHandle_t variable that can then be used as a parameter to
 * xQueueSend(), xQueueReceive(), etc.
 */
struct QueueDefinition; /* Using old naming convention so as not to break kernel aware debuggers. */
typedef struct QueueDefinition   * QueueHandle_t;

typedef struct QueueDefinition   * QueueSetHandle_t;

typedef struct QueueDefinition   * QueueSetMemberHandle_t;
/* For internal use only. */
#define queueSEND_TO_BACK                     ( ( BaseType_t ) 0 )
#define queueSEND_TO_FRONT                    ( ( BaseType_t ) 1 )
#define queueOVERWRITE                        ( ( BaseType_t ) 2 )

/* For internal use only.  These definitions *must* match those in queue.c. */
#define queueQUEUE_TYPE_BASE                  ( ( uint8_t ) 0U )
#define queueQUEUE_TYPE_SET                   ( ( uint8_t ) 0U )

#define xQueueCreate( uxQueueLength, uxItemSize )    xQueueGenericCreate( ( uxQueueLength ), ( uxItemSize ), ( queueQUEUE_TYPE_BASE ) )
#define xQueueCreateStatic( uxQueueLength, uxItemSize, pucQueueStorage, pxQueueBuffer )    xQueueGenericCreateStatic( ( uxQueueLength ), ( uxItemSize ), ( pucQueueStorage ), ( pxQueueBuffer ), ( queueQUEUE_TYPE_BASE ) )


#define xQueueSendToFront( xQueue, pvItemToQueue) \
    xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), queueSEND_TO_FRONT)
#define xQueueSendToBack( xQueue, pvItemToQueue ) \
    xQueueGenericSend( ( xQueue ), ( pvItemToQueue ), queueSEND_TO_BACK )
#define xQueueSend( xQueue, pvItemToQueue ) \
    xQueueGenericSend( ( xQueue ), ( pvItemToQueue ),queueSEND_TO_BACK )
#define xQueueOverwrite( xQueue, pvItemToQueue ) \
    xQueueGenericSend( ( xQueue ), ( pvItemToQueue ),queueOVERWRITE)

BaseType_t xQueueGenericSend( QueueHandle_t xQueue,
                              const void * const pvItemToQueue,
                              const BaseType_t xCopyPosition ) ;
BaseType_t xQueuePeek( QueueHandle_t xQueue,
                       void * const pvBuffer) ;
BaseType_t xQueueReceive( QueueHandle_t xQueue,
                          void * const pvBuffer) ;
UBaseType_t uxQueueMessagesWaiting( const QueueHandle_t xQueue ) ;

UBaseType_t uxQueueSpacesAvailable( const QueueHandle_t xQueue ) ;
void vQueueDelete( QueueHandle_t xQueue ) ;
#define xQueueReset( xQueue )    xQueueGenericReset( xQueue )


QueueHandle_t xQueueGenericCreate( const UBaseType_t uxQueueLength,
                                       const UBaseType_t uxItemSize,
                                       const uint8_t ucQueueType ) ;



QueueHandle_t xQueueGenericCreateStatic( const UBaseType_t uxQueueLength,
                                             const UBaseType_t uxItemSize,
                                             uint8_t * pucQueueStorage,
                                             StaticQueue_t * pxStaticQueue,
                                             const uint8_t ucQueueType ) ;

BaseType_t xQueueGenericReset( QueueHandle_t xQueue) ;

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* QUEUE_H */
