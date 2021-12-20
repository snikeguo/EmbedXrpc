#include <thread>

#include "Sample1.Server.h"


extern EmbedXrpcObject ClientRpc;

#define AllTypeBufferLen	4096

bool ServerSend(UserDataOfTransportLayer_t* userDataOfTransportLayer, 
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去，如果你的协议没有client的request请求，这个可以为null
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
	ClientRpc.ReceivedMessage(dataLen, data, *userDataOfTransportLayer);
	return true;
}
//特化子类继承
class Inter_AddServiceProvider :public Inter_Add_Service
{
public:
	void Add(ServiceInvokeParameter* serviceInvokeParameter,
		Int32 a, Int32 b, Int32 dataLen, UInt8* data)
	{
		EmbedXrpcObject* RpcObj = 
			(EmbedXrpcObject*)serviceInvokeParameter->RpcObject;
		RpcObj->UserDataOfTransportLayerOfSuspendTimerUsed.Port = 777;
		El_TimerStart(RpcObj->SuspendTimer, serviceInvokeParameter->TargetTimeOut / 2);
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
		printf("模拟耗时操作  延时2秒\n");
		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		//strncpy((char *)Response.ReturnValue.data, "6789", dataLen + 1);
		//printf("len:%d\n", dataLen);
	}
};

//特化子类继承
class Inter_NoArgServiceProvider :public Inter_NoArg_Service
{
public:
	void NoArg(ServiceInvokeParameter* serviceInvokeParameter)
	{
		this->IsFreeResponse = true;
		Response.ReturnValue = true;
	}
};


//特化子类继承
class Inter_NoReturnServiceProvider :public Inter_NoReturn_Service
{
public:
	void NoReturn(ServiceInvokeParameter* serviceInvokeParameter,
		int a)
	{

	}

};

//特化子类继承
class Inter_NoArgAndReturnServiceProvider :public Inter_NoArgAndReturn_Service
{
public:
	void NoArgAndReturn(ServiceInvokeParameter* serviceInvokeParameter)
	{

	}
};

//定义service对象实例
Inter_AddServiceProvider Inter_AddService_Instance;
Inter_NoArgServiceProvider Inter_NoArgService_Instance;
Inter_NoReturnServiceProvider Inter_NoReturnService_Instance;
Inter_NoArgAndReturnServiceProvider Inter_NoArgAndReturnService_Instance;
RequestDescribe Requests[] = //定义请求集合
{
	{"Inter_Add",					&Inter_AddService_Instance},
	{"Inter_NoArg",					&Inter_NoArgService_Instance},
	{"Inter_NoReturn",				&Inter_NoReturnService_Instance},
	{"Inter_NoArgAndReturn",        &Inter_NoArgAndReturnService_Instance},
};
ServerNodeQuicklyInitConfig InitConfig =
{
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},
	{new UInt8[AllTypeBufferLen],AllTypeBufferLen,nullptr},
	{nullptr,0,nullptr},
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
			nullptr,//DelegateBlockBufferProvider
			nullptr,//ResponseBlockBufferProvider
			new BlockRingBufferProvider(new UInt8[AllTypeBufferLen],AllTypeBufferLen,10),//RequestBlockBufferProvider
		},
	},
	nullptr,
};

EmbedXrpcObject ServerRpc(&InitConfig);//server rpc 对象
DateTimeChange_DelegateSender DateTimeChanger(&ServerRpc);//实例化委托对象
void ServerThread()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
	DateTime_t t;
	uint8_t data[128];
	t.DateString = data;
	int testcount = 10;
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
		DateTimeChanger.Invoke(&win32UserDataOfTransportLayerTest, &t);//调用委托
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));//等待RPC调用全部完毕
	ServerRpc.DeInit();
}