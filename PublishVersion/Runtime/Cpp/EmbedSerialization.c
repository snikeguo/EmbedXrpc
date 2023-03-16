#include "EmbedSerialization.h"
#include <stdarg.h>
#include "EmbedXrpc.Port.h"

void DeserializeField(uint8_t* field_ptr, SerializationManager* sm, uint16_t field_width_serialize,uint16_t field_width_define,int isIsr)
{
	if (sm->BlockBufferProvider != NULL)
	{
		uint8_t temp[8];
		BlockRingBufferProvider_PopChars(sm->BlockBufferProvider, temp, (uint16_t)field_width_serialize,isIsr);
		El_Memcpy(field_ptr, temp, field_width_define);
	}
	else
	{
		El_Memcpy(field_ptr, &sm->Buf[sm->Index], field_width_define>= field_width_serialize? field_width_serialize: field_width_define);
		sm->Index += field_width_serialize;
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
