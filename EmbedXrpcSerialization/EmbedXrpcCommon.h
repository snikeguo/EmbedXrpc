#ifndef EmbedXrpcCommon_H
#define EmbedXrpcCommon_H


#include "EmbedXrpc.Port.h"
#include "EmbedSerialization.h"
#define EmbedXrpcSuspendSid 0x01
#define EmbedXrpcUnsupportedSid 0x2
 enum RequestResponseState
 {
     RequestState_Ok=1,
     RequestState_Failed=2,
     ResponseState_Ok=3,
     ResponseState_Timeout=4,
     ResponseState_SidError=5,
     ResponseState_UnsupportedSid = 6,
 };
 enum ReceiveType_t
 {
     ReceiveType_Request,
	 ReceiveType_Response,
	 ReceiveType_Delegate,
 };
 class IDelegate
 {
 public:
     virtual uint16_t GetSid() = 0;
	 virtual void Invoke(SerializationManager& recManager) = 0;
 };


 class IService
 {
 public:
     virtual uint16_t GetSid() = 0;
	 virtual void Invoke(SerializationManager& recManager, SerializationManager& sendManager) = 0;
 };
 struct RequestMessageMap
 {
     const char* Name;
     IService* Service;
 };
 struct ResponseDelegateMessageMap
 {
     const char* Name;
     uint16_t Sid;//有可能是Response/Delegate
     ReceiveType_t ReceiveType;
     IDelegate* Delegate;
 };
 struct ResponseDelegateMessageMapCollection
 {
     uint32_t Count;
     ResponseDelegateMessageMap* Map;
 };
 struct RequestMessageMapCollection
 {
     uint32_t Count;
     RequestMessageMap* Map;
 };
 /*struct EmbeXrpcRawData
 {
     uint16_t Sid;
     uint16_t TargetTimeout;
	 uint8_t* Data;
	 uint32_t DataLen;
 };*/
 class IEmbedXrpcSession
 {

 };
typedef bool (*SendPack_t)(void *rpcObj,uint32_t dataLen, uint8_t* data);


#endif