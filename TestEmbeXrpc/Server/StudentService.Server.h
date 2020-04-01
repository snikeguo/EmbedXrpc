#ifndef StudentService_Server_H
#define StudentService_Server_H
#include"StudentService.h"
#include"EmbedXrpcServerObject.h"
#include"StudentService.EmbedXrpcSerialization.h"
class BroadcastDataTimeDelegate
{
public:
	EmbedXrpcServerObject* RpcServerObject = nullptr;
	BroadcastDataTimeDelegate(EmbedXrpcServerObject* rpcobj)
	{
		this->RpcServerObject = rpcobj;
	}
	uint32_t GetSid() { return BroadcastDataTime_ServiceId; }
	void  Invoke(DateTime_t t)
	{
		//write serialization code:BroadcastDataTime(t,)
		static BroadcastDataTimeStruct sendData;
		RpcServerObject->porter->TakeMutex(RpcServerObject->BufMutexHandle, 100);
		SerializationManager sm;
		sm.Reset();
		sm.Buf = RpcServerObject->Buffer;
		sm.BufferLen = RpcServerObject->BufferLen;
		memcpy(&sendData.t, &t, sizeof(sendData.t));
		BroadcastDataTimeStruct_Type.Serialize(sm, 0, &sendData);
		RpcServerObject->Send(BroadcastDataTime_ServiceId, sm.Index, sm.Buf);
		sm.Reset();
		RpcServerObject->porter->ReleaseMutex(RpcServerObject->BufMutexHandle);
	}
};
class GetStudentInfoFormStudentIdService :public IService
{
public:
	uint32_t GetSid() { return GetStudentInfoFormStudentId_ServiceId; }
	GetStudentInfoFormStudentId_Response Response;
	void GetStudentInfoFormStudentId(Byte StudentIdLen, Byte StudentId[100], Int32 arg2, Int32 arg3);
	void Invoke(SerializationManager& recManager, SerializationManager& sendManager)
	{
		static GetStudentInfoFormStudentId_Request request;
		GetStudentInfoFormStudentId_Request_Type.Deserialize(recManager, &request);
		GetStudentInfoFormStudentId(request.StudentIdLen, request.StudentId, request.arg2, request.arg3);
		GetStudentInfoFormStudentId_Request_Type.Free(&request);
		GetStudentInfoFormStudentId_Response_Type.Serialize(sendManager, 0, &Response);
		GetStudentInfoFormStudentId_Response_Type.Free(&Response);
	}
};
class GetStudentsInfoFormAgeService :public IService
{
public:
	uint32_t GetSid() { return GetStudentsInfoFormAge_ServiceId; }
	GetStudentsInfoFormAge_Response Response;
	void GetStudentsInfoFormAge();
	void Invoke(SerializationManager& recManager, SerializationManager& sendManager)
	{
		static GetStudentsInfoFormAge_Request request;
		GetStudentsInfoFormAge_Request_Type.Deserialize(recManager, &request);
		GetStudentsInfoFormAge();
		GetStudentsInfoFormAge_Request_Type.Free(&request);
		GetStudentsInfoFormAge_Response_Type.Serialize(sendManager, 0, &Response);
		GetStudentsInfoFormAge_Response_Type.Free(&Response);
	}
};
class TestService :public IService
{
public:
	uint32_t GetSid() { return Test_ServiceId; }
	void Test();
	void Invoke(SerializationManager& recManager, SerializationManager& sendManager)
	{
		static Test_Request request;
		Test_Request_Type.Deserialize(recManager, &request);
		Test();
		Test_Request_Type.Free(&request);
	}
};
extern RequestMessageMap RequestMessages[3];

#endif
