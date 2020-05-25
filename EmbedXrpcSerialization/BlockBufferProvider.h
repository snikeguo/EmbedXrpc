#ifndef BlockBufferProvider_H
#define BlockBufferProvider_H
#include "ringbuffer.h"
#include "EmbedXrpc.Port.h"
class  BlockBufferProvider:public IBlockBufferProvider
{
public:
    EmbedXrpc_Queue_t Queue;
    struct rt_ringbuffer RingBuffer;
    EmbedXrpc_Mutex_t Mutex;

    uint8_t *Pool;
    int16_t Size;

    IEmbeXrpcPort *Porter;

    BlockBufferProvider(uint8_t *pool,int16_t size,IEmbeXrpcPort *porter);
    bool GetChar(uint8_t *ch);  
    bool Receive(BlockBufferItemInfo *item, uint32_t timeout);
    uint32_t GetSum(uint8_t *d,int16_t len);
    bool Send( uint8_t *buf, int16_t bufLen);
};
#endif