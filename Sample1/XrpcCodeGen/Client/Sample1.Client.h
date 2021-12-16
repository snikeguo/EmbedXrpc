#ifndef Sample1_Client_H
#define Sample1_Client_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
template<class DTL>
class DateTimeChange_DelegateReceiver :public IDelegate<DTL>
{
public:
	uint16_t GetSid() { return DateTimeChange_ServiceId; }
	virtual void DateTimeChange(DTL* userDataOfTransportLayer, DateTime_t now[1]) = 0;
	DateTimeChange_Parameter request;
	void Invoke(EmbedXrpcConfig* rpcConfig, DTL* userDataOfTransportLayer, SerializationManager* recManager)
	{
		DateTimeChange_Parameter_Deserialize(recManager, &request);
		if (rpcConfig->CheckSumValid == true)
		{
			El_Assert(recManager->GetReferenceSum() == recManager->GetCalculateSum());
		}//if(rpcConfig->CheckSumValid==true)
		DateTimeChange(userDataOfTransportLayer, request.now);
		DateTimeChange_Parameter_FreeData(&request);
	}
};


template<class DTL>
class TestDelegate_DelegateReceiver :public IDelegate<DTL>
{
public:
	uint16_t GetSid() { return TestDelegate_ServiceId; }
	virtual void TestDelegate(DTL* userDataOfTransportLayer, DateTime_t now[1]) = 0;
	TestDelegate_Parameter request;
	void Invoke(EmbedXrpcConfig* rpcConfig, DTL* userDataOfTransportLayer, SerializationManager* recManager)
	{
		TestDelegate_Parameter_Deserialize(recManager, &request);
		if (rpcConfig->CheckSumValid == true)
		{
			El_Assert(recManager->GetReferenceSum() == recManager->GetCalculateSum());
		}//if(rpcConfig->CheckSumValid==true)
		TestDelegate(userDataOfTransportLayer, request.now);
		TestDelegate_Parameter_FreeData(&request);
	}
};


template<class DTL>
class InterClientImpl
{
public:
	EmbedXrpcObject<DTL>* RpcObject = nullptr;
	InterClientImpl(EmbedXrpcObject<DTL>* rpcobj) :RpcObject(rpcobj)
	{}
	Inter_Add_Parameter Add_SendData;
	Inter_Add_Return Add_reqresp;
	Inter_Add_Return& Add(DTL* userDataOfTransportLayer)
	{//write serialization code:Add()
		SerializationManager sm;
		sm.Reset();
		auto result = false;
		auto waitstate = ResponseState_Timeout;
		El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
		if (RpcObject->RpcConfig->UseRingBufferWhenReceiving == true)
		{
			RpcObject->ResponseBlockBufferProvider->Reset();
		}
		else
		{
			El_ResetQueue(RpcObject->ResponseBlockQueue);
		}

		sm.Reset();
		sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
		sm.BufferLen = RpcObject->DataLinkLayoutBufferLen - 4;
		Inter_Add_Parameter_Serialize(&sm, &Add_SendData);
		RpcObject->DataLinkLayoutBuffer[0] = (uint8_t)(Inter_Add_ServiceId & 0xff);
		RpcObject->DataLinkLayoutBuffer[1] = (uint8_t)(Inter_Add_ServiceId >> 8 & 0xff);
		RpcObject->DataLinkLayoutBuffer[2] = (uint8_t)(RpcObject->TimeOut >> 0 & 0xff);
		RpcObject->DataLinkLayoutBuffer[3] = (uint8_t)((RpcObject->TimeOut >> 8 & 0xff) & 0x3FF);
		RpcObject->DataLinkLayoutBuffer[3] |= (uint8_t)((uint8_t)(ReceiveType_Request) << 6);
		result = RpcObject->Sender.Invoke(userDataOfTransportLayer, RpcObject, sm.Index + 4, RpcObject->DataLinkLayoutBuffer);
		sm.Reset();
		if (result == false)
		{
			Add_reqresp.State = RequestState_Failed;
			goto exi;
		}
		else
		{
			Add_reqresp.State = RequestState_Ok;
		}
		ReceiveItemInfo<DTL> recData;
		waitstate = RpcObject->Wait(Inter_Add_ServiceId, &recData);
		if (waitstate == RequestResponseState::ResponseState_Ok)
		{
			if (RpcObject->RpcConfig->UseRingBufferWhenReceiving == true)
			{
				sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;
			}
			else
			{
				sm.Reset();
				sm.BufferLen = recData.DataLen;
				sm.Buf = recData.Data;
			}
			if (RpcObject->RpcConfig->CheckSumValid == true)
			{
				sm.SetCalculateSum(0);
				sm.SetReferenceSum(recData.CheckSum);
			}
			Inter_Add_Return_Deserialize(&sm, &Add_reqresp);
			if (RpcObject->RpcConfig->CheckSumValid == true)
			{
				El_Assert(sm.GetReferenceSum() == sm.GetCalculateSum());
			}
		}
		if (RpcObject->RpcConfig->UseRingBufferWhenReceiving == false)
		{
			if (waitstate != RequestResponseState::ResponseState_Timeout)
			{
				if (recData.DataLen > 0)
				{
					El_Free(recData.Data);
				}
			}
		}//if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==false)
		Add_reqresp.State = waitstate;
	exi:
		El_ReleaseMutex(RpcObject->ObjectMutexHandle);
		return Add_reqresp;
	}
	void Free_Add(Inter_Add_Return* response)
	{
		if (response->State == ResponseState_Ok)
		{
			Inter_Add_Return_FreeData(response);
		}
	}




