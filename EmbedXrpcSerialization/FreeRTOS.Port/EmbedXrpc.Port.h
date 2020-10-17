#ifndef EmbedXrpc_Port_H
#define EmbedXrpc_Port_H

typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Mutex_t;
typedef void* EmbedXrpc_Thread_t;
typedef void* EmbedXrpc_Queue_t;
typedef void* EmbedXrpc_Semaphore_t;
typedef void* EmbedXrpc_Timer_t;
#define EmbedXrpc_WAIT_FOREVER	0xFFFFFFFF


#define Server_ThreadPriority				0x3
#define Server_RequestQueue_MaxItemNumber	20

#define Client_ThreadPriority				0x3
#define Client_DelegateMessageQueue_MaxItemNumber	20
#define Client_ResponseMessageQueue_MaxItemNumber	20


#endif