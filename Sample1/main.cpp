#include <QtCore/QCoreApplication>
#include "Sample1.Client.h"
#include "Sample1.Server.h"
#include "Win32EmbedXrpcPort.h"
#include <QDateTime>
#include <QDebug>

Win32EmbedXrpcPort Win32Port;
extern EmbedXrpcClientObject ClientRpc;
extern EmbedXrpcServerObject ServerRpc;

//-------------------------------------------------------------------------
//client 
uint8_t ClientBuffer[2048];//发送buffer
void ClientSend(void* rpcObj, uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去
{
	ServerRpc.ReceivedMessage(dataLen, data);
}
ResponseDelegateMessageMapCollection rdCollection[1] = { {Inter_ResponseDelegateMessages_Count,Inter_ResponseDelegateMessages} };//client可以处理的service集合

EmbedXrpcClientObject ClientRpc(ClientSend, 500, ClientBuffer, 2048, &Win32Port, 1, rdCollection, nullptr);//client rpc 对象

void DateTimeChangeClientImpl::DateTimeChange(DateTime_t now[1])//server广播后，client接受到的
{
	printf("%d-%d-%d %d:%d:%d\n", now[0].Year, now[0].Month, now[0].Day, now[0].Hour, now[0].Min, now[0].Sec);
}
InterClientImpl Client(&ClientRpc);
void ClientThread()
{
	QThread::msleep(500);
	while (true)
	{
		auto sum=Client.Add(1, 2);
		if (sum.State == ResponseState_Ok)
		{
			printf("sum:%d\n", sum.ReturnValue);
		}
		QThread::msleep(1000);
	}
}
//--------------------------------------------------------------------
//server
uint8_t ServerBuffer[2048];//发送buffer
void ServerSend(void* rpcObj, uint32_t dataLen, uint8_t* data)//client 最终通过这个函数发送出去
{
	ClientRpc.ReceivedMessage(dataLen, data);
}
RequestMessageMapCollection rmCollection[1] = { {Inter_RequestMessages_Count,Inter_RequestMessages} };
EmbedXrpcServerObject ServerRpc(ServerSend, 500, ServerBuffer, 2048, &Win32Port, 1, rmCollection, nullptr);//server rpc 对象
DateTimeChangeDelegate DateTimeChanger(&ServerRpc);
void ServerThread()
{
	QThread::msleep(500);
	while (true)
	{
		QThread::msleep(1000);
		DateTime_t t;
		auto date = QDateTime::currentDateTime().date();
		auto time = QDateTime::currentDateTime().time();
		t.Day = date.day();
		t.Hour = time.hour();
		t.Min = time.minute();
		t.Month = date.month();
		t.Sec = time.second();
		t.Year = date.year();
		DateTimeChanger.Invoke(&t);
	}
}

void Inter_AddService::Add(Int32 a, Int32 b)
{
	Response.ReturnValue = a + b;
}
int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	ClientRpc.Init();
	QThread* c = QThread::create(ClientThread);
	c->start();

	ServerRpc.Init();
	QThread* s = QThread::create(ServerThread);
	s->start();

	return a.exec();
}
/*
运行提示QObject::startTimer: Timers cannot be started from another thread  
是因为QT不允许其他线程操作QT的定时器，你可以临时把Win32EmbedXrpcPort.cpp的
t->Start(interval);、t->Reset();、t->Stop();注释掉  这样就没有这个问题了。

*/