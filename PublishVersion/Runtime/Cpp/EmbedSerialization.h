#ifndef EmbedSerialization_H
#define EmbedSerialization_H


#include "EmbedLibrary.h"
#include "BlockBufferProvider.h"

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

//template<class DTL>
class  BlockRingBufferProvider;


class SerializationManager
{
public:
	uint32_t Index;
	uint8_t* Buf;
	uint32_t BufferLen;
	BlockRingBufferProvider* BlockBufferProvider;

	uint32_t ReferenceSum ;
	uint32_t CalculateSum ;

	void  Reset() 
	{
		Index = 0;
		Buf = nullptr;
		BufferLen = 0;
		BlockBufferProvider = nullptr;
		ReferenceSum = 0;
		CalculateSum = 0;
	}
	void SetReferenceSum(uint32_t sum)
	{
		if (BlockBufferProvider != NULL)
		{
			BlockBufferProvider->SetReferenceSum(sum);
		}
		else
		{
			ReferenceSum = sum;
		}
	}
	void SetCalculateSum(uint32_t sum)
	{
		if (BlockBufferProvider != NULL)
		{
			BlockBufferProvider->SetCalculateSum(sum);
		}
		else
		{
			CalculateSum = sum;
		}
	}
	uint32_t GetReferenceSum()
	{
		if (BlockBufferProvider != NULL)
		{
			return BlockBufferProvider->GetReferenceSum();
		}
		else
		{
			return ReferenceSum;
		}
	}
	uint32_t GetCalculateSum()
	{
		if (BlockBufferProvider != NULL)
		{
			return BlockBufferProvider->GetCalculateSum();
		}
		else
		{
			return CalculateSum;
		}
	}
	void AppendSumToCalculateSum(uint32_t sum)//只有ringbuffer mode 为0的情况下使用。
	{
		CalculateSum += sum;
	}
	
};
void DeserializeField(uint8_t* field_ptr, SerializationManager* sm, uint16_t field_width);
#endif
