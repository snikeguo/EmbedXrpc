#ifndef EmbedXrpcSerialization_H
#define EmbedXrpcSerialization_H
/*
#include <stddef.h>
#include <stdint.h>
#include "stdio.h"
#include <cassert>
*/

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
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint64_t arrayLen) {}
	virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v) {}
	virtual void Deserialize(SerializationManager& manager, void* v) {}
	virtual void Deserialize(SerializationManager& manager, void* v, uint64_t arrayLen) {}

};

class IField :public IType
{
public:
	virtual IField* GetArrayLenField() { return nullptr; }
	virtual uint8_t GetArrayLenFieldLen() { return 0; }
	virtual uint32_t GetOffset() { return 0; }
	virtual const char* GetName() = 0;
};
class ArrayType :public IType
{
public:
	IType* ArrayElementType;
	uint64_t ElementTypeLen;
	ArrayType(IType* aet, uint64_t elementTypeLen)
	{
		ArrayElementType = aet;
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
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint64_t arrayLen)
	{
		for (int i = 0; i < arrayLen; i++)
		{
			uint8_t* d = (uint8_t*)v;
			ArrayElementType->Serialize(manager, Tag, (void*)(d + i * ElementTypeLen));
		}
	}
	void Deserialize(SerializationManager& manager, void* v, uint64_t arrayLen)
	{
		for (int i = 0; i < arrayLen; i++)
		{
			memcpy((void*)((uint8_t*)v + i * ElementTypeLen), &manager.Buf[manager.Index], ElementTypeLen);
			manager.Index += ElementTypeLen;
		}

	}
};
class ArrayField :public IField
{
public:
	const char* Name;
	uint32_t Offset;
	IField* ArrayLenField;
	ArrayType t;
	ArrayField(const char* name, IType* aet, uint64_t elementTypeLen, uint32_t offset, IField* alf) :t(aet, elementTypeLen)
	{
		Offset = offset;
		ArrayLenField = alf;
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
	IField* GetArrayLenField()
	{
		return ArrayLenField;
	}
	uint8_t GetArrayLenFieldLen()
	{
		switch (ArrayLenField->GetType())
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
	//type部分
	const char* GetTypeName()
	{
		return t.GetTypeName();
	}
	Type_t GetType()
	{
		return t.GetType();
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint64_t arrayLen)
	{
		t.Serialize(manager, Tag, v, arrayLen);
	}
	void Deserialize(SerializationManager& manager, void* v, uint64_t arrayLen)
	{
		t.Deserialize(manager, v, arrayLen);

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
	Type_t GetType()
	{
		return t.GetType();
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
	Type_t GetType()\
	{\
		return t.GetType();\
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
class ObjectType :public IType
{
public:
	uint32_t FieldCount;
	IField** SubFieldsType;
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
		SubFieldsType = ftds;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)
	{
		for (uint32_t i = 0; i < FieldCount; i++)
		{
			void* d = (void*)((uint8_t*)v + SubFieldsType[i]->GetOffset());
			if (SubFieldsType[i]->GetType() != TYPE_ARRAY)
			{

				SubFieldsType[i]->Serialize(manager, Tag, d);
			}
			else
			{
				IField* a = (IField*)SubFieldsType[i]->GetArrayLenField();
				void* voidLen = (void*)((uint8_t*)v + a->GetOffset());
				ArrayField* arrayfield = (ArrayField*)SubFieldsType[i];
				uint64_t len = 0;
				memcpy(&len, voidLen, arrayfield->GetArrayLenFieldLen());
				SubFieldsType[i]->Serialize(manager, Tag, d, len);
			}
		}
	}
	void Deserialize(SerializationManager& manager, void* v)
	{
		for (uint32_t i = 0; i < FieldCount; i++)
		{
			void* d = (void*)((uint8_t*)v + SubFieldsType[i]->GetOffset());
			if (SubFieldsType[i]->GetType() != TYPE_ARRAY)
			{

				SubFieldsType[i]->Deserialize(manager, d);
			}
			else
			{
				IField* a = (IField*)SubFieldsType[i]->GetArrayLenField();
				void* voidLen = (void*)((uint8_t*)v + a->GetOffset());
				ArrayField* arrayfield = (ArrayField*)SubFieldsType[i];
				uint64_t len = 0;
				memcpy(&len, voidLen, arrayfield->GetArrayLenFieldLen());
				SubFieldsType[i]->Deserialize(manager, d, len);
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
	Type_t GetType()
	{
		return t.GetType();
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

#endif