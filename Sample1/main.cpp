#if 1
#include <thread>
#include "Sample1.Client.h"
#include "Sample1.Server.h"
extern EmbedXrpcObject ClientRpc;
extern EmbedXrpcObject ServerRpc;
extern void ClientThread();
extern void ServerThread();
extern void Client_Init();
extern void Server_Init();
int main(int argc, char *argv[])
{

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
