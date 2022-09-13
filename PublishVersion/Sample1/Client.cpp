#include <thread>
#include "Sample1.Client.h"
extern EmbedXrpcObject ServerRpc;


#define AllTypeBufferLen	4096



bool ClientSend(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	assert(ServerRpc.ReceivedMessage(dataLen, data, *userDataOfTransportLayer) == ReceivedMessageStatus::Ok);
	return true;
}
//�ػ�����̳�
class DateTimeChange_ServiceReceiver :public DateTimeChange_Service
{
public:
	void DateTimeChange(ServiceInvokeParameter* serviceInvokeParameter, DateTime_t now[1])//server�㲥��client���ܵ���
	{
		printf("client:%u-%u-%u %u:%u:%u!client\r\n\0", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
		//printf("%s", now[0].DateString);
	}
};
//�ػ�����̳�
class Test2_ServiceReceiver :public Test2_Service
{
public:
	void Test2(ServiceInvokeParameter* serviceInvokeParameter, DateTime_t now[1])//server�㲥��client���ܵ���
	{
		printf("%u-%u-%u %u:%u:%u!client\r\n\0", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
		//printf("%s", now[0].DateString);
	}
};

DateTimeChange_ServiceReceiver DateTimeChange;
Test2_ServiceReceiver Test2;

static ServiceDescribe AllServices[2] =//����ί�ж��󼯺�
{
	{"DateTimeChange"  ,&DateTimeChange},
	{"Test2"  ,&Test2},
};//client���Դ����Request����

static RequestServiceDescribe AllRequests[4] =//����ظ� ID ����
{
	{"Inter_Add"   ,     Add_ServiceId},
	{"Inter_NoArg"     ,   NoArg_ServiceId},
	{"Inter_NoReturn"   ,     NoReturn_ServiceId},
	{"Inter_NoArgAndReturn"    ,    NoArgAndReturn_ServiceId},
};


static InitConfig InitCfg =
{
	"Client",
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,false},//Buffer for Request
	{nullptr,0,false},
	ClientSend,
	1000,
	AllRequests,
	4,
	AllServices,
	2,
	{
		true,//CheckSumValid
		osPriorityAboveNormal,//ServiceThreadPriority
		2048,
		false,//UseRingBufferWhenReceiving
		{
			true,//IsSendToQueue
			10,//MessageQueueOfRequestService_MaxItemNumber
			10,//ServiceMessageQueue_MaxItemNumber
		},
		{
			{new UInt8[AllTypeBufferLen],AllTypeBufferLen,10},//BlockBufferOfRequestService_Config
			{nullptr,0,0},//ServiceBlockBufferConfig
		},

	},
	nullptr,
};
EmbedXrpcObject ClientRpc;//client rpc ����

void Client_Init()
{
	ClientRpc.Init(&InitCfg);
}

Add_Requester Client(&ClientRpc);//����request����
void ClientThread()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
	int a = 1000, b = 5000;
	uint8_t Bytes[7] = "123456";
	UInt32 testcount = 0xffffffff;
	Win32UserDataOfTransportLayerTest win32UserDataOfTransportLayerTest;
	strcpy(win32UserDataOfTransportLayerTest.IpAddress, "127.0.0.1");
	win32UserDataOfTransportLayerTest.Port = 6666;
	while (testcount-- > 0)
	{
		a++;
		b++;
		Client.Add_SendData.a = a;
		Client.Add_SendData.b = b;
		Client.Add_SendData.dataLen = 4;
		Client.Add_SendData.data = (UInt8*)"123";
		auto sum = Client.Add(&win32UserDataOfTransportLayerTest);//request��������service
		if (sum.State != ResponseState_Ok)
		{
			printf("Client was requested the \"Add\" service Faild! error code:%d\n", sum.State);
		}
		else
		{
			printf("client:sum7 is:%d\n", sum.ReturnValue.Sum7);
		}
		Client.Free_Add(&sum);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));//�ȴ�RPC����ȫ�����
	ClientRpc.DeInit();
}