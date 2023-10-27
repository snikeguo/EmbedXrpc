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
#define EmbedXrpc_NameMaxLen	32
#define EmbedXrpc_UsingOs	1
#ifdef __cplusplus
}
#endif


#endif