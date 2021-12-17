#include <thread>

#include "Sample2.Server.h"
#include "Sample2.Client.h"
#include "DTL.h"

#define DataLinkBufferLen	4096
extern EmbedXrpcObject<AB_DTL> AClient_RpcObject;

#define AllTypeBufferLen	4096

uint8_t BServer_DelBuffer[AllTypeBufferLen];
uint8_t BServer_ResBuffer[AllTypeBufferLen];
uint8_t BServer_ReqBuffer[AllTypeBufferLen];
uint8_t BServer_DataLinkBuffer[AllTypeBufferLen];
bool BServer_Send(AB_DTL* userDataOfTransportLayer,
	EmbedXrpcObject<AB_DTL>* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去，如果你的协议没有client的request请求，这个可以为null
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
	AClient_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer);
	return true;
}

extern InterClientImpl<BC_DTL> BClient;
class BServer_GetSumProvider :public Inter_GetSumService<AB_DTL>
{
public:
	void GetSum(ServiceInvokeParameter<AB_DTL>* serviceInvokeParameter, Int32 a, Int32 b)
	{
		BC_DTL btl = { "BMS!" };
		printf("AClient call BServer's GetSum Service is Executing.BClient call CServer' Get Sum Service now:%s\n",
			serviceInvokeParameter->request_UserDataOfTransportLayer->IpAddress);
		El_TimerStart(serviceInvokeParameter->rpcObject->SuspendTimer, serviceInvokeParameter->targetTimeOut / 2);
		auto ret = BClient.GetSum(&btl, a, b);
		if (ret.State == ResponseState_Ok && ret.ReturnValue.IsSuccess==1)
		{
			this->Response.ReturnValue = ret.ReturnValue;
		}
		else
		{
			this->Response.ReturnValue.IsSuccess = false;
			this->Response.ReturnValue.Value = 0;
		}
	}
};
BServer_GetSumProvider BServerGetSumProvider;
RequestDescribe<AB_DTL> Requests[] = //定义请求集合
{
	{"Inter_Add",					&BServerGetSumProvider},
};


ServerNodeQuicklyInitConfig<AB_DTL> BServer_InitConfig =
{
	BServer_DataLinkBuffer,
	AllTypeBufferLen,
	 { BServer_Send },
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
			new BlockRingBufferProvider<AB_DTL>(BServer_DelBuffer,AllTypeBufferLen,10),//DelegateBlockBufferProvider
			new BlockRingBufferProvider<AB_DTL>(BServer_ResBuffer,AllTypeBufferLen,10),//ResponseBlockBufferProvider
			new BlockRingBufferProvider<AB_DTL>(BServer_ReqBuffer,AllTypeBufferLen,10),//RequestBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject<AB_DTL> BServer_RpcObject(&BServer_InitConfig);//server rpc 对象
