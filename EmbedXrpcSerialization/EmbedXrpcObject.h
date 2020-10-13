#pragma once
#ifndef EmbedXrpcObject_H
#define EmbedXrpcObject_H
#include "EmbedXrpcCommon.h"
#include "BlockBufferProvider.h"
class EmbedXrpcObject
{
public:
	//SerializationManager BufManager;


	uint8_t *DataLinkLayoutBuffer;
	uint32_t DataLinkLayoutBufferLen;

	uint32_t TimeOut;
	SendPack_t Send;
	IEmbeXrpcPort* porter;
	EmbedXrpc_Mutex_t ObjectMutexHandle;

	uint8_t *DelegateRingBuffer;
	int16_t DelegateRingBufferSize;
	BlockRingBufferProvider *DelegateBlockBufferProvider;

	uint8_t *ResponseRingBuffer;
	int16_t ResponseRingBufferSize;
	BlockRingBufferProvider *ResponseBlockBufferProvider;



	EmbedXrpc_Thread_t DelegateServiceThreadHandle;

	uint32_t ResponseDelegateMessageMapsCount;
	ResponseDelegateMessageMapCollection * ResponseDelegateMessageMaps;

	void* UserData;

	bool DeInitFlag;

	bool IsEnableMataDataEncode;

	//server:
	EmbedXrpc_Thread_t ResponseServiceThreadHandle;
	//EmbedXrpc_Mutex_t ObjectMutexHandle;

	uint8_t* RequestRingBuffer;
	int16_t RequestRingBufferSize;
	BlockRingBufferProvider* RequestBlockBufferProvider;

	EmbedXrpc_Timer_t SuspendTimer;

	uint32_t RequestMessageMapsCount;
	RequestMessageMapCollection* RequestMessageMaps;

	EmbedXrpcObject(
		SendPack_t send,
		uint32_t timeOut,

		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,

		uint8_t *responseRingBuffer,//接收到回复数据
		int16_t responseRingBufferSize,

		uint8_t *delegateRingBuffer,//接收到代理广播数据
		int16_t delegateRingBufferSize,

		ResponseDelegateMessageMapCollection* responseDelegateMessageMaps,//代理的services
		uint32_t responseDelegateMessageMapsCount,
		

		uint8_t* requestRingBuffer,//server  接收到请求数据
		int16_t requestRingBufferSize,//server

		RequestMessageMapCollection* requestMessageMaps,//server 请求的services
		uint32_t requestMessageMapsCount,//server

		IEmbeXrpcPort* port,
		bool isEnableMataDataEncode,
		void* ud = nullptr):Send(send),
		TimeOut(timeOut),
		DataLinkLayoutBuffer(dataLinkLayoutBuffer),
		DataLinkLayoutBufferLen(dataLinkLayoutBufferLen),
		ResponseRingBuffer(responseRingBuffer),
		ResponseRingBufferSize(responseRingBufferSize),
		DelegateRingBuffer(delegateRingBuffer),
		DelegateRingBufferSize(delegateRingBufferSize),
		ResponseDelegateMessageMaps(responseDelegateMessageMaps),
		ResponseDelegateMessageMapsCount(responseDelegateMessageMapsCount),
		RequestMessageMaps(requestMessageMaps),
		RequestMessageMapsCount(requestMessageMapsCount),
		RequestRingBuffer(requestRingBuffer),
		RequestRingBufferSize(requestRingBufferSize),
		porter(port),
		IsEnableMataDataEncode(isEnableMataDataEncode),
		UserData(ud),
		//last
		DeInitFlag(false),
		ObjectMutexHandle(nullptr),
		DelegateBlockBufferProvider(nullptr),
		ResponseBlockBufferProvider(nullptr),
		DelegateServiceThreadHandle(nullptr),
		ResponseServiceThreadHandle(nullptr),
		RequestBlockBufferProvider(nullptr),
		SuspendTimer(nullptr)
	{
		
	}
	//client节点构造函数
	EmbedXrpcObject(SendPack_t send,
		uint32_t timeOut,

		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,

		uint8_t* responseRingBuffer,
		int16_t responseRingBufferSize,

		uint8_t* delegateRingBuffer,
		int16_t delegateRingBufferSize,

		ResponseDelegateMessageMapCollection* mapCollection,
		uint32_t collectionCount,

		IEmbeXrpcPort* port,
		bool isEnableMataDataEncode,
		void* ud = nullptr):EmbedXrpcObject(send,
			timeOut,

			dataLinkLayoutBuffer,
			dataLinkLayoutBufferLen,

			responseRingBuffer,
			responseRingBufferSize,

			delegateRingBuffer,
			delegateRingBufferSize,

			mapCollection,
			collectionCount,

			nullptr,
			0,

			nullptr,
			0,

			port,
			isEnableMataDataEncode,
			ud
		)
	{
		
	}
	//server节点的构造函数
	EmbedXrpcObject(SendPack_t send,
		uint32_t timeOut,

		uint8_t* dataLinkLayoutBuffer,
		uint32_t dataLinkLayoutBufferLen,

		uint8_t* requestRingBuffer,
		int16_t requestRingBufferSize,

		RequestMessageMapCollection* mapCollection,
		uint32_t collectionCount,

		IEmbeXrpcPort* port,
		bool isEnableMataDataEncode,
		void* ud = nullptr):EmbedXrpcObject(send,
			timeOut,

			dataLinkLayoutBuffer,
			dataLinkLayoutBufferLen,

			nullptr,
			0,

			nullptr,
			0,

			nullptr,
			0,

			requestRingBuffer,
			requestRingBufferSize,

			mapCollection,
			collectionCount,

			port,
			isEnableMataDataEncode,
			ud
		)
	{
		
	}

