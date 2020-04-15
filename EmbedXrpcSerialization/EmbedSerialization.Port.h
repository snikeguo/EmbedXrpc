#ifndef EmbedSerialization_Port_H
#define EmbedSerialization_Port_H

#ifdef __cplusplus
#include <cstdint>
#else
#include "stdint.h"
#endif

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

#ifndef WIN32
#include <rtthread.h>
#define MALLOC	rt_malloc
#define FREE	rt_free
#define MEMCPY	rt_memcpy
#define Debug  rt_kprintf
#else 
#include <cstring>
#include <cstdlib>
#include <cstdio>
#define MALLOC	malloc
#define FREE	free
#define MEMCPY	memcpy
#define Debug  
#endif

#endif // EmbedSerialization_Port_H
