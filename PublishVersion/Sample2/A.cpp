#include <thread>

#include "EmbedXrpcObject.h"
#include "EmbedXrpc.Port.h"
#include "EmbedLibrary.h"
#include "Sample2.Client.h"
extern EmbedXrpcObject B_RpcObject;
#define AllTypeBufferLen	4096
static bool Send(RequestParameter* rp,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	assert(B_RpcObject.ReceivedMessage(dataLen, data, *(rp->Udtl),0) == ReceivedMessageStatus::Ok);
	return true;
}



static ClientNodeQuicklyInitConfig InitCfg =
{
	"A",
	{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,false},//DataLinkBufferForRequest
	{ Send },
	1000,
	{
		1,//ServiceThreadPriority
		2048,
		false,//UseRingBufferWhenReceiving
		{
			false,//IsSendToQueue
			10,//MessageQueueOfRequestService_MaxItemNumber
			10,//ServiceMessageQueue_MaxItemNumber
		},
		{
			{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,10},//BlockBufferProviderOfRequestService
			{nullptr,0,0},//ServiceBlockBufferProvider
		},
		
	},
	nullptr,
};
EmbedXrpcObject A_RpcObject;//client rpc ����
void A_Init()
{
	A_RpcObject.Init(&InitCfg);
}
GetSum_Requester A_Requester(&A_RpcObject);//����request����
void ClientThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	int a = 1000, b = 5000;
	uint8_t Bytes[7] = "123456";
	int testcount = 5;
	DTL abdtl;
	abdtl.SourceAddress = 'A';
	abdtl.DestAddress = 'B';
	RequestParameter rp;
	rp.Udtl = &abdtl;
	rp.IsIsr = 0;
	while (testcount-- > 0)
	{
		a++;
		b++;
		printf("A:Calling B's service....\n");
		auto sum= A_Requester.GetSum(&rp,a, b);
		El_Assert(sum.State == ResponseState_Ok);
		printf("A:sum:%d\n\r\n", sum.ReturnValue.Value);
		A_Requester.Free_GetSum(&sum);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));//�ȴ�RPC����ȫ�����
	A_RpcObject.DeInit();
	
}