#include "EmbedSerialization.h"
#include <stdarg.h>
#include "EmbedXrpc.Port.h"

uint32_t GetSum(uint8_t* d, uint32_t len)
{
	uint32_t sum = 0;
	for (uint32_t i = 0; i < len; i++)
	{
		sum += d[i];
	}
	return sum;
}


void  SerializationManager_Reset(SerializationManager* sm)
{
	sm->Index = 0;
}

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
	sm->CalculateSum += sum;
}


void DeserializeField(uint8_t* field_ptr, SerializationManager* sm, uint16_t field_width)
{
	if (sm->BlockBufferProvider != NULL)
	{
		BlockRingBufferProvider_PopChars(sm->BlockBufferProvider,field_ptr, (uint16_t)field_width);
}
	else
	{
		El_Memcpy(field_ptr, &sm->Buf[sm->Index], field_width);
		SerializationManager_AppendSumToCalculateSum(sm,GetSum(&sm->Buf[sm->Index], field_width));
		sm->Index += field_width;
	}
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
