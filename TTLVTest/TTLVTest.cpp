// TTLVTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "XrpcCodeGen/StudentService.EmbedXrpcSerialization.h"


unsigned char sendBuf[1024];
Student s, v;
void *MyMalloc(size_t size)
{
    void* ptr = malloc(size);
    printf("malloc :0x%x\n", ptr);
    return ptr;
}
void MyFree(void* ptr)
{
    printf("free :0x%x\n", ptr);
    free(ptr);
}
int main()
{
   
    SerializationManager TestEncodeSm;
    TestEncodeSm.Buf = sendBuf;
    TestEncodeSm.BufferLen = 1024;
    TestEncodeSm.Reset();
    TestEncodeSm.SerializeKey(0x123456, TYPE_INT8);
    TestEncodeSm.Reset();
    uint32_t fn = 0;
    Type_t tp = TYPE_UINT8;
    TestEncodeSm.GetKeyFromSerializationManager(&fn, &tp);
    return 0;
    s.ChineseAchievement.Arlen = 3;

    s.ChineseAchievement.Ar = (Test*)malloc(sizeof(Test)*2);
    s.ChineseAchievement.Ar[0].IntPtrsLen = 10;
    s.ChineseAchievement.Ar[0].IntPtr =(int *) malloc(sizeof(int)*10);
    for (size_t i = 0; i < 10; i++)
    {
        s.ChineseAchievement.Ar[0].IntPtr[i] = 0x1A2B3C4D;
    }

    s.ChineseAchievement.Ar[1].IntPtrsLen = 7;
    s.ChineseAchievement.Ar[1].IntPtr = (int*)malloc(sizeof(int)*7);
    for (size_t i = 0; i < 7; i++)
    {
        s.ChineseAchievement.Ar[1].IntPtr[i] = 0x5A5A5A5A;
    }

    s.ChineseAchievement.Ar[2].IntPtrsLen = 7;
    s.ChineseAchievement.Ar[2].IntPtr = (int*)malloc(sizeof(int) * 7);
    for (size_t i = 0; i < 7; i++)
    {
        s.ChineseAchievement.Ar[2].IntPtr[i] = 0xBCBCBCBC;
    }
    SerializationManager Sm;
    

    Sm.Buf = sendBuf;
    Sm.BufferLen = 1024;
    Sm.Reset();

    Sm.Serialize(&Student_Type,&s,0);
    /*printf("send:\n");
    for (size_t i = 0; i < Sm.Index; i++)
    {
        printf("%d,", Sm.Buf[i]);
        if ((i + 1) % 10 == 0)
        {
            printf("\n");
        }
    }*/

    Sm.BufferLen = Sm.Index;
    Sm.Reset();
    Sm.Deserialize(&Student_Type, &v,0);
    Sm.Free(&Student_Type, &v);
    getchar();
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
