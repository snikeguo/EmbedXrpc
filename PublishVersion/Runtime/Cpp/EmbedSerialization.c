#include "EmbedSerialization.h"
#include <stdarg.h>
#include "EmbedXrpc.Port.h"
#if EmbedXrpc_CheckSumValid==1
uint32_t GetSum(uint8_t* d, uint32_t len)
{
	uint32_t sum = 0;
	for (uint32_t i = 0; i < len; i++)
	{
		sum += d[i];
	}
	return sum;
}
#endif
#if EmbedXrpc_CheckSumValid==1
#define SerializationManagerAppendDataSum(sm,sum)    SerializationManager_SetCalculateSum(sm,SerializationManager_GetCalculateSum(sm)+sum)
#else
#define SerializationManagerAppendDataSum(sm,sum)
#endif
void  SerializationManager_Reset(SerializationManager* sm)
{
	sm->Index = 0;
}
#if EmbedXrpc_CheckSumValid==1
void SerializationManager_SetReferenceSum(SerializationManager* sm, uint32_t sum)
{
	if (sm->BlockBufferProvider != NULL)
	{
		BlockRingBufferProvider_SetReferenceSum(sm->BlockBufferProvider,sum);
	}
	else
	{
		sm->ReferenceSum = sum;
	}
}
void SerializationManager_SetCalculateSum(SerializationManager* sm, uint32_t sum)
{
	if (sm->BlockBufferProvider != NULL)
	{
		BlockRingBufferProvider_SetCalculateSum(sm->BlockBufferProvider,sum);
	}
	else
	{
		sm->CalculateSum = sum;
	}
}
uint32_t SerializationManager_GetReferenceSum(SerializationManager* sm)
{
	if (sm->BlockBufferProvider != NULL)
	{
		return BlockRingBufferProvider_GetReferenceSum(sm->BlockBufferProvider);
	}
	else
	{
		return sm->ReferenceSum;
	}
}
uint32_t SerializationManager_GetCalculateSum(SerializationManager* sm)
{
	if (sm->BlockBufferProvider != NULL)
	{
		return BlockRingBufferProvider_GetCalculateSum(sm->BlockBufferProvider);
	}
	else
	{
		return sm->CalculateSum;
	}
}

void SerializationManager_AppendSumToCalculateSum(SerializationManager* sm, uint32_t sum)//只有ringbuffer mode 为0的情况下使用。
{
	(void)sum;
#if EmbedXrpc_UseRingBufferWhenReceiving==0
	sm->CalculateSum += sum;
#endif
}

#endif
void DeserializeField(uint8_t* field_ptr, SerializationManager* sm, uint16_t field_width)
{
#if EmbedXrpc_UseRingBufferWhenReceiving==1 
	BlockRingBufferProvider_PopChars(sm->BlockBufferProvider,field_ptr, (uint16_t)field_width);
#else 
	El_Memcpy(field_ptr, &sm->Buf[sm->Index], field_width);
	SerializationManagerAppendDataSum(sm, GetSum(&sm->Buf[sm->Index], field_width));
	sm->Index += field_width;
#endif 
}

//static const char* FilterStrings[] = FilterStringHeader;
void EmbedSerializationShowMessage(const char* filter_string, const char* fmt, ...)
{
	(void)filter_string;
	return;
	/*va_list args;
	va_start(args, fmt);
	for (int i = 0; i < sizeof(FilterStrings) / sizeof(const char*); i++)
	{
		if (strcmp(FilterStrings[i], filter_string) == 0)
		{
			StringOutput(fmt, args);
		}
	}
	va_end(args);
	*/
}
