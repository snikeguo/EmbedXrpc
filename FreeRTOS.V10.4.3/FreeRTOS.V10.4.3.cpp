// FreeRTOS.V10.4.3.cpp : 定义静态库的函数。
//

// TODO: 这是一个库函数示例
#include "windows.h"
#include "FreeRTOS.h"
#include "task.h"
HANDLE OsInitThread;
DWORD WINAPI InternalInit(LPVOID arg)
{
	uint32_t heapSize = (uint32_t)(arg);
	auto ptr = (uint8_t*)malloc(heapSize);
	HeapRegion_t Heaps[2] =
	{
		{ptr,heapSize},
		{nullptr,0}
	};
	vPortDefineHeapRegions(Heaps);
	vTaskStartScheduler();
	return S_OK;
}
extern "C"  void WINAPI FreeRTOSInit(uint32_t heapSize) 
{
	OsInitThread=CreateThread(NULL, 0, InternalInit, (LPVOID)heapSize, NULL, NULL);
}
