#ifndef EmbedXrpcCommon_H
#define EmbedXrpcCommon_H


#include "EmbedXrpcPortInterface.h"
#include "EmbedSerialization.h"
#define EmbedXrpcNoReceivedSid  0x0
#define EmbedXrpcSuspendSid 0x01
#define EmbedXrpcUnsupportedSid 0x2

class EmbedXrpcObject;
enum RequestResponseState
{
    RequestState_Ok = 1,
    RequestState_Failed = 2,
    ResponseState_Ok = 3,
    ResponseState_Timeout = 4,
    //ResponseState_SidError = 5,
    //ResponseState_UnsupportedSid = 6,
    ResponseState_NoReceived = 7,//only no os mode
 };

 struct EmbedXrpcConfig
 {
     int ServiceThreadPriority;
     uint32_t ServiceThreadStackSize;
     bool IsSendToQueue;

     //client部分:
     uint32_t ClientUseRespondedDataMaxSize;

     //server部分:
     uint32_t ServerUseRequestedDataMaxSize;
 };
 enum ReceiveType_t
 {
     ReceiveType_Request=0x1,
	 ReceiveType_Response=0x2,
 };

 class RequestParameter
 {
 public:
     int IsIsr = 0;
     bool IsProvideBuffer;
     uint32_t BufferLen;
     uint8_t* Buffer;
 };

 class ServiceInvokeParameter
 {
 public:
     EmbedXrpcObject* RpcObject;
     uint16_t TargetTimeOut;
     int IsIsr = 0;
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
         ServiceInvokeParameter * serviceInvokeParameter,
         SerializationManager* recManager, 
         SerializationManager* sendManager) = 0;
 };
 struct ServiceDescribe
 {
     const char* Name;
     IService* Service;
 };

typedef bool (*SendPack_t)(RequestParameter* rp, EmbedXrpcObject* rpcObj,uint32_t dataLen, uint8_t* data);

#endif
