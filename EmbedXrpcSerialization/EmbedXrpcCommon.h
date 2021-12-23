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
 struct BlockBufferProviderConfig
 {
     uint8_t* Buffer;
     uint32_t Size;
     uint32_t MaxItemNumber;
 };
 struct EmbedXrpcConfig
 {
     bool CheckSumValid;
     uint32_t ServiceThreadPriority;
     bool UseRingBufferWhenReceiving;//���Ϊ1����ʹ��RingBufferConfig ����ʹ��DynamicMemoryConfig
     struct
     {
         bool IsSendToQueue;

         //client����:
         uint32_t MessageQueueOfRequestService_MaxItemNumber;

         //server����:
         uint32_t ServiceMessageQueue_MaxItemNumber;
     }DynamicMemoryConfig;
     struct
     {
         //client
         BlockBufferProviderConfig BlockBufferOfRequestService_Config;

         //server
         BlockBufferProviderConfig ServiceBlockBufferConfig;
     }RingBufferConfig;

 };
 enum ReceiveType_t
 {
     ReceiveType_Request=0x1,
	 ReceiveType_Response=0x2,
 };


 class ServiceInvokeParameter
 {
 public:
     UserDataOfTransportLayer_t* Request_UserDataOfTransportLayer;
     UserDataOfTransportLayer_t Response_UserDataOfTransportLayer;
     EmbedXrpcObject* RpcObject;
     uint16_t TargetTimeOut;
 };

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
         ServiceInvokeParameter * serviceInvokeParameter,
         SerializationManager* recManager, 
         SerializationManager* sendManager) = 0;
 };
 struct ServiceDescribe
 {
     const char* Name;
     IService* Service;
 };

 struct RequestServiceDescribe
 {
     const char* Name;
     uint16_t Sid;
 };
typedef bool (*SendPack_t)(UserDataOfTransportLayer_t* userDataOfTransportLayer, EmbedXrpcObject* rpcObj,uint32_t dataLen, uint8_t* data);

#endif
