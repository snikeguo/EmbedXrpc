// TTLVTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "XrpcCodeGen/StudentService.EmbedXrpcSerialization.h"


unsigned char sendBuf[1024];
Student s, v;
SerializationManager Sm;
void *MyMalloc(size_t size)
{
    void* ptr = malloc(size);
    printf("malloc :0x%x\n", (uint32_t)ptr);
    return ptr;
}
void MyFree(void* ptr)
{
    printf("free :0x%x\n", (uint32_t)ptr);
    free(ptr);
}
static uint8_t TestData[] =
{
    0xB,0x4B,0xAB,0x1,0x7F,0x8A,0x1,0x1,0x2,0xAB,
0x1,0x7F,0xBA,0x1,0x2,0x51,0x3,0x0,0x0,0x0,
0x4,0x0,0x0,0x0,0x7F,0x7F,0x7F,
};
int main()
{
   
    /*SerializationManager TestEncodeSm;
    TestEncodeSm.Buf = sendBuf;
    TestEncodeSm.BufferLen = 1024;
    TestEncodeSm.Reset();
    TestEncodeSm.SerializeKey(0x12345678, TYPE_INT8);
    TestEncodeSm.SerializeLen(0x12345678);
    TestEncodeSm.Reset();
    uint32_t fn = 0;
    Type_t tp = TYPE_UINT8;
    TestEncodeSm.GetKeyFromSerializationManager(&fn, &tp);
    TestEncodeSm.RemoveKeyFromSerializationManager();
    uint64_t len = 0;
    TestEncodeSm.GetArrayLenFromSerializationManager(&len);
    return 0;*/

    Sm.IsEnableMataDataEncode = false;

    s.ChineseAchievement.Arlen = 1;

    s.ChineseAchievement.Ar = (Test*)malloc(sizeof(Test)*2);
    s.ChineseAchievement.Ar[0].IntPtrsLen = 2;
    s.ChineseAchievement.Ar[0].IntPtr =(int *) malloc(sizeof(int)*2);
    s.ChineseAchievement.Ar[0].IntPtr[0] = 0x3;
    s.ChineseAchievement.Ar[0].IntPtr[1] = 0x4;

    Sm.Buf = sendBuf;
    Sm.BufferLen = 1024;
    Sm.Reset();

    Sm.Serialize(&Student_Type,&s,0);
    printf("send:\n");
    for (size_t i = 0; i < Sm.Index; i++)
    {
        printf("0x%.2x,", Sm.Buf[i]);
        if ((i + 1) % 10 == 0)
        {
            printf("\n");
        }
    }
    
    /*Sm.Buf = TestData;
    Sm.BufferLen = sizeof(TestData);*/
    Sm.Reset();
    Sm.Deserialize(&Student_Type, &v,0);
    Sm.Free(&Student_Type, &v);
    getchar();
}

