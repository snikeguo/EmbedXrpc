#ifndef EmbedSerialization_Port_H
#define EmbedSerialization_Port_H

#ifdef __cplusplus
#include "stdint.h"
#else
#include "stdint.h"
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "stdio.h"
typedef  uint8_t Byte;
typedef  uint8_t UInt8;
typedef  int8_t SByte;
typedef  int8_t Int8;
typedef  uint16_t UInt16;
typedef  int16_t Int16;
typedef  uint32_t UInt32;
typedef  int32_t Int32;
typedef  uint64_t UInt64;
typedef  int64_t Int64;
typedef  float Float;
typedef  double Double;
typedef BaseType_t Boolean;

#ifndef Bool
#define Bool uint8_t
#endif

#ifndef True
#define True 1
#endif

#ifndef False
#define False 0
#endif

typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Mutex_t;
typedef void* EmbedXrpc_Thread_t;
typedef void* EmbedXrpc_Queue_t;
typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Timer_t;

#define FilterStringHeader	{"EmbedXrpcPort"}
extern "C" void rt_kprintf(const char *fmt, ...);
#define StringOutput(...)  vprintf(__VA_ARGS__)
#define EmbedSerializationAssert configASSERT


#endif // EmbedSerialization_Port_H
