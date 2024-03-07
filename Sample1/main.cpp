#if 1
#include <thread>
#include "Sample1.Client.h"
#include "Sample1.Server.h"
#include "EmbedLibrary.h"
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
				break;
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
		if (xQueueReceive(qh, &a, 100)==pdPASS)
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
				break;
			}
		}
	}
}

MessageBufferHandle_t mbh;
uint32_t mbs = 0;
uint32_t mbs_count = 0;
static void mbTask1()
{
	ULONGLONG t = GetTickCount64();
	bool g = true;
	bool show = false;
	static uint8_t txData[4096];
	srand(t);
	while (1)
	{
		if (g)
		{
			uint32_t cnt = rand() % 4096;
			if (cnt == 0)
				continue;
			xMessageBufferSend(mbh, txData, cnt, 0);
			mbs++;
			mbs_count += cnt;
		}
		ULONGLONG n = GetTickCount64();
		if ((n - t) > 10000)
		{
			g = false;
			if (show == false)
			{
				printf("mbs:%d,mbs_count:%u\n", mbs, mbs_count);
				show = true;
				break;
			}
		}
	}
}
uint32_t mbr = 0;
uint32_t mbr_count = 0;
static void mbTask2()
{
	ULONGLONG t = GetTickCount64();
	bool show = false;
	static uint8_t rxData[4096];
	while (1)
	{
		size_t size=xMessageBufferReceive(mbh, rxData, sizeof(rxData), 100);
		if (size > 0)
		{
			mbr++;
			mbr_count += size;
		}
		
		ULONGLONG n = GetTickCount64();
		if ((n - t) > 15000)
		{
			if (show == false)
			{
				printf("mbr:%d,mbr_count:%u\n", mbr, mbr_count);
				show = true;
				break;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	if(1)
	{
		Sleep(5000);
		qh = xQueueCreate(10000000, sizeof(int));
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		std::thread t1 = std::thread(Task1);
		t1.detach();
		std::thread t2 = std::thread(Task2);
		t2.detach();

		std::this_thread::sleep_for(std::chrono::milliseconds(20000));
		vQueueDelete(qh);
	}

	if (1)
	{
		Sleep(5000);
		mbh = xMessageBufferCreate(4096000);
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		std::thread t1 = std::thread(mbTask1);
		t1.detach();
		std::thread t2 = std::thread(mbTask2);
		t2.detach();

		std::this_thread::sleep_for(std::chrono::milliseconds(20000));
		vMessageBufferDelete(mbh);
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
