#include <thread>

#include "Sample1.Server.h"
#include "DTL.h"

#define DataLinkBufferLen	4096
extern EmbedXrpcObject<UserDataOfTransportLayer_t> ClientRpc;

#define AllTypeBufferLen	4096

uint8_t ServerDelBuffer[AllTypeBufferLen];
uint8_t ServerResBuffer[AllTypeBufferLen];
uint8_t ServerReqBuffer[AllTypeBufferLen];
uint8_t ServerDataLinkBuffer[AllTypeBufferLen];
bool ServerSend(UserDataOfTransportLayer_t* userDataOfTransportLayer, 
	EmbedXrpcObject<UserDataOfTransportLayer_t>* rpcObj,
	uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ��������Э��û��client��request�����������Ϊnull
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
	ClientRpc.ReceivedMessage(dataLen, data, *userDataOfTransportLayer);
	return true;
}
//�ػ�����̳�
class Inter_AddServiceProvider :public Inter_AddService<Win32UserDataOfTransportLayerTest>
{
public:
	void Add(ServiceInvokeParameter<Win32UserDataOfTransportLayerTest>* serviceInvokeParameter,
		Int32 a, Int32 b, Int32 dataLen, UInt8* data)
	{
		EmbedXrpcObject<Win32UserDataOfTransportLayerTest>* RpcObj = 
			(EmbedXrpcObject<Win32UserDataOfTransportLayerTest>*)serviceInvokeParameter->rpcObject;
		RpcObj->UserDataOfTransportLayerOfSuspendTimerUsed.Port = 777;
		El_TimerStart(RpcObj->SuspendTimer, serviceInvokeParameter->targetTimeOut / 2);
		this->IsFreeResponse = true;
		Response.ReturnValue.Sum = 1;
		Response.ReturnValue.Sum2 = 2;
		Response.ReturnValue.Sum3 = 3;

		Response.ReturnValue.Sum4 = 1;
		Response.ReturnValue.Sum5 = 2;
		Response.ReturnValue.Sum6 = 3;
		Response.ReturnValue.Sum7 = 0x66661111;

		Response.ReturnValue.dataLen = 0;
		Response.ReturnValue.data = NULL;
		printf("ģ���ʱ����  ��ʱ2��\n");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		//strncpy((char *)Response.ReturnValue.data, "6789", dataLen + 1);
		//printf("len:%d\n", dataLen);
	}
};

//�ػ�����̳�
class Inter_NoArgServiceProvider :public Inter_NoArgService<Win32UserDataOfTransportLayerTest>
{
public:
	void NoArg(ServiceInvokeParameter<Win32UserDataOfTransportLayerTest>* serviceInvokeParameter)
	{
		this->IsFreeResponse = true;
		Response.ReturnValue = true;
	}
};


//�ػ�����̳�
class Inter_NoReturnServiceProvider :public Inter_NoReturnService<Win32UserDataOfTransportLayerTest>
{
public:
	void NoReturn(ServiceInvokeParameter<Win32UserDataOfTransportLayerTest>* serviceInvokeParameter,
		int a)
	{

	}

};

//�ػ�����̳�
class Inter_NoArgAndReturnServiceProvider :public Inter_NoArgAndReturnService<Win32UserDataOfTransportLayerTest>
{
public:
	void NoArgAndReturn(ServiceInvokeParameter<Win32UserDataOfTransportLayerTest>* serviceInvokeParameter)
	{

	}
};

//����service����ʵ��
Inter_AddServiceProvider Inter_AddService_Instance;
Inter_NoArgServiceProvider Inter_NoArgService_Instance;
Inter_NoReturnServiceProvider Inter_NoReturnService_Instance;
Inter_NoArgAndReturnServiceProvider Inter_NoArgAndReturnService_Instance;
RequestDescribe<UserDataOfTransportLayer_t> Requests[] = //�������󼯺�
{
	{"Inter_Add",					&Inter_AddService_Instance},
	{"Inter_NoArg",					&Inter_NoArgService_Instance},
	{"Inter_NoReturn",				&Inter_NoReturnService_Instance},
	{"Inter_NoArgAndReturn",        &Inter_NoArgAndReturnService_Instance},
};
SendAction<UserDataOfTransportLayer_t> ServerSendAction = { ServerSend };//���巢�Ͷ���

ServerNodeQuicklyInitConfig<UserDataOfTransportLayer_t> InitConfig =
{
	ServerDataLinkBuffer,
	AllTypeBufferLen,
	 { ServerSend },
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
			new BlockRingBufferProvider<UserDataOfTransportLayer_t>(ServerDelBuffer,AllTypeBufferLen,10),//DelegateBlockBufferProvider
			new BlockRingBufferProvider<UserDataOfTransportLayer_t>(ServerResBuffer,AllTypeBufferLen,10),//ResponseBlockBufferProvider
			new BlockRingBufferProvider<UserDataOfTransportLayer_t>(ServerReqBuffer,AllTypeBufferLen,10),//RequestBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject<UserDataOfTransportLayer_t> ServerRpc(&InitConfig);//server rpc ����
DateTimeChange_DelegateSender<UserDataOfTransportLayer_t> DateTimeChanger(&ServerRpc);//ʵ����ί�ж���
void ServerThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
	DateTime_t t;
	uint8_t data[128];
	t.DateString = data;
	int testcount = 1;
	Win32UserDataOfTransportLayerTest win32UserDataOfTransportLayerTest;
	strcpy(win32UserDataOfTransportLayerTest.IpAddress, "192.168.1.101");
	win32UserDataOfTransportLayerTest.Port = 7777;
	while (testcount-- > 0)
	{

		auto ti = (time(nullptr));
		auto localti = localtime(&ti);
		t.Day = localti->tm_mday;
		t.Hour = localti->tm_hour;
		t.Min = localti->tm_min;
		t.Month = localti->tm_mon;
		t.Sec = localti->tm_sec;
		t.Year = localti->tm_year + 1900;
		t.Sex = t.Sec % 2 == 0 ? Sex::Man : Sex::WoMan;
		memset(t.DateString, 0x0, 128);
		sprintf((char*)t.DateString, "%d-%d-%d %d:%d:%d!server\r\n", t.Year, t.Month, t.Day, t.Hour, t.Min, t.Sec);
		t.DateStringLen = (UInt8)strlen((const char*)t.DateString) + 1;

		t.David.a = 1;
		t.David.b = 5;
		t.David.AddressType = Student_u2_FieldNumber;
		t.David.u2 = 0x66778899;
		t.David.uend1 = 1;
		t.David.uend2 = 2;
		DateTimeChanger.Invoke(&win32UserDataOfTransportLayerTest, &t);//����ί��
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));//�ȴ�RPC����ȫ�����
	ServerRpc.DeInit();
}