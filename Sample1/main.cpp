#include <thread>
#include "Sample1.Client.h"
#include "Sample1.Server.h"
#include "Win32EmbedXrpcPort.h"



Win32EmbedXrpcPort Win32Port;
extern EmbedXrpcObject ClientRpc;
extern EmbedXrpcObject ServerRpc;

//-------------------------------------------------------------------------
//client 
uint8_t ClientSendBuffer[2048];//发送buffer
uint8_t ClientResponseBuffer[2048];
uint8_t ClientDelegateBuffer[2048];
bool ClientSend(void* rpcObj, uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去
{
	ServerRpc.ReceivedMessage(dataLen, data);
	return true;
}
ResponseDelegateMessageMapCollection rdCollection[1] = { {Inter_ResponseDelegateMessages_Count,Inter_ResponseDelegateMessages} };//client可以处理的service集合

EmbedXrpcObject ClientRpc(ClientSend,
	500,
	ClientSendBuffer,
	2048,
	ClientResponseBuffer,
	2048,
	ClientDelegateBuffer,
	2048,
	rdCollection,
	1,
	&Win32Port,
	nullptr);//client rpc 对象

void DateTimeChangeClientImpl::DateTimeChange(DateTime_t now[1])//server广播后，client接受到的
{
	printf("%d-%d-%d %d:%d:%d\n", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
}

InterClientImpl Client(&ClientRpc);
void ClientThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	while (true)
	{
		auto sum=Client.Add(5, 2);
		if (sum.State == ResponseState_Ok)
		{
			printf("sum:%d\n", sum.ReturnValue);
		}
		Client.NoArg();
		Client.NoReturn();
		Client.NoArgAndReturn();
		std::this_thread::sleep_for(std::chrono::milliseconds(0));
	}
}
//--------------------------------------------------------------------
//server
uint8_t ServerSendBuffer[2048];
uint8_t ServerRequestBuffer[2048];//发送buffer
bool ServerSend(void* rpcObj, uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去
{
	ClientRpc.ReceivedMessage(dataLen, data);
	return true;
}
RequestMessageMapCollection rmCollection[1] = { {Inter_RequestMessages_Count,Inter_RequestMessages} };
EmbedXrpcObject ServerRpc(ServerSend,
	500,
	ServerSendBuffer,
	2048,
	ServerRequestBuffer,
	2048, 
	rmCollection,
	1,
	&Win32Port,
	nullptr);//server rpc 对象
DateTimeChangeDelegate DateTimeChanger(&ServerRpc);
void ServerThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	while (true)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		DateTime_t t;
		auto ti = (time(nullptr));
		auto localti = localtime(&ti);
		t.Day = localti->tm_mday;
		t.Hour = localti->tm_hour;
		t.Min = localti->tm_min;
		t.Month = localti->tm_mon;
		t.Sec = localti->tm_sec;
		t.Year = localti->tm_year+1900;
		DateTimeChanger.Invoke(&t);
	}
}

void Inter_AddService::Add(Byte a, Byte b)
{
	Response.ReturnValue = a + b;
}
void Inter_NoArgService::NoArg()
{
	Response.ReturnValue = true;
}
void Inter_NoReturnService::NoReturn()
{

}
void Inter_NoArgAndReturnService::NoArgAndReturn()
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
运行提示QObject::startTimer: Timers cannot be started from another thread  
是因为QT不允许其他线程操作QT的定时器，你可以临时把Win32EmbedXrpcPort.cpp的
t->Start(interval);、t->Reset();、t->Stop();注释掉  这样就没有这个问题了。

*/