	Inter_NoArg_Parameter NoArg_SendData;
	Inter_NoArg_Return NoArg_reqresp;
	Inter_NoArg_Return& NoArg(DTL* userDataOfTransportLayer)
	{
		//write serialization code:NoArg()
		SerializationManager sm;
		sm.Reset();
		auto result = false;
		auto waitstate = ResponseState_Timeout;
		El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
		if (RpcObject->RpcConfig->UseRingBufferWhenReceiving == true)
		{
			RpcObject->ResponseBlockBufferProvider->Reset();
		}
		else
		{
			El_ResetQueue(RpcObject->ResponseBlockQueue);
		}

		sm.Reset();
		sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
		sm.BufferLen = RpcObject->DataLinkLayoutBufferLen - 4;
		Inter_NoArg_Parameter_Serialize(&sm, &NoArg_SendData);
		RpcObject->DataLinkLayoutBuffer[0] = (uint8_t)(Inter_NoArg_ServiceId & 0xff);
		RpcObject->DataLinkLayoutBuffer[1] = (uint8_t)(Inter_NoArg_ServiceId >> 8 & 0xff);
		RpcObject->DataLinkLayoutBuffer[2] = (uint8_t)(RpcObject->TimeOut >> 0 & 0xff);
		RpcObject->DataLinkLayoutBuffer[3] = (uint8_t)((RpcObject->TimeOut >> 8 & 0xff) & 0x3FF);
		RpcObject->DataLinkLayoutBuffer[3] |= (uint8_t)((uint8_t)(ReceiveType_Request) << 6);
		result = RpcObject->Sender.Invoke(userDataOfTransportLayer, RpcObject, sm.Index + 4, RpcObject->DataLinkLayoutBuffer);
		sm.Reset();
		if (result == false)
		{
			NoArg_reqresp.State = RequestState_Failed;
			goto exi;
		}
		else
		{
			NoArg_reqresp.State = RequestState_Ok;
		}
		ReceiveItemInfo<DTL> recData;
		waitstate = RpcObject->Wait(Inter_NoArg_ServiceId, &recData);
		if (waitstate == RequestResponseState::ResponseState_Ok)
		{
			if (RpcObject->RpcConfig->UseRingBufferWhenReceiving == true)
			{
				sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;
			}
			else
			{
				sm.Reset();
				sm.BufferLen = recData.DataLen;
				sm.Buf = recData.Data;
			}
			if (RpcObject->RpcConfig->CheckSumValid == true)
			{
				sm.SetCalculateSum(0);
				sm.SetReferenceSum(recData.CheckSum);
			}
			Inter_NoArg_Return_Deserialize(&sm, &NoArg_reqresp);
			if (RpcObject->RpcConfig->CheckSumValid == true)
			{
				El_Assert(sm.GetReferenceSum() == sm.GetCalculateSum());
			}
		}
		if (RpcObject->RpcConfig->UseRingBufferWhenReceiving == false)
		{
			if (waitstate != RequestResponseState::ResponseState_Timeout)
			{
				if (recData.DataLen > 0)
				{
					El_Free(recData.Data);
				}
			}
		}//if(RpcObject->RpcConfig->UseRingBufferWhenReceiving==false)
		NoArg_reqresp.State = waitstate;
	exi:
		El_ReleaseMutex(RpcObject->ObjectMutexHandle);
		return NoArg_reqresp;
	}
	void Free_NoArg(Inter_NoArg_Return* response)
	{
		if (response->State == ResponseState_Ok)
		{
			Inter_NoArg_Return_FreeData(response);
		}
	}




