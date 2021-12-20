#include <thread>
#include "Sample2.Client.h"
extern EmbedXrpcObject B_RpcObject;
#define AllTypeBufferLen	4096
static bool Send(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	assert(B_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer) == true);
	return true;
}


static ResponseDescribe Responses[1] =//����ظ� ID ����
{
	{"Inter_Add"   ,     Inter_GetSum_ServiceId},
};
static ClientNodeQuicklyInitConfig InitConfig =
{
	"A",
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},
	{ Send },
	1000,
	Responses,
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
EmbedXrpcObject A_RpcObject(&InitConfig);//client rpc ����
Inter_Requester A_Requester(&A_RpcObject);//����request����
void ClientThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	int a = 1000, b = 5000;
	uint8_t Bytes[7] = "123456";
	int testcount = 10;
	DTL abdtl;
	abdtl.SourceAddress = 'A';
	abdtl.DestAddress = 'B';
	while (testcount-- > 0)
	{
		a++;
		b++;
		auto sum= A_Requester.GetSum(&abdtl,a, b);
		El_Assert(sum.State == ResponseState_Ok);
		printf("sum:%d\n\r\n", sum.ReturnValue.Value);
		A_Requester.Free_GetSum(&sum);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));//�ȴ�RPC����ȫ�����
	A_RpcObject.DeInit();
}