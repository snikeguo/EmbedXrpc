#include"Sample1.Client.h"
DateTimeChange_Return& DateTimeChange_Requester::DateTimeChange(RequestParameter* rp,DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER,rp->IsIsr);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService,rp->IsIsr);
}
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
DateTimeChange_SendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(&sm,&DateTimeChange_SendData);
buffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
buffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
DateTimeChange_reqresp.State=RequestState_Failed;
}
else
{
DateTimeChange_reqresp.State=RequestState_Ok;
}
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
}
return DateTimeChange_reqresp;
}

DateTimeChange_Return& DateTimeChange_Requester::NoOs_DateTimeChange(RequestParameter* rp,DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
if (rp->IsProvideBuffer == false)
{
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
RpcObject->CurrentRequestSid=DateTimeChange_ServiceId;
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
DateTimeChange_SendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(&sm,&DateTimeChange_SendData);
buffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
buffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
DateTimeChange_reqresp.State=RequestState_Failed;
}
else
{
DateTimeChange_reqresp.State=RequestState_Ok;
}
return DateTimeChange_reqresp;
}

Test2_Return& Test2_Requester::Test2(RequestParameter* rp)
{//write serialization code:Test2()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER,rp->IsIsr);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService,rp->IsIsr);
}
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
Test2_Parameter_Serialize(&sm,&Test2_SendData);
buffer[0]=(uint8_t)(Test2_ServiceId&0xff);
buffer[1]=(uint8_t)(Test2_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
Test2_reqresp.State=RequestState_Failed;
}
else
{
Test2_reqresp.State=RequestState_Ok;
}
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
}
return Test2_reqresp;
}

Test2_Return& Test2_Requester::NoOs_Test2(RequestParameter* rp)
{//write serialization code:Test2()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
if (rp->IsProvideBuffer == false)
{
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
RpcObject->CurrentRequestSid=Test2_ServiceId;
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
Test2_Parameter_Serialize(&sm,&Test2_SendData);
buffer[0]=(uint8_t)(Test2_ServiceId&0xff);
buffer[1]=(uint8_t)(Test2_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
Test2_reqresp.State=RequestState_Failed;
}
else
{
Test2_reqresp.State=RequestState_Ok;
}
return Test2_reqresp;
}

Add_Return& Add_Requester::Add(RequestParameter* rp)
{//write serialization code:Add()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
auto waitstate=ResponseState_Timeout;
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER,rp->IsIsr);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService,rp->IsIsr);
}
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
Add_Parameter_Serialize(&sm,&Add_SendData);
buffer[0]=(uint8_t)(Add_ServiceId&0xff);
buffer[1]=(uint8_t)(Add_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
Add_reqresp.State=RequestState_Failed;
}
else
{
Add_reqresp.State=RequestState_Ok;
ReceiveItemInfo recData;
waitstate=RpcObject->Wait(Add_ServiceId,&recData,rp->IsIsr);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;
}
else
{
sm.Index=0;
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
Add_Return_Deserialize(&sm,&Add_reqresp,rp->IsIsr);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
{
if (recData.DataLen > 0)
{
El_Free(recData.Data);
}
}//if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
Add_reqresp.State=waitstate;
}
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
}
return Add_reqresp;
}
void Add_Requester::Free_Add(Add_Return *response)
{
if(response->State==ResponseState_Ok)
{
//!Add_Return_FreeData(response);
}
}

