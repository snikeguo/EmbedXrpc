#ifndef EmbedSerialization_Port_H
#define EmbedSerialization_Port_H

#ifdef __cplusplus
#include <cstdint>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
typedef bool Boolean;
#else
#include "stdint.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
typedef uint8_t Boolean;
#endif
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

typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Mutex_t;
typedef void* EmbedXrpc_Thread_t;
typedef void* EmbedXrpc_Queue_t;
typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Timer_t;

#define FilterStringHeader	{"EmbedXrpcObject"}
#define StringOutput(...)  vprintf(__VA_ARGS__)
void* Malloc(uint32_t size);
void Free(void* ptr);
void Memcpy(void* d, const void* s, uint32_t size);
void Memset(void* d, int v, uint32_t size);
#define EmbedSerializationAssert assert



#endif // EmbedSerialization_Port_H
