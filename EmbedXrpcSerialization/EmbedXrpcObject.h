#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedLibrary.h"
#include "EmbedXrpcCommon.h"


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
	EmbedXrpcBufferConfig ClientUseDataLinkBufferConfig;
	SendPack_t Sender;
	uint32_t TimeOut;

	EmbedXrpcConfig RpcConfig;

	void* UserData;
};


struct ServerNodeQuicklyInitConfig
{
	char Name[EmbedXrpc_NameMaxLen];
	EmbedXrpcBufferConfig ServerUseDataLinkBufferConfig;

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

	EmbedXrpcBufferConfig ClientUseDataLinkBufferConfig;
	EmbedXrpcBufferConfig ServerUseDataLinkBufferConfig;
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

	EmbedXrpcBuffer ClientUseDataLinkBuffer;
	MessageBufferHandle_t ClientUseRespondedData ;


	uint32_t TimeOut;
	SendPack_t Send;
	
	void* UserData;
	volatile bool DeInitFlag;
	volatile bool ServiceThreadExitState;
	EmbedXrpcConfig RpcConfig;

	//server:
	El_Thread_t ServiceThreadHandle;

	EmbedXrpcBuffer ServerUseDataLinkBuffer;
	MessageBufferHandle_t	ServerUseRequestedData;

	El_Timer_t SuspendTimer;

	uint32_t ServicesCount;
	ServiceDescribe* Services;
	uint16_t CurrentRequestSid;
	void NoOs_ServiceExecute(int IsIsr);
public:
	//client节点构造函数
	void Init(ClientNodeQuicklyInitConfig* client)
	{
		InitConfig cfg;
		El_Memset(&cfg, 0, sizeof(InitConfig));
		El_Strncpy(cfg.Name, client->Name, EmbedXrpc_NameMaxLen);
		cfg.ClientUseDataLinkBufferConfig = client->ClientUseDataLinkBufferConfig;
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
		cfg.ServerUseDataLinkBufferConfig = server->ServerUseDataLinkBufferConfig;
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
	ReceivedMessageStatus ReceivedMessage(uint32_t allDataLen, uint8_t* allData, int isIsr);
	static void SuspendTimerCallBack(void* arg);


	static void ServiceThread(void* arg);
	

	RequestResponseState Wait(uint32_t sid, ReceiveItemInfo* recData,int IsIsr);

	
};

#endif
