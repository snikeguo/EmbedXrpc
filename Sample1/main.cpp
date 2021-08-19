#include <thread>
#include "Sample1.Client.h"
#include "Sample1.Server.h"

extern EmbedXrpcObject ClientRpc;
extern EmbedXrpcObject ServerRpc;

//-------------------------------------------------------------------------
//client 
bool ClientSend(UserDataOfTransportLayer_t* userDataOfTransportLayer,EmbedXrpcObject* rpcObj, uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	assert(ServerRpc.ReceivedMessage(dataLen, data, *userDataOfTransportLayer)==true);
	return true;
}
DateTimeChangeClientImpl DateTimeChange;
TestDelegateClientImpl Test;

DelegateDescribe AllDelegates[2] =
{
	{"DateTimeChange"  ,&DateTimeChange},
	{"TestDelegate"  ,&Test},
};//client���Դ����Delegate����

ResponseDescribe Responses[4] =
{
	{"Inter_Add"   ,     Inter_Add_ServiceId},
	{"Inter_NoArg"     ,   Inter_NoArg_ServiceId},
	{"Inter_NoReturn"   ,     Inter_NoReturn_ServiceId},
	{"Inter_NoArgAndReturn"    ,    Inter_NoArgAndReturn_ServiceId},
};

EmbedXrpcObject ClientRpc(ClientSend,
	1000,
	Responses,
	4,
	AllDelegates,
	2,
	nullptr);//client rpc ����

void DateTimeChangeClientImpl::DateTimeChange(UserDataOfTransportLayer_t* userDataOfTransportLayer, DateTime_t now[1])//server�㲥��client���ܵ���
{
	printf("%u-%u-%u %u:%u:%u!client\r\n\0", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
	//printf("%s", now[0].DateString);
}
void TestDelegateClientImpl::TestDelegate(UserDataOfTransportLayer_t* userDataOfTransportLayer, DateTime_t now[1])//server�㲥��client���ܵ���
{
	printf("%u-%u-%u %u:%u:%u!client\r\n\0", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
	//printf("%s", now[0].DateString);
}
InterClientImpl Client(&ClientRpc);
void ClientThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	int a=1000, b = 5000;
	uint8_t Bytes[7] = "123456";
	int testcount = 5;
	Win32UserDataOfTransportLayerTest win32UserDataOfTransportLayerTest;
	strcpy(win32UserDataOfTransportLayerTest.IpAddress, "127.0.0.1");
	win32UserDataOfTransportLayerTest.Port = 6666;
	while (testcount-- > 0)
	{
		a ++;
		b ++;
		Client.Add_SendData.a = a;
		Client.Add_SendData.b = b;
		Client.Add_SendData.dataLen = 4;
		Client.Add_SendData.data = (UInt8 *)"123";
		auto sum=Client.Add(&win32UserDataOfTransportLayerTest);
		if (sum.State == ResponseState_Ok)
		{
			printf("%d+%d=%d\n", a,b,sum.ReturnValue.Sum);
		}
		Client.Free_Add(&sum);
		std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));//�ȴ�RPC����ȫ�����
	ClientRpc.DeInit();
}
//--------------------------------------------------------------------
//server
bool ServerSend(UserDataOfTransportLayer_t* userDataOfTransportLayer, EmbedXrpcObject* rpcObj, uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ��������Э��û��client��request�����������Ϊnull
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
	ClientRpc.ReceivedMessage(dataLen, data, *userDataOfTransportLayer);
	return true;
}
Inter_AddService Inter_AddService_Instance;
Inter_NoArgService Inter_NoArgService_Instance;
Inter_NoReturnService Inter_NoReturnService_Instance;
Inter_NoArgAndReturnService Inter_NoArgAndReturnService_Instance;
RequestDescribe Requests[] =
{
	{"Inter_Add",					&Inter_AddService_Instance},
	{"Inter_NoArg",					&Inter_NoArgService_Instance},
	{"Inter_NoReturn",				&Inter_NoReturnService_Instance},
	{"Inter_NoArgAndReturn",        &Inter_NoArgAndReturnService_Instance},
};
EmbedXrpcObject ServerRpc(ServerSend,
	500,
	Requests,
	4,
	nullptr);//server rpc ����
DateTimeChangeDelegate DateTimeChanger(&ServerRpc);
void ServerThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(-1));
	DateTime_t t;
	uint8_t data[128];
	t.DateString = data;
	int testcount = 5;
	Win32UserDataOfTransportLayerTest win32UserDataOfTransportLayerTest;
	strcpy(win32UserDataOfTransportLayerTest.IpAddress, "192.168.1.101");
	win32UserDataOfTransportLayerTest.Port = 7777;
	while (testcount-->0)
	{		
		
		auto ti = (time(nullptr));
		auto localti = localtime(&ti);
		t.Day = localti->tm_mday;
		t.Hour = localti->tm_hour;
		t.Min = localti->tm_min;
		t.Month = localti->tm_mon;
		t.Sec = localti->tm_sec;
		t.Year = localti->tm_year+1900;
		t.Sex = t.Sec % 2 == 0 ? Sex::Man : Sex::WoMan;
		memset(t.DateString, 0x0, 128);
		sprintf((char *)t.DateString, "%d-%d-%d %d:%d:%d!server\r\n", t.Year, t.Month, t.Day, t.Hour, t.Min, t.Sec);
		t.DateStringLen = (UInt8)strlen((const char *)t.DateString)+1;
		
		t.David.a = 1;
		t.David.b = 5;
		t.David.AddressType = Student_u2_FieldNumber;
		t.David.u2 = 0x66778899;
		t.David.uend1 = 1;
		t.David.uend2 = 2;
		DateTimeChanger.Invoke(&win32UserDataOfTransportLayerTest,&t);
		std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));//�ȴ�RPC����ȫ�����
	ServerRpc.DeInit();
}

void Inter_AddService::Add(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer,
	UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,
	EmbedXrpcObject* rpcObject,
	uint16_t targetTimeOut,
	Int32 a, Int32 b, Int32 dataLen, UInt8* data)
{
	EmbedXrpcObject* RpcObj = (EmbedXrpcObject*)rpcObject;
	RpcObj->UserDataOfTransportLayerOfSuspendTimerUsed.Port = 777;
	EmbedXrpc_TimerStart(RpcObj->SuspendTimer, targetTimeOut/2);
	IsFreeResponse = false;
	Response.ReturnValue.Sum = a + b;
	Response.ReturnValue.dataLen = 0;
	Response.ReturnValue.data = NULL;
	printf("ģ���ʱ����  ��ʱ2��\n");
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	//strncpy((char *)Response.ReturnValue.data, "6789", dataLen + 1);
	//printf("len:%d\n", dataLen);
}


void Inter_NoArgService::NoArg(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, 
	UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,
	EmbedXrpcObject* rpcObject,
	uint16_t targetTimeOut)
{
	IsFreeResponse = true;
	Response.ReturnValue = true;
}

void Inter_NoReturnService::NoReturn(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer,
	UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,
	EmbedXrpcObject* rpcObject,
	uint16_t targetTimeOut,
	int a)
{

}
void Inter_NoArgAndReturnService::NoArgAndReturn(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer,
	UserDataOfTransportLayer_t* response_UserDataOfTransportLayer, 
	EmbedXrpcObject* rpcObject,
	uint16_t targetTimeOut)
{

}
int main(int argc, char *argv[])
{
	

	ClientRpc.Init();
	std::thread c = std::thread(ClientThread);
	c.detach();

	ServerRpc.Init();
	std::thread s = std::thread(ServerThread);
	s.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
}

/*
������ʾQObject::startTimer: Timers cannot be started from another thread  
����ΪQT�����������̲߳���QT�Ķ�ʱ�����������ʱ��Win32EmbedXrpcPort.cpp��
t->Start(interval);��t->Reset();��t->Stop();ע�͵�  ������û����������ˡ�
*/