	void Init()
	{
		DeInitFlag = false;

		ObjectMutexHandle = porter->CreateMutex("ObjectMutex");

		if (ResponseDelegateMessageMapsCount > 0)
		{
			DelegateServiceThreadHandle = porter->CreateThread("DelegateServiceThread", Client_ThreadPriority, DelegateServiceThread, this);
			DelegateBlockBufferProvider = new BlockRingBufferProvider(DelegateRingBuffer, DelegateRingBufferSize, porter);
			ResponseBlockBufferProvider = new BlockRingBufferProvider(ResponseRingBuffer, ResponseRingBufferSize, porter);
			porter->ThreadStart(DelegateServiceThreadHandle);
		}
		
		if (RequestMessageMapsCount > 0)
		{
			//server
			ResponseServiceThreadHandle = porter->CreateThread("ResponseServiceThread", Server_ThreadPriority, ResponseServiceThread, this);
			RequestBlockBufferProvider = new BlockRingBufferProvider(RequestRingBuffer, RequestRingBufferSize, porter);
			SuspendTimer = porter->CreateTimer("SuspendTimer", EmbedXrpc_WAIT_FOREVER, SuspendTimerCallBack, this);
			porter->ThreadStart(ResponseServiceThreadHandle);
		}
		

	}
	void DeInit()
	{
		DeInitFlag = true;
		porter->DeleteMutex(ObjectMutexHandle);
		if (ResponseDelegateMessageMapsCount > 0)
		{
			porter->DeleteThread(DelegateServiceThreadHandle);
			delete DelegateBlockBufferProvider;
			delete ResponseBlockBufferProvider;
		}
		
		if (RequestMessageMapsCount > 0)
		{
			//server
			porter->DeleteThread(ResponseServiceThreadHandle);
			porter->DeleteMutex(ObjectMutexHandle);
			delete RequestBlockBufferProvider;
		}
		

	}
	bool ReceivedMessage(uint32_t allDataLen, uint8_t* allData)
	{
		if (allDataLen < 4)
			return false;
		bool FindFlag = false;
		bool SendQueueResult = false;

		BlockBufferItemInfo raw;
		uint16_t serviceId = (uint16_t)(allData[0] | allData[1] << 8);
		uint16_t targettimeout = (uint16_t)(allData[2] | allData[3] << 8)&0x3FFF;
		uint32_t dataLen = allDataLen - 4;
		uint8_t* data = &allData[4];
		ReceiveType_t rt = (ReceiveType_t)(allData[3] >> 6);
		if (rt== ReceiveType_Response|| rt == ReceiveType_Delegate)
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Client ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			if (serviceId == EmbedXrpcSuspendSid)
			{
				raw.Sid = serviceId;
				raw.DataLen = dataLen;
				raw.TargetTimeout = targettimeout;
				return ResponseBlockBufferProvider->Send(&raw, nullptr, 0);
			}
			for (uint32_t collectionIndex = 0; collectionIndex < ResponseDelegateMessageMapsCount; collectionIndex++)
			{
				auto MessageMaps = ResponseDelegateMessageMaps[collectionIndex].Map;
				for (uint32_t i = 0; i < ResponseDelegateMessageMaps[collectionIndex].Count; i++)
				{

					auto iter = &MessageMaps[i];
					if (iter->Sid == serviceId)
					{
						FindFlag = true;
						raw.Sid = serviceId;
						raw.DataLen = dataLen;
						raw.TargetTimeout = targettimeout;
						if (iter->ReceiveType == ReceiveType_Response)
						{
							SendQueueResult = ResponseBlockBufferProvider->Send(&raw, data, dataLen);
						}
						else if (iter->ReceiveType == ReceiveType_Delegate)
						{
							SendQueueResult = DelegateBlockBufferProvider->Send(&raw, data, dataLen);
						}
						goto sqr;
					}
				}
			}
		}
		else if (rt == ReceiveType_Request)//server
		{
			//EmbedSerializationShowMessage("EmbedXrpcObject","Server ReceivedMessage  Malloc :0x%x,size:%d\n", (uint32_t)raw.Data, dataLen);
			raw.Sid = serviceId;
			raw.DataLen = dataLen;
			raw.TargetTimeout = targettimeout;
			SendQueueResult=RequestBlockBufferProvider->Send(&raw, data, dataLen);
		}
		sqr:
		return SendQueueResult;
	}
	static void SuspendTimerCallBack(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		//obj->porter->TakeMutex(obj->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);//不需要加锁 这里不使用obj->buffer全局buffer
		uint8_t sb[4];
		sb[0] = (uint8_t)(EmbedXrpcSuspendSid & 0xff);
		sb[1] = (uint8_t)(EmbedXrpcSuspendSid >> 8 & 0xff);
		sb[2] = (uint8_t)(obj->TimeOut & 0xff);
		sb[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff)&0x3FFF);
		sb[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
		obj->Send(obj, 4, sb);
		//obj->porter->ReleaseMutex(obj->ObjectMutexHandle);
	}
	static void DelegateServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		BlockBufferItemInfo recData;
		uint32_t i = 0;
		bool isContain = false;
		for (;;)
		{
			if (obj->DelegateBlockBufferProvider->Receive(&recData, 1)==true)
			{
				isContain = false;
				for (uint32_t collectionIndex = 0; collectionIndex < obj->ResponseDelegateMessageMapsCount; collectionIndex++)
				{
					auto MessageMaps = obj->ResponseDelegateMessageMaps[collectionIndex].Map;
					for (i = 0; i < obj->ResponseDelegateMessageMaps[collectionIndex].Count; i++)
					{
						if ((MessageMaps[i].ReceiveType == ReceiveType_Delegate) && (MessageMaps[i].Delegate->GetSid() == recData.Sid))
						{
							SerializationManager rsm;
							rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
							rsm.Reset();
							rsm.BufferLen = recData.DataLen;
							rsm.BlockBufferProvider=obj->DelegateBlockBufferProvider;
							rsm.BlockBufferProvider->SetCalculateSum(0);
							rsm.BlockBufferProvider->SetReferenceSum(recData.CheckSum);
							MessageMaps[i].Delegate->Invoke(rsm);
							isContain = true;
							goto _break;
						}
					}
				}
				_break:
				if (isContain == false)
				{
					obj->DelegateBlockBufferProvider->PopChars(nullptr, recData.DataLen);
				}
				//obj->porter->Free(recData.Data);
				//EmbedSerializationShowMessage("EmbedXrpcObject","Client ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
			}
			if (obj->DeInitFlag == true)
			{
				return;
			}
		}
	}
	static void ResponseServiceThread(void* arg)
	{
		EmbedXrpcObject* obj = (EmbedXrpcObject*)arg;
		BlockBufferItemInfo recData;
		uint32_t i = 0;
		bool isContain = false;
		for (;;)
		{
			if (obj->RequestBlockBufferProvider->Receive(&recData, EmbedXrpc_WAIT_FOREVER) != true)
			{
				continue;
			}
			isContain = false;
			for (uint32_t collectionIndex = 0; collectionIndex < obj->RequestMessageMapsCount; collectionIndex++)
			{
				auto MessageMaps = obj->RequestMessageMaps[collectionIndex].Map;
				for (i = 0; i < obj->RequestMessageMaps[collectionIndex].Count; i++)
				{
					if (MessageMaps[i].Service->GetSid() == recData.Sid)
					{

						SerializationManager rsm;
						SerializationManager sendsm;
						rsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
						rsm.Reset();
						rsm.BufferLen = recData.DataLen;
						rsm.BlockBufferProvider = obj->RequestBlockBufferProvider;
						rsm.BlockBufferProvider->SetCalculateSum(0);
						rsm.BlockBufferProvider->SetReferenceSum(recData.CheckSum);
						obj->porter->TakeMutex(obj->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);//由于使用obj->Buffer这个全局变量，所以添加锁
						sendsm.IsEnableMataDataEncode = obj->IsEnableMataDataEncode;
						sendsm.Reset();
						sendsm.Buf = &obj->DataLinkLayoutBuffer[4];
						sendsm.BufferLen = obj->DataLinkLayoutBufferLen - 4;

						obj->porter->TimerReset(obj->SuspendTimer);
						obj->porter->TimerStart(obj->SuspendTimer, recData.TargetTimeout / 2);
						MessageMaps[i].Service->Invoke(rsm, sendsm);
						obj->porter->TimerStop(obj->SuspendTimer);

						if (sendsm.Index > 0)//
						{
							obj->DataLinkLayoutBuffer[0] = (uint8_t)(recData.Sid >> 0 & 0xff);
							obj->DataLinkLayoutBuffer[1] = (uint8_t)(recData.Sid >> 8 & 0xff);
							obj->DataLinkLayoutBuffer[2] = (uint8_t)(obj->TimeOut >> 0 & 0xff);
							obj->DataLinkLayoutBuffer[3] = (uint8_t)((obj->TimeOut >> 8 & 0xff) & 0x3FFF);
							obj->DataLinkLayoutBuffer[3] |= (uint8_t)(((uint8_t)(ReceiveType_Response)) << 6);
							obj->Send(obj, sendsm.Index + 4, obj->DataLinkLayoutBuffer);
						}
						obj->porter->ReleaseMutex(obj->ObjectMutexHandle);

						isContain = true;
						goto _break;
					}
				}
			}
		_break:
			if (isContain == false)
			{
				obj->RequestBlockBufferProvider->PopChars(nullptr, recData.DataLen);
			}
			//obj->porter->Free(recData.Data);
			//EmbedSerializationShowMessage("EmbedXrpcObject","Server ServiceThread Free 0x%x\n", (uint32_t)recData.Data);
		}
	}
	RequestResponseState Wait(uint32_t sid, const ObjectType *type,void * response)
	{
		RequestResponseState ret= ResponseState_Ok;
		BlockBufferItemInfo recData;
		while (true)
		{	
			if (ResponseBlockBufferProvider->Receive(&recData, TimeOut) == false)
			{
				return ResponseState_Timeout;
			}
			if (recData.Sid == EmbedXrpcSuspendSid)
			{
				EmbedSerializationShowMessage("EmbedXrpcObject", "Client:recData.Sid == EmbedXrpcSuspendSid\n");
				continue;
			}
			if (sid != recData.Sid)
			{
				ret = ResponseState_SidError;
			}
			else
			{
				EmbedSerializationShowMessage("EmbedXrpcObject", "sid == recData.Sid\n");
				ret = ResponseState_Ok;
			}
			break;
		}
		if (ret == ResponseState_Ok)
		{
			SerializationManager rsm;
			rsm.IsEnableMataDataEncode = IsEnableMataDataEncode;
			rsm.Reset();
			rsm.BufferLen = recData.DataLen;
			rsm.BlockBufferProvider = ResponseBlockBufferProvider;
			rsm.BlockBufferProvider->SetCalculateSum(0);
			rsm.BlockBufferProvider->SetReferenceSum(recData.CheckSum);
			rsm.Deserialize(type, response, 0);
			EmbedSerializationAssert(rsm.BlockBufferProvider->GetReferenceSum() == rsm.BlockBufferProvider->GetCalculateSum());
		}
		return ret;
	}
	
};

#endif
