#ifndef StudentService_Server_H
#define StudentService_Server_H
#include"StudentService.h"
#include"EmbedXrpcServerObject.h"
#include"StudentService.EmbedXrpcSerialization.h"
class GetValueService:public IService
{
public:
uint32_t Sid=GetValue_Response_ServiceId;
GetValue_Response GetValue();
void Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
GetValue_Request request;
GetValue_Request_Type.Deserialize(recManager,&request);
GetValue_Response returnValue=GetValue();
GetValue_Request_Type.Free(&request);
GetValue_Response_Type.Serialize(sendManager,0,&returnValue);
}
};
class SetValueService:public IService
{
public:
uint32_t Sid=SetValue_Response_ServiceId;
void SetValue(UInt16 v);
void Invoke(SerializationManager &recManager, SerializationManager& sendManager)
{
SetValue_Request request;
SetValue_Request_Type.Deserialize(recManager,&request);
SetValue(request.v);
SetValue_Request_Type.Free(&request);
}
};

#endif
