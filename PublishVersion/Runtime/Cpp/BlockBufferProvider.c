#include "BlockBufferProvider.h"
void BlockRingBufferProvider_Init(BlockRingBufferProvider* obj, uint8_t* pool, uint16_t size, uint32_t queue_item_max_number)
{
	if (size == 0 || pool == NULL)
		return;
	El_Memset(obj, 0, sizeof(BlockRingBufferProvider));
	obj->CalculateSumValue = 0;
	obj->ReferenceSumValue = 0;

	obj->Pool = pool;
	obj->Size = size;

	rt_ringbuffer_init(&obj->RingBuffer, pool, size);

	osMessageQueueAttr_t queueAttr;
	memset(&queueAttr, 0, sizeof(osMessageQueueAttr_t));
	queueAttr.name = "ringbufqueue";
	obj->Queue = osMessageQueueNew(queue_item_max_number, sizeof(ReceiveItemInfo), &queueAttr);

	osMutexAttr_t mutexAttr;
	memset(&mutexAttr, 0, sizeof(osMutexAttr_t));
	obj->Mutex = osMutexNew(&mutexAttr);
}

void BlockRingBufferProvider_DeInit(BlockRingBufferProvider* obj)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return;
	rt_ringbuffer_reset(&obj->RingBuffer);
	osMutexDelete(obj->Mutex);
	osMessageQueueDelete(obj->Queue);
}
osStatus_t BlockRingBufferProvider_GetChar(BlockRingBufferProvider* obj, uint8_t* ch)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return osError;
	uint8_t tch = 0;
	uint32_t size = 0;
	osMutexAcquire(obj->Mutex, El_WAIT_FOREVER);
	size = rt_ringbuffer_getchar(&obj->RingBuffer, &tch);
	osMutexRelease(obj->Mutex);
	if (size == 0)
	{
		return osError;
	}
	if(ch!=NULL)
		*ch = tch;
	obj->CalculateSumValue += tch;
	return  osOK;
}
osStatus_t BlockRingBufferProvider_ViewChar(BlockRingBufferProvider* obj,  uint8_t* ch,uint16_t offset)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return osError;
	uint8_t tch = 0;
	uint32_t size = 0;
	osMutexAcquire(obj->Mutex, El_WAIT_FOREVER);
	size = rt_ringbuffer_viewchar(&obj->RingBuffer, &tch,offset);
	osMutexRelease(obj->Mutex);
	if (size == 0)
	{
		return osError;
	}
	if (ch != NULL)
		*ch = tch;
	return  osOK;
}
osStatus_t BlockRingBufferProvider_PopChars(BlockRingBufferProvider* obj,  uint8_t* ch, uint16_t len)
{
	/*for (uint16_t i = 0; i < len; i++)
	{
		if (ch != NULL)
		{
			if (GetChar(&ch[i]) == False) 
				return False;
		}
		else
		{
			if (GetChar(NULL) == False)
				return False;
		}
	}*/
	uint8_t tch=0;
	osMutexAcquire(obj->Mutex, El_WAIT_FOREVER);
	for (uint16_t i = 0; i < len; i++)
	{
		rt_ringbuffer_getchar(&obj->RingBuffer, &tch);
		obj->CalculateSumValue += tch;
		if (ch != NULL)
		{
			ch[i] = tch;
		}
	}
	osMutexRelease(obj->Mutex);
	return osOK;
}
osStatus_t BlockRingBufferProvider_Receive(BlockRingBufferProvider* obj,  ReceiveItemInfo* item, uint32_t timeout)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return osError;
	
	return osMessageQueueGet(obj->Queue, item,0, timeout);
}
uint32_t BlockRingBufferProvider_CalculateSum( uint8_t* d, uint16_t len)
{
	uint32_t sum = 0;
	int16_t i = 0;
	for (i = 0; i < len; i++)
	{
		sum += d[i];
	}
	return sum;
}
osStatus_t BlockRingBufferProvider_Send(BlockRingBufferProvider* obj,  ReceiveItemInfo* header,uint8_t* buf)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return osError;

	int16_t putlen = 0;
	//bool insert_flag = False;
	osStatus_t result = osError;

	osMutexAcquire(obj->Mutex, El_WAIT_FOREVER);
	if (rt_ringbuffer_space_len(&obj->RingBuffer) >= (header->DataLen) && osMessageQueueGetSpace(obj->Queue)>0)
	{
		//insert_flag = True;
		putlen = rt_ringbuffer_put(&obj->RingBuffer, buf, header->DataLen);
		if (putlen != header->DataLen)
		{
			result= osError;
			goto _unlock;
		}
		//item->DataLen = bufLen;
		//item->CheckSum = CalculateSum(buf, bufLen);
		result = osMessageQueuePut(obj->Queue, header, 0, 0);
	}
_unlock:
	osMutexRelease(obj->Mutex);
	return result;
}
void BlockRingBufferProvider_Reset(BlockRingBufferProvider* obj)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return ;
	rt_ringbuffer_reset(&obj->RingBuffer);
	osMessageQueueReset(obj->Queue);
}
