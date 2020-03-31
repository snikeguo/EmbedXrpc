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
 class IDelegate
 {
 public:
	 uint32_t Sid;
	 virtual void Invoke(SerializationManager& recManager) = 0;
 };


 class IService
 {
 public:
	 uint32_t Sid;
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