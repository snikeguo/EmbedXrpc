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

	//type ����
	virtual const char* GetTypeName() const = 0;
	//virtual Type_t GetType() = 0;
	virtual const IType* GetTypeInstance() const { return nullptr; }
};

class ArrayType :public IType
{
public:
	const IType* ElementType;
	const uint32_t LengthOfSingleElement;
	ArrayType(const IType* ElementType, const uint32_t lengthOfSingleElement) :
		ElementType(ElementType),
		LengthOfSingleElement(lengthOfSingleElement)
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
	ArrayField(uint32_t fieldNumber,const char* name, const bool isFixed, const IType* arrayElementType, const uint32_t lengthOfSingleElement, const uint32_t offset,const IField* arrayLenField)
		:t(arrayElementType, lengthOfSingleElement),
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
	//type����
	const char* GetTypeName() const
	{
		return t.GetTypeName();
	}
	const IType * GetTypeInstance() const
	{
		return &t;
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

	//type����
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

	//type����
	const char* GetTypeName() const 
	{
		return t.GetTypeName();
	}
	const IType* GetTypeInstance() const
	{
		return &t;
	}
};

class SerializationManager
{
public:
	uint32_t Index = 0;
	uint8_t* Buf = nullptr;
	uint32_t BufferLen = 0;
	BlockRingBufferProvider* BlockBufferProvider;

