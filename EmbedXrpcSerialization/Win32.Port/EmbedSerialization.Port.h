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



#define FilterStringHeader	{"NONE"}
#define StringOutput(...)  vprintf(__VA_ARGS__)
void* Malloc(uint32_t size);
void Free(void* ptr);
void Memcpy(void* d, const void* s, uint32_t size);
#define EmbedSerializationAssert assert



#endif // EmbedSerialization_Port_H
