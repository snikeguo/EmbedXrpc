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
typedef bool Boolean;

#include <FreeRTOS.h>
#define MALLOC	pvPortMalloc
#define FREE	vPortFree
#define MEMCPY	memcpy
#define Debug(filter_string,...)  //rt_kprintf
#define EmbedSerializationAssert configASSERT


#endif // EmbedSerialization_Port_H
