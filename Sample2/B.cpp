#include <thread>
#include "EmbedXrpc.Port.h"
#include <EmbedXrpcObject.h>
#include "EmbedLibrary.h"
#include "Sample2.Server.h"
#include "Sample2.Client.h"

extern EmbedXrpcObject A_RpcObject;
extern EmbedXrpcObject C_RpcObject;
extern GetSum_Requester B_Requester;
#define AllTypeBufferLen	4096

static bool Send(RequestParameter* rp,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ��������Э��û��client��request�����������Ϊnull
{
	if (rp->Udtl->SourceAddress=='B'&& rp->Udtl->DestAddress == 'A')
	{
		A_RpcObject.ReceivedMessage(dataLen, data, *rp->Udtl,0);
	}
	else if (rp->Udtl->SourceAddress == 'B' && rp->Udtl->DestAddress == 'C')
	{
		C_RpcObject.ReceivedMessage(dataLen, data, *rp->Udtl, 0);
	}
	else
	{
		assert(0);
	}
	return true;
}

class BServer_GetSumProvider :public GetSum_Service
{
public:
	void GetSum(ServiceInvokeParameter* serviceInvokeParameter, int32_t a, int32_t b)
	{
		
		printf("B:Starting suppendTimer and Calling C's service...\n");

		serviceInvokeParameter->RpcObject->UserDataOfTransportLayerOfSuspendTimerUsed.SourceAddress = 'B';
		serviceInvokeParameter->RpcObject->UserDataOfTransportLayerOfSuspendTimerUsed.DestAddress = 'A';
		El_TimerStart(serviceInvokeParameter->RpcObject->SuspendTimer, serviceInvokeParameter->TargetTimeOut / 2,0);

		DTL bcdtl;
		bcdtl.SourceAddress = 'B';
		bcdtl.DestAddress = 'C';
		RequestParameter rp;
		rp.IsIsr = 0;
		rp.Udtl = &bcdtl;
		auto ret = B_Requester.GetSum(&rp, a, b);
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
		serviceInvokeParameter->Response_UserDataOfTransportLayer.SourceAddress = 'B';//B��A��
		serviceInvokeParameter->Response_UserDataOfTransportLayer.DestAddress = 'A';
	}
};
BServer_GetSumProvider BServerGetSumProvider;
static ServiceDescribe AllServices[] = //�������󼯺�
{
	{"Inter_Add",					&BServerGetSumProvider},
};

static InitConfig InitCfg =
{
	"B",
	{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,false},// buffer for request
	{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,false},//DataLinkBufferForResponse
	Send,
	500,
	AllServices,//Request
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
			{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,10},//BlockBufferProviderOfRequestService
			{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,10},//ServiceBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject B_RpcObject;//server rpc ����
void B_Init()
{
	B_RpcObject.Init(&InitCfg);
}
GetSum_Requester B_Requester(&B_RpcObject);
