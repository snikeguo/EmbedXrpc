#include <thread>
#include "EmbedXrpc.Port.h"
#include <EmbedXrpcObject.h>
#include "EmbedLibrary.h"
#include "Sample2.Client.h"
extern EmbedXrpcObject B_RpcObject;
#define AllTypeBufferLen	4096
static bool Send(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去
{
	assert(B_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer,0) == ReceivedMessageStatus::Ok);
	return true;
}



static ClientNodeQuicklyInitConfig InitCfg =
{
	"A",
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,false},//DataLinkBufferForRequest
	{ Send },
	1000,
	{
		true,//CheckSumValid
		1,//ServiceThreadPriority
		2048,
		false,//UseRingBufferWhenReceiving
		{
			false,//IsSendToQueue
			10,//MessageQueueOfRequestService_MaxItemNumber
			10,//ServiceMessageQueue_MaxItemNumber
		},
		{
			{new UInt8[AllTypeBufferLen],AllTypeBufferLen,10},//BlockBufferProviderOfRequestService
			{nullptr,0,0},//ServiceBlockBufferProvider
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