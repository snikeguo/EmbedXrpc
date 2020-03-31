#ifndef EmbedSerialization_H
#define EmbedSerialization_H

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#ifndef offsetof
#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))
#endif
#define MALLOC	malloc
#define FREE	free
#define Debug  
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
	void Append(uint32_t  Tag, Type_t   Field, size_t	Len, void* v)
	{
		//Buf[Index++]=Tag;
		//Buf[Index++]=Field;
		//Buf[Index++]=Len;
		memcpy(&Buf[Index], v, Len);
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
	virtual const char* GetTypeName() = 0;
	virtual Type_t GetType() = 0;
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint32_t arrayLen) {}
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) {}
	virtual void Deserialize(SerializationManager& manager, void* v) {}
	virtual void Deserialize(SerializationManager& manager, void* v, uint32_t arrayLen) {}
	virtual void Free(void* ptr, uint32_t arrayLen=1) {}
	virtual void* Malloc(uint32_t arrayLen) { return nullptr; }
};

class IField //:public IType
{
public:
	
	virtual uint32_t GetOffset() { return 0; }
	virtual const char* GetName() = 0;
	

	virtual IField* GetArrayLenField() { return nullptr; }
	virtual uint8_t GetArrayLenFieldLen() { return 0; }
	virtual bool IsFixed() { return true; }

	//type 部分
	virtual const char* GetTypeName() = 0;
	//virtual Type_t GetType() = 0;
	virtual IType* GetTypeInstance() { return nullptr; }
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint32_t arrayLen) {}
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) {}
	virtual void Deserialize(SerializationManager& manager, void* v) {}
	virtual void Deserialize(SerializationManager& manager, void* v, uint32_t arrayLen) {}

	virtual void Free(void* ptr, uint32_t arrayLen=1) {}
	virtual void* Malloc(uint32_t arrayLen) { return nullptr; }

	
};

class ArrayType :public IType
{
public:
	IType* ArrayElementType;
	uint32_t ElementTypeLen;
	ArrayType(IType* arrayElementType, uint32_t elementTypeLen)
	{
		ArrayElementType = arrayElementType;
		ElementTypeLen = elementTypeLen;
	}
	const char* GetTypeName()
	{
		return "array";
	}
	Type_t GetType()
	{
		return TYPE_ARRAY;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint32_t arrayLen)
	{
		for (uint32_t i = 0; i < arrayLen; i++)
		{
			uint8_t* d = (uint8_t*)v;
			ArrayElementType->Serialize(manager, Tag, (void*)(d + i * ElementTypeLen));
		}
	}
	void Deserialize(SerializationManager& manager, void* v, uint32_t arrayLen)
	{
		for (uint32_t i = 0; i < arrayLen; i++)
		{
			//memcpy((void*)((uint8_t*)v + i * ElementTypeLen), &manager.Buf[manager.Index], ElementTypeLen);
			ArrayElementType->Deserialize(manager, (void*)((uint8_t*)v + i * ElementTypeLen));
			//manager.Index += ElementTypeLen;
		}

	}
	
	void* Malloc(uint32_t arrayLen)
	{
		void *ptr=MALLOC(arrayLen * ElementTypeLen);
		Debug("malloc :0x%x,arrayLen:%d\n", (uint32_t)ptr, arrayLen);
		return ptr;
	}
	void Free(void* ptr, uint32_t arrayLen=1)
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
	uint32_t Offset;
	IField* ArrayLenField;
	ArrayType t;
	bool _IsFixed;
	ArrayField(const char* name,bool isFixed, IType* arrayElementType, uint32_t elementTypeLen, uint32_t offset, IField* arrayLenField) :t(arrayElementType, elementTypeLen)
	{
		Offset = offset;
		ArrayLenField = arrayLenField;
		Name = name;
		_IsFixed = isFixed;
	}

	const char* GetName()
	{
		return Name;
	}

	uint32_t GetOffset()
	{
		return Offset;
	}
	IField* GetArrayLenField()
	{
		return ArrayLenField;
	}
	uint8_t GetArrayLenFieldLen()
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
	bool IsFixed()
	{
		return _IsFixed;
	}
	//type部分
	const char* GetTypeName()
	{
		return t.GetTypeName();
	}
	IType * GetTypeInstance()
	{
		return &t;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint32_t arrayLen)
	{
		t.Serialize(manager, Tag, v, arrayLen);
	}
	void Deserialize(SerializationManager& manager, void* v, uint32_t arrayLen)
	{
		t.Deserialize(manager, v, arrayLen);
	}
	void* Malloc(uint32_t arrayLen)
	{
		return t.Malloc(arrayLen);
	}
	void Free(void* ptr, uint32_t arrayLen=1)
	{
		t.Free(ptr,arrayLen);
	}
};
class Uint8Type :public IType
{
public:
	const char* GetTypeName()
	{
		return "uint8_t";
	}
	Type_t GetType()
	{
		return TYPE_UINT8;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)
	{
		manager.Append(Tag, TYPE_UINT8, 1, v);
	}
	void Deserialize(SerializationManager& manager, void* v)
	{
		memcpy(v, &manager.Buf[manager.Index], 1);
		manager.Index++;
	}
};
extern Uint8Type Uint8TypeInstance;
class Uint8Field : public IField
{
public:
	const char* Name;
	uint32_t Offset;
	Uint8Type t;
	Uint8Field(const char* name, uint32_t offset) :Name(name), Offset(offset)
	{

	}
	const char* GetName()
	{
		return Name;
	}

