#ifndef EmbedXrpc_Port_H
#define EmbedXrpc_Port_H


typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Mutex_t;
typedef void* EmbedXrpc_Thread_t;
typedef void* EmbedXrpc_Queue_t;
typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Timer_t;
#define EmbedXrpc_WAIT_FOREVER	0xFFFFFFFF

//common
#define EmbedXrpc_SendBufferSize			128//发送Buffer大小

//client部分:RingBuffer 模式下 and Dynamic Memory 下 都需要配置的
#define EmbedXrpc_DelegateBlockQueue_MaxItemNumber	0
#define EmbedXrpc_ResponseBlockQueue_MaxItemNumber	0

//server部分:RingBuffer 模式下 and Dynamic Memory 下 都需要配置的
#define EmbedXrpc_RequestBlockQueue_MaxItemNumber	100

#define Server_ThreadPriority				0x2
#define Client_ThreadPriority				0x6


#define EmbedXrpc_UseRingBufferWhenReceiving 0

#if EmbedXrpc_UseRingBufferWhenReceiving==1
//client
#define EmbedXrpc_DelegateRingBufferSize	1024//接收到委托数据后要把数据插入到DelegateRingBuffer
#define EmbedXrpc_ResponseRingBufferSize	1024//接收到回复数据后要把数据插入到ResponseRingBuffer

//server
#define EmbedXrpc_RequestRingBufferSize		1024//接收到请求数据后要把数据插入到RequestRingBuffer

#endif




#endif