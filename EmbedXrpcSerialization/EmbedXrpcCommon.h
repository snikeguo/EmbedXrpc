#ifndef EmbedXrpcCommon_H
#define EmbedXrpcCommon_H


#include "EmbedXrpc.Port.h"

#define EmbedXrpcSuspendSid 0x01
 enum ResponseState
 {
	 ResponseState_Ok=1,
	 ResponseState_Timeout=2,
	 ResponseState_SidError=3,
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
     virtual uint32_t GetSid() = 0;
	 virtual void Invoke(SerializationManager& recManager) = 0;
 };


 class IService
 {
 public:
     virtual uint32_t GetSid() = 0;
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
     uint32_t Sid;//有可能是Response/Delegate
     ReceiveType_t ReceiveType;
     IDelegate* Delegate;
 };
 struct EmbeXrpcRawData
 {
	 uint32_t Sid;
	 uint8_t* Data;
	 uint32_t DataLen;
 };
typedef void (*SendPack_t)(uint32_t serviceId, uint32_t dataLen, uint8_t* data);


#endif