	uint32_t GetOffset()
	{
		return Offset;
	}

	//type部分
	const char* GetTypeName()
	{
		return t.GetTypeName();
	}
	IType* GetTypeInstance()
	{
		return &t;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)
	{
		t.Serialize(manager, Tag, v);
	}
	void Deserialize(SerializationManager& manager, void* v)
	{
		t.Deserialize(manager, v);
	}

};

#define CodeGenBaseValueClass(prefix,typeName,type,len)	\
class prefix##Type:public IType	\
{								\
public:							\
	const char* GetTypeName()	\
	{							\
		return typeName;		\
	}							\
	Type_t GetType()\
	{\
		return type;\
	}\
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)	\
	{	\
		manager.Append(Tag, type, len, v);\
	}\
	void Deserialize(SerializationManager& manager, void* v)\
	{\
		memcpy(v, &manager.Buf[manager.Index], len);\
		manager.Index+=len;\
	}\
};\
extern prefix##Type prefix##TypeInstance; \
class prefix##Field : public IField	\
{\
public:\
	const char* Name;\
	uint32_t Offset;\
	prefix##Type t;\
	prefix##Field(const char* name, uint32_t offset) :Name(name), Offset(offset)\
	{																			\
	}																			\
	const char* GetName()\
	{\
		return Name;\
	}\
	uint32_t GetOffset()\
	{\
		return Offset;\
	}\
	const char* GetTypeName()\
	{\
		return t.GetTypeName();\
	}\
	IType * GetTypeInstance()	\
	{	\
		return &t;\
	}\
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)\
	{\
		t.Serialize(manager, Tag, v);\
	}\
	void Deserialize(SerializationManager& manager, void* v)\
	{\
		t.Deserialize(manager, v);\
	}\
}
CodeGenBaseValueClass(Int8, "int8_t", TYPE_INT8, 1);
CodeGenBaseValueClass(Uint16, "uint16_t", TYPE_UINT16, 2);
CodeGenBaseValueClass(Int16, "int16_t", TYPE_INT16, 2);
CodeGenBaseValueClass(Uint32, "uint32_t", TYPE_UINT32, 4);
CodeGenBaseValueClass(Int32, "int32_t", TYPE_UINT32, 4);
CodeGenBaseValueClass(Uint64, "uint64_t", TYPE_UINT64, 8);
CodeGenBaseValueClass(Int64, "int64_t", TYPE_UINT64, 8);
CodeGenBaseValueClass(Float, "float", TYPE_FLOAT, 4);
CodeGenBaseValueClass(Double, "double", TYPE_DOUBLE, 8);

#define DefineBaseValueInstance(prefix) prefix##Type prefix##TypeInstance;


class ObjectType :public IType
{
public:
	uint32_t FieldCount;
	IField** SubFields;
	const char* GetTypeName()
	{
		return "Object";
	}
	Type_t GetType()
	{
		return TYPE_OBJECT;
	}
	ObjectType(uint32_t fc, IField* ftds[])
	{
		FieldCount = fc;
		SubFields = ftds;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)
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
					memcpy(&len, voidLenPtr, arrayfield->GetArrayLenFieldLen());
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
	void Deserialize(SerializationManager& manager, void* v)
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
					memcpy(&len, voidLenPtr, arrayfield->GetArrayLenFieldLen());
				}
				void* ptr = d;
				if (arrayfield->IsFixed() == false)
				{
					ptr = arrayfield->Malloc(len);
					memcpy((void *)((ptr_t*)d),&ptr,sizeof(ptr_t));
				}
				SubFields[i]->Deserialize(manager, ptr, len);
			}
		}
	}
	void Free(void* v, uint32_t arrayLen=1)
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
						memcpy(&len, voidLenPtr, arrayfield->GetArrayLenFieldLen());
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
	uint32_t Offset;
	ObjectType t;
	ObjectField(const char* name, uint32_t fc, IField* ftds[], uint32_t offset) :t(fc, ftds)
	{
		Offset = offset;
		Name = name;
	}
	const char* GetName()
	{
		return Name;
	}
	uint32_t GetOffset()
	{
		return Offset;
	}

	//type部分
	const char* GetTypeName()
	{
		return t.GetTypeName();
	}
	IType* GetTypeInstance()
	{
		return &t;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)
	{
		t.Serialize(manager, Tag, v);
	}
	void Deserialize(SerializationManager& manager, void* v)
	{
		t.Deserialize(manager, v);
	}
	void Free(void* v, uint32_t arrayLen=1)
	{
		t.Free(v, arrayLen);
	}
};

#endif