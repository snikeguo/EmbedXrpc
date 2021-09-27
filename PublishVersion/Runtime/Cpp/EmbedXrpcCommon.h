#ifndef EmbedXrpcCommon_H
#define EmbedXrpcCommon_H


#include "EmbedXrpcPortInterface.h"
#include "EmbedSerialization.h"
#define EmbedXrpcSuspendSid 0x01
#define EmbedXrpcUnsupportedSid 0x2
class EmbedXrpcObject;
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
     ReceiveType_Request=0x1,
	 ReceiveType_Response=0x2,
	 ReceiveType_Delegate=0x3,
 };
 class IDelegate
 {
 public:
     void* UserData;
     virtual uint16_t GetSid() = 0;
	 virtual void Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,
         SerializationManager* recManager) = 0;
 };


 class IService
 {
 public:
     void* UserData;
     /*
        IsFreeResponse字段非常重要,作用如下:
        当Server接收到一个Service请求,并执行了用户的业务逻辑代码后，有时候会需要给Client返回数据(相当于有返回值的Service请求,无返回值的这里不考虑)
        如果返回结构体有动态内存的话，势必在用户的业务逻辑里会malloc或者指定某个固定内存。那么这块内存可以选择让生成的代码去free，或者不free
        如果不想free的话，就把这个设置为false 否则为true.默认是true
     */
     bool IsFreeResponse = true;
     virtual uint16_t GetSid() = 0;
	 virtual void Invoke(
         UserDataOfTransportLayer_t* request_UserDataOfTransportLayer,
         UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,
         EmbedXrpcObject* rpcObject,
         uint16_t targetTimeOut,
         SerializationManager* recManager, 
         SerializationManager* sendManager) = 0;
 };
 struct RequestDescribe
 {
     const char* Name;
     IService* Service;
 };
 struct DelegateDescribe
 {
     const char* Name;
     IDelegate* Delegate;
 };
 struct ResponseDescribe
 {
     const char* Name;
     uint16_t Sid;
 };
 /*struct ResponseDelegateMessageMap
 {
     const char* Name;
     uint16_t Sid;//有可能是Response/Delegate
     ReceiveType_t ReceiveType;
     IDelegate* Delegate;
 };*/
 /*struct ResponseDelegateMessageMapCollection
 {
     uint32_t Count;
     ResponseDelegateMessageMap* Map;
 };*/
 /*struct RequestMessageMapCollection
 {
     uint32_t Count;
     RequestMessageMap* Map;
 };*/
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

typedef bool (*SendPack_t)(UserDataOfTransportLayer_t* userDataOfTransportLayer, EmbedXrpcObject* rpcObj,uint32_t dataLen, uint8_t* data);

#endif
