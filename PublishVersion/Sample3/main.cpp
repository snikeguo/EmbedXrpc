#include <stdio.h>
#include <Sample3.EmbedXrpcSerialization.h>
#include "float.h"
int main(int argc, char *argv[])
{
	Student stu;

	stu.u8 = 0xff;
	stu.s8 = 0xff;
	stu.u16 = 0xffff;
	stu.i16 = 0xffff;
	stu.u32 = UINT32_MAX;
	stu.i32 = UINT32_MAX;
	stu.u64 = UINT64_MAX;
	stu.i64 = UINT64_MAX;
	stu.f = FLT_MAX;
	stu.d = DBL_MAX;

	uint8_t buf[128];
	SerializationManager sm;
	memset(&sm, 0, sizeof(sm));

	sm.Buf = buf;
	sm.BufferLen = sizeof(buf);
	Student_Serialize(&sm, &stu);

	printf("student 序列化后的数据是:\n");
	for (size_t i = 0; i < sm.Index; i++)
	{
		printf("0x%x,", sm.Buf[i]);
		if ((i + 1) % 10 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
	Student dest;
	sm.Index = 0;
	Student_Deserialize(&sm, &dest, 0);
	printf("反序列化化后:\n");
	printf("a:0x%d\n", dest.u8);
	printf("b:0x%d\n", dest.s8);
	printf("c:0x%d\n", dest.u16);
	printf("d:0x%d\n", dest.i16);
	printf("e:0x%d\n", dest.u32);
	printf("e:0x%d\n", dest.i32);
	printf("e:0x%d\n", dest.u64);
	printf("e:0x%d\n", dest.i64);
	printf("e:0x%f\n", dest.f);
	printf("e:0x%lf\n", dest.d);
	getchar();
}


