#include <thread>
#include "Sample1.Client.h"
#include "DTL.h"
extern EmbedXrpcObject<UserDataOfTransportLayer_t> ServerRpc;


#define AllTypeBufferLen	4096

uint8_t ClientDelBuffer[AllTypeBufferLen];
uint8_t ClientResBuffer[AllTypeBufferLen];
uint8_t ClientReqBuffer[AllTypeBufferLen];
uint8_t ClientDataLinkBuffer[AllTypeBufferLen];

bool ClientSend(UserDataOfTransportLayer_t* userDataOfTransportLayer,
	EmbedXrpcObject<UserDataOfTransportLayer_t>* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	assert(ServerRpc.ReceivedMessage(dataLen, data, *userDataOfTransportLayer) == true);
	return true;
}
//�ػ�����̳�
class DateTimeChangeDelegateReceiver :public DateTimeChange_DelegateReceiver<UserDataOfTransportLayer_t>
{
public:
	void DateTimeChange(UserDataOfTransportLayer_t* userDataOfTransportLayer, DateTime_t now[1])//server�㲥��client���ܵ���
	{
		printf("%u-%u-%u %u:%u:%u!client\r\n\0", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
		//printf("%s", now[0].DateString);
	}
};
//�ػ�����̳�
class TestDelegateDelegateReceiver :public TestDelegate_DelegateReceiver<UserDataOfTransportLayer_t>
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

DelegateDescribe<UserDataOfTransportLayer_t> AllDelegates[2] =//����ί�ж��󼯺�
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


ClientNodeQuicklyInitConfig<UserDataOfTransportLayer_t> InitConfig =
{
	ClientDataLinkBuffer,
	AllTypeBufferLen,
	{ ClientSend },
	1000,
	Responses,
	4,
	AllDelegates,
	2,
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
			new BlockRingBufferProvider<UserDataOfTransportLayer_t>(ClientDelBuffer,AllTypeBufferLen,10),//DelegateBlockBufferProvider
			new BlockRingBufferProvider<UserDataOfTransportLayer_t>(ClientResBuffer,AllTypeBufferLen,10),//ResponseBlockBufferProvider
			new BlockRingBufferProvider<UserDataOfTransportLayer_t>(ClientReqBuffer,AllTypeBufferLen,10),//RequestBlockBufferProvider
		},
		
	},
	nullptr,
};
EmbedXrpcObject<UserDataOfTransportLayer_t> ClientRpc(&InitConfig);//client rpc ����


InterClientImpl<UserDataOfTransportLayer_t> Client(&ClientRpc);//����request����
void ClientThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	int a = 1000, b = 5000;
	uint8_t Bytes[7] = "123456";
	int testcount = 10;
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
		Client.Free_Add(&sum);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));//�ȴ�RPC����ȫ�����
	ClientRpc.DeInit();
}