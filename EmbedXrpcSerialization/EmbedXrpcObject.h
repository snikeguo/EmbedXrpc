#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedLibrary.h"
#include "EmbedXrpcCommon.h"
#include "BlockBufferProvider.h"
#define EmbedXrpcObjectVersion	"2.1.0"


struct EmbedXrpcBuffer
{
	uint8_t* Buffer;
	uint32_t BufferLen;
	El_Mutex_t MutexHandle;
};


struct ClientNodeQuicklyInitConfig
{
	char Name[EmbedXrpc_NameMaxLen];
	EmbedXrpcBuffer DataLinkBufferForRequest;

	SendPack_t Sender;
	uint32_t TimeOut;

	ResponseDescribe* Responses;
	uint32_t ResponsesCount;

	DelegateDescribe* Delegates;
	uint32_t DelegatesCount;

	EmbedXrpcConfig RpcConfig;

	void* UserData;
};


struct ServerNodeQuicklyInitConfig
{
	char Name[EmbedXrpc_NameMaxLen];
	EmbedXrpcBuffer DataLinkBufferForResponse;
	EmbedXrpcBuffer DataLinkBufferForDelegate;

	SendPack_t Sender;
	uint32_t TimeOut;

	RequestDescribe* Requests;//server 请求的services
	uint32_t RequestsCount;//server

	EmbedXrpcConfig RpcConfig;
	void* UserData;
};


struct InitConfig
{
	char Name[EmbedXrpc_NameMaxLen];

	EmbedXrpcBuffer DataLinkBufferForRequest;

	EmbedXrpcBuffer DataLinkBufferForResponse;
	EmbedXrpcBuffer DataLinkBufferForDelegate;


	SendPack_t Sender;
	uint32_t TimeOut;

	ResponseDescribe* Responses;
	uint32_t ResponsesCount;

	DelegateDescribe* Delegates;//代理的services
	uint32_t DelegatesCount;

	RequestDescribe* Requests;//server 请求的services
	uint32_t RequestsCount;//server

	EmbedXrpcConfig RpcConfig;
	void* UserData;
};

class EmbedXrpcObject
{
public:

	char Name[EmbedXrpc_NameMaxLen];

	EmbedXrpcBuffer DataLinkBufferForRequest;
	EmbedXrpcBuffer DataLinkBufferForResponse;
	EmbedXrpcBuffer DataLinkBufferForDelegate;
	

	uint32_t TimeOut;
	SendPack_t Send;


	BlockRingBufferProvider* DelegateMessageBlockBufferProvider = nullptr;
	BlockRingBufferProvider* ResponseMessageBlockBufferProvider = nullptr;
	//#else
	El_Queue_t DelegateMessageQueue = nullptr;
	El_Queue_t ResponseMessageQueue = nullptr;


	El_Thread_t DelegateServiceThreadHandle;

	uint32_t DelegatesCount;
	DelegateDescribe* Delegates;

	uint32_t ResponsesCount;
	ResponseDescribe* Responses;

	void* UserData;

	volatile bool DeInitFlag;
	volatile bool DelegateServiceThreadExitState;
	volatile bool RequestProcessServiceThreadExitState;
	EmbedXrpcConfig RpcConfig;

	//server:
	El_Thread_t RequestProcessServiceThreadHandle; 
	//El_Mutex_t ObjectMutexHandle;
	BlockRingBufferProvider* RequestMessageBlockBufferProvider;
	El_Queue_t	RequestMessageQueue;
	El_Timer_t SuspendTimer;
	UserDataOfTransportLayer_t UserDataOfTransportLayerOfSuspendTimerUsed;

	uint32_t RequestsCount;
	RequestDescribe* Requests;
private:
	void ClassInit(InitConfig* cfg)
	{
		El_Strncpy(Name, cfg->Name, EmbedXrpc_NameMaxLen);
		DataLinkBufferForRequest = cfg->DataLinkBufferForRequest;
		DataLinkBufferForResponse = cfg->DataLinkBufferForResponse;
		DataLinkBufferForDelegate = cfg->DataLinkBufferForDelegate;
		

		TimeOut = cfg->TimeOut;
		Send = cfg->Sender;
		//ObjectMutexHandle(nullptr;
		DelegateServiceThreadHandle = nullptr;

		DelegatesCount = cfg->DelegatesCount;
		Delegates = cfg->Delegates;

		ResponsesCount = cfg->ResponsesCount;
		Responses = cfg->Responses;

		UserData = cfg->UserData;
		DeInitFlag = false;

		DelegateMessageBlockBufferProvider = cfg->RpcConfig.RingBufferConfig.DelegateMessageBlockBufferProvider;
		ResponseMessageBlockBufferProvider = cfg->RpcConfig.RingBufferConfig.ResponseMessageBlockBufferProvider;
		RequestMessageBlockBufferProvider = cfg->RpcConfig.RingBufferConfig.RequestMessageBlockBufferProvider;

		RequestProcessServiceThreadHandle = nullptr;
		SuspendTimer = nullptr;

		RequestsCount = cfg->RequestsCount;
		Requests = cfg->Requests;
		RpcConfig = cfg->RpcConfig;
	}
public:
	EmbedXrpcObject(
		InitConfig* cfg)
	{
		ClassInit(cfg);
	}
	//client节点构造函数
	EmbedXrpcObject(
		ClientNodeQuicklyInitConfig* client)
	{
		InitConfig cfg;
		El_Memset(&cfg, 0, sizeof(InitConfig));
		El_Strncpy(cfg.Name, client->Name, EmbedXrpc_NameMaxLen);
		cfg.DataLinkBufferForRequest = client->DataLinkBufferForRequest;
		cfg.Sender = client->Sender;
		cfg.TimeOut = client->TimeOut;
		cfg.Responses = client->Responses;
		cfg.ResponsesCount = client->ResponsesCount;
		cfg.Delegates = client->Delegates;
		cfg.DelegatesCount = client->DelegatesCount;
		cfg.RpcConfig = client->RpcConfig;
		cfg.UserData = client->UserData;
		ClassInit(&cfg);
	}
	//server节点的构造函数
	EmbedXrpcObject(
		ServerNodeQuicklyInitConfig* server) 
	{
		InitConfig cfg;
		El_Memset(&cfg, 0, sizeof(InitConfig));
		El_Strncpy(cfg.Name, server->Name, EmbedXrpc_NameMaxLen);
		cfg.DataLinkBufferForResponse = server->DataLinkBufferForResponse;
		cfg.DataLinkBufferForDelegate = server->DataLinkBufferForDelegate;
		cfg.Sender = server->Sender;
		cfg.TimeOut = server->TimeOut;
		cfg.Requests = server->Requests;
		cfg.RequestsCount = server->RequestsCount;
		cfg.RpcConfig = server->RpcConfig;
		cfg.UserData = server->UserData;
		ClassInit(&cfg);
	}

	void Init();
	void DeInit();
	static void DelegateServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData);
	static void ResponseServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData);
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData, UserDataOfTransportLayer_t userDataOfTransportLayer);
	static void SuspendTimerCallBack(void* arg);


	static void DelegateServiceThread(void* arg);

	static void RequestProcessServiceThread(void* arg);

	RequestResponseState Wait(uint32_t sid, ReceiveItemInfo* recData);
	
};

#endif
