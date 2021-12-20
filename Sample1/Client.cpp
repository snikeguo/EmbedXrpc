#include <thread>
#include "Sample1.Client.h"
extern EmbedXrpcObject ServerRpc;


#define AllTypeBufferLen	4096



bool ClientSend(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	assert(ServerRpc.ReceivedMessage(dataLen, data, *userDataOfTransportLayer) == true);
	return true;
}
//�ػ�����̳�
class DateTimeChangeDelegateReceiver :public DateTimeChange_DelegateReceiver
{
public:
	void DateTimeChange(UserDataOfTransportLayer_t* userDataOfTransportLayer, DateTime_t now[1])//server�㲥��client���ܵ���
	{
		printf("client:%u-%u-%u %u:%u:%u!client\r\n\0", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
		//printf("%s", now[0].DateString);
	}
};
//�ػ�����̳�
class TestDelegateDelegateReceiver :public TestDelegate_DelegateReceiver
{
public:
	void TestDelegate(UserDataOfTransportLayer_t* userDataOfTransportLayer, DateTime_t now[1])//server�㲥��client���ܵ���
	{
		printf("%u-%u-%u %u:%u:%u!client\r\n\0", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
		//printf("%s", now[0].DateString);
	}
};
//����ί�ж���
DateTimeChangeDelegateReceiver DateTimeChange;
TestDelegateDelegateReceiver Test;

DelegateDescribe AllDelegates[2] =//����ί�ж��󼯺�
{
	{"DateTimeChange"  ,&DateTimeChange},
	{"TestDelegate"  ,&Test},
};//client���Դ����Delegate����

ResponseDescribe Responses[4] =//����ظ� ID ����
{
	{"Inter_Add"   ,     Inter_Add_ServiceId},
	{"Inter_NoArg"     ,   Inter_NoArg_ServiceId},
	{"Inter_NoReturn"   ,     Inter_NoReturn_ServiceId},
	{"Inter_NoArgAndReturn"    ,    Inter_NoArgAndReturn_ServiceId},
};


static ClientNodeQuicklyInitConfig InitCfg =
{
	"Client",
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,false},//Buffer for Request
	ClientSend,
	1000,
	Responses,
	1,
	AllDelegates,
	2,
	{
		true,//CheckSumValid
		6,//ServerThreadPriority
		6,//ClientThreadPriority
		false,//UseRingBufferWhenReceiving
		{
			true,//IsSendToQueue
			10,//DelegateBlockQueue_MaxItemNumber
			10,//ResponseBlockQueue_MaxItemNumber
			10,//RequestBlockQueue_MaxItemNumber
		},
		{
			{new UInt8[AllTypeBufferLen],AllTypeBufferLen,10},//DelegateBlockBufferProvider
			{new UInt8[AllTypeBufferLen],AllTypeBufferLen,10},//ResponseBlockBufferProvider
			{nullptr,0,0},//RequestBlockBufferProvider
		},

	},
	nullptr,
};
EmbedXrpcObject ClientRpc;//client rpc ����

void Client_Init()
{
	ClientRpc.Init(&InitCfg);
}

Inter_Requester Client(&ClientRpc);//����request����
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
		El_Assert(sum.State == ResponseState_Ok);
		printf("client:sum7 is:%d\n", sum.ReturnValue.Sum7);
		Client.Free_Add(&sum);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));//�ȴ�RPC����ȫ�����
	ClientRpc.DeInit();
}