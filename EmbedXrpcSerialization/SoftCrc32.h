
#ifndef __SOFTCRC32_H__
#define __SOFTCRC32_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include "stdint.h"
#define Crc32_InitValue   (0xFFFFFFFF)
#define Crc32_OutXor      (0xFFFFFFFF)//(0x00000000)

  typedef struct
  {
    uint32_t CurrentValue; /* current Check value */
  } CrcCalculate_t;

  void Crc32Init(CrcCalculate_t *obj, uint32_t initValue);
  void Crc32Compute(CrcCalculate_t *obj, uint8_t *data, uint32_t len);
  uint32_t Crc32Finish(CrcCalculate_t *obj);
#ifdef __cplusplus
}
#endif

#endif
