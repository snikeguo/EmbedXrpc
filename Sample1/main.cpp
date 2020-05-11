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
uint8_t ClientBuffer[2048];//����buffer
void ClientSend(void* rpcObj, uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	ServerRpc.ReceivedMessage(dataLen, data);
}
ResponseDelegateMessageMapCollection rdCollection[1] = { {Inter_ResponseDelegateMessages_Count,Inter_ResponseDelegateMessages} };//client���Դ����service����

EmbedXrpcClientObject ClientRpc(ClientSend, 500, ClientBuffer, 2048, &Win32Port, 1, rdCollection, nullptr);//client rpc ����

void DateTimeChangeClientImpl::DateTimeChange(DateTime_t now[1])//server�㲥��client���ܵ���
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
uint8_t ServerBuffer[2048];//����buffer
void ServerSend(void* rpcObj, uint32_t dataLen, uint8_t* data)//client ����ͨ������������ͳ�ȥ
{
	ClientRpc.ReceivedMessage(dataLen, data);
}
RequestMessageMapCollection rmCollection[1] = { {Inter_RequestMessages_Count,Inter_RequestMessages} };
EmbedXrpcServerObject ServerRpc(ServerSend, 500, ServerBuffer, 2048, &Win32Port, 1, rmCollection, nullptr);//server rpc ����
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
������ʾQObject::startTimer: Timers cannot be started from another thread  
����ΪQT�����������̲߳���QT�Ķ�ʱ�����������ʱ��Win32EmbedXrpcPort.cpp��
t->Start(interval);��t->Reset();��t->Stop();ע�͵�  ������û����������ˡ�

*/