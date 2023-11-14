#include "BlockBufferProvider.h"
void BlockRingBufferProvider_Init(BlockRingBufferProvider* obj, uint8_t* pool, uint16_t size, uint32_t queue_item_max_number)
{
	if (size == 0 || pool == NULL)
		return;
	El_Memset(obj, 0, sizeof(BlockRingBufferProvider));


	obj->Pool = pool;
	obj->Size = size;

	rt_ringbuffer_init(&obj->RingBuffer, pool, size);
	obj->Queue = El_CreateQueue("ringbufqueue", sizeof(ReceiveItemInfo), queue_item_max_number);
	obj->Mutex = El_CreateMutex("ringbufmutex");
}

void BlockRingBufferProvider_DeInit(BlockRingBufferProvider* obj)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return;
	rt_ringbuffer_reset(&obj->RingBuffer);
	El_DeleteMutex(obj->Mutex);
	El_DeleteQueue(obj->Queue);
}
Bool BlockRingBufferProvider_GetChar(BlockRingBufferProvider* obj, uint8_t* ch, int isIsr)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return False;
	uint8_t tch = 0;
	uint32_t size = 0;
	El_TakeMutex(obj->Mutex, El_WAIT_FOREVER,isIsr);
	size = rt_ringbuffer_getchar(&obj->RingBuffer, &tch);
	El_ReleaseMutex(obj->Mutex,isIsr);
	if (size == 0)
	{
		return False;
	}
	if(ch!=NULL)
		*ch = tch;

	return  True;
}
Bool BlockRingBufferProvider_ViewChar(BlockRingBufferProvider* obj,  uint8_t* ch,uint16_t offset, int isIsr)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return False;
	uint8_t tch = 0;
	uint32_t size = 0;
	El_TakeMutex(obj->Mutex, El_WAIT_FOREVER,isIsr);
	size = rt_ringbuffer_viewchar(&obj->RingBuffer, &tch,offset);
	El_ReleaseMutex(obj->Mutex,isIsr);
	if (size == 0)
	{
		return False;
	}
	if (ch != NULL)
		*ch = tch;
	return  True;
}
Bool BlockRingBufferProvider_PopChars(BlockRingBufferProvider* obj,  uint8_t* ch, uint16_t len, int isIsr)
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
	El_TakeMutex(obj->Mutex, El_WAIT_FOREVER,isIsr);
	for (uint16_t i = 0; i < len; i++)
	{
		rt_ringbuffer_getchar(&obj->RingBuffer, &tch);
		if (ch != NULL)
		{
			ch[i] = tch;
		}
	}
	El_ReleaseMutex(obj->Mutex,isIsr);
	return True;
}
Bool BlockRingBufferProvider_Receive(BlockRingBufferProvider* obj,  ReceiveItemInfo* item, uint32_t timeout, int isIsr)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return False;
	Bool r = El_ReceiveQueue(obj->Queue, item, sizeof(ReceiveItemInfo), timeout,isIsr) == QueueState_OK ? True : False;
	return r;
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
Bool BlockRingBufferProvider_Send(BlockRingBufferProvider* obj,  ReceiveItemInfo* header,uint8_t* buf,int isIsr)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return False;

	int16_t putlen = 0;
	//bool insert_flag = False;
	Bool result = False;

	El_TakeMutex(obj->Mutex, El_WAIT_FOREVER,isIsr);
	if (rt_ringbuffer_space_len(&obj->RingBuffer) >= (header->DataLen) && El_QueueSpacesAvailable(obj->Queue,isIsr)>0)
	{
		//insert_flag = True;
		putlen = rt_ringbuffer_put(&obj->RingBuffer, buf, header->DataLen);
		if (putlen != header->DataLen)
		{
			result=False;
			goto _unlock;
		}
		//item->DataLen = bufLen;
		//item->CheckSum = CalculateSum(buf, bufLen);
		if (El_SendQueue(obj->Queue, header, sizeof(ReceiveItemInfo), isIsr) == QueueState_OK)
		{
			result = True;
			goto _unlock;
		}
		else
		{
			result = False;
			goto _unlock;
		}
	}
_unlock:
	El_ReleaseMutex(obj->Mutex,isIsr);
	return result;
}
void BlockRingBufferProvider_Reset(BlockRingBufferProvider* obj, int isIsr)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return ;
	rt_ringbuffer_reset(&obj->RingBuffer);
	El_ResetQueue(obj->Queue,isIsr);
}
uint32_t BlockRingBufferProvider_DataLen(BlockRingBufferProvider* obj, int isIsr)
{
	if (obj->Size == 0 || obj->Pool == NULL)
		return 0;
	return rt_ringbuffer_data_len(&obj->RingBuffer);
}
