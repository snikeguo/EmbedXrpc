#include "BlockBufferProvider.h"
BlockRingBufferProvider::BlockRingBufferProvider(uint8_t* pool, int16_t size, IEmbeXrpcPort* porter)
{
	CalculateSumValue = 0;
	ReferenceSumValue = 0;

	Pool = pool;
	Size = size;
	Porter = porter;
	rt_ringbuffer_init(&RingBuffer, pool, size);
	Queue = porter->CreateQueue("ringbufqueue", sizeof(BlockBufferItemInfo), 5);
	Mutex = porter->CreateMutex("ringbufmutex");
}
BlockRingBufferProvider::~BlockRingBufferProvider()
{
	rt_ringbuffer_reset(&RingBuffer);
	Porter->DeleteMutex(Mutex);
	Porter->DeleteQueue(Queue);
}
bool BlockRingBufferProvider::GetChar(uint8_t* ch)
{
	uint8_t tch = 0;
	uint32_t size = 0;
	Porter->TakeMutex(Mutex, EmbedXrpc_WAIT_FOREVER);
	size = rt_ringbuffer_getchar(&RingBuffer, &tch);
	Porter->ReleaseMutex(Mutex);
	if (size == 0)
	{
		return false;
	}
	*ch = tch;
	CalculateSumValue += tch;
	return  true;
}
bool BlockRingBufferProvider::Receive(BlockBufferItemInfo* item, uint32_t timeout)
{
	auto r = Porter->ReceiveQueue(Queue, item, sizeof(BlockBufferItemInfo), timeout) == QueueState_OK ? true : false;
	return r;
}
uint32_t BlockRingBufferProvider::CalculateSum(uint8_t* d, int16_t len)
{
	uint32_t sum = 0;
	int16_t i = 0;
	for (i = 0; i < len; i++)
	{
		sum += d[i];
	}
	return sum;
}
bool BlockRingBufferProvider::Send(BlockBufferItemInfo* item,uint8_t* buf, int16_t bufLen)
{
	int16_t putlen = 0;

	Porter->TakeMutex(Mutex, EmbedXrpc_WAIT_FOREVER);
	putlen = rt_ringbuffer_put(&RingBuffer, buf, bufLen);
	Porter->ReleaseMutex(Mutex);
	if (putlen != bufLen)
	{
		return false;
	}
	item->DataLen = bufLen;
	item->CheckSum = CalculateSum(buf, bufLen);
	if (Porter->SendQueue(Queue, item, sizeof(BlockBufferItemInfo)) == QueueState_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void BlockRingBufferProvider::Reset()
{
	rt_ringbuffer_reset(&RingBuffer);
	Porter->ResetQueue(Queue);
}
