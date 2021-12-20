#include <thread>

#include "Sample2.Server.h"


extern EmbedXrpcObject B_RpcObject;

#define AllTypeBufferLen	4096
static bool Send(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去，如果你的协议没有client的request请求，这个可以为null
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
	B_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer);
	return true;
}
class CServer_GetSumProvider :public Inter_GetSum_Service
{
public:
	void GetSum(ServiceInvokeParameter* serviceInvokeParameter, Int32 a, Int32 b)
	{
		
		El_TimerStart(serviceInvokeParameter->RpcObject->SuspendTimer, serviceInvokeParameter->TargetTimeOut / 2);
		this->Response.ReturnValue.IsSuccess = 1;
		this->Response.ReturnValue.Value = a + b;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
};
CServer_GetSumProvider CServerGetSumProvider;
static RequestDescribe Requests[] = //定义请求集合
{
	{"Inter_Add",					&CServerGetSumProvider},
};


static ServerNodeQuicklyInitConfig InitConfig =
{
	"C",
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},//ResponseBuffer
	{nullptr,0,nullptr},//DelegateBuffer
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},//SuspendTimerBuffer
	 { Send },
	500,
	Requests,
	4,
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
			nullptr,//ResponseBlockBufferProvider
			new BlockRingBufferProvider(new UInt8[AllTypeBufferLen],AllTypeBufferLen,10),//RequestBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject C_RpcObject(&InitConfig);//server rpc 对象
