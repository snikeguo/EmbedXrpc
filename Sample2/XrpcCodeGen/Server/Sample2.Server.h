#ifndef Sample2_Server_H
#define Sample2_Server_H
#include"EmbedXrpcObject.h"
#include"Sample2.EmbedXrpcSerialization.h"
class Inter_GetSum_Service:public IService
{
public:
uint16_t GetSid(){return Inter_GetSum_ServiceId;}
Inter_GetSum_Return Response;
virtual void GetSum(ServiceInvokeParameter * serviceInvokeParameter,Int32 a,Int32 b){}
Inter_GetSum_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


/*
The Requests Of Inter:
name                   type

"Inter_GetSum",           Inter_GetSumService

RequestsCount: 1
*/


#endif
