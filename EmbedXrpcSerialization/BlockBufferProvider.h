#ifndef BlockBufferProvider_H
#define BlockBufferProvider_H
#include "ringbuffer.h"
//#include "EmbedXrpcPortInterface.h"


template<class DTL>
struct ReceiveItemInfo;
    template<class DTL>
    class  BlockRingBufferProvider
    {
    public:
        El_Queue_t Queue;
        struct rt_ringbuffer RingBuffer;
        El_Mutex_t Mutex;
        uint8_t* Pool;
        int16_t Size;
        uint32_t CalculateSumValue;
        uint32_t ReferenceSumValue;

        
        BlockRingBufferProvider(     uint8_t* pool, uint16_t size, uint32_t queue_item_max_number)
        {
            if (size == 0 || pool == NULL)
                return;
            CalculateSumValue = 0;
            ReferenceSumValue = 0;

            Pool = pool;
            Size = size;

            rt_ringbuffer_init(&RingBuffer, pool, size);
            Queue = El_CreateQueue("ringbufqueue", sizeof(ReceiveItemInfo<DTL>), queue_item_max_number);
            Mutex = El_CreateMutex("ringbufmutex");
        }
        void DeInit(   )
        {
            if (Size == 0 || Pool == NULL)
                return;
            rt_ringbuffer_reset(&RingBuffer);
            El_DeleteMutex(Mutex);
            El_DeleteQueue(Queue);
        }

        Bool GetChar(  uint8_t* ch)
        {
            if (Size == 0 || Pool == NULL)
                return False;
            uint8_t tch = 0;
            uint32_t size = 0;
            El_TakeMutex(Mutex, El_WAIT_FOREVER);
            size = rt_ringbuffer_getchar(&RingBuffer, &tch);
            El_ReleaseMutex(Mutex);
            if (size == 0)
            {
                return False;
            }
            if (ch != NULL)
                *ch = tch;
            CalculateSumValue += tch;
            return  True;
        }
        Bool ViewChar( uint8_t* ch, uint16_t offset)
        {
            if (Size == 0 || Pool == NULL)
                return False;
            uint8_t tch = 0;
            uint32_t size = 0;
            El_TakeMutex(Mutex, El_WAIT_FOREVER);
            size = rt_ringbuffer_viewchar(&RingBuffer, &tch, offset);
            El_ReleaseMutex(Mutex);
            if (size == 0)
            {
                return False;
            }
            if (ch != NULL)
                *ch = tch;
            return  True;
        }

        Bool PopChars( uint8_t* ch, uint16_t len)
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
            uint8_t tch = 0;
            El_TakeMutex(Mutex, El_WAIT_FOREVER);
            for (uint16_t i = 0; i < len; i++)
            {
                rt_ringbuffer_getchar(&RingBuffer, &tch);
                CalculateSumValue += tch;
                if (ch != NULL)
                {
                    ch[i] = tch;
                }
            }
            El_ReleaseMutex(Mutex);
            return True;
        }
        //template<class DTL>
        Bool Receive(  ReceiveItemInfo<DTL>* header, uint32_t timeout)
        {
            if (Size == 0 || Pool == NULL)
                return False;
            Bool r = El_ReceiveQueue(Queue, header, sizeof(ReceiveItemInfo<DTL>), timeout) == QueueState_OK ? True : False;
            return r;
        }
        //template<class DTL>
        Bool Send(ReceiveItemInfo<DTL>* header, uint8_t* data)
        {
            if (Size == 0 || Pool == NULL)
                return False;

            int16_t putlen = 0;
            //bool insert_flag = False;
            Bool result = False;

            El_TakeMutex(Mutex, El_WAIT_FOREVER);
            if (rt_ringbuffer_space_len(&RingBuffer) >= (header->DataLen) && El_QueueSpacesAvailable(Queue) > 0)
            {
                //insert_flag = True;
                putlen = rt_ringbuffer_put(&RingBuffer, data, header->DataLen);
                if (putlen != header->DataLen)
                {
                    result = False;
                    goto _unlock;
                }
                //header->DataLen = bufLen;
                //header->CheckSum = CalculateSum(buf, bufLen);
                if (El_SendQueue(Queue, header, sizeof(ReceiveItemInfo<DTL>)) == QueueState_OK)
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
            El_ReleaseMutex(Mutex);
            return result;
        }
        void Reset()
        {
            if (Size == 0 || Pool == NULL)
                return;
            rt_ringbuffer_reset(&RingBuffer);
            El_ResetQueue(Queue);
        }


        inline void      SetCalculateSum(uint32_t s) {CalculateSumValue = s; }
        inline uint32_t  GetCalculateSum() { return CalculateSumValue; }
        inline void      SetReferenceSum(uint32_t ref) { ReferenceSumValue = ref; }
        inline uint32_t  GetReferenceSum() { return ReferenceSumValue; }
        static uint32_t CalculateSum(uint8_t* d, uint16_t len)
        {
            uint32_t sum = 0;
            int16_t i = 0;
            for (i = 0; i < len; i++)
            {
                sum += d[i];
            }
            return sum;
        }
    };
#include <EmbedXrpcCommon.h>
#endif
