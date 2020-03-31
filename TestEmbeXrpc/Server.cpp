#include "StudentService.Server.h"
#include "Win32EmbedXrpcPort.h"
Win32EmbedXrpcPort ServerWin32Port;
uint8_t ServerBuffer[2048];

#include "StudentService.Client.h"
extern EmbedXrpcClientObject ClientRpcObject;
static void ServerSend(uint32_t sid, uint32_t dataLen, uint8_t* data)
{
	ClientRpcObject.ReceivedMessage(sid, dataLen, data);

}
EmbedXrpcServerObject ServerRpcObject(ServerSend,
	5000,
	ServerBuffer,
	2048,
	&ServerWin32Port,
	sizeof(RequestMessages)/sizeof(RequestMessageMap),
	RequestMessages);

BroadcastDataTimeDelegate BroadcastDataTimeProxy(&ServerRpcObject);

GetStudentInfoFormStudentId_Response GetStudentInfoFormStudentIdService::GetStudentInfoFormStudentId(Byte StudentIdLen, Byte StudentId[100], Int32 arg2, Int32 arg3)
{
	GetStudentInfoFormStudentId_Response x;
	x.ReturnValue.Age = 1;
	return x;
}
GetStudentsInfoFormAge_Response GetStudentsInfoFormAgeService::GetStudentsInfoFormAge()
{
	GetStudentsInfoFormAge_Response x;
	memset(&x, 0x0, sizeof(GetStudentsInfoFormAge_Response));
	x.ReturnValue.StudentIdLen = 1;
	x.ReturnValue.Students[0].Age = 2;
	x.ReturnValue.Students[0].Name =(uint8_t *) "123";
	x.ReturnValue.Students[0].Sex = Woman;
	return x;
}
void TestService::Test()
{
	std::cout << "server:test!" << std::endl;
}
void ServerInit()
{
	ServerRpcObject.Init();
}
#include<chrono>//给演示
#include <thread>
void ServerTest()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));//延时3秒，先让客户端测试
	DateTime_t t;
	t.Day = 2;
	//BroadcastDataTimeProxy.Invoke(t);
}
