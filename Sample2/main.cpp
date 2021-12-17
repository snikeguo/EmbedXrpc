#if 1
#include <thread>
#include "Sample2.Client.h"
#include "Sample2.Server.h"
#include "DTL.h"
extern EmbedXrpcObject<AB_DTL> AClient_RpcObject;
extern EmbedXrpcObject<AB_DTL> BServer_RpcObject;
extern EmbedXrpcObject<BC_DTL> BClient_RpcObject;
extern EmbedXrpcObject<BC_DTL> CServer_RpcObject;
extern void ClientThread();
extern void ServerThread();
int main(int argc, char *argv[])
{

	AClient_RpcObject.Init();
	BServer_RpcObject.Init();
	BClient_RpcObject.Init();
	CServer_RpcObject.Init();

	std::thread c = std::thread(ClientThread);
	c.detach();

	

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