Add_Return& Add_Requester::NoOs_Add(RequestParameter* rp)
{//write serialization code:Add()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
auto waitstate=ResponseState_Timeout;
if (rp->IsProvideBuffer == false)
{
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
RpcObject->CurrentRequestSid=Add_ServiceId;
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
Add_Parameter_Serialize(&sm,&Add_SendData);
buffer[0]=(uint8_t)(Add_ServiceId&0xff);
buffer[1]=(uint8_t)(Add_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
Add_reqresp.State=RequestState_Failed;
}
else
{
Add_reqresp.State=RequestState_Ok;
}
return Add_reqresp;
}

Add_Return& Add_Requester::NoOs_QueryServiceState(RequestParameter* rp)
{SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
uint32_t nowTick = El_GetTick(rp->IsIsr);
ReceiveItemInfo currentReceivedResponseData;
memset(&currentReceivedResponseData,0,sizeof(currentReceivedResponseData));
if((RequestTick+RpcObject->TimeOut)<nowTick)
{
Add_reqresp.State = RequestResponseState::ResponseState_Timeout;
return Add_reqresp;
}
if(BlockRingBufferProvider_Receive(RpcObject->BlockBufferProviderOfRequestService, &currentReceivedResponseData,0,rp->IsIsr) == False)
{
Add_reqresp.State = RequestResponseState::ResponseState_NoReceived;
return Add_reqresp;
}
if(currentReceivedResponseData.Sid == EmbedXrpcNoReceivedSid)
{
Add_reqresp.State = RequestResponseState::ResponseState_NoReceived;
}
else if (currentReceivedResponseData.Sid == Add_ServiceId)
{
sm.Index=0;
sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;
Add_Return_Deserialize(&sm,&Add_reqresp,rp->IsIsr);
Add_reqresp.State = RequestResponseState::ResponseState_Ok;
}
else if (currentReceivedResponseData.Sid == EmbedXrpcSuspendSid)
{
Add_reqresp.State = RequestResponseState::ResponseState_NoReceived;
RequestTick=nowTick;
}
else
{
Add_reqresp.State = RequestResponseState::ResponseState_NoReceived;
}
return Add_reqresp;
}
NoArg_Return& NoArg_Requester::NoArg(RequestParameter* rp)
{
//write serialization code:NoArg()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
auto waitstate=ResponseState_Timeout;
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER,rp->IsIsr);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService,rp->IsIsr);
}
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
NoArg_Parameter_Serialize(&sm,&NoArg_SendData);
buffer[0]=(uint8_t)(NoArg_ServiceId&0xff);
buffer[1]=(uint8_t)(NoArg_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
NoArg_reqresp.State=RequestState_Failed;
}
else
{
NoArg_reqresp.State=RequestState_Ok;
ReceiveItemInfo recData;
waitstate=RpcObject->Wait(NoArg_ServiceId,&recData,rp->IsIsr);
if(waitstate == RequestResponseState::ResponseState_Ok)
{
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;
}
else
{
sm.Index=0;
sm.BufferLen = recData.DataLen;
sm.Buf = recData.Data;
}
NoArg_Return_Deserialize(&sm,&NoArg_reqresp,rp->IsIsr);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
{
if (recData.DataLen > 0)
{
El_Free(recData.Data);
}
}//if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)
NoArg_reqresp.State=waitstate;
}
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
}
return NoArg_reqresp;
}
void NoArg_Requester::Free_NoArg(NoArg_Return *response)
{
if(response->State==ResponseState_Ok)
{
//!NoArg_Return_FreeData(response);
}
}

NoArg_Return& NoArg_Requester::NoOs_NoArg(RequestParameter* rp)
{
//write serialization code:NoArg()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
auto waitstate=ResponseState_Timeout;
if (rp->IsProvideBuffer == false)
{
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
RpcObject->CurrentRequestSid=NoArg_ServiceId;
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
NoArg_Parameter_Serialize(&sm,&NoArg_SendData);
buffer[0]=(uint8_t)(NoArg_ServiceId&0xff);
buffer[1]=(uint8_t)(NoArg_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
NoArg_reqresp.State=RequestState_Failed;
}
else
{
NoArg_reqresp.State=RequestState_Ok;
}
return NoArg_reqresp;
}

NoArg_Return& NoArg_Requester::NoOs_QueryServiceState(RequestParameter* rp)
{SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
uint32_t nowTick = El_GetTick(rp->IsIsr);
ReceiveItemInfo currentReceivedResponseData;
memset(&currentReceivedResponseData,0,sizeof(currentReceivedResponseData));
if((RequestTick+RpcObject->TimeOut)<nowTick)
{
NoArg_reqresp.State = RequestResponseState::ResponseState_Timeout;
return NoArg_reqresp;
}
if(BlockRingBufferProvider_Receive(RpcObject->BlockBufferProviderOfRequestService, &currentReceivedResponseData,0,rp->IsIsr) == False)
{
NoArg_reqresp.State = RequestResponseState::ResponseState_NoReceived;
return NoArg_reqresp;
}
if(currentReceivedResponseData.Sid == EmbedXrpcNoReceivedSid)
{
NoArg_reqresp.State = RequestResponseState::ResponseState_NoReceived;
}
else if (currentReceivedResponseData.Sid == NoArg_ServiceId)
{
sm.Index=0;
sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;
NoArg_Return_Deserialize(&sm,&NoArg_reqresp,rp->IsIsr);
NoArg_reqresp.State = RequestResponseState::ResponseState_Ok;
}
else if (currentReceivedResponseData.Sid == EmbedXrpcSuspendSid)
{
NoArg_reqresp.State = RequestResponseState::ResponseState_NoReceived;
RequestTick=nowTick;
}
else
{
NoArg_reqresp.State = RequestResponseState::ResponseState_NoReceived;
}
return NoArg_reqresp;
}
NoReturn_Return& NoReturn_Requester::NoReturn(RequestParameter* rp,int32_t a)
{
//write serialization code:NoReturn(a,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER,rp->IsIsr);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService,rp->IsIsr);
}
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
NoReturn_SendData.a=a;
NoReturn_Parameter_Serialize(&sm,&NoReturn_SendData);
buffer[0]=(uint8_t)(NoReturn_ServiceId&0xff);
buffer[1]=(uint8_t)(NoReturn_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
NoReturn_reqresp.State=RequestState_Failed;
}
else
{
NoReturn_reqresp.State=RequestState_Ok;
}
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
}
return NoReturn_reqresp;
}

