#ifndef Sample1_Server_H
#define Sample1_Server_H
#include"EmbedXrpcObject.h"
#include"Sample1.EmbedXrpcSerialization.h"
class DateTimeChange_Service:public IService
{
public:
uint16_t GetSid(){return DateTimeChange_ServiceId;}
virtual void DateTimeChange(ServiceInvokeParameter * serviceInvokeParameter,DateTime_t now[1])=0;
DateTimeChange_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class Test2_Service:public IService
{
public:
uint16_t GetSid(){return Test2_ServiceId;}
virtual void Test2(ServiceInvokeParameter * serviceInvokeParameter,DateTime_t now[1])=0;
Test2_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class Add_Service:public IService
{
public:
uint16_t GetSid(){return Add_ServiceId;}
Add_Return Response;
virtual void Add(ServiceInvokeParameter * serviceInvokeParameter,int32_t a,int32_t b,int32_t dataLen,uint8_t* data,TestSerialize test[1])=0;
Add_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class NoArg_Service:public IService
{
public:
uint16_t GetSid(){return NoArg_ServiceId;}
NoArg_Return Response;
virtual void NoArg(ServiceInvokeParameter * serviceInvokeParameter)=0;
NoArg_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class NoReturn_Service:public IService
{
public:
uint16_t GetSid(){return NoReturn_ServiceId;}
virtual void NoReturn(ServiceInvokeParameter * serviceInvokeParameter,int32_t a)=0;
NoReturn_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};


class NoArgAndReturn_Service:public IService
{
public:
uint16_t GetSid(){return NoArgAndReturn_ServiceId;}
virtual void NoArgAndReturn(ServiceInvokeParameter * serviceInvokeParameter)=0;
NoArgAndReturn_Parameter request;
void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);
};



#endif
