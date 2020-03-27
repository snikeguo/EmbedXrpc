#ifndef BaseType_H
#define BaseType_H

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
 typedef  float Float;
 typedef  double Double;
template<class T>
class Option
{
public :
    int IsSuccess;
    T Value;
};
class IDelegate
{
    override void Invoke(uint8_t* data, uint32_t len);
};
#endif