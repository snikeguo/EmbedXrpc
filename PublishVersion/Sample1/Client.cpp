#include <thread>
#include "Sample1.Client.h"
#include "Sample1.Server.h"
extern EmbedXrpcObject ServerRpc;


#define AllTypeBufferLen	4096


uint8_t ServerBuffer[AllTypeBufferLen];
bool ClientSend(RequestParameter* rp,
	EmbedXrpcObject* rpcObj,
	uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去
{
	memcpy(ServerBuffer, data, dataLen);
	auto x = ServerRpc.ReceivedMessage(dataLen, ServerBuffer, false);
	assert(x == ReceivedMessageStatus::ReceivedMessageStatus_Ok);
	return true;
}
//特化子类继承
class DateTimeChange_ServiceReceiver :public DateTimeChange_Service
{
public:
	void DateTimeChange(ServiceInvokeParameter* serviceInvokeParameter, DateTime_t now[1])//server广播后，client接受到的
	{
		printf("client:%u-%u-%u %u:%u:%u!client\r\n", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
		//printf("%s", now[0].DateString);
	}
};
//特化子类继承
class Test2_ServiceReceiver :public Test2_Service
{
public:
	void Test2(ServiceInvokeParameter* serviceInvokeParameter, DateTime_t now[1])//server广播后，client接受到的
	{
		printf("%u-%u-%u %u:%u:%u!client\r\n", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
		//printf("%s", now[0].DateString);
	}
};

DateTimeChange_ServiceReceiver DateTimeChange;
Test2_ServiceReceiver Test2;

static ServiceDescribe AllServices[2] =//定义委托对象集合
{
	{"DateTimeChange"  ,&DateTimeChange},
	{"Test2"  ,&Test2},
};//client可以处理的Request集合




static InitConfig InitCfg =
{
	"Client",
	{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,false},//Buffer for Request
	{new uint8_t[AllTypeBufferLen],AllTypeBufferLen,false},//Buffer for Request
	ClientSend,
	10000,
	AllServices,
	2,
	{
		1,//ServiceThreadPriority
		2048,
		true,//IsSendToQueue
		40960,//MessageQueueOfRequestService_MaxItemNumber
		40960,//ServiceMessageQueue_MaxItemNumber
	},
	nullptr,
};
EmbedXrpcObject ClientRpc;//client rpc 对象

void Client_Init()
{
	ClientRpc.Init(&InitCfg);
}

Add_Requester Client(&ClientRpc);//定义request对象
uint8_t requestBuffer[4096];
void ClientThread()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
	int a = 1000, b = 5000;
	uint8_t Bytes[7] = "123456";
	uint32_t testcount = 0xffffffff;
	Win32UserDataOfTransportLayerTest win32UserDataOfTransportLayerTest;
	strcpy(win32UserDataOfTransportLayerTest.IpAddress, "127.0.0.1");
	win32UserDataOfTransportLayerTest.Port = 6666;
	RequestParameter rp;
	rp.IsIsr = 0;
	rp.IsProvideBuffer = true;
	rp.Buffer = requestBuffer;
	rp.BufferLen = sizeof(requestBuffer);
	while (testcount-- > 0)
	{
#if 1
		//vTaskDelay(-1);
		a++;
		b++;
		Client.Add_SendData.a = a;
		Client.Add_SendData.b = b;
		Client.Add_SendData.dataLen = 4;
		Client.Add_SendData.data = (uint8_t*)"123";
		Client.Add_SendData.test[0].EnumArrayLen = 1;
		Client.Add_SendData.test[0].EnumArray = (Sex*)malloc(sizeof(Sex) * 1);
		Client.Add_SendData.test[0].EnumArray[0] = Sex::Man;
		Client.Add_SendData.test[0].ObjectArrayLen = 2;
		Client.Add_SendData.test[0].DateTimeArray = (DateTime_t*)malloc(sizeof(DateTime_t) * 2);
		memset(Client.Add_SendData.test[0].DateTimeArray, 0, sizeof(DateTime_t) * 2);
		Client.Add_SendData.test[0].DateTimeArray[0].Year = 1; 

#if EmbedXrpc_UsingOs==0
		auto sum = Client.NoOs_Add(&rp);//request对象请求service 
		while (1)
		{
			sum = Client.NoOs_QueryServiceState(&rp);
			if (sum.State != RequestResponseState::ResponseState_NoReceived)
			{
				break;
			}
			//Sleep(1);
		}
#else
		auto sum = Client.Add(&rp);//request对象请求service 
#endif
		if (sum.State != ResponseState_Ok)
		{
			printf("Client was requested the \"Add\" service Faild! error code:%d\n", sum.State);
		}
		else
		{
			printf("client:sum7 is:%d\n", sum.ReturnValue.Sum7);
		}
		Client.Free_Add(&sum);
		
#endif
#if EmbedXrpc_UsingOs==0
		ClientRpc.NoOs_ServiceExecute(0);
#endif
		Sleep(500);
	}
	Sleep(3000);//等待RPC调用全部完毕
	ClientRpc.DeInit();
}