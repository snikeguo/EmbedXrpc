#include "EmbedSerialization.h"
#include <stdarg.h>
#include "EmbedLibrary.h"

uint32_t GetSum(uint8_t* d, uint32_t len)
{
	uint32_t sum = 0;
	for (uint32_t i = 0; i < len; i++)
	{
		sum += d[i];
	}
	return sum;
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
