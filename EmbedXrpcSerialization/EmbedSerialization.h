#ifndef EmbedSerialization_H
#define EmbedSerialization_H


#include "EmbedSerialization.Port.h"
#ifndef offsetof
#define offsetof(s, m) (uint32_t)((char*)(&((s*)0)->m))
#endif

typedef  uint32_t ptr_t;
/*
用来测试
static void* MALLOC(uint32_t size)
{
	void* ptr = malloc(size);
	printf("malloc :0x%x\n", ptr);
	return ptr;
}

static void FREE(void* ptr)
{
	printf("free :0x%x\n", ptr);
	free(ptr);
}*/
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


class SerializationManager
{
public:
	uint32_t Index = 0;
	uint8_t* Buf = nullptr;
	uint32_t BufferLen = 0;
	void Append(uint32_t  Tag, Type_t   Field, uint32_t	Len, void* v)
	{
		//Buf[Index++]=Tag;
		//Buf[Index++]=Field;
		//Buf[Index++]=Len;
		MEMCPY(&Buf[Index], v, Len);
		Index += Len;
	}
	void Reset()
	{
		Index = 0;
	}
};
/*
class SerializationField_t
{
public:
	uint32_t		Tag;
	Field_t     Field;
	size_t			Len;
	uint8_t		    *Data;
};
*/
class IType
{
public:
	virtual const char* GetTypeName() const = 0;
	virtual const Type_t GetType() const = 0;
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint32_t arrayLen)  const  {}
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) const {}
	virtual void Deserialize(SerializationManager& manager, void* v)const {}
	virtual void Deserialize(SerializationManager& manager, void* v, uint32_t arrayLen)const {}
	virtual void Free(void* ptr, uint32_t arrayLen=1)const {}
	virtual void* Malloc(uint32_t arrayLen)const { return nullptr; }
};

class IField //:public IType
{
public:
	
	virtual const uint32_t GetOffset() const { return 0; }
	virtual const char* GetName() const = 0;
	

	virtual const IField* GetArrayLenField() const { return nullptr; }
	virtual const uint8_t GetArrayLenFieldLen() const { return 0; }
	virtual const bool IsFixed() const { return true; }

	//type 部分
	virtual const char* GetTypeName() const = 0;
	//virtual Type_t GetType() = 0;
	virtual const IType* GetTypeInstance() const { return nullptr; }
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint32_t arrayLen) const {}
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) const {}
	virtual void Deserialize(SerializationManager& manager, void* v) const {}
	virtual void Deserialize(SerializationManager& manager, void* v, uint32_t arrayLen) const {}

	virtual void Free(void* ptr, uint32_t arrayLen=1) const {}
	virtual void* Malloc(uint32_t arrayLen) const { return nullptr; }

	
};

class ArrayType :public IType
{
public:
	const IType* ArrayElementType;
	const uint32_t ElementTypeLen;
	ArrayType(const IType* arrayElementType, const uint32_t elementTypeLen) :
		ArrayElementType(arrayElementType),
		ElementTypeLen(elementTypeLen) 
	{
	}
	const char* GetTypeName() const
	{
		return "array";
	}
	const Type_t GetType()const 
	{
		return TYPE_ARRAY;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint32_t arrayLen) const
	{
		for (uint32_t i = 0; i < arrayLen; i++)
		{
			uint8_t* d = (uint8_t*)v;
			ArrayElementType->Serialize(manager, Tag, (void*)(d + i * ElementTypeLen));
		}
	}
	void Deserialize(SerializationManager& manager, void* v, uint32_t arrayLen) const
	{
		for (uint32_t i = 0; i < arrayLen; i++)
		{
			//MEMCPY((void*)((uint8_t*)v + i * ElementTypeLen), &manager.Buf[manager.Index], ElementTypeLen);
			ArrayElementType->Deserialize(manager, (void*)((uint8_t*)v + i * ElementTypeLen));
			//manager.Index += ElementTypeLen;
		}

	}
	
	void* Malloc(uint32_t arrayLen) const
	{
		void *ptr=MALLOC(arrayLen * ElementTypeLen);
		Debug("malloc :0x%x,arrayLen:%d\n", (uint32_t)ptr, arrayLen);
		return ptr;
	}
	void Free(void* ptr, uint32_t arrayLen=1) const
	{
		for (uint32_t i = 0; i < arrayLen; i++)
		{
			ArrayElementType->Free((void*)((uint8_t*)ptr + i * ElementTypeLen), 1);
		}
		Debug("free :0x%x\n", (uint32_t)ptr);
		FREE(ptr);
	}
};
class ArrayField :public IField
{
public:
	const char* Name;
	const uint32_t Offset;
	const IField* ArrayLenField;
	const ArrayType t;
	const bool _IsFixed;
	ArrayField(const char* name, const bool isFixed, const IType* arrayElementType, const uint32_t elementTypeLen, const uint32_t offset,const IField* arrayLenField)
		:t(arrayElementType, elementTypeLen),
		ArrayLenField(arrayLenField),
		Offset(offset),
		Name(name),
		_IsFixed(isFixed)
	{

	}

