#include "StudentService.Client.h"
#include "Win32EmbedXrpcPort.h"
Win32EmbedXrpcPort ClientWin32Port;
uint8_t ClientBuffer[2048];


#include "StudentService.Server.h"
extern EmbedXrpcServerObject ServerRpcObject;

static void ClientSend(uint32_t sid, uint32_t dataLen, uint8_t* data)
{
    ServerRpcObject.ReceivedMessage(sid, dataLen, data);
}
EmbedXrpcClientObject ClientRpcObject(
    ClientSend,
    5000,
    ClientBuffer,
    2048,
    &ClientWin32Port,
    sizeof(ResponseDelegateMessages)/sizeof(ResponseDelegateMessageMap),
    ResponseDelegateMessages);

IMyInterfaceClientImpl Client(&ClientRpcObject);

void BroadcastDataTimeClientImpl::BroadcastDataTime(DateTime_t t)
{
    printf("client %4d-%2d-%2d  %2d:%2d:%2d\n", t.Year, t.Month, t.Day, t.Hour, t.Min, t.Sec);
}

void ClientInit()
{
    ClientRpcObject.Init();
}
#include<chrono>//¸øÑÝÊ¾
#include <thread>
void ClientTest()
{
    while (true)
    {
		Client.Test();
		std::cout << "client:GetStudentsInfoFormAge" << std::endl;
		GetStudentsInfoFormAge_Response x = Client.GetStudentsInfoFormAge();
		//your code
		Client.Free_GetStudentsInfoFormAge(&x);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
   
}