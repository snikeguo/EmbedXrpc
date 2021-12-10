#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedLibrary.h"
#include "EmbedXrpcCommon.h"
#if EmbedXrpc_UseRingBufferWhenReceiving==1
#include "BlockBufferProvider.h"

#if EmbedXrpc_IsSendToQueue==0
#error "EmbedXrpc_UseRingBufferWhenReceiving=1的前置条件必须是EmbedXrpc_IsSendToQueue=1"
#endif

#endif
#define EmbedXrpcObjectVersion	"2.0.6"
class EmbedXrpcObject
{
public:

	uint8_t *DataLinkLayoutBuffer;
	uint32_t DataLinkLayoutBufferLen;

	uint32_t TimeOut;
	SendPack_t Send;
	El_Mutex_t ObjectMutexHandle;

#if EmbedXrpc_UseRingBufferWhenReceiving==1
	uint8_t DelegateRingBuffer[EmbedXrpc_DelegateRingBufferSize];
	BlockRingBufferProvider* DelegateBlockBufferProvider = nullptr;

	uint8_t ResponseRingBuffer[EmbedXrpc_ResponseRingBufferSize];
	BlockRingBufferProvider* ResponseBlockBufferProvider = nullptr;
#else
	El_Queue_t DelegateBlockQueue = nullptr;
	El_Queue_t ResponseBlockQueue = nullptr;
#endif


	El_Thread_t DelegateServiceThreadHandle;

	uint32_t DelegatesCount;
	DelegateDescribe* Delegates;

	uint32_t ResponsesCount;
	ResponseDescribe* Responses;

	void* UserData;

	volatile bool DeInitFlag;
	volatile bool DelegateServiceThreadExitState;
	volatile bool RequestProcessServiceThreadExitState;
	//bool IsEnableMataDataEncode;

	//server:
	El_Thread_t RequestProcessServiceThreadHandle; 
	//El_Mutex_t ObjectMutexHandle;
#if EmbedXrpc_UseRingBufferWhenReceiving==1
	uint8_t RequestRingBuffer[EmbedXrpc_RequestRingBufferSize];
	BlockRingBufferProvider* RequestBlockBufferProvider;
#else
	El_Queue_t	RequestBlockQueue;
#endif
	El_Timer_t SuspendTimer;
	UserDataOfTransportLayer_t UserDataOfTransportLayerOfSuspendTimerUsed;

	uint32_t RequestsCount;
	RequestDescribe* Requests;

	EmbedXrpcObject(
		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,
		SendPack_t send,
		uint32_t timeOut,

		ResponseDescribe* responses,
		uint32_t responsesCount,

		DelegateDescribe* delegates,//代理的services
		uint32_t delegatesCount,

		RequestDescribe* requests,//server 请求的services
		uint32_t requestsCount,//server

		//bool isEnableMataDataEncode,
		void* ud = nullptr) :

		DataLinkLayoutBuffer(dataLinkLayoutBuffer),
		DataLinkLayoutBufferLen(dataLinkLayoutBufferLen),

		TimeOut(timeOut),
		Send(send),
		ObjectMutexHandle(nullptr),
		DelegateServiceThreadHandle(nullptr),

		DelegatesCount(delegatesCount),
		Delegates(delegates),

		ResponsesCount(responsesCount),
		Responses(responses),

		UserData(ud),
		DeInitFlag(false),
		//IsEnableMataDataEncode(isEnableMataDataEncode),

		RequestProcessServiceThreadHandle(nullptr),
		SuspendTimer(nullptr),

		RequestsCount(requestsCount),
		Requests(requests)
	{

	}
	//client节点构造函数
	EmbedXrpcObject(
		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,
		SendPack_t send,
		uint32_t timeOut,

		ResponseDescribe* responses,
		uint32_t responsesCount,

		DelegateDescribe* delegates,
		uint32_t delegatesCount,

		//bool isEnableMataDataEncode,
		void* ud = nullptr) :EmbedXrpcObject(dataLinkLayoutBuffer, 
			dataLinkLayoutBufferLen,

			send,
			timeOut,

			responses,
			responsesCount,

			delegates,
			delegatesCount,

			nullptr,
			0,

			//isEnableMataDataEncode,
			ud
		)
	{

	}
	//server节点的构造函数
	EmbedXrpcObject(
		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,
		SendPack_t send,
		uint32_t timeOut,

		RequestDescribe* requests,//server 请求的services
		uint32_t requestsCount,//server

		//bool isEnableMataDataEncode,
		void* ud = nullptr) :EmbedXrpcObject(dataLinkLayoutBuffer, 
			dataLinkLayoutBufferLen,

			send,
			timeOut,

			nullptr,
			0,

			nullptr,
			0,

			requests,
			requestsCount,

			//isEnableMataDataEncode,
			ud
		)
	{

	}

	void Init();
	void DeInit();
	static void DelegateServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData);
	static void ResponseServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData);
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData, UserDataOfTransportLayer_t userDataOfTransportLayer);
	static void SuspendTimerCallBack(void* arg);

#if EmbedXrpc_DelegateBlockQueue_MaxItemNumber>0 &&EmbedXrpc_IsSendToQueue==1
	static void DelegateServiceThread(void* arg);
#endif

#if EmbedXrpc_RequestBlockQueue_MaxItemNumber>0 && EmbedXrpc_IsSendToQueue==1
	static void RequestProcessServiceThread(void* arg);
#endif

#if EmbedXrpc_ResponseBlockQueue_MaxItemNumber>0
	RequestResponseState Wait(uint32_t sid, ReceiveItemInfo* recData);
#endif	
};

#endif
