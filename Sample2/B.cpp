#include <thread>

#include "Sample2.Server.h"
#include "Sample2.Client.h"

extern EmbedXrpcObject A_RpcObject;
extern EmbedXrpcObject C_RpcObject;
extern Inter_Requester B_Requester;
#define AllTypeBufferLen	4096

static bool Send(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去，如果你的协议没有client的request请求，这个可以为null
{
	if (userDataOfTransportLayer->SourceAddress=='B'&& userDataOfTransportLayer->DestAddress == 'A')
	{
		A_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer);
	}
	else if (userDataOfTransportLayer->SourceAddress == 'B' && userDataOfTransportLayer->DestAddress == 'C')
	{
		C_RpcObject.ReceivedMessage(dataLen, data, *userDataOfTransportLayer);
	}
	else
	{
		assert(0);
	}
	return true;
}

class BServer_GetSumProvider :public Inter_GetSum_Service
{
public:
	void GetSum(ServiceInvokeParameter* serviceInvokeParameter, Int32 a, Int32 b)
	{
		
		printf("B:Starting suppendTimer and Calling C's service...\n");

		serviceInvokeParameter->RpcObject->UserDataOfTransportLayerOfSuspendTimerUsed.SourceAddress = 'B';
		serviceInvokeParameter->RpcObject->UserDataOfTransportLayerOfSuspendTimerUsed.DestAddress = 'A';
		El_TimerStart(serviceInvokeParameter->RpcObject->SuspendTimer, serviceInvokeParameter->TargetTimeOut / 2);

		DTL bcdtl;
		bcdtl.SourceAddress = 'B';
		bcdtl.DestAddress = 'C';
		auto ret = B_Requester.GetSum(&bcdtl, a, b);
		if (ret.State == ResponseState_Ok && ret.ReturnValue.IsSuccess==1)
		{
			this->Response.ReturnValue = ret.ReturnValue;
		}
		else
		{
			this->Response.ReturnValue.IsSuccess = false;
			this->Response.ReturnValue.Value = 0;
		}
		printf("B:Call Complete!\n");
		serviceInvokeParameter->Response_UserDataOfTransportLayer.SourceAddress = 'B';//B给A发
		serviceInvokeParameter->Response_UserDataOfTransportLayer.DestAddress = 'A';
	}
};
BServer_GetSumProvider BServerGetSumProvider;
static RequestDescribe Requests[] = //定义请求集合
{
	{"Inter_Add",					&BServerGetSumProvider},
};

static ResponseDescribe Responses[1] =//定义回复 ID 集合
{
	{"Inter_Add"   ,     Inter_GetSum_ServiceId},
};
static InitConfig InitCfg =
{
	"B",
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},// buffer for request
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},//DataLinkBufferForResponse
	{nullptr,0,nullptr},//DataLinkBufferForDelegate
	Send,
	500,
	Responses,//Responses
	1,
	nullptr,//Delegate
	0,
	Requests,//Request
	1,
	{
		true,//CheckSumValid
		6,//ServerThreadPriority
		6,//ClientThreadPriority
		true,//UseRingBufferWhenReceiving
		{
			false,//IsSendToQueue
			10,//DelegateMessageQueue_MaxItemNumber
			10,//ResponseMessageQueue_MaxItemNumber
			10,//RequestMessageQueue_MaxItemNumber
		},
		{
			nullptr,//DelegateMessageBlockBufferProvider
			new BlockRingBufferProvider(new UInt8[AllTypeBufferLen],AllTypeBufferLen,10),//ResponseMessageBlockBufferProvider
			new BlockRingBufferProvider(new UInt8[AllTypeBufferLen],AllTypeBufferLen,10),//RequestMessageBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject B_RpcObject(&InitCfg);//server rpc 对象
Inter_Requester B_Requester(&B_RpcObject);