NoReturn_Return& NoReturn_Requester::NoOs_NoReturn(RequestParameter* rp,int32_t a)
{
//write serialization code:NoReturn(a,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
if (rp->IsProvideBuffer == false)
{
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
RpcObject->CurrentRequestSid=NoReturn_ServiceId;
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
NoReturn_SendData.a=a;
NoReturn_Parameter_Serialize(&sm,&NoReturn_SendData);
buffer[0]=(uint8_t)(NoReturn_ServiceId&0xff);
buffer[1]=(uint8_t)(NoReturn_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
NoReturn_reqresp.State=RequestState_Failed;
}
else
{
NoReturn_reqresp.State=RequestState_Ok;
}
return NoReturn_reqresp;
}

NoArgAndReturn_Return& NoArgAndReturn_Requester::NoArgAndReturn(RequestParameter* rp)
{
//write serialization code:NoArgAndReturn()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER,rp->IsIsr);
}
if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)
{
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
}
else
{
El_ResetQueue(RpcObject->MessageQueueOfRequestService,rp->IsIsr);
}
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
NoArgAndReturn_Parameter_Serialize(&sm,&NoArgAndReturn_SendData);
buffer[0]=(uint8_t)(NoArgAndReturn_ServiceId&0xff);
buffer[1]=(uint8_t)(NoArgAndReturn_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
NoArgAndReturn_reqresp.State=RequestState_Failed;
}
else
{
NoArgAndReturn_reqresp.State=RequestState_Ok;
}
if (rp->IsProvideBuffer == false)
{
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
}
return NoArgAndReturn_reqresp;
}

NoArgAndReturn_Return& NoArgAndReturn_Requester::NoOs_NoArgAndReturn(RequestParameter* rp)
{
//write serialization code:NoArgAndReturn()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
uint8_t* buffer = nullptr;
uint32_t bufferLen = 0;
BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);
if (rp->IsProvideBuffer == false)
{
buffer = RpcObject->DataLinkBufferForRequest.Buffer;
bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;
}
else
{
buffer = rp->Buffer;
bufferLen = rp->BufferLen;
}
RpcObject->CurrentRequestSid=NoArgAndReturn_ServiceId;
sm.Index=0;
sm.Buf = &buffer[4+4+4];
sm.BufferLen = bufferLen-4-4-4;
NoArgAndReturn_Parameter_Serialize(&sm,&NoArgAndReturn_SendData);
buffer[0]=(uint8_t)(NoArgAndReturn_ServiceId&0xff);
buffer[1]=(uint8_t)(NoArgAndReturn_ServiceId>>8&0xff);
buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

buffer[4]=(uint8_t)(sm.Index&0xff);
buffer[5]=(uint8_t)(sm.Index>>8&0xff);
buffer[6]=(uint8_t)(sm.Index>>16&0xff);
buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
buffer[8]=(uint8_t)(bufcrc&0xff);
buffer[9]=(uint8_t)(bufcrc>>8&0xff);
buffer[10]=(uint8_t)(bufcrc>>16&0xff);
buffer[11]=(uint8_t)(bufcrc>>24&0xff);

RequestTick=El_GetTick(rp->IsIsr);
result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);
sm.Index=0;
if(result==false)
{
NoArgAndReturn_reqresp.State=RequestState_Failed;
}
else
{
NoArgAndReturn_reqresp.State=RequestState_Ok;
}
return NoArgAndReturn_reqresp;
}

