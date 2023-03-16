#include <stdio.h>
#include <Sample3.EmbedXrpcSerialization.h>
int main(int argc, char *argv[])
{
	Student stu;

	stu.a = 1;
	stu.b = 2;
	stu.c = 3;
	stu.d = 4;
	stu.e = 0xABCD1234;

	uint8_t buf[32];
	SerializationManager sm;
	memset(&sm, 0, sizeof(sm));

	sm.Buf = buf;
	sm.BufferLen = 32;
	Student_Serialize(&sm, &stu);

	printf("student 序列化后的数据是:\n");
	for (size_t i = 0; i < sm.Index; i++)
	{
		printf("%x,", sm.Buf[i]);
		if ((i + 1) % 10 == 0)
		{
			printf("\n");
		}
	}
	printf("\n");
	Student dest;
	sm.Index = 0;
	Student_Deserialize(&sm, &dest,0);
	printf("反序列化化后:\n");
	printf("a:0x%x\n", dest.a);
	printf("b:0x%x\n", dest.b);
	printf("c:0x%x\n", dest.c);
	printf("d:0x%x\n", dest.d);
	printf("e:0x%x\n", dest.e);
	getchar();
}


