#include <thread>
#include "Sample2.Client.h"
extern EmbedXrpcObject B_RpcObject;
#define AllTypeBufferLen	4096
static bool Send(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去
{
	assert(B_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer) == true);
	return true;
}


static ResponseDescribe Responses[1] =//定义回复 ID 集合
{
	{"Inter_Add"   ,     GetSum_ServiceId},
};
static ClientNodeQuicklyInitConfig InitCfg =
{
	"A",
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,false},//DataLinkBufferForRequest
	{ Send },
	1000,
	Responses,
	1,
	{
		true,//CheckSumValid
		6,//ServerThreadPriority
		6,//ClientThreadPriority
		false,//UseRingBufferWhenReceiving
		{
			false,//IsSendToQueue
			10,//ResponseMessageQueue_MaxItemNumber
			10,//RequestMessageQueue_MaxItemNumber
		},
		{
			{new UInt8[AllTypeBufferLen],AllTypeBufferLen,10},//ResponseMessageBlockBufferProvider
			{nullptr,0,0},//RequestMessageBlockBufferProvider
		},
		
	},
	nullptr,
};
EmbedXrpcObject A_RpcObject;//client rpc 对象
void A_Init()
{
	A_RpcObject.Init(&InitCfg);
}
GetSum_Requester A_Requester(&A_RpcObject);//定义request对象
void ClientThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	int a = 1000, b = 5000;
	uint8_t Bytes[7] = "123456";
	int testcount = 5;
	DTL abdtl;
	abdtl.SourceAddress = 'A';
	abdtl.DestAddress = 'B';
	while (testcount-- > 0)
	{
		a++;
		b++;
		printf("A:Calling B's service....\n");
		auto sum= A_Requester.GetSum(&abdtl,a, b);
		El_Assert(sum.State == ResponseState_Ok);
		printf("A:sum:%d\n\r\n", sum.ReturnValue.Value);
		A_Requester.Free_GetSum(&sum);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));//等待RPC调用全部完毕
	A_RpcObject.DeInit();
	
}