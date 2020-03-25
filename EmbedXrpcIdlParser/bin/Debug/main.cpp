#include <iostream>
#include "StudentService.h"
#include "StudentService.EmbedXrpcSerialization.h"
using namespace std;
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

	cin.get();
	return 0;
}


