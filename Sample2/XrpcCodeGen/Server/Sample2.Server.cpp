#include"Sample2.Server.h"
void Inter_GetSum_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_GetSum_Parameter_Deserialize(recManager,&request);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
GetSum(serviceInvokeParameter,request.a,request.b);
Inter_GetSum_Parameter_FreeData(&request);
Inter_GetSum_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true) Inter_GetSum_Return_FreeData(&Response);
}