	const char* GetName() const
	{
		return Name;
	}

	const uint32_t GetOffset() const
	{
		return Offset;
	}
	const IField* GetArrayLenField() const
	{
		return ArrayLenField;
	}
	const uint8_t GetArrayLenFieldLen() const
	{
		switch (ArrayLenField->GetTypeInstance()->GetType())
		{
		case TYPE_UINT8:
			return 1;
			break;
		case TYPE_INT8:
			return 1;
			break;
		case TYPE_UINT16:
			return 2;
			break;
		case TYPE_INT16:
			return 2;
			break;
		case TYPE_UINT32:
			return 4;
			break;
		case TYPE_INT32:
			return 4;
			break;
		case TYPE_UINT64:
			return 8;
			break;
		case TYPE_INT64:
			return 8;
			break;
		default:
			return 0;
			break;
		}
		return 0;
	}
	const bool IsFixed() const
	{
		return _IsFixed;
	}
	//type部分
	const char* GetTypeName() const
	{
		return t.GetTypeName();
	}
	const IType * GetTypeInstance() const
	{
		return &t;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint32_t arrayLen) const
	{
		t.Serialize(manager, Tag, v, arrayLen);
	}
	void Deserialize(SerializationManager& manager, void* v, uint32_t arrayLen) const
	{
		t.Deserialize(manager, v, arrayLen);
	}
	void* Malloc(uint32_t arrayLen) const
	{
		return t.Malloc(arrayLen);
	}
	void Free(void* ptr, uint32_t arrayLen=1) const
	{
		t.Free(ptr,arrayLen);
	}
};
class UInt8Type :public IType
{
public:
	const char* GetTypeName() const
	{
		return "uint8_t";
	}
	const Type_t GetType() const
	{
		return TYPE_UINT8;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) const
	{
		manager.Append(Tag, TYPE_UINT8, 1, v);
	}
	void Deserialize(SerializationManager& manager, void* v) const
	{
		MEMCPY(v, &manager.Buf[manager.Index], 1);
		manager.Index++;
	}
};
extern const UInt8Type UInt8TypeInstance;
class UInt8Field : public IField
{
public:
	const char* Name;
	const uint32_t Offset;
	const UInt8Type t;
	UInt8Field(const char* name, const uint32_t offset) :Name(name), Offset(offset)
	{

	}
	const char* GetName() const
	{
		return Name;
	}

	const uint32_t GetOffset() const
	{
		return Offset;
	}

	//type部分
	const char* GetTypeName() const
	{
		return t.GetTypeName();
	}
	const IType* GetTypeInstance() const
	{
		return &t;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) const
	{
		t.Serialize(manager, Tag, v);
	}
	void Deserialize(SerializationManager& manager, void* v) const
	{
		t.Deserialize(manager, v);
	}

};

#define CodeGenBaseValueClass(prefix,typeName,type,len)	\
class prefix##Type:public IType	\
{								\
public:							\
	const char* GetTypeName() const	\
	{							\
		return typeName;		\
	}							\
	const Type_t GetType() const \
	{\
		return type;\
	}\
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) const	\
	{	\
		manager.Append(Tag, type, len, v);\
	}\
	void Deserialize(SerializationManager& manager, void* v) const \
	{\
		MEMCPY(v, &manager.Buf[manager.Index], len);\
		manager.Index+=len;\
	}\
};\
extern const prefix##Type prefix##TypeInstance; \
class prefix##Field : public IField	\
{\
public:\
	const char* Name;\
	const uint32_t Offset;\
	const prefix##Type t;\
	prefix##Field(const char* name, const uint32_t offset) :Name(name), Offset(offset)\
	{																			\
	}																			\
	const char* GetName() const \
	{\
		return Name;\
	}\
	const uint32_t GetOffset() const \
	{\
		return Offset;\
	}\
	const char* GetTypeName() const \
	{\
		return t.GetTypeName();\
	}\
	const IType * GetTypeInstance()	const \
	{	\
		return &t;\
	}\
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) const \
	{\
		t.Serialize(manager, Tag, v);\
	}\
	void Deserialize(SerializationManager& manager, void* v) const \
	{\
		t.Deserialize(manager, v);\
	}\
}
CodeGenBaseValueClass(Int8, "int8_t", TYPE_INT8, 1);
CodeGenBaseValueClass(UInt16, "uint16_t", TYPE_UINT16, 2);
CodeGenBaseValueClass(Int16, "int16_t", TYPE_INT16, 2);
CodeGenBaseValueClass(UInt32, "uint32_t", TYPE_UINT32, 4);
CodeGenBaseValueClass(Int32, "int32_t", TYPE_UINT32, 4);
CodeGenBaseValueClass(UInt64, "uint64_t", TYPE_UINT64, 8);
CodeGenBaseValueClass(Int64, "int64_t", TYPE_UINT64, 8);
CodeGenBaseValueClass(Float, "float", TYPE_FLOAT, 4);
CodeGenBaseValueClass(Double, "double", TYPE_DOUBLE, 8);

