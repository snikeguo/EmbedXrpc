#include <thread>
#include "EmbedLibrary.h"
#include "EmbedXrpc.Port.h"
#include <EmbedXrpcObject.h>
#include "Sample2.Server.h"

extern EmbedXrpcObject B_RpcObject;

#define AllTypeBufferLen	4096
static bool Send(RequestParameter* rp,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去，如果你的协议没有client的request请求，这个可以为null
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
	B_RpcObject.ReceivedMessage(dataLen, data, *rp->Udtl,0);
	return true;
}
class CServer_GetSumProvider :public GetSum_Service
{
public:
	void GetSum(ServiceInvokeParameter* serviceInvokeParameter, int32_t a, int32_t b)
	{
		printf("C:start suppendtimer and sleep 2s.\n");
		El_TimerStart(serviceInvokeParameter->RpcObject->SuspendTimer, serviceInvokeParameter->TargetTimeOut / 2,0);
		this->Response.ReturnValue.IsSuccess = 1;
		this->Response.ReturnValue.Value = a + b;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
};
CServer_GetSumProvider CServerGetSumProvider;
static ServiceDescribe AllServices[] = //定义请求集合
{
	{"Inter_Add",					&CServerGetSumProvider},
};


static ServerNodeQuicklyInitConfig InitCfg =
{
	"C",
	{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,false},//DataLinkBufferForResponse
	 { Send },
	500,
	AllServices,
	1,
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
			{nullptr,0,0},//BlockBufferProviderOfRequestService
			{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,10},//ServiceBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject C_RpcObject;//server rpc 对象
void C_Init()
{
	C_RpcObject.Init(&InitCfg);
}