	Inter_NoReturn_Parameter NoReturn_SendData;
	Inter_NoReturn_Return NoReturn_reqresp;
	Inter_NoReturn_Return& NoReturn(DTL* userDataOfTransportLayer, Int32 a)
	{
		//write serialization code:NoReturn(a,)
		SerializationManager sm;
		sm.Reset();
		auto result = false;
		El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
		if (RpcObject->RpcConfig->UseRingBufferWhenReceiving == true)
		{
			RpcObject->ResponseBlockBufferProvider->Reset();
		}
		else
		{
			El_ResetQueue(RpcObject->ResponseBlockQueue);
		}

		sm.Reset();
		sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
		sm.BufferLen = RpcObject->DataLinkLayoutBufferLen - 4;
		NoReturn_SendData.a = a;
		Inter_NoReturn_Parameter_Serialize(&sm, &NoReturn_SendData);
		RpcObject->DataLinkLayoutBuffer[0] = (uint8_t)(Inter_NoReturn_ServiceId & 0xff);
		RpcObject->DataLinkLayoutBuffer[1] = (uint8_t)(Inter_NoReturn_ServiceId >> 8 & 0xff);
		RpcObject->DataLinkLayoutBuffer[2] = (uint8_t)(RpcObject->TimeOut >> 0 & 0xff);
		RpcObject->DataLinkLayoutBuffer[3] = (uint8_t)((RpcObject->TimeOut >> 8 & 0xff) & 0x3FF);
		RpcObject->DataLinkLayoutBuffer[3] |= (uint8_t)((uint8_t)(ReceiveType_Request) << 6);
		result = RpcObject->Sender.Invoke(userDataOfTransportLayer, RpcObject, sm.Index + 4, RpcObject->DataLinkLayoutBuffer);
		sm.Reset();
		if (result == false)
		{
			NoReturn_reqresp.State = RequestState_Failed;
			goto exi;
		}
		else
		{
			NoReturn_reqresp.State = RequestState_Ok;
		}
	exi:
		El_ReleaseMutex(RpcObject->ObjectMutexHandle);
		return NoReturn_reqresp;
	}




	Inter_NoArgAndReturn_Parameter NoArgAndReturn_SendData;
	Inter_NoArgAndReturn_Return NoArgAndReturn_reqresp;
	Inter_NoArgAndReturn_Return& NoArgAndReturn(DTL* userDataOfTransportLayer)
	{
		//write serialization code:NoArgAndReturn()
		SerializationManager sm;
		sm.Reset();
		auto result = false;
		El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);
		if (RpcObject->RpcConfig->UseRingBufferWhenReceiving == true)
		{
			RpcObject->ResponseBlockBufferProvider->Reset();
		}
		else
		{
			El_ResetQueue(RpcObject->ResponseBlockQueue);
		}

		sm.Reset();
		sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];
		sm.BufferLen = RpcObject->DataLinkLayoutBufferLen - 4;
		Inter_NoArgAndReturn_Parameter_Serialize(&sm, &NoArgAndReturn_SendData);
		RpcObject->DataLinkLayoutBuffer[0] = (uint8_t)(Inter_NoArgAndReturn_ServiceId & 0xff);
		RpcObject->DataLinkLayoutBuffer[1] = (uint8_t)(Inter_NoArgAndReturn_ServiceId >> 8 & 0xff);
		RpcObject->DataLinkLayoutBuffer[2] = (uint8_t)(RpcObject->TimeOut >> 0 & 0xff);
		RpcObject->DataLinkLayoutBuffer[3] = (uint8_t)((RpcObject->TimeOut >> 8 & 0xff) & 0x3FF);
		RpcObject->DataLinkLayoutBuffer[3] |= (uint8_t)((uint8_t)(ReceiveType_Request) << 6);
		result = RpcObject->Sender.Invoke(userDataOfTransportLayer, RpcObject, sm.Index + 4, RpcObject->DataLinkLayoutBuffer);
		sm.Reset();
		if (result == false)
		{
			NoArgAndReturn_reqresp.State = RequestState_Failed;
			goto exi;
		}
		else
		{
			NoArgAndReturn_reqresp.State = RequestState_Ok;
		}
	exi:
		El_ReleaseMutex(RpcObject->ObjectMutexHandle);
		return NoArgAndReturn_reqresp;
	}




};

/*
The Delegates Of Inter:
name                   type

"DateTimeChange"        DateTimeChangeClientImpl,
"TestDelegate"        TestDelegateClientImpl,

DelegatesCount:2



The Responses Of Inter:
name                   sid

"Inter_Add"        Inter_Add_ServiceId
"Inter_NoArg"        Inter_NoArg_ServiceId
"Inter_NoReturn"        Inter_NoReturn_ServiceId
"Inter_NoArgAndReturn"        Inter_NoArgAndReturn_ServiceId

ResponsesCount:4
*/


#endif
