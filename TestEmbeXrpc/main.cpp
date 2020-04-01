#include <iostream>
#include "StudentService.Client.h"
#include "StudentService.Server.h"
#include "Win32EmbedXrpcPort.h"
using namespace std;



#if 0
int main()
{
	SerializationManager Manager;
	uint8_t buf[1024];
	Manager.Buf = buf;
	Manager.BufferLen = 1024;
	Manager.Reset();
	DateTime_t sDataTime, rDataTime;
	sDataTime.Year = 1;
	sDataTime.Month = 2;
	sDataTime.Day = 3;
	sDataTime.Hour = 4;
	sDataTime.Min = 5;
	sDataTime.Sec = 6;
	DateTime_t_Type.Serialize(Manager,0, &sDataTime);
	Manager.Reset();
	//假定接受到数据了:
	DateTime_t_Type.Deserialize(Manager, &rDataTime);
	DateTime_t_Type.Free(&rDataTime);

	Manager.Reset();
	Student_t sStu,rStu;
	sStu.ResultsLen = 1;
	sStu.Results[0].Value = 2;
	sStu.Results[0].NameLen = 3;
	sStu.Results[0].Name[0] = 4;
	sStu.Results[0].Name[1] = 5;
	sStu.Results[0].Name[2] = 6;
	sStu.Results[0].Name[3] = 7;
	sStu.Results[0].Arg1 = 8;
	sStu.Age = 9;
	uint8_t nameBuf[3] = {0xa,0xb,0xc};
	sStu.Name = nameBuf;
	sStu.StudentIdLen = 0xd;
	sStu.StudentId[0] = 0xe;
	sStu.StudentId[1] = 0xf;
	sStu.Sex = Woman;
	Student_t_Type.Serialize(Manager, 0, &sStu);

	Manager.Reset();
	Student_t_Type.Deserialize(Manager, &rStu);
	Student_t_Type.Free(&rStu);
	//test rpc

	Client = new EmbedXrpcClientObject(5000, ClientBuf, 1024, &Win32Port, 0, nullptr, 0, nullptr);

	Client->Init();

	IMyInterfaceClientImpl interfaceImpl(Client);

	auto response = interfaceImpl.GetStudentInfoFormStudentId(0, nullptr, 1, 2);
	//your code...
	interfaceImpl.Free_GetStudentInfoFormStudentId(&response);

	return 0;
}


#else

#include "BlockingQueue.h"

extern void ServerInit();
extern void ServerTest();
extern void ClientInit();
extern void ClientTest();
int main()
{
	ServerInit();
	ClientInit();
	std::thread clientThread([] {ClientTest(); });
	std::thread ServerThread([] {ServerTest(); });
	
	cin.get();
}
#endif
