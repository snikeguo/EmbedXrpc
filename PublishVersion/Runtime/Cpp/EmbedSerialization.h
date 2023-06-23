#ifndef EmbedSerialization_H
#define EmbedSerialization_H


#include "EmbedLibrary.h"
#include "BlockBufferProvider.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef offsetof
#define offsetof(s, m) (uint32_t)((char*)(&((s*)0)->m))
#endif

void EmbedSerializationShowMessage(const char* filter_string, const char* fmt, ...);
uint32_t GetSum(uint8_t* d, uint32_t len);
typedef  uint32_t ptr_t;

typedef enum {
	TYPE_UINT8,
	TYPE_INT8,

	TYPE_UINT16,
	TYPE_INT16,

	TYPE_UINT32,
	TYPE_INT32,

	TYPE_UINT64,
	TYPE_INT64,

	TYPE_FLOAT,
	TYPE_DOUBLE,

	TYPE_ARRAY,   /*array*/
	TYPE_OBJECT,  /*object*/


} Type_t;


typedef struct _SerializationManager
{

	uint32_t Index;
	uint8_t* Buf;
	uint32_t BufferLen;
	BlockRingBufferProvider* BlockBufferProvider;
}SerializationManager;

static inline void DeserializeField(uint8_t* field_ptr, SerializationManager* sm, uint16_t field_width_serialize, uint16_t field_width_define, int isIsr)
{
	if (sm->BlockBufferProvider != NULL)
	{
		uint8_t temp[8];
		BlockRingBufferProvider_PopChars(sm->BlockBufferProvider, temp, (uint16_t)field_width_serialize, isIsr);
		if(field_ptr!=NULL)
		El_Memcpy(field_ptr, temp, field_width_define);
}
	else
	{
		if (field_ptr != NULL)
		El_Memcpy(field_ptr, &sm->Buf[sm->Index], field_width_define >= field_width_serialize ? field_width_serialize : field_width_define);
		sm->Index += field_width_serialize;
	}
}

#ifdef __cplusplus
}
#endif

#endif
