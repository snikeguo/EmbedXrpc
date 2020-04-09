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
	void ConnectButtonClicked();
private:
	Ui::TestEmbedXrpcForQtClass ui;
	EmbedXrpcServerObject ServerRpcObject;
	Win32EmbedXrpcPort ServerWin32Port;
	uint8_t ServerBuffer[2048];
	BroadcastDataTimeDelegate BroadcastDataTimeProxy;
	static void ServerSend(uint32_t sid, uint32_t dataLen, uint8_t* data);

	QTcpSocket Socket;
};
