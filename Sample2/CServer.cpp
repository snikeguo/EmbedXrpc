#include <thread>

#include "Sample2.Server.h"
#include "DTL.h"

#define DataLinkBufferLen	4096
extern EmbedXrpcObject<BC_DTL> BClient_RpcObject;

#define AllTypeBufferLen	4096

uint8_t CServer_DelBuffer[AllTypeBufferLen];
uint8_t CServer_ResBuffer[AllTypeBufferLen];
uint8_t CServer_ReqBuffer[AllTypeBufferLen];
uint8_t CServer_DataLinkBuffer[AllTypeBufferLen];
bool CServer_Send(BC_DTL* userDataOfTransportLayer,
	EmbedXrpcObject<BC_DTL>* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去，如果你的协议没有client的request请求，这个可以为null
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
	BClient_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer);
	return true;
}
class CServer_GetSumProvider :public Inter_GetSumService<BC_DTL>
{
public:
	void GetSum(ServiceInvokeParameter<BC_DTL>* serviceInvokeParameter, Int32 a, Int32 b)
	{
		printf("CServer's GetSum Service is Executing:%s\n",serviceInvokeParameter->request_UserDataOfTransportLayer->Name);
		El_TimerStart(serviceInvokeParameter->rpcObject->SuspendTimer, serviceInvokeParameter->targetTimeOut / 2);
		this->Response.ReturnValue.IsSuccess = 1;
		this->Response.ReturnValue.Value = a + b;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
};
CServer_GetSumProvider CServerGetSumProvider;
RequestDescribe<BC_DTL> Requests[] = //定义请求集合
{
	{"Inter_Add",					&CServerGetSumProvider},
};


ServerNodeQuicklyInitConfig<BC_DTL> CServer_InitConfig =
{
	CServer_DataLinkBuffer,
	AllTypeBufferLen,
	 { CServer_Send },
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
			new BlockRingBufferProvider<BC_DTL>(CServer_DelBuffer,AllTypeBufferLen,10),//DelegateBlockBufferProvider
			new BlockRingBufferProvider<BC_DTL>(CServer_ResBuffer,AllTypeBufferLen,10),//ResponseBlockBufferProvider
			new BlockRingBufferProvider<BC_DTL>(CServer_ReqBuffer,AllTypeBufferLen,10),//RequestBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject<BC_DTL> CServer_RpcObject(&CServer_InitConfig);//server rpc 对象
