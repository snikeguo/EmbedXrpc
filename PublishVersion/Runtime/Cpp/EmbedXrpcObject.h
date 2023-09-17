#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedLibrary.h"
#include "EmbedXrpcCommon.h"
#include "BlockBufferProvider.h"

struct EmbedXrpcBufferConfig
{
	uint8_t* Buffer;
	uint32_t BufferLen;
	bool IsUseMutex;
};

struct EmbedXrpcBuffer
{
	uint8_t* Buffer;
	uint32_t BufferLen;
	El_Mutex_t MutexHandle;
};


struct ClientNodeQuicklyInitConfig
{
	char Name[EmbedXrpc_NameMaxLen];
	EmbedXrpcBufferConfig DataLinkBufferConfigForRequest;
	SendPack_t Sender;
	uint32_t TimeOut;

	EmbedXrpcConfig RpcConfig;

	void* UserData;
};


struct ServerNodeQuicklyInitConfig
{
	char Name[EmbedXrpc_NameMaxLen];
	EmbedXrpcBufferConfig DataLinkBufferConfigForResponse;

	SendPack_t Sender;
	uint32_t TimeOut;

	ServiceDescribe* Services;//server 请求的services
	uint32_t ServicesCount;//server

	EmbedXrpcConfig RpcConfig;
	void* UserData;
};


struct InitConfig
{
	char Name[EmbedXrpc_NameMaxLen];

	EmbedXrpcBufferConfig DataLinkBufferConfigForRequest;
	EmbedXrpcBufferConfig DataLinkBufferConfigForResponse;
	SendPack_t Sender;
	uint32_t TimeOut;

	ServiceDescribe* Services;//server 请求的services
	uint32_t ServicesCount;//server

	EmbedXrpcConfig RpcConfig;
	void* UserData;
};
enum ReceivedMessageStatus
{
	InvalidData,
	QueueFull,
	Ok,
};
uint32_t GetBufferCrc(uint32_t len, uint8_t* Buf);
class EmbedXrpcObject
{
public:

	char Name[EmbedXrpc_NameMaxLen];

	EmbedXrpcBuffer DataLinkBufferForRequest;
	EmbedXrpcBuffer DataLinkBufferForResponse;
	

	uint32_t TimeOut;
	SendPack_t Send;


	BlockRingBufferProvider* BlockBufferProviderOfRequestService = nullptr;
	
	El_Queue_t MessageQueueOfRequestService = nullptr;

	void* UserData;

	volatile bool DeInitFlag;
	volatile bool ServiceThreadExitState;
	EmbedXrpcConfig RpcConfig;

	//server:
	El_Thread_t ServiceThreadHandle; 
	
	BlockRingBufferProvider* ServiceBlockBufferProvider;
	El_Queue_t	ServiceMessageQueue;
	El_Timer_t SuspendTimer;
	UserDataOfTransportLayer_t UserDataOfTransportLayerOfSuspendTimerUsed;

	uint32_t ServicesCount;
	ServiceDescribe* Services;

	ReceiveItemInfo CurrentReceivedData;
public:
	//client节点构造函数
	void Init(ClientNodeQuicklyInitConfig* client)
	{
		InitConfig cfg;
		El_Memset(&cfg, 0, sizeof(InitConfig));
		El_Strncpy(cfg.Name, client->Name, EmbedXrpc_NameMaxLen);
		cfg.DataLinkBufferConfigForRequest = client->DataLinkBufferConfigForRequest;
		cfg.Sender = client->Sender;
		cfg.TimeOut = client->TimeOut;
		cfg.RpcConfig = client->RpcConfig;
		cfg.UserData = client->UserData;
		Init(&cfg);
	}
	//server节点的构造函数
	void Init(ServerNodeQuicklyInitConfig* server)
	{
		InitConfig cfg;
		El_Memset(&cfg, 0, sizeof(InitConfig));
		El_Strncpy(cfg.Name, server->Name, EmbedXrpc_NameMaxLen);
		cfg.DataLinkBufferConfigForResponse = server->DataLinkBufferConfigForResponse;
		cfg.Sender = server->Sender;
		cfg.TimeOut = server->TimeOut;
		cfg.Services = server->Services;
		cfg.ServicesCount = server->ServicesCount;
		cfg.RpcConfig = server->RpcConfig;
		cfg.UserData = server->UserData;
		Init(&cfg);
	}

	void Init(InitConfig* cfg);
	void DeInit();
	static void ServiceExecute(EmbedXrpcObject* obj, ReceiveItemInfo& recData, bool isFreeData, int isIsr);
	ReceivedMessageStatus ReceivedMessage(uint32_t allDataLen, uint8_t* allData, UserDataOfTransportLayer_t userDataOfTransportLayer,int isIsr);
	static void SuspendTimerCallBack(void* arg);


	static void ServiceThread(void* arg);

	RequestResponseState Wait(uint32_t sid, ReceiveItemInfo* recData,int IsIsr);

	
};

#endif
