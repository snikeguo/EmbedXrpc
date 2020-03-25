#include"EmbedXrpcBaseType.h"
typedef enum _Sex_t
{
Man = 0,
Woman = 1,
}Sex_t;
typedef struct _Result_t
{
Int32 Value;
Byte NameLen;
Byte Name[32];
SByte Arg1;
}Result_t;
typedef struct _Student_t
{
UInt16 ResultsLen2;
Result_t Results[32];
Int32 Age;
Byte NameLen;
Byte* Name;
Byte StudentIdLen;
Int16 StudentId[100];
Sex_t Sex;
}Student_t;
typedef struct _DateTime_t
{
Int32 Year;
Int32 Month;
Int32 Day;
Int32 Hour;
Int32 Min;
Int32 Sec;
}DateTime_t;
typedef struct _StudentArray_t
{
Byte StudentIdLen;
Student_t Students[64];
}StudentArray_t;
typedef void (*BroadcastDataTime)(DateTime_t t);
