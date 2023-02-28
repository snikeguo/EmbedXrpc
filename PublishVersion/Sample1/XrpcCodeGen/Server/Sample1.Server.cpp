#include"Sample1.Server.h"
void DateTimeChange_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
DateTimeChange_Parameter_Deserialize(recManager,&request,serviceInvokeParameter->IsIsr);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
DateTimeChange(serviceInvokeParameter,request.now);
DateTimeChange_Parameter_FreeData(&request);
}


void Test2_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Test2_Parameter_Deserialize(recManager,&request,serviceInvokeParameter->IsIsr);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
Test2(serviceInvokeParameter,request.now);
Test2_Parameter_FreeData(&request);
}


void Add_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
Add_Parameter_Deserialize(recManager,&request,serviceInvokeParameter->IsIsr);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
Add(serviceInvokeParameter,request.a,request.b,request.dataLen,request.data);
Add_Parameter_FreeData(&request);
Add_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true){
//!Add_Return_FreeData(&Response);
}
}


void NoArg_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
NoArg_Parameter_Deserialize(recManager,&request,serviceInvokeParameter->IsIsr);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoArg(serviceInvokeParameter);
//!NoArg_Parameter_FreeData(&request);
NoArg_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true){
//!NoArg_Return_FreeData(&Response);
}
}


void NoReturn_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
NoReturn_Parameter_Deserialize(recManager,&request,serviceInvokeParameter->IsIsr);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoReturn(serviceInvokeParameter,request.a);
//!NoReturn_Parameter_FreeData(&request);
}


void NoArgAndReturn_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)
{
NoArgAndReturn_Parameter_Deserialize(recManager,&request,serviceInvokeParameter->IsIsr);
if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)
{
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
}
NoArgAndReturn(serviceInvokeParameter);
//!NoArgAndReturn_Parameter_FreeData(&request);
}


