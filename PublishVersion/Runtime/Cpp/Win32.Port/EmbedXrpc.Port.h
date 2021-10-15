#ifndef EmbedXrpc_Port_H
#define EmbedXrpc_Port_H

#ifdef __cplusplus
extern "C" {
#endif
	struct Win32UserDataOfTransportLayerTest
	{
		char IpAddress[16];
		int Port;
	};
	typedef struct Win32UserDataOfTransportLayerTest UserDataOfTransportLayer_t;


	//common
//#define EmbedXrpc_SendBufferSize			1024//����Buffer��С
#define EmbedXrpc_CheckSumValid				1
#define EmbedXrpc_IsSendToQueue				1


//client����:RingBuffer ģʽ�� and Dynamic Memory �� ����Ҫ���õ�
#define EmbedXrpc_DelegateBlockQueue_MaxItemNumber	20
#define EmbedXrpc_ResponseBlockQueue_MaxItemNumber	20

//server����:RingBuffer ģʽ�� and Dynamic Memory �� ����Ҫ���õ�
#define EmbedXrpc_RequestBlockQueue_MaxItemNumber	50

#define Server_ThreadPriority				0x6
#define Client_ThreadPriority				0x6


#define EmbedXrpc_UseRingBufferWhenReceiving 0

#if EmbedXrpc_UseRingBufferWhenReceiving==1
//client
#define EmbedXrpc_DelegateRingBufferSize	1024//���յ�ί�����ݺ�Ҫ�����ݲ��뵽DelegateRingBuffer
#define EmbedXrpc_ResponseRingBufferSize	1024//���յ��ظ����ݺ�Ҫ�����ݲ��뵽ResponseRingBuffer

//server
#define EmbedXrpc_RequestRingBufferSize		1024//���յ��������ݺ�Ҫ�����ݲ��뵽RequestRingBuffer

#endif


#ifdef __cplusplus
}
#endif


#endif