#define DefineBaseValueInstance(prefix) const prefix##Type prefix##TypeInstance;


class ObjectType :public IType
{
public:
	const uint32_t FieldCount;
	const IField** SubFields;
	const char* GetTypeName() const
	{
		return "Object";
	}
	const Type_t GetType() const 
	{
		return TYPE_OBJECT;
	}
	ObjectType(const uint32_t fc, const IField* ftds[]):
		FieldCount(fc),
		SubFields(ftds)
	{

	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) const
	{
		for (uint32_t i = 0; i < FieldCount; i++)
		{
			void* d = (void*)((uint8_t*)v + SubFields[i]->GetOffset());
			Debug("Serialize:%s\n", SubFields[i]->GetName());
			if (SubFields[i]->GetTypeInstance()->GetType() != TYPE_ARRAY)
			{
				SubFields[i]->Serialize(manager, Tag, d);
			}
			else
			{
				ArrayField* arrayfield = (ArrayField*)SubFields[i];
				IField* arrayLenField = (IField*)arrayfield->GetArrayLenField();
				uint32_t len = 1;
				if (arrayLenField != nullptr)
				{
					void* voidLenPtr = (void*)((uint8_t*)v + arrayLenField->GetOffset());
					MEMCPY(&len, voidLenPtr, arrayfield->GetArrayLenFieldLen());
				}

				ptr_t* ptr =(ptr_t*) d;
				if (arrayfield->IsFixed() == false)
				{
					ptr = (*(ptr_t**)d);
				}
				SubFields[i]->Serialize(manager, Tag, ptr, len);
			}
		}
	}
	void Deserialize(SerializationManager& manager, void* v) const
	{
		for (uint32_t i = 0; i < FieldCount; i++)
		{
			void* d = (void*)((uint8_t*)v + SubFields[i]->GetOffset());
			Debug("Deserialize:%s\n", SubFields[i]->GetName());
			if (SubFields[i]->GetTypeInstance()->GetType() != TYPE_ARRAY)
			{

				SubFields[i]->Deserialize(manager, d);
			}
			else
			{
				ArrayField* arrayfield = (ArrayField*)SubFields[i];
				IField* arrayLenField = (IField*)arrayfield->GetArrayLenField();
				uint32_t len = 1;
				if (arrayLenField != nullptr)
				{
					void* voidLenPtr = (void*)((uint8_t*)v + arrayLenField->GetOffset());
					MEMCPY(&len, voidLenPtr, arrayfield->GetArrayLenFieldLen());
				}
				void* ptr = d;
				if (arrayfield->IsFixed() == false)
				{
					ptr = arrayfield->Malloc(len);
					MEMCPY((void *)((ptr_t*)d),&ptr,sizeof(ptr_t));
				}
				SubFields[i]->Deserialize(manager, ptr, len);
			}
		}
	}
	void Free(void* v, uint32_t arrayLen=1) const
	{
		for (uint32_t i = 0; i < FieldCount; i++)
		{
			if (SubFields[i]->GetTypeInstance()->GetType() == TYPE_ARRAY)
			{		
				ArrayField* arrayfield = (ArrayField*)SubFields[i];				
				if (arrayfield->IsFixed() == false)
				{
					IField* arrayLenField = (IField*)arrayfield->GetArrayLenField();
					uint32_t len = 1;
					if (arrayLenField != nullptr)
					{
						void* voidLenPtr = (void*)((uint8_t*)v + arrayLenField->GetOffset());
						MEMCPY(&len, voidLenPtr, arrayfield->GetArrayLenFieldLen());
					}					
					uint8_t** arrayfieldPtr = (uint8_t**)((uint8_t*)v + arrayfield->GetOffset());
					arrayfield->Free(*arrayfieldPtr, len);
				}
			}

		}
	}
};
class ObjectField :public IField
{
public:

	const char* Name;
	const uint32_t Offset;
	const ObjectType t;
	ObjectField(const char* name, const uint32_t fc, const IField* ftds[], const uint32_t offset) :
		t(fc, ftds),
		Offset(offset),
		Name(name)
	{

	}
	const char* GetName() const
	{
		return Name;
	}
	const uint32_t GetOffset() const
	{
		return Offset;
	}

	//type部分
	const char* GetTypeName() const 
	{
		return t.GetTypeName();
	}
	const IType* GetTypeInstance() const
	{
		return &t;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) const
	{
		t.Serialize(manager, Tag, v);
	}
	void Deserialize(SerializationManager& manager, void* v) const
	{
		t.Deserialize(manager, v);
	}
	void Free(void* v, uint32_t arrayLen=1) const
	{
		t.Free(v, arrayLen);
	}
};

#endif