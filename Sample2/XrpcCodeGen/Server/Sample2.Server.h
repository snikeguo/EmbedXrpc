#ifndef Sample2_Server_H
#define Sample2_Server_H
#include"EmbedXrpcObject.h"
#include"Sample2.EmbedXrpcSerialization.h"
class GetSum_Service:public IService
{
public:
uint16_t GetSid(){return GetSum_ServiceId;}
GetSum_Return Response;
virtual void GetSum(ServiceInvokeParameter * serviceInvokeParameter,Int32 a,Int32 b){}
GetSum_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};



#endif
