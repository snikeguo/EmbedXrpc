#ifndef EmbedSerialization_H
#define EmbedSerialization_H


#include "EmbedSerialization.Port.h"
#include "BlockBufferProvider.h"


#ifndef offsetof
#define offsetof(s, m) (uint32_t)((char*)(&((s*)0)->m))
#endif

void EmbedSerializationShowMessage(const char* filter_string, const char* fmt, ...);
uint32_t GetSum(uint8_t* d, uint32_t len);
typedef  uint32_t ptr_t;
class SerializationManager;

typedef enum {
	TYPE_UINT8,
	TYPE_INT8,

	TYPE_UINT16,
	TYPE_INT16,

	TYPE_UINT32,
	TYPE_INT32,

	TYPE_UINT64,
	TYPE_INT64,

	TYPE_FLOAT,
	TYPE_DOUBLE,

	TYPE_ARRAY,   /*array*/
	TYPE_OBJECT,  /*object*/


} Type_t;
extern const char* TypeString[];
struct IType
{
	//const char* (*const GetTypeName)();
	//const Type_t(*const GetType)();
	Type_t Type;
};
struct IField //:public IType
{
	//const uint32_t(* const GetOffset)(const IField* const a);
	//const char* (*const GetName)(const IField* const a);

	//const bool (*const IsArrayLenField)(const IField* const a);
	//const IField* (*const GetArrayLenField)(const IField* const a);
	//const bool (*const IsFixed)(const IField* const a);
	//const uint32_t(*const GetFieldNumber)(const IField* const a);

	//type 部分
	//const char* (*const GetTypeName)(const IField* const a);
	//const IType* (*const GetTypeInstance)(const IField* const  a);
	Type_t Type;
};
struct ArrayType
{
	const IType BaseClass;
	const IType* ElementTypeInstance;
	const uint32_t LengthOfSingleElement;
};
struct ArrayField
{
	const IField BaseClass;
	const char* Name;
	const uint32_t Offset;
	const IField* ArrayLenField;
	const ArrayType *TypeInstance;
	const bool _IsFixed;
	const uint32_t FieldNumber;
};
const char* ArrayType_GetTypeName();
const Type_t ArrayType_GetType();
const uint32_t  ArrayField_GetOffset(const IField* const a);
const char* ArrayField_GetName(const IField* const a);
const IField* ArrayField_GetArrayLenField(const IField* const a);
const bool ArrayField_IsFixed(const IField* const a);
const uint32_t ArrayField_GetFieldNumber(const IField* const a);
const char* ArrayField_GetTypeName(const IField* const a);
const IType* ArrayField_GetTypeInstance(const IField* const a);

struct BaseValueType
{
	const IType BaseClass;
	const uint32_t DataWidth;
};

//head file extern
#define ExternBaseValueStruct(prefix)	\
struct prefix##Field	\
{\
	const IField BaseClass;			\
	const char* Name;\
	const uint32_t Offset;\
	const BaseValueType *TypeInstance;\
	const uint32_t FieldNumber;\
	const bool ArrayLenFieldFlag;\
};\
extern const BaseValueType prefix##_TypeInstance;
ExternBaseValueStruct(UInt8);
ExternBaseValueStruct(Int8);
ExternBaseValueStruct(UInt16);
ExternBaseValueStruct(Int16);
ExternBaseValueStruct(UInt32);
ExternBaseValueStruct(Int32);
ExternBaseValueStruct(UInt64);
ExternBaseValueStruct(Int64);
ExternBaseValueStruct(Float);
ExternBaseValueStruct(Double);


#define ExternBaseValueStructFunction(prefix)	\
extern const char* prefix##Type_GetTypeName();\
extern const Type_t  prefix##Type_GetType();\
extern const uint32_t prefix##Field_GetFieldNumber( const IField *const a );\
extern const char* prefix##Field_GetName(const IField *const a);\
extern const uint32_t prefix##Field_GetOffset(const IField *const a);\
extern const char* prefix##Field_GetTypeName(const IField *const a);\
extern const IType * prefix##Field_GetTypeInstance(const IField *const a);\
extern const bool prefix##Field_IsArrayLenField(const IField *const a);

ExternBaseValueStructFunction(UInt8);
ExternBaseValueStructFunction(Int8);
ExternBaseValueStructFunction(UInt16);
ExternBaseValueStructFunction(Int16);
ExternBaseValueStructFunction(UInt32);
ExternBaseValueStructFunction(Int32);
ExternBaseValueStructFunction(UInt64);
ExternBaseValueStructFunction(Int64);
ExternBaseValueStructFunction(Float);
ExternBaseValueStructFunction(Double);