	/*
		��һ���ֽڣ�FieldNumber<<4|Field;���FieldNumber������16,�����λΪ1,��һ�ֽ�Ҳ��FieldNumber
		�ڶ��ֽڣ�����FieldNumber.....
		...
	*/
	void SerializeKey(uint32_t  FieldNumber, Type_t   Field)
	{
		/*Buf[Index++] = FieldNumber;
		Buf[Index++] = Field;
		printf("SerializeKey FieldNumber:%d,Type:%s\n", FieldNumber, TypeString[Field]);*/

		uint32_t shiftfn = 0;
		uint32_t next_shiftfn = 0;
		if ((FieldNumber >> 3) != 0)
		{
			Buf[Index++] =(uint8_t)(0x80 | (FieldNumber<<4) | Field);
		}
		else
		{
			Buf[Index++] = (uint8_t)((FieldNumber <<4) | Field);
		}
		shiftfn = FieldNumber >> 3;
		while(shiftfn!=0)
		{
			next_shiftfn= shiftfn >> 7;
			if (next_shiftfn == 0)
			{
				Buf[Index++] = shiftfn & 0x7f;
				break;
			}
			else
			{
				Buf[Index++] = shiftfn | 0x80;
			}
			shiftfn = shiftfn >> 7;
		} 
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
	void SerializeArrayElementFlag(uint8_t flag)
	{
		Buf[Index] = flag;
		Index += 1;
		printf("SerializeArrayElementFlag:0x%x\n", flag);
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
	bool Pop(uint8_t* out_buf, uint32_t len)
	{
		if (len + Index > BufferLen)
		{
			return false;
		}
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
		return true;
	}
	void Reset()
	{
		Index = 0;
	}
	uint32_t GetKeyFromSerializationManager(uint32_t* fn, Type_t* type)
	{
		/*if (fn != nullptr)
		{
			*fn = Buf[Index];
		}
		if (type != nullptr)
		{
			*type = (Type_t)Buf[Index + 1];
		}*/
		uint32_t f=0;
		uint8_t used = 0;
		uint8_t temp = 0;
		if (type != nullptr)
		{
			*type = (Type_t)(Buf[Index] & 0x0F);
			EmbedSerializationAssert(*type <= TYPE_OBJECT);
		}
		temp = (Buf[Index] >> 4) & 0x07;//�Ȱ������λ��������
		used++;
		if (((Buf[Index] >> 4) & 0x07) != 0)
		{
			do
			{
				f = ((Buf[Index + used] & 0x7f) << (used -1) * 7) | f;
				if ((Buf[Index + used] & 0x80) == 0)
				{
					break;
				}
				used++;
			} while (true);
		}
		f = f << 3 | temp;
		if (fn != nullptr)
		{
			*fn = f;
		}
		return used;
	}
	void RemoveKeyFromSerializationManager()
	{
		uint32_t ind = GetKeyFromSerializationManager(nullptr, nullptr);
		Index += ind;
	}
	uint32_t GetArrayLenFromSerializationManager(uint64_t* arrayLen)
	{
		if (arrayLen != nullptr)
		{
			*arrayLen = Buf[Index];
		}
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
public:
	void Serialize(const ObjectType* objectType, void* objectData, uint32_t fieldNumber)
	{
		SerializeKey(fieldNumber, TYPE_OBJECT);
		SerializeSubField(objectType, objectData);
	}
private:
	void SerializeSubField(const ObjectType* objectType, void* objectData)
	{
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
				IType* arrayElementType = (IType*)(arrayType->ElementType);

				IField* arrayLenField = (IField*)arrayfield->GetArrayLenField();
				//EmbedSerializationAssert(arrayLenField->GetTypeInstance()->GetType() <= TYPE_INT64);
				uint8_t sizeOfLenField = 0; //like:1,2,4,8

				SerializeKey(objectType->SubFields[i]->GetFieldNumber(), TYPE_ARRAY);

				uint64_t len = 1;

				if (arrayLenField != nullptr)
				{
					sizeOfLenField = BaseValueInfos[arrayLenField->GetTypeInstance()->GetType()].DataWidth; //like:1,2,4,8
					void* voidLenPtr = (void*)((uint8_t*)objectData + arrayLenField->GetOffset());
					MEMCPY(&len, voidLenPtr, sizeOfLenField);
				}

				uint8_t* ptr = (uint8_t*)fieldData;
				if (arrayfield->IsFixed() == false)
				{
					ptr = (*(uint8_t**)fieldData);
				}
				SerializeLen(len);

				uint8_t baseValueTypeFlag = 0;
				if (arrayElementType->GetType() <= TYPE_DOUBLE)
				{
					baseValueTypeFlag = (arrayElementType->GetType() << 4) | 0x01;
					SerializeArrayElementFlag(baseValueTypeFlag);
					for (uint32_t j = 0; j < len; j++)
					{
						SerializeValue(BaseValueInfos[arrayElementType->GetType()].DataWidth,
							ptr + j * BaseValueInfos[arrayElementType->GetType()].DataWidth);
					}
				}
				else
				{
					baseValueTypeFlag = 0x02;
					SerializeArrayElementFlag(baseValueTypeFlag);
					ObjectType* ot = (ObjectType*)arrayElementType;

					for (uint32_t j = 0; j < len; j++)
					{
						SerializeSubField(ot, ptr + j * (arrayType->LengthOfSingleElement));
					}
				}
			}
			else if (typeOfSubField == TYPE_OBJECT)
			{
				ObjectField* of = (ObjectField*)objectType->SubFields[i];
				ObjectType* ot = (ObjectType*)(of->GetTypeInstance());
				Serialize(ot, fieldData, of->GetFieldNumber());
			}
		}
		SerializeEndFlag();
	}
public:
	bool Deserialize(const ObjectType* objectType, void* objectPoint,uint32_t fieldNumber=0)
	{
		uint32_t fn = 0;//Popһ��KEY ��Ϊ�����ʱ���ǰ���field����ģ���������Ҫ��KEY ҪPOP����һ��
		Type_t tp = TYPE_UINT8;
		GetKeyFromSerializationManager(&fn, &tp);
		RemoveKeyFromSerializationManager();
		EmbedSerializationAssert(tp == TYPE_OBJECT);
		EmbedSerializationAssert(fieldNumber == fn);
		if (fn == 0 && tp == TYPE_OBJECT)
		{
			return DeserializeSubField(objectType, objectPoint);//���fieldNumber Ϊ0 ˵�����ǵ�һ�ν���,Ҳ��������Ľṹ�壬����Ľṹ��ִ����Ϻ�,��Ҫ�˳�
		}
		return false;
	}
private:
#define FuntionReturn(r)	if(r==false){return false;}
	bool DeserializeSubField(const ObjectType* objectType, void* objectPoint)//�������������֮ǰ ����ѽṹ���Tagȥ��
	{
		uint32_t fn = 0;
		Type_t tp = TYPE_UINT8;
		while (!IsEnd())//��û�е��ṹ�嶨����Լ� û�б�����������	&& Index < BufferLen
		{
			GetKeyFromSerializationManager(&fn, &tp);
			RemoveKeyFromSerializationManager();
			if (tp <= TYPE_DOUBLE)
			{
				const IType* typeInstance = BaseValueInfos[tp].TypeInstance;//��ȡ����Ӧ��TP
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
				FuntionReturn(Pop((uint8_t*)d, BaseValueInfos[tp].DataWidth));
			}
			else if (tp == TYPE_ARRAY)
			{
				uint64_t arraylen = 0;
				
				uint32_t sizeOfArrayLenInStream=GetArrayLenFromSerializationManager(&arraylen);
				RemoveArrayLenFromSerializationManager();

				uint8_t baseValueTypeFlag = GetArrayElementFlag();
				RemoveArrayElementFlagFromSerializationManager();

				EmbedSerializationAssert((((baseValueTypeFlag & 0x0F) == 1) && ((baseValueTypeFlag>>4) <= TYPE_DOUBLE)) || (baseValueTypeFlag == 0x02));

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
					if (arrayLenField != nullptr)//���len�ֶβ�Ϊnull �Ͱ�len���ݸ���len�ֶ�
					{
						uint8_t* arrayLenAddr = ((uint8_t*)objectPoint + arrayLenField->GetOffset());
						MEMCPY(arrayLenAddr, &arraylen, sizeOfArrayLenInStream);
					}
					if (arrayfield->IsFixed() == false)
					{
						Debug("malloc:arrayfield:%s,", arrayfield->GetName());
						ptr = MALLOC(arraylen* arrayType->LengthOfSingleElement);
						MEMCPY((void*)((uint8_t*)d), &ptr, sizeof(uint8_t*));
					}					
				}

				if ((baseValueTypeFlag & 0x0F) == 0x01)//base value type
				{
					Type_t aet = (Type_t)(baseValueTypeFlag >> 4 & 0x0F);
					EmbedSerializationAssert(aet <= TYPE_DOUBLE);
					for (uint32_t j = 0; j < arraylen; j++)
					{
						if (ptr != nullptr)
						{
							FuntionReturn(Pop((uint8_t*)ptr + j * BaseValueInfos[aet].DataWidth, BaseValueInfos[aet].DataWidth));
						}
						else
						{
							FuntionReturn(Pop(nullptr, BaseValueInfos[aet].DataWidth));
						}
					}
				}
				else
				{

					ObjectType* ot = nullptr;// (ObjectType*)(arrayType->ArrayElementType);
					if (arrayType != nullptr)
					{
						ot = (ObjectType*)(arrayType->ElementType);
					}
					for (uint32_t j = 0; j < arraylen; j++)
					{
						//����û�нṹ���tag ���Ե��õ���DeserializeSubField
						if (ptr != nullptr)
						{
							FuntionReturn(DeserializeSubField(ot, (uint8_t*)ptr + j * arrayType->LengthOfSingleElement));
						}
						else
						{
							FuntionReturn(DeserializeSubField(ot, nullptr));
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
				FuntionReturn(DeserializeSubField(subObjectType, d));//��Ϊ�ṹ���TAG�Ѿ���whileѭ����ȥ���ˡ�
			}
		}
		RemoveEndFlagFromSerializationManager();
		return true;
	}
public:
	void Free(const ObjectType* objectType, void* objectData)
	{
		for (uint32_t i = 0; i < objectType->FieldCount; i++)
		{
			if (objectType->SubFields[i]->GetTypeInstance()->GetType() == TYPE_ARRAY)
			{
				ArrayField* arrayfield = (ArrayField*)objectType->SubFields[i];
				if (arrayfield->IsFixed() == false)
				{
					IField* arrayLenField = (IField*)arrayfield->GetArrayLenField();
					uint64_t len = 1;
					if (arrayLenField != nullptr)
					{
						void* voidLenPtr = (void*)((uint8_t*)objectData + arrayLenField->GetOffset());
						EmbedSerializationAssert(arrayLenField->GetTypeInstance()->GetType() <= TYPE_INT64);
						MEMCPY(&len, voidLenPtr, BaseValueInfos[arrayLenField->GetTypeInstance()->GetType()].DataWidth);
					}

					const ArrayType* at = (const ArrayType*)arrayfield->GetTypeInstance();
					uint8_t** arrayfieldDataPoint = (uint8_t**)((uint8_t*)objectData + arrayfield->GetOffset());
					const IType* elementType = at->ElementType;
					EmbedSerializationAssert(elementType->GetType() <= TYPE_DOUBLE || elementType->GetType() == TYPE_OBJECT);

					if (elementType->GetType() == TYPE_OBJECT)//�������Ԫ���Ƕ��󣬾���freeÿ��Ԫ�أ�����ͷ�ָ��
					{
						for (uint64_t j = 0; j < len; j++)
						{
							const ObjectType* elementObjectType = (const ObjectType*)elementType;
							Free(elementObjectType, (uint8_t*)(*arrayfieldDataPoint) + j * at->LengthOfSingleElement);
						}
					}
					FREE((uint8_t*)(*arrayfieldDataPoint));
				}
			}
			else if (objectType->SubFields[i]->GetTypeInstance()->GetType() == TYPE_OBJECT)
			{
				void* fieldData = (void*)((uint8_t*)objectData + objectType->SubFields[i]->GetOffset());
				const ObjectType* subObjectType = (const ObjectType*)objectType->SubFields[i]->GetTypeInstance();
				Free(subObjectType, fieldData);
			}
		}
	}
};

#endif