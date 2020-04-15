#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TestEmbedXrpcForQt.h"
#include <QTcpSocket>
#include "StudentService.Server.h"
#include "Win32EmbedXrpcPort.h"
class TestEmbedXrpcForQt : public QMainWindow
{
	Q_OBJECT

public:
	TestEmbedXrpcForQt(QWidget *parent = Q_NULLPTR);
	void readyRead();
	void connected();
	QTcpSocket Socket;
private slots:
    void on_ConnectButton_clicked();

private:
	Ui::TestEmbedXrpcForQtClass ui;
	EmbedXrpcServerObject ServerRpcObject;
	Win32EmbedXrpcPort ServerWin32Port;
	uint8_t ServerBuffer[2048];
	BroadcastDataTimeDelegate BroadcastDataTimeProxy;
	static void ServerSend(void* rpcObj, uint32_t dataLen, uint8_t* data);
	char SocketBuffer[10240];
};
