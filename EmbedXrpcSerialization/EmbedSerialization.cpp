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
void DeserializeField(uint8_t* field_ptr, SerializationManager* sm, uint16_t field_width)
{
	if (sm->BlockBufferProvider != nullptr)
	{
		sm->BlockBufferProvider->PopChars(field_ptr, (uint16_t)field_width);
	}
	else
	{
		El_Memcpy(field_ptr, &sm->Buf[sm->Index], field_width);
		sm->AppendSumToCalculateSum(GetSum(&sm->Buf[sm->Index], field_width));
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
