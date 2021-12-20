#ifndef BlockBufferProvider_H
#define BlockBufferProvider_H
#include "ringbuffer.h"
#include "EmbedXrpcPortInterface.h"
#ifdef __cplusplus
extern "C" {
#endif
    struct  BlockRingBufferProvider
    {
        El_Queue_t Queue;
        struct rt_ringbuffer RingBuffer;
        El_Mutex_t Mutex;
        uint8_t* Pool;
        int16_t Size;
        uint32_t CalculateSumValue;
        uint32_t ReferenceSumValue;
        BlockRingBufferProvider(uint8_t* pool, uint16_t size, uint32_t queue_item_max_number);
    };
    void BlockRingBufferProvider_Init(BlockRingBufferProvider* obj, uint8_t* pool, uint16_t size, uint32_t queue_item_size);
    void BlockRingBufferProvider_DeInit(BlockRingBufferProvider* obj);
    Bool BlockRingBufferProvider_GetChar(BlockRingBufferProvider* obj, uint8_t* ch);
    Bool BlockRingBufferProvider_ViewChar(BlockRingBufferProvider* obj, uint8_t* ch, uint16_t offset);
    Bool BlockRingBufferProvider_PopChars(BlockRingBufferProvider* obj, uint8_t* ch, uint16_t len);
    Bool BlockRingBufferProvider_Receive(BlockRingBufferProvider* obj, ReceiveItemInfo* item, uint32_t timeout);
    Bool BlockRingBufferProvider_Send(BlockRingBufferProvider* obj, ReceiveItemInfo* item, uint8_t* buf);
    void BlockRingBufferProvider_Reset(BlockRingBufferProvider* obj);

    static inline void BlockRingBufferProvider_SetCalculateSum(BlockRingBufferProvider* obj, uint32_t s) { obj->CalculateSumValue = s; }
    static inline uint32_t BlockRingBufferProvider_GetCalculateSum(BlockRingBufferProvider* obj) { return obj->CalculateSumValue; }
    static inline void BlockRingBufferProvider_SetReferenceSum(BlockRingBufferProvider* obj, uint32_t ref) { obj->ReferenceSumValue = ref; }
    static inline uint32_t BlockRingBufferProvider_GetReferenceSum(BlockRingBufferProvider* obj) { return obj->ReferenceSumValue; }
    uint32_t BlockRingBufferProvider_CalculateSum(uint8_t* d, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif
