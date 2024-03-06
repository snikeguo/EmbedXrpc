#include <thread>
#include "Sample1.Client.h"
#include "Sample1.Server.h"


extern EmbedXrpcObject ClientRpc;

#define AllTypeBufferLen	4096

uint8_t ClientBuffer[AllTypeBufferLen];
bool ServerSend(RequestParameter* rp,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去，如果你的协议没有client的request请求，这个可以为null
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
	memcpy(ClientBuffer, data, dataLen);
	ClientRpc.ReceivedMessage(dataLen, ClientBuffer, false);
	return true;
}

void Add_Parameter_Deserialize2(SerializationManager* sm, Add_Parameter* obj, int isIsr)
{
	DeserializeField((uint8_t*)&obj->a, sm, 4, sizeof(int32_t), isIsr);
	DeserializeField((uint8_t*)&obj->b, sm, 4, sizeof(int32_t), isIsr);
	DeserializeField((uint8_t*)&obj->dataLen, sm, 4, sizeof(int32_t), isIsr);
	//obj->data = (uint8_t*)El_Malloc(sizeof(uint8_t) * obj->dataLen);
	//El_Memset(obj->data, 0, sizeof(uint8_t) * obj->dataLen);
	obj->data = &sm->Buf[sm->Index];
	sm->Index += obj->dataLen;


	/*
	for (int32_t data_index = 0; data_index < obj->dataLen; data_index++)
	{
		DeserializeField((uint8_t*)&obj->data[data_index], sm, 1, sizeof(uint8_t), isIsr);
	}
	*/
	for (uint32_t test_index = 0; test_index < 1; test_index++)
	{
		TestSerialize_Deserialize(sm, &obj->test[test_index], isIsr);
	}
}

//特化子类继承
class Inter_AddServiceProvider :public Add_Service
{
public:
	void Add(ServiceInvokeParameter* serviceInvokeParameter, int32_t a, int32_t b, int32_t dataLen, uint8_t* data, TestSerialize test[1])//int32_t a, int32_t b, int32_t dataLen, uint8_t* data,TestSerialize* test
	{
		EmbedXrpcObject* RpcObj = 
			(EmbedXrpcObject*)serviceInvokeParameter->RpcObject;
		//El_TimerStart(RpcObj->SuspendTimer, serviceInvokeParameter->TargetTimeOut / 2,0);
		this->IsFreeResponse = true;

		Response.ReturnValue.Sum = 1;
		Response.ReturnValue.Sum2 = 2;
		Response.ReturnValue.Sum3 = 3;

		Response.ReturnValue.Sum4 = 1;
		Response.ReturnValue.Sum5 = 2;
		Response.ReturnValue.Sum6 = 3;
		Response.ReturnValue.Sum7 = a+ b;

		Response.ReturnValue.dataLen = 0;
		Response.ReturnValue.data = NULL;
		printf("server: addservice 模拟耗时操作  延时2秒\n");
		//std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		//strncpy((char *)Response.ReturnValue.data, "6789", dataLen + 1);
		//printf("len:%d\n", dataLen);
	}
};

//特化子类继承
class Inter_NoArgServiceProvider :public NoArg_Service
{
public:
	void NoArg(ServiceInvokeParameter* serviceInvokeParameter)
	{
		this->IsFreeResponse = true;
		Response.ReturnValue = true;
	}
};


//特化子类继承
class Inter_NoReturnServiceProvider :public NoReturn_Service
{
public:
	void NoReturn(ServiceInvokeParameter* serviceInvokeParameter,
		int a)
	{

	}

};

//特化子类继承
class Inter_NoArgAndReturnServiceProvider :public NoArgAndReturn_Service
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
static ServiceDescribe Services[] = //定义请求集合
{
	{"Inter_Add",					&Inter_AddService_Instance},
	{"Inter_NoArg",					&Inter_NoArgService_Instance},
	{"Inter_NoReturn",				&Inter_NoReturnService_Instance},
	{"Inter_NoArgAndReturn",        &Inter_NoArgAndReturnService_Instance},
};


static InitConfig InitCfg =
{
	"Server",
	{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,false},// DataLinkBufferConfigForRequest
	{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,false},//DataLinkBufferConfigForResponse
	ServerSend,
	500,
	Services,
	sizeof(Services)/sizeof(ServiceDescribe),
	{
		1,//ServiceThreadPriority
		2048,
		true,//IsSendToQueue
		40960,//MessageQueueOfRequestService_MaxItemNumber
		40960,//ServiceMessageQueue_MaxItemNumber
	},
	nullptr,
};

EmbedXrpcObject ServerRpc;//server rpc 对象
void Server_Init()
{
	ServerRpc.Init(&InitCfg);
}
DateTimeChange_Requester DateTimeChanger(&ServerRpc);//实例化委托对象
void ServerThread()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
	DateTime_t t;
	uint8_t data[128];
	t.DateString = data;
	uint32_t testcount = 0xffffffff;
	Win32UserDataOfTransportLayerTest win32UserDataOfTransportLayerTest;
	strcpy(win32UserDataOfTransportLayerTest.IpAddress, "192.168.1.101");
	win32UserDataOfTransportLayerTest.Port = 7777;
	RequestParameter rp;
	rp.IsIsr = 0;
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
		sprintf((char*)t.DateString, "server:%d-%d-%d %d:%d:%d!server\r\n", t.Year, t.Month, t.Day, t.Hour, t.Min, t.Sec);
		t.DateStringLen = (uint8_t)strlen((const char*)t.DateString) + 1;

		t.David.a = 1;
		t.David.b = 5;
		t.David.AddressType = Student_u2_FieldNumber;
		t.David.u2 = 0x66778899;
		t.David.uend1 = 1;
		t.David.uend2 = 2;
		DateTimeChanger.DateTimeChange(&rp, &t);//调用委托
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
#if EmbedXrpc_UsingOs==0
		ServerRpc.NoOs_ServiceExecute(0);
#endif
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));//等待RPC调用全部完毕
	ServerRpc.DeInit();
}