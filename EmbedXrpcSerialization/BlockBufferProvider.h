#ifndef BlockBufferProvider_H
#define BlockBufferProvider_H
#include "ringbuffer.h"
#include "EmbedXrpcPortInterface.h"
#include "EmbedLibrary.h"
#ifdef __cplusplus
extern "C" {
#endif
    typedef struct  _BlockRingBufferProvider
    {
        El_Queue_t Queue;
        struct rt_ringbuffer RingBuffer;
        El_Mutex_t Mutex;
        uint8_t* Pool;
        int16_t Size;
    }BlockRingBufferProvider;
    void BlockRingBufferProvider_Init(BlockRingBufferProvider* obj, uint8_t* pool, uint16_t size, uint32_t queue_item_size);
    void BlockRingBufferProvider_DeInit(BlockRingBufferProvider* obj);
    Bool BlockRingBufferProvider_GetChar(BlockRingBufferProvider* obj, uint8_t* ch,int isIsr);
    Bool BlockRingBufferProvider_ViewChar(BlockRingBufferProvider* obj, uint8_t* ch, uint16_t offset, int isIsr);
    Bool BlockRingBufferProvider_PopChars(BlockRingBufferProvider* obj, uint8_t* ch, uint16_t len, int isIsr);
    Bool BlockRingBufferProvider_Receive(BlockRingBufferProvider* obj, ReceiveItemInfo* item, uint32_t timeout, int isIsr);
    Bool BlockRingBufferProvider_Send(BlockRingBufferProvider* obj, ReceiveItemInfo* item, uint8_t* buf,int isIsr);
    void BlockRingBufferProvider_Reset(BlockRingBufferProvider* obj, int isIsr);
    uint32_t BlockRingBufferProvider_DataLen(BlockRingBufferProvider* obj, int isIsr);
#ifdef __cplusplus
}
#endif

#endif
