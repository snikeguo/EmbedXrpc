#if 1
#include <thread>
#include "Sample1.Client.h"
#include "Sample1.Server.h"
#include "queue_noos_and_win32.h"
extern EmbedXrpcObject ClientRpc;
extern EmbedXrpcObject ServerRpc;
extern void ClientThread();
extern void ServerThread();
extern void Client_Init();
extern void Server_Init();

QueueHandle_t qh;
uint32_t s = 0;
void Task1()
{
	ULONGLONG t = GetTickCount64();
	bool g = true;
	bool show = false;
	int a = 0;
	while (true)
	{
		if (g)
		{
			xQueueGenericSend(qh,&a, queueSEND_TO_BACK);
			s++;
			a++;
		}
		ULONGLONG n = GetTickCount64();
		if ((n - t) > 10000)
		{
			g = false;
			if (show == false)
			{
				printf("s:%d\n", s);
				show = true;
			}
		}
	}
}
uint32_t r = 0;
static void Task2()
{
	ULONGLONG t = GetTickCount64();
	bool show = false;
	int a = 0;
	while (1)
	{
		if (xQueueReceive(qh, &a, 10)==pdPASS)
		{
			r++;
		}
		ULONGLONG n = GetTickCount64();
		if ((n - t) > 15000)
		{
			if (show == false)
			{
				printf("r:%d,a:%d,queue:%d\n", r,a,uxQueueMessagesWaiting(qh));
				show = true;
			}
		}
	}
}



int main(int argc, char *argv[])
{
	if(0)
	{
		//Sleep(5000);
		qh = xQueueCreate(10000000, sizeof(int));
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		std::thread t1 = std::thread(Task1);
		t1.detach();
		std::thread t2 = std::thread(Task2);
		t2.detach();

		std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
	}

	Client_Init();
	std::thread c = std::thread(ClientThread);
	c.detach();

	Server_Init();
	std::thread s = std::thread(ServerThread);
	s.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(0xffffffff));
}

/*
运行提示QObject::startTimer: Timers cannot be started from another thread  
是因为QT不允许其他线程操作QT的定时器，你可以临时把Win32EmbedXrpcPort.cpp的
t->Start(interval);、t->Reset();、t->Stop();注释掉  这样就没有这个问题了。
*/

#else

template<class T>
class IClass
{
public:
	virtual T Test()=0;
};
class MyClass :public IClass<int>
{
public:
	int Test()
	{
		return 5;
	}
};
int main()
{
	MyClass Tester;
	auto v = Tester.Test();

	IClass<int>* base = &Tester;
	auto v2=base->Test();

}
#endif
