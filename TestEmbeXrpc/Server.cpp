#include "StudentService.Server.h"
#include "Win32EmbedXrpcPort.h"

#include<chrono>//给演示
#include <thread>
#include <time.h>
Win32EmbedXrpcPort ServerWin32Port;
uint8_t ServerBuffer[2048];

#include "StudentService.Client.h"
extern EmbedXrpcClientObject ClientRpcObject;
static void ServerSend(uint32_t sid, uint32_t dataLen, uint8_t* data)
{
	ClientRpcObject.ReceivedMessage(sid, dataLen, data);

}
EmbedXrpcServerObject ServerRpcObject(ServerSend,
	1000,
	ServerBuffer,
	2048,
	&ServerWin32Port,
	sizeof(RequestMessages)/sizeof(RequestMessageMap),
	RequestMessages);

BroadcastDataTimeDelegate BroadcastDataTimeProxy(&ServerRpcObject);

void GetStudentInfoFormStudentIdService::GetStudentInfoFormStudentId(Byte StudentIdLen, Byte StudentId[100], Int32 arg2, Int32 arg3)
{
	Response.ReturnValue.Age = 1;
}
void GetStudentsInfoFormAgeService::GetStudentsInfoFormAge()
{
	memset(&Response, 0x0, sizeof(GetStudentsInfoFormAge_Response));
	Response.ReturnValue.StudentIdLen = 1;
	Response.ReturnValue.Students[0].Age = 2;
	Response.ReturnValue.Students[0].Name =(uint8_t *) "123";
	Response.ReturnValue.Students[0].Sex = HHHH;
	for (int i = 0; i < 20; i++)
	{
		Response.ReturnValue.Students[0].StudentId[i] = i + 0x30;
	}
	Response.ReturnValue.Students[0].StudentIdLen = 15;//假定只传输15个
	std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}
void TestService::Test()
{
	std::cout << "server:test!" << std::endl;
}
void ServerInit()
{
	ServerRpcObject.Init();
}

void ServerTest()
{
	
	DateTime_t t;

	while (true)
	{
		auto now_time =time(NULL);
		tm* t_tm = localtime(&now_time);
		t.Day = t_tm->tm_mday;
		t.Hour = t_tm->tm_hour;
		t.Min = t_tm->tm_min;
		t.Month = t_tm->tm_mon + 1;
		t.Sec = t_tm->tm_sec;
		t.Year = t_tm->tm_year+1900;
		BroadcastDataTimeProxy.Invoke(t);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	
}
