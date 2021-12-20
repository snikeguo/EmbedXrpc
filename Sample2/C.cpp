#include <thread>

#include "Sample2.Server.h"


extern EmbedXrpcObject B_RpcObject;

#define AllTypeBufferLen	4096
static bool Send(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ��������Э��û��client��request�����������Ϊnull
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
		printf("C:start suppendtimer and sleep 2s.\n");
		El_TimerStart(serviceInvokeParameter->RpcObject->SuspendTimer, serviceInvokeParameter->TargetTimeOut / 2);
		this->Response.ReturnValue.IsSuccess = 1;
		this->Response.ReturnValue.Value = a + b;
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	}
};
CServer_GetSumProvider CServerGetSumProvider;
static RequestDescribe Requests[] = //�������󼯺�
{
	{"Inter_Add",					&CServerGetSumProvider},
};


static ServerNodeQuicklyInitConfig InitConfig =
{
	"C",
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},//DataLinkBufferForResponse
	{nullptr,0,nullptr},//DataLinkBufferForDelegate
	 { Send },
	500,
	Requests,
	4,
	{
		true,//CheckSumValid
		6,//ServerThreadPriority
		6,//ClientThreadPriority
		false,//UseRingBufferWhenReceiving
		{
			false,//IsSendToQueue
			10,//DelegateMessageQueue_MaxItemNumber
			10,//ResponseMessageQueue_MaxItemNumber
			10,//RequestMessageQueue_MaxItemNumber
		},
		{
			nullptr,//DelegateMessageBlockBufferProvider
			nullptr,//ResponseMessageBlockBufferProvider
			new BlockRingBufferProvider(new UInt8[AllTypeBufferLen],AllTypeBufferLen,10),//RequestMessageBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject C_RpcObject(&InitConfig);//server rpc ����
