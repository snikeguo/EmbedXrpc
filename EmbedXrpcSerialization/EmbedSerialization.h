#ifndef EmbedSerialization_H
#define EmbedSerialization_H


#include "EmbedSerialization.Port.h"
#include "BlockBufferProvider.h"
#ifndef offsetof
#define offsetof(s, m) (uint32_t)((char*)(&((s*)0)->m))
#endif

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
class IType
{
public:
	virtual const char* GetTypeName() const = 0;
	virtual const Type_t GetType() const = 0;

	virtual void Free(void* ptr, uint32_t arrayLen=1)const {}
	virtual void* Malloc(uint32_t arrayLen)const { return nullptr; }
};
extern const char* TypeString[12];
class BaseValueInfo
{
public:
	const IType* TypeInstance;
	const uint8_t DataWidth;
};
extern const BaseValueInfo BaseValueInfos[10];
class IField //:public IType
{
public:
	
	virtual const uint32_t GetOffset() const { return 0; }
	virtual const char* GetName() const = 0;
	
	virtual const bool IsArrayLenField() const { return false; }
	virtual const IField* GetArrayLenField() const { return nullptr; }
	virtual const bool IsFixed() const { return true; }
	virtual const uint32_t GetFieldNumber() const { return 0; }

	//type 部分
	virtual const char* GetTypeName() const = 0;
	//virtual Type_t GetType() = 0;
	virtual const IType* GetTypeInstance() const { return nullptr; }

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
	const uint32_t FieldNumber;
	ArrayField(uint32_t fieldNumber,const char* name, const bool isFixed, const IType* arrayElementType, const uint32_t elementTypeLen, const uint32_t offset,const IField* arrayLenField)
		:t(arrayElementType, elementTypeLen),
		ArrayLenField(arrayLenField),
		Offset(offset),
		Name(name),
		_IsFixed(isFixed),
		FieldNumber(fieldNumber)
	{

	}
	const uint32_t GetFieldNumber() const { return FieldNumber; }
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
};
extern const UInt8Type UInt8TypeInstance;
class UInt8Field : public IField
{
public:
	const char* Name;
	const uint32_t Offset;
	const UInt8Type t;
	const uint32_t FieldNumber;
	const bool ArrayLenFieldFlag;
	UInt8Field(const uint32_t fieldNumber,const char* name, const uint32_t offset,const bool isArrayLenField) :FieldNumber(fieldNumber),Name(name), Offset(offset),ArrayLenFieldFlag(isArrayLenField)
	{

	}
	const uint32_t GetFieldNumber() const { return FieldNumber; }
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
	const bool IsArrayLenField() const { return ArrayLenFieldFlag; }
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
};\
extern const prefix##Type prefix##TypeInstance; \
class prefix##Field : public IField	\
{\
public:\
	const char* Name;\
	const uint32_t Offset;\
	const prefix##Type t;\
	const uint32_t FieldNumber;\
	const bool ArrayLenFieldFlag;\
	prefix##Field(const uint32_t fieldNumber,const char* name, const uint32_t offset,const bool isArrayLenField) :FieldNumber(fieldNumber),Name(name), Offset(offset),ArrayLenFieldFlag(isArrayLenField)\
	{																			\
	}	\
	const uint32_t GetFieldNumber() const { return FieldNumber; }\
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
	const bool IsArrayLenField() const { return ArrayLenFieldFlag; }\
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
						EmbedSerializationAssert(arrayLenField->GetTypeInstance()->GetType() <= TYPE_INT64);
						MEMCPY(&len, voidLenPtr, BaseValueInfos[arrayLenField->GetTypeInstance()->GetType()].DataWidth);
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
	const uint32_t FieldNumber;
	ObjectField(uint32_t fieldNumber,const char* name, const uint32_t fc, const IField* ftds[], const uint32_t offset) :
		t(fc, ftds),
		Offset(offset),
		Name(name),
		FieldNumber(fieldNumber)
	{

	}
	const uint32_t GetFieldNumber() const { return FieldNumber; }
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
	void Free(void* v, uint32_t arrayLen=1) const
	{
		t.Free(v, arrayLen);
	}
};

class SerializationManager
{
public:
	uint32_t Index = 0;
	uint8_t* Buf = nullptr;
	uint32_t BufferLen = 0;
	BlockRingBufferProvider* BlockBufferProvider;

	void SerializeKey(uint32_t  FieldNumber, Type_t   Field)
	{
		Buf[Index++] = FieldNumber;
		Buf[Index++] = Field;
		printf("SerializeKey FieldNumber:%d,Type:%s\n", FieldNumber, TypeString[Field]);
	}
	void SerializeLen(uint64_t  Len)
	{
		Buf[Index++] = Len;
		printf("SerializeLen:%lld\n", Len);
	}
	void SerializeEndFlag()
	{
		Buf[Index++] = 0x7F;
		printf("SerializeEnd\n");
	}
	bool IsEnd()
	{
		return Buf[Index] == 0x7F;
	}
	void SerializeValue(uint8_t  Len, void* v)//base value used
	{
		MEMCPY(&Buf[Index], v, Len);
		printf("SerializeValue:\n");
		for (size_t i = Index; i < Len+Index; i++)
		{
			printf("%d,", Buf[i]);
			if ((i-Index + 1) % 10 == 0)
			{
				printf("\n");
			}
		}
		printf("\n");
		Index += Len;
	}
	void Pop(uint8_t* out_buf, uint32_t len)
	{
		if (BlockBufferProvider == nullptr)
		{
			if (out_buf != nullptr)
			{
				MEMCPY(out_buf, &Buf[Index], len);
			}
			Index += len;
		}
		else
		{
			uint8_t d = 0;
			if (out_buf != nullptr)
			{
				for (uint8_t i = 0; i < len; i++)
				{
					EmbedSerializationAssert(BlockBufferProvider->GetChar(&d) == true);
					out_buf[i] = d;
				}
			}
		}
	}
	void Reset()
	{
		Index = 0;
	}
	uint32_t GetKeyFromSerializationManager(uint32_t* fn, Type_t* type)
	{
		if (fn != nullptr)
		{
			*fn = Buf[Index];
		}
		if (type != nullptr)
		{
			*type = (Type_t)Buf[Index + 1];
		}
		return 2;
	}
	void RemoveKeyFromSerializationManager()
	{
		uint32_t ind = GetKeyFromSerializationManager(nullptr, nullptr);
		Index += ind;
	}
	uint32_t GetArrayLenFromSerializationManager(uint32_t* arrayLen)
	{
		*arrayLen = Buf[Index];
		return 1;
	}
	void RemoveArrayLenFromSerializationManager()
	{
		uint32_t ind = GetArrayLenFromSerializationManager(nullptr);
		Index += ind;
	}
	uint8_t GetArrayElementFlag()
	{
		return Buf[Index];
	}
	void RemoveArrayElementFlagFromSerializationManager()
	{
		Index++;
	}
	void RemoveEndFlagFromSerializationManager()
	{
		Index++;
	}
	void Serialize(const ObjectType* objectType, void* objectData, uint32_t fieldNumber)
	{
		SerializeKey(fieldNumber, TYPE_OBJECT);
		for (uint32_t i = 0; i < objectType->FieldCount; i++)
		{
			void* fieldData = (void*)((uint8_t*)objectData + objectType->SubFields[i]->GetOffset());
			Debug("Serialize:%s\n", objectType->SubFields[i]->GetName());
			Type_t typeOfSubField = objectType->SubFields[i]->GetTypeInstance()->GetType();
			if (typeOfSubField <= TYPE_DOUBLE && objectType->SubFields[i]->IsArrayLenField() == false)
			{
				SerializeKey(objectType->SubFields[i]->GetFieldNumber(), typeOfSubField);
				SerializeValue(BaseValueInfos[typeOfSubField].DataWidth, fieldData);
			}
			else if (typeOfSubField == TYPE_ARRAY)
			{
				ArrayField* arrayfield = (ArrayField*)objectType->SubFields[i];
				ArrayType* arrayType = (ArrayType*)(arrayfield->GetTypeInstance());
				IType* arrayElementType = (IType*)(arrayType->ArrayElementType);

				IField* arrayLenField = (IField*)arrayfield->GetArrayLenField();
				EmbedSerializationAssert(arrayLenField->GetTypeInstance()->GetType() <= TYPE_INT64);
				uint8_t sizeOfLenField = BaseValueInfos[arrayLenField->GetTypeInstance()->GetType()].DataWidth; //like:1,2,4,8

				SerializeKey(objectType->SubFields[i]->GetFieldNumber(), TYPE_ARRAY);

				uint64_t len = 1;

				if (arrayLenField != nullptr)
				{
					void* voidLenPtr = (void*)((uint8_t*)objectData + arrayLenField->GetOffset());
					MEMCPY(&len, voidLenPtr, sizeOfLenField);
				}

				ptr_t* ptr = (ptr_t*)fieldData;
				if (arrayfield->IsFixed() == false)
				{
					ptr = (*(ptr_t**)fieldData);
				}
				SerializeLen(len);

				uint8_t baseValueTypeFlag = 0;
				if (arrayElementType->GetType() <= TYPE_DOUBLE)
				{
					baseValueTypeFlag = (arrayElementType->GetType() << 4) | 0x01;
					SerializeValue(1, &baseValueTypeFlag);
					for (uint32_t j = 0; j < len; j++)
					{
						SerializeValue(BaseValueInfos[arrayElementType->GetType()].DataWidth,
							ptr + j * BaseValueInfos[arrayElementType->GetType()].DataWidth);
					}
				}
				else
				{
					baseValueTypeFlag = 0x02;
					SerializeValue(1, &baseValueTypeFlag);
					ObjectType* ot = (ObjectType*)arrayElementType;

					for (uint32_t j = 0; j < len; j++)
					{
						Serialize(ot, ptr + j * (arrayType->ElementTypeLen), 0);
					}
				}
			}
			else
			{
				ObjectField* of = (ObjectField*)objectType->SubFields[i];
				ObjectType* ot = (ObjectType*)(of->GetTypeInstance());
				Serialize(ot, fieldData, of->GetFieldNumber());
			}
		}
		SerializeEndFlag();
	}
	void Deserialize(const ObjectType* objectType, void* objectPoint,uint32_t recBuferLen)
	{
		uint32_t fn = 0;
		Type_t tp = TYPE_UINT8;

		//GetKeyFromSerializationManager(&fn, &tp);
		//RemoveKeyFromSerializationManager();
		//EmbedSerializationAssert(tp == TYPE_OBJECT);

		
		while (!IsEnd() && Index < recBuferLen)//并没有到结构体定界符以及 没有遍历完数据流
		{
			GetKeyFromSerializationManager(&fn, &tp);
			RemoveKeyFromSerializationManager();
			//EmbedSerializationAssert(tp == TYPE_OBJECT);
			if (fn == 0 && tp==TYPE_OBJECT)
			{
				Deserialize(objectType, objectPoint, recBuferLen);//如果fieldNumber 为0 说明这是第一次进来,也就是最顶级的结构体，最顶级的结构体执行完毕后,就要退出
			}
			else if (tp <= TYPE_DOUBLE)
			{
				const IType* typeInstance = BaseValueInfos[tp].TypeInstance;//获取到对应的TP
				void* d = nullptr;
				if (objectType != nullptr)
				{
					for (uint32_t i = 0; i < objectType->FieldCount; i++)
					{
						if (fn == objectType->SubFields[i]->GetFieldNumber() && tp == objectType->SubFields[i]->GetTypeInstance()->GetType())
						{
							d = (void*)((uint8_t*)objectPoint + objectType->SubFields[i]->GetOffset());
							Debug("Deserialize:%s\n", objectType->SubFields[i]->GetName());
							break;
						}
					}
				}
				Pop((uint8_t*)d, BaseValueInfos[tp].DataWidth);
			}
			else if (tp == TYPE_ARRAY)
			{
				uint32_t arraylen = 0;

				GetArrayLenFromSerializationManager(&arraylen);
				RemoveArrayLenFromSerializationManager();

				uint8_t baseValueTypeFlag = GetArrayElementFlag();
				RemoveArrayElementFlagFromSerializationManager();

				EmbedSerializationAssert((((baseValueTypeFlag & 0x0F) == 1) && (baseValueTypeFlag <= TYPE_DOUBLE)) || (baseValueTypeFlag == 0x02));

				void* d = nullptr;
				ArrayField* arrayfield = nullptr;
				ArrayType* arrayType = nullptr;
				void* ptr = d;
				IField* arrayLenField = nullptr;

				if (objectType != nullptr)
				{
					for (uint32_t i = 0; i < objectType->FieldCount; i++)
					{
						if (fn == objectType->SubFields[i]->GetFieldNumber() && tp == objectType->SubFields[i]->GetTypeInstance()->GetType())
						{
							d = (void*)((uint8_t*)objectPoint + objectType->SubFields[i]->GetOffset());
							ptr = d;
							arrayfield = (ArrayField*)objectType->SubFields[i];
							arrayType = (ArrayType*)(arrayfield->GetTypeInstance());
							Debug("Deserialize:%s\n", objectType->SubFields[i]->GetName());
							break;
						}
					}
				}
				if (arrayfield != nullptr)
				{
					arrayLenField = (IField*)arrayfield->GetArrayLenField();
					if (arrayfield->IsFixed() == false)
					{
						ptr = arrayfield->Malloc(arraylen);
						MEMCPY((void*)((ptr_t*)d), &ptr, sizeof(ptr_t));
					}
				}

				if ((baseValueTypeFlag & 0x0F) == 0x01)//base value type
				{
					Type_t aet = (Type_t)(baseValueTypeFlag >> 4 & 0x0F);
					EmbedSerializationAssert(aet <= TYPE_DOUBLE);
					const IType* Instance = BaseValueInfos[aet].TypeInstance;
					for (uint32_t j = 0; j < arraylen; j++)
					{
						if (ptr != nullptr)
						{
							Pop((uint8_t*)ptr + j * BaseValueInfos[aet].DataWidth, BaseValueInfos[aet].DataWidth);
						}
						else
						{
							Pop(nullptr, BaseValueInfos[aet].DataWidth);
						}
					}
				}
				else
				{
					
					ObjectType* ot = nullptr;// (ObjectType*)(arrayType->ArrayElementType);
					if (arrayType != nullptr)
					{
						ot = (ObjectType*)(arrayType->ArrayElementType);
					}
					for (uint32_t j = 0; j < arraylen; j++)
					{
						if (ptr != nullptr)
						{
							Deserialize(ot, (uint8_t*)ptr + j * arrayType->ElementTypeLen, recBuferLen);
						}
						else
						{
							Deserialize(ot, nullptr, recBuferLen);
						}
					}
				}
			}
			else if (tp == TYPE_OBJECT)
			{
				void* d = nullptr;
				ObjectField* subObjectField = nullptr;
				ObjectType* subObjectType = nullptr;
				if (objectType != nullptr)
				{
					for (uint32_t i = 0; i < objectType->FieldCount; i++)
					{
						if (fn == objectType->SubFields[i]->GetFieldNumber() && tp == objectType->SubFields[i]->GetTypeInstance()->GetType())
						{
							d = (void*)((uint8_t*)objectPoint + objectType->SubFields[i]->GetOffset());
							subObjectField = (ObjectField*)objectType->SubFields[i];
							subObjectType = (ObjectType*)subObjectField->GetTypeInstance();
							Debug("Deserialize:%s\n", objectType->SubFields[i]->GetName());
							break;
						}
					}
				}
				Deserialize(subObjectType, d, recBuferLen);
			}
		}
		RemoveEndFlagFromSerializationManager();

	}
};

#endif