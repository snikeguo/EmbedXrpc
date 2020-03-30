#ifndef EmbedXrpcCommon_H
#define EmbedXrpcCommon_H


#include "EmbedXrpc.Port.h"
 enum ResponseState
 {
	 ResponseState_Ok,
	 ResponseState_Timeout,
	 ResponseState_SidError,
 };
 enum ReceiveType_t
 {
     ReceiveType_Request,
	 ReceiveType_Response,
	 ReceiveType_Delegate,
 };
 struct MessageMap
 {
     const char* Name;
	 uint32_t Sid;
	 ReceiveType_t ReceiveType;
	 IType* MessageType;
 };
 struct EmbeXrpcRawData
 {
	 uint32_t Sid;
	 IType* MessageType;
	 uint8_t* Data;
	 uint32_t DataLen;
 };

typedef void (*SendPack_t)(uint32_t serviceId, uint32_t dataLen, uint8_t* data);

class IDelegate
{
public:
    uint32_t Sid;
    virtual void Invoke(SerializationManager& recManager)=0;
};


class IService
{
public:
	uint32_t Sid;
	virtual void Invoke(SerializationManager &recManager, SerializationManager& sendManager) = 0;
};
#endif