struct ObjectType
{
	const IType BaseClass;
	const uint32_t FieldCount;
	const IField** SubFields;
};
struct ObjectField
{
	const IField BaseValue;
	const char* Name;
	const uint32_t Offset;
	const ObjectType *TypeInstance;
	const uint32_t FieldNumber;
};
const char* ObjectType_GetTypeName();
const Type_t ObjectType_GetType();

const uint32_t ObjectField_GetOffset(const IField* const  a);
const char* ObjectField_GetName(const IField* const  a);
const uint32_t ObjectField_GetFieldNumber(const IField* const a);

const char* ObjectField_GetTypeName(const IField* const a);
const IType* ObjectField_GetTypeInstance(const IField* const a);

class SerializationManager
{
public:
	uint32_t Index = 0;
	uint8_t* Buf = nullptr;
	uint32_t BufferLen = 0;
	BlockRingBufferProvider* BlockBufferProvider = nullptr;
#if EmbedXrpc_CheckSumValid==1
	void SetReferenceSum(uint32_t sum)
	{
		if (BlockBufferProvider != nullptr)
		{
			BlockBufferProvider->SetReferenceSum(sum);
		}
		else
		{
			ReferenceSum = sum;
		}
	}
	void SetCalculateSum(uint32_t sum)
	{
		if (BlockBufferProvider != nullptr)
		{
			BlockBufferProvider->SetCalculateSum(sum);
		}
		else
		{
			CalculateSum = sum;
		}
	}
	uint32_t GetReferenceSum()
	{
		if (BlockBufferProvider != nullptr)
		{
			return BlockBufferProvider->GetReferenceSum();
		}
		else
		{
			return ReferenceSum;
		}
	}
	uint32_t GetCalculateSum()
	{
		if (BlockBufferProvider != nullptr)
		{
			return BlockBufferProvider->GetCalculateSum();
		}
		else
		{
			return CalculateSum;
		}
	}

	void AppendSumToCalculateSum(uint32_t sum)//只有ringbuffer mode 为0的情况下使用。
	{
		(void)sum;
#if EmbedXrpc_UseRingBufferWhenReceiving==0
		CalculateSum += sum;
#endif
	}

#endif
	bool IsEnableMataDataEncode = false;
	void SerializeKey(uint32_t  FieldNumber, Type_t   Field);
	void SerializeLen(uint32_t  Len);
	void SerializeEndFlag();
	bool IsEnd();
	void SerializeArrayElementFlag(uint8_t flag);
	void SerializeValue(uint8_t  Len, void* v);
	bool Pop(uint8_t* out_buf, uint32_t len);
	void Reset();

	uint32_t GetKeyFromSerializationManager(uint32_t* fn, Type_t* type);
	void RemoveKeyFromSerializationManager();
	uint8_t GetArrayLenFromSerializationManager(uint32_t* arrayLen);
	void RemoveArrayLenFromSerializationManager();
	uint8_t GetArrayElementFlag();
	void RemoveArrayElementFlagFromSerializationManager();
	void RemoveEndFlagFromSerializationManager();
	void Serialize(const ObjectType* objectType, void* objectData, uint32_t fieldNumber = 0);
	bool Deserialize(const ObjectType* objectType, void* objectPoint, uint32_t fieldNumber = 0);
	static void FreeData(const ObjectType* objectType, void* objectData);
private:
	void SerializeSubField(const ObjectType* objectType, void* objectData);
	void NoMataData_SerializeSubField(const ObjectType* objectType, void* objectData);
	bool DeserializeSubField(const ObjectType* objectType, void* objectPoint);
	bool NoMataData_DeserializeSubField(const ObjectType* objectType, void* objectPoint);
#if EmbedXrpc_CheckSumValid==1
	uint32_t ReferenceSum = 0;
	uint32_t CalculateSum = 0;
#endif
};
#if EmbedXrpc_CheckSumValid==1
#define SerializationManagerAppendDataSum(sm,sum)    sm.SetCalculateSum(sm.GetCalculateSum()+sum)
#else
#define SerializationManagerAppendDataSum(sm,sum)
#endif

inline void DeserializeField(uint8_t* field_ptr, SerializationManager& sm, uint16_t field_width)
{
#if EmbedXrpc_UseRingBufferWhenReceiving==1 
	sm.BlockBufferProvider->PopChars(field_ptr, (uint16_t)field_width);
#else 
	Memcpy(field_ptr, &sm.Buf[sm.Index], field_width);
	SerializationManagerAppendDataSum(sm, GetSum(&sm.Buf[sm.Index], field_width));
	sm.Index += field_width;
#endif 
}
#endif