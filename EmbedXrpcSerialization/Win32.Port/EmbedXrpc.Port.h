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
#define EmbedXrpc_SendBufferSize			1024//����Buffer��С

//client����:RingBuffer ģʽ�� and Dynamic Memory �� ����Ҫ���õ�
#define EmbedXrpc_DelegateBlockQueue_MaxItemNumber	20
#define EmbedXrpc_ResponseBlockQueue_MaxItemNumber	20

//server����:RingBuffer ģʽ�� and Dynamic Memory �� ����Ҫ���õ�
#define EmbedXrpc_RequestBlockQueue_MaxItemNumber	50

#define Server_ThreadPriority				0x6
#define Client_ThreadPriority				0x6


#define EmbedXrpc_UseRingBufferWhenReceiving 1

#if EmbedXrpc_UseRingBufferWhenReceiving==1
//client
#define EmbedXrpc_DelegateRingBufferSize	1024//���յ�ί�����ݺ�Ҫ�����ݲ��뵽DelegateRingBuffer
#define EmbedXrpc_ResponseRingBufferSize	1024//���յ��ظ����ݺ�Ҫ�����ݲ��뵽ResponseRingBuffer

//server
#define EmbedXrpc_RequestRingBufferSize		1024//���յ��������ݺ�Ҫ�����ݲ��뵽RequestRingBuffer

#endif





#endif