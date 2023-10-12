#include"Sample1.Client.h"
DateTimeChange_Return& DateTimeChange_Requester::DateTimeChange(RequestParameter* rp,DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
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

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
DateTimeChange_SendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(&sm,&DateTimeChange_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
DateTimeChange_reqresp.State=RequestState_Failed;
}
else
{
DateTimeChange_reqresp.State=RequestState_Ok;

}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
return DateTimeChange_reqresp;
}

DateTimeChange_Return& DateTimeChange_Requester::NoOs_DateTimeChange(RequestParameter* rp,DateTime_t now[1])
{
//write serialization code:DateTimeChange(now,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
RequestTick=El_GetTick(rp->IsIsr);
RpcObject->CurrentReceivedData.Sid=EmbedXrpcNoReceivedSid;
RpcObject->CurrentReceivedData.DataLen=0;
RpcObject->CurrentReceivedData.Data=NULL;
sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
DateTimeChange_SendData.now[0]=now[0];
DateTimeChange_Parameter_Serialize(&sm,&DateTimeChange_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(DateTimeChange_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(DateTimeChange_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
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

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
Test2_Parameter_Serialize(&sm,&Test2_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Test2_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Test2_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
Test2_reqresp.State=RequestState_Failed;
}
else
{
Test2_reqresp.State=RequestState_Ok;

}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
return Test2_reqresp;
}

Test2_Return& Test2_Requester::NoOs_Test2(RequestParameter* rp)
{//write serialization code:Test2()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
RequestTick=El_GetTick(rp->IsIsr);
RpcObject->CurrentReceivedData.Sid=EmbedXrpcNoReceivedSid;
RpcObject->CurrentReceivedData.DataLen=0;
RpcObject->CurrentReceivedData.Data=NULL;
sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
Test2_Parameter_Serialize(&sm,&Test2_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Test2_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Test2_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
auto waitstate=ResponseState_Timeout;
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

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
Add_Parameter_Serialize(&sm,&Add_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Add_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Add_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
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
auto waitstate=ResponseState_Timeout;
RequestTick=El_GetTick(rp->IsIsr);
RpcObject->CurrentReceivedData.Sid=EmbedXrpcNoReceivedSid;
RpcObject->CurrentReceivedData.DataLen=0;
RpcObject->CurrentReceivedData.Data=NULL;
sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
Add_Parameter_Serialize(&sm,&Add_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(Add_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(Add_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
if((RequestTick+RpcObject->TimeOut)<nowTick)
{
Add_reqresp.State = RequestResponseState::ResponseState_Timeout;
return Add_reqresp;
}
if(RpcObject->CurrentReceivedData.Sid == EmbedXrpcNoReceivedSid)
{
Add_reqresp.State = RequestResponseState::ResponseState_NoReceived;
return Add_reqresp;
}
else if (RpcObject->CurrentReceivedData.Sid == Add_ServiceId)
{
sm.Index=0;
sm.BufferLen = RpcObject->CurrentReceivedData.DataLen;
sm.Buf = RpcObject->CurrentReceivedData.Data;
Add_Return_Deserialize(&sm,&Add_reqresp,rp->IsIsr);
Add_reqresp.State = RequestResponseState::ResponseState_Ok;
return Add_reqresp;
}
else if (RpcObject->CurrentReceivedData.Sid == EmbedXrpcSuspendSid)
{
Add_reqresp.State = RequestResponseState::ResponseState_NoReceived;
RequestTick=nowTick;
return Add_reqresp;
}
else
{
Add_reqresp.State = RequestResponseState::ResponseState_SidError;
return Add_reqresp;
}
}
NoArg_Return& NoArg_Requester::NoArg(RequestParameter* rp)
{
//write serialization code:NoArg()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
auto waitstate=ResponseState_Timeout;
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

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoArg_Parameter_Serialize(&sm,&NoArg_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoArg_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoArg_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
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
auto waitstate=ResponseState_Timeout;
RequestTick=El_GetTick(rp->IsIsr);
RpcObject->CurrentReceivedData.Sid=EmbedXrpcNoReceivedSid;
RpcObject->CurrentReceivedData.DataLen=0;
RpcObject->CurrentReceivedData.Data=NULL;
sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoArg_Parameter_Serialize(&sm,&NoArg_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoArg_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoArg_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
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
if((RequestTick+RpcObject->TimeOut)<nowTick)
{
NoArg_reqresp.State = RequestResponseState::ResponseState_Timeout;
return NoArg_reqresp;
}
if(RpcObject->CurrentReceivedData.Sid == EmbedXrpcNoReceivedSid)
{
NoArg_reqresp.State = RequestResponseState::ResponseState_NoReceived;
return NoArg_reqresp;
}
else if (RpcObject->CurrentReceivedData.Sid == NoArg_ServiceId)
{
sm.Index=0;
sm.BufferLen = RpcObject->CurrentReceivedData.DataLen;
sm.Buf = RpcObject->CurrentReceivedData.Data;
NoArg_Return_Deserialize(&sm,&NoArg_reqresp,rp->IsIsr);
NoArg_reqresp.State = RequestResponseState::ResponseState_Ok;
return NoArg_reqresp;
}
else if (RpcObject->CurrentReceivedData.Sid == EmbedXrpcSuspendSid)
{
NoArg_reqresp.State = RequestResponseState::ResponseState_NoReceived;
RequestTick=nowTick;
return NoArg_reqresp;
}
else
{
NoArg_reqresp.State = RequestResponseState::ResponseState_SidError;
return NoArg_reqresp;
}
}
NoReturn_Return& NoReturn_Requester::NoReturn(RequestParameter* rp,int32_t a)
{
//write serialization code:NoReturn(a,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
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

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoReturn_SendData.a=a;
NoReturn_Parameter_Serialize(&sm,&NoReturn_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoReturn_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoReturn_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
NoReturn_reqresp.State=RequestState_Failed;
}
else
{
NoReturn_reqresp.State=RequestState_Ok;

}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
return NoReturn_reqresp;
}

NoReturn_Return& NoReturn_Requester::NoOs_NoReturn(RequestParameter* rp,int32_t a)
{
//write serialization code:NoReturn(a,)
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
RequestTick=El_GetTick(rp->IsIsr);
RpcObject->CurrentReceivedData.Sid=EmbedXrpcNoReceivedSid;
RpcObject->CurrentReceivedData.DataLen=0;
RpcObject->CurrentReceivedData.Data=NULL;
sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoReturn_SendData.a=a;
NoReturn_Parameter_Serialize(&sm,&NoReturn_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoReturn_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoReturn_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
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

sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoArgAndReturn_Parameter_Serialize(&sm,&NoArgAndReturn_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoArgAndReturn_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoArgAndReturn_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
sm.Index=0;
if(result==false)
{
NoArgAndReturn_reqresp.State=RequestState_Failed;
}
else
{
NoArgAndReturn_reqresp.State=RequestState_Ok;

}
if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)
{
El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);
}
return NoArgAndReturn_reqresp;
}

NoArgAndReturn_Return& NoArgAndReturn_Requester::NoOs_NoArgAndReturn(RequestParameter* rp)
{
//write serialization code:NoArgAndReturn()
SerializationManager sm;
El_Memset(&sm,0,sizeof(SerializationManager));
auto result=false;
RequestTick=El_GetTick(rp->IsIsr);
RpcObject->CurrentReceivedData.Sid=EmbedXrpcNoReceivedSid;
RpcObject->CurrentReceivedData.DataLen=0;
RpcObject->CurrentReceivedData.Data=NULL;
sm.Index=0;
sm.Buf = &RpcObject->DataLinkBufferForRequest.Buffer[4+4+4];
sm.BufferLen = RpcObject->DataLinkBufferForRequest.BufferLen-4-4-4;
NoArgAndReturn_Parameter_Serialize(&sm,&NoArgAndReturn_SendData);
RpcObject->DataLinkBufferForRequest.Buffer[0]=(uint8_t)(NoArgAndReturn_ServiceId&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[1]=(uint8_t)(NoArgAndReturn_ServiceId>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);
RpcObject->DataLinkBufferForRequest.Buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);

RpcObject->DataLinkBufferForRequest.Buffer[4]=(uint8_t)(sm.Index&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[5]=(uint8_t)(sm.Index>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[6]=(uint8_t)(sm.Index>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[7]=(uint8_t)(sm.Index>>24&0xff);

uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);
RpcObject->DataLinkBufferForRequest.Buffer[8]=(uint8_t)(bufcrc&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[9]=(uint8_t)(bufcrc>>8&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[10]=(uint8_t)(bufcrc>>16&0xff);
RpcObject->DataLinkBufferForRequest.Buffer[11]=(uint8_t)(bufcrc>>24&0xff);

result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,RpcObject->DataLinkBufferForRequest.Buffer);
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

