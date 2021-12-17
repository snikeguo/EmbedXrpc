#ifndef Sample2_Server_H
#define Sample2_Server_H
#include"EmbedXrpcObject.h"
#include"Sample2.EmbedXrpcSerialization.h"
template<class DTL>
class Inter_GetSumService:public IService<DTL>
{
public:
uint16_t GetSid(){return Inter_GetSum_ServiceId;}
Inter_GetSum_Return Response;
virtual void GetSum(ServiceInvokeParameter<DTL> * serviceInvokeParameter,Int32 a,Int32 b)=0;
Inter_GetSum_Parameter request;
void Invoke(ServiceInvokeParameter<DTL> * serviceInvokeParameter,SerializationManager<DTL> *recManager, SerializationManager<DTL>* sendManager)
{
Inter_GetSum_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->rpcObject->RpcConfig->CheckSumValid==true)
{
El_Assert(recManager->GetReferenceSum()==recManager->GetCalculateSum());
}
GetSum(serviceInvokeParameter,request.a,request.b);
Inter_GetSum_Parameter_FreeData<DTL>(&request);
Inter_GetSum_Return_Serialize(sendManager,&Response);
if(this->IsFreeResponse==true) Inter_GetSum_Return_FreeData<DTL>(&Response);
}
};


/*
The Requests Of Inter:
name                   type

"Inter_GetSum",           Inter_GetSumService

RequestsCount: 1
*/


#endif
