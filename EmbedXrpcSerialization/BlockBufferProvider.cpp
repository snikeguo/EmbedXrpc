#include "BlockBufferProvider.h"
BlockBufferProvider::BlockBufferProvider(uint8_t *pool,int16_t size,IEmbeXrpcPort *porter)
{
        Pool=pool;
        Size=size;
        Porter=porter;
        rt_ringbuffer_init(&RingBuffer,pool,size);
        Queue=porter->CreateQueue("ringbufqueue",sizeof(BlockBufferItemInfo),5);
        Mutex=porter->CreateMutex("ringbufmutex");
}
bool BlockBufferProvider::GetChar(uint8_t *ch)
{
    uint8_t tch=0;
    uint32_t size=0;
    Porter->TakeMutex(Mutex);
    size=rt_ringbuffer_getchar(&RingBuffer,&tch);
    Porter->ReleaseMutex(Mutex);
    if(size==0)
    {
        return false;
    }
    *ch=tch;
    return  true;
}
bool BlockBufferProvider::Receive(BlockBufferItemInfo *item, uint32_t timeout)
{
    auto r=Porter->ReceiveQueue(Queue,item,timeout);
    return r;
}
uint32_t BlockBufferProvider::GetSum(uint8_t *d,int16_t len)
{
    uint32_t sum=0;
    int16_t i=0;
    for(i=0;i<len;i++)
    {
        sum+=d[i];
    }
    return sum;
}
bool BlockBufferProvider::Send( uint8_t *buf, int16_t bufLen)
{
    int16_t putlen=0;
    struct BlockBufferItemInfo item;

    Porter->TakeMutex(Mutex);
    putlen=rt_ringbuffer_put(&RingBuffer,buf,bufLen);
    Porter->ReleaseMutex(Mutex);
    if(putlen!=bufLen)
    {
        return false;
    }
    item.DataLen=bufLen;
    item.CheckSum=GetSum(buf,bufLen);
    if(SendQueue(brb->Queue,&item)==true)
    {
        return true;
    }
    else
    {
        return false;
    }
}