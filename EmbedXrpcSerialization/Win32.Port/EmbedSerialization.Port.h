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

#include <cstring>
#include <cstdlib>
#include <cassert>
#include <cstdio>
#define FilterStringHeader	{"NONE"}
#define StringOutput(...)  vprintf(__VA_ARGS__)
#define MALLOC	MyMalloc
#define FREE	MyFree
#define MEMCPY	memcpy
#define EmbedSerializationAssert assert

void* MyMalloc(size_t size);
void MyFree(void* ptr);

#endif // EmbedSerialization_Port_H
