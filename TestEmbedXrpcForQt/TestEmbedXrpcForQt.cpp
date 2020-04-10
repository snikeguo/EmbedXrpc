#include "TestEmbedXrpcForQt.h"
#include <QPushButton>
#include <QDateTime>
static TestEmbedXrpcForQt* GuiInstance;
TestEmbedXrpcForQt::TestEmbedXrpcForQt(QWidget* parent) : QMainWindow(parent), ServerRpcObject(ServerSend,
	1000,
	ServerBuffer,
	2048,
	&ServerWin32Port,
	sizeof(RequestMessages) / sizeof(RequestMessageMap),
	RequestMessages),
	BroadcastDataTimeProxy(&ServerRpcObject),
	Socket(this)
{
	ui.setupUi(this);
	connect(&Socket, &QTcpSocket::readyRead, this, &TestEmbedXrpcForQt::readyRead);
	connect(&Socket, &QTcpSocket::connected, this, &TestEmbedXrpcForQt::connected);
	ServerRpcObject.Init();
	GuiInstance = this;
}


void  TestEmbedXrpcForQt::readyRead()
{
	qint64 len=Socket.read(SocketBuffer, 10240);
	if (len < 6)
		return;
	if ((SocketBuffer[0] == (char)0xff) && (SocketBuffer[1] ==(char) 0xff))
	{
		uint32_t sid = SocketBuffer[2] | SocketBuffer[3] << 8 | SocketBuffer[4] << 16 | SocketBuffer[5] << 24;
		ServerRpcObject.ReceivedMessage(sid, len - 6, (uint8_t *)&SocketBuffer[6]);
	}
}
void TestEmbedXrpcForQt::connected()
{
	QThread* b = QThread::create([this] {while (true) {
		DateTime_t t;
		auto date = QDateTime::currentDateTime().date();
		auto time = QDateTime::currentDateTime().time();
		t.Day = date.day();
		t.Hour = time.hour();
		t.Min = time.minute();
		t.Month = date.month();
		t.Sec = time.second();
		t.Year = date.year();
		this->BroadcastDataTimeProxy.Invoke(t);
		QThread::msleep(1000);
	}});
	b->start();
}
void TestEmbedXrpcForQt::ServerSend(uint32_t sid, uint32_t dataLen, uint8_t* data)
{
	//ClientRpcObject.ReceivedMessage(sid, dataLen, data);
	QByteArray b;
	b.append(0xff);
	b.append(0xff);
	b.append(sid&0xff);
	b.append(sid>>8 & 0xff);
	b.append(sid>>16 & 0xff);
	b.append(sid>>24 & 0xff);
	b.append((char *)data, dataLen);
	GuiInstance->Socket.write(b);
	GuiInstance->Socket.waitForBytesWritten(-1);
}



void GetStudentInfoFormStudentIdService::GetStudentInfoFormStudentId(Byte StudentIdLen, Byte StudentId[100], Int32 arg2, Int32 arg3)
{
	Response.ReturnValue.Age = 1;
}
void GetStudentsInfoFormAgeService::GetStudentsInfoFormAge()
{
	memset(&Response, 0x0, sizeof(GetStudentsInfoFormAge_Response));
	Response.ReturnValue.StudentIdLen = 1;
	Response.ReturnValue.Students[0].Age = 2;
	Response.ReturnValue.Students[0].Name = (uint8_t*)"123";
	Response.ReturnValue.Students[0].Sex = HHHH;
	for (int i = 0; i < 20; i++)
	{
		Response.ReturnValue.Students[0].StudentId[i] = i + 0x30;
	}
	Response.ReturnValue.Students[0].StudentIdLen = 10;//�ٶ�ֻ����15��
	//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}
void TestService::Test()
{
	std::cout << "server:test!" << std::endl;
}
#if 0
void ServerInit()
{
	ServerRpcObject.Init();
}

void ServerTest()
{

	DateTime_t t;

	while (true)
	{
		auto now_time = time(NULL);
		tm* t_tm = localtime(&now_time);
		t.Day = t_tm->tm_mday;
		t.Hour = t_tm->tm_hour;
		t.Min = t_tm->tm_min;
		t.Month = t_tm->tm_mon + 1;
		t.Sec = t_tm->tm_sec;
		t.Year = t_tm->tm_year + 1900;
		BroadcastDataTimeProxy.Invoke(t);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

}
#endif
void TestEmbedXrpcForQt::on_ConnectButton_clicked()
{
	Socket.connectToHost("127.0.0.1", 5567);
}
