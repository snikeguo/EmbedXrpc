#ifndef BlockBufferProvider_H
#define BlockBufferProvider_H
#include "ringbuffer.h"
#include "EmbedXrpc.Port.h"
struct BlockBufferItemInfo
{
    uint32_t DataLen=0;
    uint32_t CheckSum=0;

    uint16_t Sid=0;
    uint16_t TargetTimeout=0;
};
class  BlockRingBufferProvider
{
public:
    EmbedXrpc_Queue_t Queue;
    struct rt_ringbuffer RingBuffer;
    EmbedXrpc_Mutex_t Mutex;

    uint8_t *Pool;
    int16_t Size;

    IEmbeXrpcPort *Porter;

    BlockRingBufferProvider(uint8_t *pool, uint16_t size,IEmbeXrpcPort *porter);
    ~BlockRingBufferProvider();
    bool GetChar(uint8_t *ch);
    bool ViewChar(uint8_t* ch, uint16_t offset);
    bool PopChars(uint8_t* ch, uint16_t len);
    bool Receive(BlockBufferItemInfo *item, uint32_t timeout);
    bool Send(BlockBufferItemInfo* item, uint8_t *buf, uint16_t bufLen);
    void Reset();
    
    void SetCalculateSum(uint32_t s) { CalculateSumValue = s; }
    uint32_t GetCalculateSum() { return CalculateSumValue; }
    void SetReferenceSum(uint32_t ref) { ReferenceSumValue = ref; }
    uint32_t GetReferenceSum() { return ReferenceSumValue; }

    static uint32_t CalculateSum(uint8_t *d, uint16_t len);
private:
    
    uint32_t CalculateSumValue;
    uint32_t ReferenceSumValue;
};
#endif