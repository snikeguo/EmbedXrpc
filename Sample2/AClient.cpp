#include <thread>
#include "Sample2.Client.h"
#include "DTL.h"
extern EmbedXrpcObject<AB_DTL> BServer_RpcObject;


#define AllTypeBufferLen	4096

uint8_t AClient_DelBuffer[AllTypeBufferLen];
uint8_t AClient_ResBuffer[AllTypeBufferLen];
uint8_t AClient_ReqBuffer[AllTypeBufferLen];
uint8_t AClient_DataLinkBuffer[AllTypeBufferLen];

bool AClient_Send(AB_DTL* userDataOfTransportLayer,
	EmbedXrpcObject<AB_DTL>* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去
{
	assert(BServer_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer) == true);
	return true;
}


ResponseDescribe AB_Responses[1] =//定义回复 ID 集合
{
	{"Inter_Add"   ,     Inter_GetSum_ServiceId},
};
ClientNodeQuicklyInitConfig<AB_DTL> AClient_InitConfig =
{
	AClient_DataLinkBuffer,
	AllTypeBufferLen,
	{ AClient_Send },
	1000,
	AB_Responses,
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
			new BlockRingBufferProvider<AB_DTL>(AClient_DelBuffer,AllTypeBufferLen,10),//DelegateBlockBufferProvider
			new BlockRingBufferProvider<AB_DTL>(AClient_ResBuffer,AllTypeBufferLen,10),//ResponseBlockBufferProvider
			new BlockRingBufferProvider<AB_DTL>(AClient_ReqBuffer,AllTypeBufferLen,10),//RequestBlockBufferProvider
		},
		
	},
	nullptr,
};
EmbedXrpcObject<AB_DTL> AClient_RpcObject(&AClient_InitConfig);//client rpc 对象
InterClientImpl<AB_DTL> AClient(&AClient_RpcObject);//定义request对象
void ClientThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	int a = 1000, b = 5000;
	uint8_t Bytes[7] = "123456";
	int testcount = 10;
	AB_DTL abdtl;
	strcpy(abdtl.IpAddress, "127.0.0.1");
	abdtl.Port = 6666;
	while (testcount-- > 0)
	{
		a++;
		b++;
		auto sum= AClient.GetSum(&abdtl,a, b);
		El_Assert(sum.State == ResponseState_Ok);
		printf("sum:%d\n\r\n", sum.ReturnValue.Value);
		AClient.Free_GetSum(&sum);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));//等待RPC调用全部完毕
	AClient_RpcObject.DeInit();
}