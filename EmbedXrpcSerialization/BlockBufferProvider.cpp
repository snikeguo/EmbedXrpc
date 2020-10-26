#include "BlockBufferProvider.h"
BlockRingBufferProvider::BlockRingBufferProvider(uint8_t* pool, uint16_t size, uint32_t queue_item_size)
{
	if (size == 0 || pool == nullptr)
		return;
	CalculateSumValue = 0;
	ReferenceSumValue = 0;

	Pool = pool;
	Size = size;

	rt_ringbuffer_init(&RingBuffer, pool, size);
	Queue = EmbedXrpc_CreateQueue("ringbufqueue", sizeof(ReceiveItemInfo), queue_item_size);
	Mutex = EmbedXrpc_CreateMutex("ringbufmutex");
}
BlockRingBufferProvider::~BlockRingBufferProvider()
{
	if (Size == 0 || Pool == nullptr)
		return;
	rt_ringbuffer_reset(&RingBuffer);
	EmbedXrpc_DeleteMutex(Mutex);
	EmbedXrpc_DeleteQueue(Queue);
}
bool BlockRingBufferProvider::GetChar(uint8_t* ch)
{
	if (Size == 0 || Pool == nullptr)
		return false;
	uint8_t tch = 0;
	uint32_t size = 0;
	EmbedXrpc_TakeMutex(Mutex, EmbedXrpc_WAIT_FOREVER);
	size = rt_ringbuffer_getchar(&RingBuffer, &tch);
	EmbedXrpc_ReleaseMutex(Mutex);
	if (size == 0)
	{
		return false;
	}
	if(ch!=nullptr)
		*ch = tch;
	CalculateSumValue += tch;
	return  true;
}
bool BlockRingBufferProvider::ViewChar(uint8_t* ch,uint16_t offset)
{
	if (Size == 0 || Pool == nullptr)
		return false;
	uint8_t tch = 0;
	uint32_t size = 0;
	EmbedXrpc_TakeMutex(Mutex, EmbedXrpc_WAIT_FOREVER);
	size = rt_ringbuffer_viewchar(&RingBuffer, &tch,offset);
	EmbedXrpc_ReleaseMutex(Mutex);
	if (size == 0)
	{
		return false;
	}
	if (ch != nullptr)
		*ch = tch;
	return  true;
}
bool BlockRingBufferProvider::PopChars(uint8_t* ch, uint16_t len)
{
	/*for (uint16_t i = 0; i < len; i++)
	{
		if (ch != nullptr)
		{
			if (GetChar(&ch[i]) == false) 
				return false;
		}
		else
		{
			if (GetChar(nullptr) == false)
				return false;
		}
	}*/
	uint8_t tch=0;
	EmbedXrpc_TakeMutex(Mutex, EmbedXrpc_WAIT_FOREVER);
	for (uint16_t i = 0; i < len; i++)
	{
		rt_ringbuffer_getchar(&RingBuffer, &tch);
		CalculateSumValue += tch;
		if (ch != nullptr)
		{
			ch[i] = tch;
		}
	}
	EmbedXrpc_ReleaseMutex(Mutex);
	return true;
}
bool BlockRingBufferProvider::Receive(ReceiveItemInfo* item, uint32_t timeout)
{
	if (Size == 0 || Pool == nullptr)
		return false;
	auto r = EmbedXrpc_ReceiveQueue(Queue, item, sizeof(ReceiveItemInfo), timeout) == QueueState_OK ? true : false;
	return r;
}
uint32_t BlockRingBufferProvider::CalculateSum(uint8_t* d, uint16_t len)
{
	uint32_t sum = 0;
	int16_t i = 0;
	for (i = 0; i < len; i++)
	{
		sum += d[i];
	}
	return sum;
}
bool BlockRingBufferProvider::Send(ReceiveItemInfo* item,uint8_t* buf, uint16_t bufLen)
{
	if (Size == 0 || Pool == nullptr)
		return false;

	int16_t putlen = 0;
	//bool insert_flag = false;
	bool result = false;

	EmbedXrpc_TakeMutex(Mutex, EmbedXrpc_WAIT_FOREVER);
	if (rt_ringbuffer_space_len(&RingBuffer) >= bufLen && EmbedXrpc_QueueSpacesAvailable(Queue)>0)
	{
		//insert_flag = true;
		putlen = rt_ringbuffer_put(&RingBuffer, buf, bufLen);
		if (putlen != bufLen)
		{
			result=false;
			goto _unlock;
		}
		//item->DataLen = bufLen;
		//item->CheckSum = CalculateSum(buf, bufLen);
		if (EmbedXrpc_SendQueue(Queue, item, sizeof(ReceiveItemInfo)) == QueueState_OK)
		{
			result = true;
			goto _unlock;
		}
		else
		{
			result = false;
			goto _unlock;
		}
	}
_unlock:
	EmbedXrpc_ReleaseMutex(Mutex);
	return result;
}
void BlockRingBufferProvider::Reset()
{
	if (Size == 0 || Pool == nullptr)
		return ;
	rt_ringbuffer_reset(&RingBuffer);
	EmbedXrpc_ResetQueue(Queue);
}