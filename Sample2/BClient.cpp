#include <thread>
#include "Sample2.Client.h"
#include "DTL.h"
extern EmbedXrpcObject<BC_DTL> CServer_RpcObject;


#define AllTypeBufferLen	4096

uint8_t BClient_DelBuffer[AllTypeBufferLen];
uint8_t BClient_ResBuffer[AllTypeBufferLen];
uint8_t BClient_ReqBuffer[AllTypeBufferLen];
uint8_t BClient_DataLinkBuffer[AllTypeBufferLen];

bool BClient_Send(BC_DTL* userDataOfTransportLayer,
	EmbedXrpcObject<BC_DTL>* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	assert(CServer_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer) == true);
	return true;
}


ResponseDescribe BC_Responses[1] =//����ظ� ID ����
{
	{"Inter_Add"   ,     Inter_GetSum_ServiceId},
};
ClientNodeQuicklyInitConfig<BC_DTL> BClient_InitConfig =
{
	BClient_DataLinkBuffer,
	AllTypeBufferLen,
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
			new BlockRingBufferProvider<BC_DTL>(BClient_DelBuffer,AllTypeBufferLen,10),//DelegateBlockBufferProvider
			new BlockRingBufferProvider<BC_DTL>(BClient_ResBuffer,AllTypeBufferLen,10),//ResponseBlockBufferProvider
			new BlockRingBufferProvider<BC_DTL>(BClient_ReqBuffer,AllTypeBufferLen,10),//RequestBlockBufferProvider
		},

	},
	nullptr,
};
EmbedXrpcObject<BC_DTL> BClient_RpcObject(&BClient_InitConfig);//client rpc ����
InterClientImpl<BC_DTL> BClient(&BClient_RpcObject);//����request����
