#ifndef EmbedXrpc_PortInterface_H
#define EmbedXrpc_PortInterface_H
#include "EmbedXrpc.Port.h"

#ifdef __cplusplus
extern "C" {
#endif
   
    typedef struct ReceiveItemInfo
    {
        uint32_t DataLen;
        uint16_t Sid;
        uint16_t TargetTimeout;
        uint8_t* Data;
    }ReceiveItemInfo;
#ifdef __cplusplus
}
#endif

#endif
