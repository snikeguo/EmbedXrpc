#ifndef El_PortInterface_H
#define El_PortInterface_H
#include "stdint.h"
#include "assert.h"
#include "string.h"
//#include "console.h"
#ifdef __cplusplus
extern "C" {
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

#define El_WAIT_FOREVER	0xFFFFFFFF

#include "cmsis_os2.h"

#define El_Strncpy strncpy
#define El_Strncmp    strncmp
#define El_Strlen      strlen
#define El_Strcat		strcat
#define El_Assert		assert
#define CallFunction(Function,...) do{if (Function != NULL){Function(__VA_ARGS__);}}while(0); //using C99 Mode.

	void* El_Malloc(uint32_t size);
	void El_Free(void* ptr);
	void El_Memcpy(void* d, const void* s, uint32_t size);
	void El_Memset(void* d, int v, uint32_t size);
#ifdef __cplusplus
}
#endif

#endif
