#include"Sample1.Server.h"
void  DateTimeChangeDelegate::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
SendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(&sm,&SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);
RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
SerializationManager_Reset(&sm);
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
}


void  TestDelegateDelegate::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer)
{//write serialization code:TestDelegate(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
SerializationManager_Reset(&sm);
sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
sm.BufferLen = EmbedXrpc_SendBufferSize-4;
TestDelegate_Parameter_Serialize(&sm,&SendData);
RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)(TestDelegate_ServiceId&0xff);
RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)(TestDelegate_ServiceId>>8&0xff);
RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);
RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);
SerializationManager_Reset(&sm);
El_ReleaseMutex(RpcObject->ObjectMutexHandle);
}


void Inter_AddService::Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_Add_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_CheckSumValid==1
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
#endif
Add(request_UserDataOfTransportLayer,response_UserDataOfTransportLayer,rpcObject,targetTimeOut,request.a,request.b,request.dataLen,request.data);
Inter_Add_Parameter_FreeData(&request);
Inter_Add_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true) Inter_Add_Return_FreeData(&Response);
}


void Inter_NoArgService::Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoArg_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_CheckSumValid==1
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
#endif
NoArg(request_UserDataOfTransportLayer,response_UserDataOfTransportLayer,rpcObject,targetTimeOut);
Inter_NoArg_Parameter_FreeData(&request);
Inter_NoArg_Return_Serialize(sendManager,&Response);
if(IsFreeResponse==true) Inter_NoArg_Return_FreeData(&Response);
}


void Inter_NoReturnService::Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoReturn_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_CheckSumValid==1
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
#endif
NoReturn(request_UserDataOfTransportLayer,response_UserDataOfTransportLayer,rpcObject,targetTimeOut,request.a);
Inter_NoReturn_Parameter_FreeData(&request);
}


void Inter_NoArgAndReturnService::Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager *recManager, SerializationManager* sendManager)
{
Inter_NoArgAndReturn_Parameter_Deserialize(recManager,&request);
#if EmbedXrpc_CheckSumValid==1
El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));
#endif
NoArgAndReturn(request_UserDataOfTransportLayer,response_UserDataOfTransportLayer,rpcObject,targetTimeOut);
Inter_NoArgAndReturn_Parameter_FreeData(&request);
}


