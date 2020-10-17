#include <thread>
#include "Sample1.Client.h"
#include "Sample1.Server.h"

extern EmbedXrpcObject ClientRpc;
extern EmbedXrpcObject ServerRpc;

//-------------------------------------------------------------------------
//client 
uint8_t ClientSendBuffer[2048];//发送buffer
uint8_t ClientResponseBuffer[2048];//Client接收到Server发送的Response回复后，要把数据放到这个里面，如果你的协议没有回复，这个可以为null
uint8_t ClientDelegateBuffer[2048];//Client接收到Server发送的Delegate广播后，要把数据放到这个里面，如果你的协议没有广播，这个可以为null
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
	true,
	nullptr);//client rpc 对象

void DateTimeChangeClientImpl::DateTimeChange(DateTime_t now[1])//server广播后，client接受到的
{
	printf("%d-%d-%d %d:%d:%d\n", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
}

InterClientImpl Client(&ClientRpc);
void ClientThread()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	int a=1000, b = 5000;
	while (true)
	{
		a ++;
		b ++;
		auto sum=Client.Add(a, b);
		if (sum.State == ResponseState_Ok)
		{
			printf("%d+%d=%d\n", a,b,sum.ReturnValue);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
//--------------------------------------------------------------------
//server
uint8_t ServerSendBuffer[2048];//发送buffer
uint8_t ServerRequestBuffer[2048];//server接收到client发送的Request数据流后，要把数据临时存到这个数组里
bool ServerSend(void* rpcObj, uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去，如果你的协议没有client的request请求，这个可以为null
{
	/*for (size_t i = 4; i < dataLen; i++)
	{
		printf("ServerSend:0x%.2x\n", data[i]);

	}*/
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
	true,
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

void Inter_AddService::Add(Int32 a, Int32 b)
{
	Response.ReturnValue = a + b;
}
void Inter_NoArgService::NoArg()
{
	Response.ReturnValue = true;
}
void Inter_NoReturnService::NoReturn(int a)
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

void* MyMalloc(size_t size)
{
	void* ptr = malloc(size);
	printf("malloc :0x%x\n", (uint32_t)ptr);
	return ptr;
}
void MyFree(void* ptr)
{
	printf("free :0x%x\n", (uint32_t)ptr);
	free(ptr);
}

/*
运行提示QObject::startTimer: Timers cannot be started from another thread  
是因为QT不允许其他线程操作QT的定时器，你可以临时把Win32EmbedXrpcPort.cpp的
t->Start(interval);、t->Reset();、t->Stop();注释掉  这样就没有这个问题了。
*/