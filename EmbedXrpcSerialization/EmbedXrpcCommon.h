#ifndef EmbedXrpcCommon_H
#define EmbedXrpcCommon_H

#include "EmbedSerializationBaseType.h"
#include "EmbedSerialization.h"
#include "EmbedXrpc.Port.h"
 enum ResponseState
 {
	 ResponseState_Ok,
	 ResponseState_Timeout,
	 ResponseState_SidError,
 };
 enum ReceiveType_t
 {
	 ReceiveType_Response,
	 ReceiveType_Delegate,
 };
 struct DeserializeMap
 {
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
template<class T>
class Option
{
public :
    ResponseState ResponseState;
    T Value;
};
class IDelegate
{
public:
    uint32_t Sid;
    virtual void Invoke(uint8_t* data, uint32_t len)=0;
};


class IService
{
public:
	uint32_t Sid;
	virtual void Invoke(SerializationManager &manager,IType &type,uint8_t* data, uint32_t len) = 0;
};
#endif