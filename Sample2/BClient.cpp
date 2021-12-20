#include <thread>
#include "Sample2.Client.h"
extern EmbedXrpcObject CServer_RpcObject;


#define AllTypeBufferLen	4096

bool BClient_Send(BC_DTL* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	assert(CServer_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer) == true);
	return true;
}


ResponseDescribe Responses[1] =//����ظ� ID ����
{
	{"Inter_Add"   ,     Inter_GetSum_ServiceId},
};
ClientNodeQuicklyInitConfig BClient_InitConfig =
{
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},
	{ BClient_Send },
	1000,
	BC_Responses,
	1,
	nullptr,
	0,
	{
		true,//CheckSumValid
		6,//ServerThreadPriority
		6,//ClientThreadPriority
		true,//UseRingBufferWhenReceiving
		{
			false,//IsSendToQueue
			10,//DelegateBlockQueue_MaxItemNumber
			10,//ResponseBlockQueue_MaxItemNumber
			10,//RequestBlockQueue_MaxItemNumber
		},
		{
			nullptr,//DelegateBlockBufferProvider
			new BlockRingBufferProvider(new UInt8[AllTypeBufferLen],AllTypeBufferLen,10),//ResponseBlockBufferProvider
			nullptr,//RequestBlockBufferProvider
		},

	},
	nullptr,
};
EmbedXrpcObject BClient_RpcObject(&BClient_InitConfig);//client rpc ����
InterClientImpl BClient(&BClient_RpcObject);//����request����
