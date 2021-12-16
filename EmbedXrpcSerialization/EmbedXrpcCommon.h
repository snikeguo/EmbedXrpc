#ifndef EmbedXrpcCommon_H
#define EmbedXrpcCommon_H


//#include "EmbedXrpcPortInterface.h"
#include "EmbedSerialization.h"
#define EmbedXrpcSuspendSid 0x01
#define EmbedXrpcUnsupportedSid 0x2
template<class DTL>
class EmbedXrpcObject;

class BlockRingBufferProvider;
class SerializationManager;

template<class DTL>
struct ReceiveItemInfo;
 enum RequestResponseState
 {
     RequestState_Ok=1,
     RequestState_Failed=2,
     ResponseState_Ok=3,
     ResponseState_Timeout=4,
     ResponseState_SidError=5,
     ResponseState_UnsupportedSid = 6,
 };

 struct EmbedXrpcConfig
 {
     bool CheckSumValid;
     bool IsSendToQueue;

     //client部分:RingBuffer 模式下 and Dynamic Memory 下 都需要配置的
     uint32_t DelegateBlockQueue_MaxItemNumber;
     uint32_t ResponseBlockQueue_MaxItemNumber;

//server部分:RingBuffer 模式下 and Dynamic Memory 下 都需要配置的
     uint32_t RequestBlockQueue_MaxItemNumber;

     uint32_t ServerThreadPriority;
     uint32_t ClientThreadPriority;


     bool UseRingBufferWhenReceiving;

    //client
     BlockRingBufferProvider* DelegateBlockBufferProvider = nullptr;
     BlockRingBufferProvider* ResponseBlockBufferProvider = nullptr;

    //server
     BlockRingBufferProvider* RequestBlockBufferProvider = nullptr;

 };
 enum ReceiveType_t
 {
     ReceiveType_Request=0x1,
	 ReceiveType_Response=0x2,
	 ReceiveType_Delegate=0x3,
 };
 template<class DTL>
 class IDelegate
 {
 public:
     void* UserData;
     virtual uint16_t GetSid() = 0;
	 virtual void Invoke(EmbedXrpcConfig *rpcConfig,DTL* userDataOfTransportLayer,
         SerializationManager* recManager) = 0;
 };
 template<class DTL>
 class ServiceInvokeParameter
 {
 public:
     DTL* request_UserDataOfTransportLayer;
     DTL response_UserDataOfTransportLayer;
     EmbedXrpcObject<DTL>* rpcObject;
     uint16_t targetTimeOut;
 };
 template<class DTL>
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
         ServiceInvokeParameter<DTL> * serviceInvokeParameter,
         SerializationManager* recManager,
         SerializationManager* sendManager) = 0;
 };
 template<class DTL>
 struct RequestDescribe
 {
     const char* Name;
     IService<DTL>* Service;
 };
 template<class DTL>
 struct DelegateDescribe
 {
     const char* Name;
     IDelegate<DTL>* Delegate;
 };
 struct ResponseDescribe
 {
     const char* Name;
     uint16_t Sid;
 };
 
 template<class DTL>
 struct ReceiveItemInfo
 {
     uint32_t DataLen;
     uint32_t CheckSum;
     uint16_t Sid;
     uint16_t TargetTimeout;
     uint8_t* Data;
     DTL UserDataOfTransportLayer;
 };

 template<class DTL>
 struct SendAction
 {
     bool (*Invoke)(DTL* userDataOfTransportLayer, EmbedXrpcObject<DTL>* rpcObj, uint32_t dataLen, uint8_t* data);
 };


#endif
