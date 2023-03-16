#include"Sample2.Server.h"
void GetSum_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
GetSum_Parameter_Deserialize(recManager,&request,serviceInvokeParameter->IsIsr);
GetSum(serviceInvokeParameter,request.a,request.b);
//!GetSum_Parameter_FreeData(&request);
GetSum_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true){
//!GetSum_Return_FreeData(&Response);
}
}


