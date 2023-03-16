#ifndef EmbedXrpc_PortInterface_H
#define EmbedXrpc_PortInterface_H
#include "EmbedXrpc.Port.h"

#ifdef __cplusplus
extern "C" {
#endif
   
    typedef struct ReceiveItemInfo
    {
        uint32_t DataLen;
        //uint32_t CheckSum;
        uint16_t Sid;
        uint16_t TargetTimeout;
#if EmbedXrpc_UseRingBufferWhenReceiving==0
        uint8_t* Data;
#endif
        UserDataOfTransportLayer_t UserDataOfTransportLayer;
    }ReceiveItemInfo;
#ifdef __cplusplus
}
#endif

#endif
