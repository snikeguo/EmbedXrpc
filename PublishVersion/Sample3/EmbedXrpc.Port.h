#ifndef EmbedXrpc_Port_H
#define EmbedXrpc_Port_H

#ifdef __cplusplus
extern "C" {
#endif
	struct DTL
	{
		char SourceAddress;
		char DestAddress;
	};
	typedef struct DTL UserDataOfTransportLayer_t;

#define EmbedXrpc_NameMaxLen	32

#ifdef __cplusplus
}
#endif


#endif