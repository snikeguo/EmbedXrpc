#ifndef EmbedXrpcCommon_H
#define EmbedXrpcCommon_H


//#include "EmbedXrpcPortInterface.h"
#include "EmbedSerialization.h"
#define EmbedXrpcSuspendSid 0x01
#define EmbedXrpcUnsupportedSid 0x2

template<class DTL>
class EmbedXrpcObject;

template<class DTL>
class BlockRingBufferProvider;

template<class DTL>
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
 template<class DTL>
 struct EmbedXrpcConfig
 {
     bool CheckSumValid;
     uint32_t ServerThreadPriority;
     uint32_t ClientThreadPriority;
     bool UseRingBufferWhenReceiving;//���Ϊ1����ʹ��RingBufferConfig ����ʹ��DynamicMemoryConfig
     struct 
     {
         bool IsSendToQueue;

         //client����:
         uint32_t DelegateBlockQueue_MaxItemNumber;
         uint32_t ResponseBlockQueue_MaxItemNumber;

         //server����:
         uint32_t RequestBlockQueue_MaxItemNumber;
     }DynamicMemoryConfig;
     struct 
     {
         //client
         BlockRingBufferProvider<DTL>* DelegateBlockBufferProvider = nullptr;
         BlockRingBufferProvider<DTL>* ResponseBlockBufferProvider = nullptr;

         //server
         BlockRingBufferProvider<DTL>* RequestBlockBufferProvider = nullptr;
     }RingBufferConfig;
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
	 virtual void Invoke(EmbedXrpcConfig<DTL>*rpcConfig,DTL* userDataOfTransportLayer,
         SerializationManager<DTL>* recManager) = 0;
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
        IsFreeResponse�ֶηǳ���Ҫ,��������:
        ��Server���յ�һ��Service����,��ִ�����û���ҵ���߼��������ʱ�����Ҫ��Client��������(�൱���з���ֵ��Service����,�޷���ֵ�����ﲻ����)
        ������ؽṹ���ж�̬�ڴ�Ļ����Ʊ����û���ҵ���߼����malloc����ָ��ĳ���̶��ڴ档��ô����ڴ����ѡ�������ɵĴ���ȥfree�����߲�free
        �������free�Ļ����Ͱ��������Ϊfalse ����Ϊtrue.Ĭ����true
     */
     bool IsFreeResponse = true;
     virtual uint16_t GetSid() = 0;
	 virtual void Invoke(
         ServiceInvokeParameter<DTL> * serviceInvokeParameter,
         SerializationManager<DTL>* recManager,
         SerializationManager<DTL>* sendManager) = 0;
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
