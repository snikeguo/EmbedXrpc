#include "SoftCrc32.h"
#include "stdint.h"



static const uint32_t Crc32_Table[]=
{//(0xEDB88320L) CRC32 Ethernet Polynomial
  0x00000000,
  0x1db71064,
  0x3b6e20c8,
  0x26d930ac,
  0x76dc4190,
  0x6b6b51f4,
  0x4db26158,
  0x5005713c,
  0xedb88320,
  0xf00f9344,
  0xd6d6a3e8,
  0xcb61b38c,
  0x9b64c2b0,
  0x86d3d2d4,
  0xa00ae278,
  0xbdbdf21c,
};

#define CRC32_INDEX(x,y) ((uint8_t)(((x)^(y))&0x0F))
#define RIGHT_NIBBLE_SHIFT(x) ((x) >> 4)
/*****************************************************************************
 *  void UpdateCrc32(uint8_t Data, uint32_t *pCrc)
 *
 *  Add data to current crc. (right shift 4 bits every step)
 *  CRC32 is used. Polynomial word is CRYLIB_CRC_POLY.
 *  The computing is done on byte nibble with table Crc32_Table.
 *
 *  Parameter:
 *  Data - input byte value
 *  pCrc - pointer to current crc / contains the updated CRC after return
 *
 *  Return: no return value defined
 *
 ****************************************************************************/
void UpdateCrc32(uint8_t Data, uint32_t *pCrc)
{
  uint32_t TempCrc;   /* store pCrc temporary */

  TempCrc = Crc32_Table[CRC32_INDEX(*pCrc, Data)] ^ RIGHT_NIBBLE_SHIFT(*pCrc);/* compute low nibble of input Data */
  Data = RIGHT_NIBBLE_SHIFT(Data);/* compute high nibble of input Data */
  *pCrc = Crc32_Table[CRC32_INDEX(TempCrc, Data)] ^ RIGHT_NIBBLE_SHIFT(TempCrc);

  return;
}
void Crc32Init(CrcCalculate_t *obj,uint32_t initValue)
{
  obj->CurrentValue=initValue;
}
void Crc32Compute(CrcCalculate_t* obj,uint8_t *data,uint32_t len)
{
  while(len > 0)
  {
    UpdateCrc32(*data, &obj->CurrentValue);/* add one byte to CRC */
    len--;/* decrease Counter */
    data++;/* next byte */
  }
}
uint32_t Crc32Finish(CrcCalculate_t* obj)
{
   obj->CurrentValue ^= Crc32_OutXor;/* finalize CRC -> complementation of result */
   return obj->CurrentValue;
}


