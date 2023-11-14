#ifndef Sample1_Client_H
#define Sample1_Client_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChange_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
DateTimeChange_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return DateTimeChange_ServiceId;}
DateTimeChange_Parameter DateTimeChange_SendData;
DateTimeChange_Return DateTimeChange_reqresp;
DateTimeChange_Return& DateTimeChange(RequestParameter* rp,DateTime_t now[1]);
//---------No Os-----------
uint32_t RequestTick;
//NoOs_DateTimeChange接口只能放到while1执行,不能放到中断里面跑
DateTimeChange_Return& NoOs_DateTimeChange(RequestParameter* rp,DateTime_t now[1]);
};

class Test2_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
Test2_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return Test2_ServiceId;}
Test2_Parameter Test2_SendData;
Test2_Return Test2_reqresp;
Test2_Return& Test2(RequestParameter* rp);//---------No Os-----------
uint32_t RequestTick;
//NoOs_Test2接口只能放到while1执行,不能放到中断里面跑
Test2_Return& NoOs_Test2(RequestParameter* rp);};

class Add_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
Add_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return Add_ServiceId;}
Add_Parameter Add_SendData;
Add_Return Add_reqresp;
Add_Return& Add(RequestParameter* rp);void Free_Add(Add_Return *response);

//---------No Os-----------
uint32_t RequestTick;
//NoOs_QueryServiceState接口只能放到while1执行,不能放到中断里面跑
Add_Return& NoOs_QueryServiceState(RequestParameter* rp);
//NoOs_Add接口只能放到while1执行,不能放到中断里面跑
Add_Return& NoOs_Add(RequestParameter* rp);};

class NoArg_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
NoArg_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return NoArg_ServiceId;}
NoArg_Parameter NoArg_SendData;
NoArg_Return NoArg_reqresp;
NoArg_Return& NoArg(RequestParameter* rp);
void Free_NoArg(NoArg_Return *response);

//---------No Os-----------
uint32_t RequestTick;
//NoOs_QueryServiceState接口只能放到while1执行,不能放到中断里面跑
NoArg_Return& NoOs_QueryServiceState(RequestParameter* rp);
//NoOs_NoArg接口只能放到while1执行,不能放到中断里面跑
NoArg_Return& NoOs_NoArg(RequestParameter* rp);
};

class NoReturn_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
NoReturn_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return NoReturn_ServiceId;}
NoReturn_Parameter NoReturn_SendData;
NoReturn_Return NoReturn_reqresp;
NoReturn_Return& NoReturn(RequestParameter* rp,int32_t a);
//---------No Os-----------
uint32_t RequestTick;
//NoOs_NoReturn接口只能放到while1执行,不能放到中断里面跑
NoReturn_Return& NoOs_NoReturn(RequestParameter* rp,int32_t a);
};

class NoArgAndReturn_Requester
{
public:
EmbedXrpcObject *RpcObject=nullptr;
NoArgAndReturn_Requester(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)
{}
uint16_t GetSid(){return NoArgAndReturn_ServiceId;}
NoArgAndReturn_Parameter NoArgAndReturn_SendData;
NoArgAndReturn_Return NoArgAndReturn_reqresp;
NoArgAndReturn_Return& NoArgAndReturn(RequestParameter* rp);
//---------No Os-----------
uint32_t RequestTick;
//NoOs_NoArgAndReturn接口只能放到while1执行,不能放到中断里面跑
NoArgAndReturn_Return& NoOs_NoArgAndReturn(RequestParameter* rp);
};


#endif
