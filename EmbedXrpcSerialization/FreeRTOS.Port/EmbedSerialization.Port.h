#ifndef EmbedSerialization_Port_H
#define EmbedSerialization_Port_H

#ifdef __cplusplus
#include <cstdint>
#else
#include "stdint.h"
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "string.h"
#include "stdio.h"
typedef  uint8_t Byte;
typedef  int8_t SByte;
typedef  uint16_t UInt16;
typedef  int16_t Int16;
typedef  uint32_t UInt32;
typedef  int32_t Int32;
typedef  uint64_t UInt64;
typedef  int64_t Int64;
typedef  float Float;
typedef  double Double;
typedef BaseType_t Boolean;

#define FilterStringHeader	{"EmbedXrpcPort"}
extern "C" void rt_kprintf(const char *fmt, ...);
#define StringOutput(...)  vprintf(__VA_ARGS__)
void* Malloc(uint32_t size);
void Free(void* ptr);
void Memcpy(void* d, const void* s, uint32_t size);
#define EmbedSerializationAssert configASSERT


#endif // EmbedSerialization_Port_H
