#include "EmbedSerialization.h"

#include "EmbedSerialization.Port.h"
#include <cstdarg>



const char* TypeString[] =
{
	"TYPE_UINT8",
	"TYPE_INT8",

	"TYPE_UINT16",
	"TYPE_INT16",

	"TYPE_UINT32",
	"TYPE_INT32",

	"TYPE_UINT64",
	"TYPE_INT64",

	"TYPE_FLOAT",
	"TYPE_DOUBLE",

	"TYPE_ARRAY",   /*array*/
	"TYPE_OBJECT",  /*object*/
};



const char* ArrayType_GetTypeName()
{
	return "array";
}
const Type_t ArrayType_GetType()
{
	return TYPE_ARRAY;
}

const uint32_t ArrayField_GetOffset(const IField* const a)
{
	return ((ArrayField*)a)->Offset;
}
const char* ArrayField_GetName(const IField* const a)
{
	return ((ArrayField*)a)->Name;
}

const IField* ArrayField_GetArrayLenField(const IField* const a)
{
	return ((ArrayField*)a)->ArrayLenField;
}

const bool ArrayField_IsFixed(const IField* const a)
{
	return ((ArrayField*)a)->_IsFixed;
}
const uint32_t ArrayField_GetFieldNumber(const IField* const a)
{
	return ((ArrayField*)a)->FieldNumber;
}
//type部分
const char* ArrayField_GetTypeName(const IField* const a)
{
	return ArrayType_GetTypeName();
}
const IType* ArrayField_GetTypeInstance(const IField* const a)
{
	return (IType*)(((ArrayField*)a)->TypeInstance);
}


//cpp file define
#if 0
#define DefineBaseValueStructInstance(prefix,dataWidth) \
		 const BaseValueType prefix##TypeInstance = { \
		.BaseClass = {.GetTypeName=prefix##Type_GetTypeName,.GetType=prefix##Type_GetType},\
		.DataWidth=dataWidth}
#endif
#define DefineBaseValueStructInstance(prefix,type,dataWidth) \
		 const BaseValueType prefix##TypeInstance = { \
		{type},\
		dataWidth}

#define DefineBaseValueStructFunction(prefix,typeName,type)	\
const char* prefix##Type_GetTypeName()\
{\
	return typeName;\
}\
const Type_t  prefix##Type_GetType()\
{\
	return type;\
}\
const uint32_t prefix##Field_GetFieldNumber(const IField * const a )  { return ((const prefix##Field* const)a)->FieldNumber; }\
const char* prefix##Field_GetName(const IField * const a)  \
{\
	return ((const prefix##Field* const)a)->Name;\
}\
const uint32_t prefix##Field_GetOffset(const IField * const a)  \
{\
	return ((const prefix##Field* const)a)->Offset;\
}\
const char* prefix##Field_GetTypeName(const IField * const a)  \
{\
	return prefix##Type_GetTypeName();\
}\
const IType * prefix##Field_GetTypeInstance(const IField * const a)	 \
{\
	return (IType *)((((const prefix##Field* const)a)->TypeInstance));\
}\
const bool prefix##Field_IsArrayLenField(const IField * const a)  { return ((const prefix##Field* const)a)->ArrayLenFieldFlag; }

DefineBaseValueStructFunction(UInt8, "uint8_t", TYPE_UINT8);
DefineBaseValueStructFunction(Int8, "int8_t", TYPE_INT8);
DefineBaseValueStructFunction(UInt16, "uint16_t", TYPE_UINT16);
DefineBaseValueStructFunction(Int16, "int16_t", TYPE_INT16);
DefineBaseValueStructFunction(UInt32, "uint32_t", TYPE_UINT32);
DefineBaseValueStructFunction(Int32, "int32_t", TYPE_INT32);
DefineBaseValueStructFunction(UInt64, "uint64_t", TYPE_UINT64);
DefineBaseValueStructFunction(Int64, "int64_t", TYPE_INT64);
DefineBaseValueStructFunction(Float, "float", TYPE_FLOAT);
DefineBaseValueStructFunction(Double, "double", TYPE_DOUBLE);

DefineBaseValueStructInstance(UInt8, TYPE_UINT8, 1);
DefineBaseValueStructInstance(Int8, TYPE_INT8, 1);
DefineBaseValueStructInstance(UInt16, TYPE_UINT16, 2);
DefineBaseValueStructInstance(Int16, TYPE_INT16, 2);
DefineBaseValueStructInstance(UInt32, TYPE_UINT32, 4);
DefineBaseValueStructInstance(Int32, TYPE_INT32, 4);
DefineBaseValueStructInstance(UInt64, TYPE_UINT64, 8);
DefineBaseValueStructInstance(Int64, TYPE_INT64, 8);
DefineBaseValueStructInstance(Float, TYPE_FLOAT, 4);
DefineBaseValueStructInstance(Double, TYPE_DOUBLE, 8);

const static BaseValueType* const BaseValueTypeInstances[] =
{
	&UInt8TypeInstance,
	&Int8TypeInstance,
	&UInt16TypeInstance,
	&Int16TypeInstance,
	&UInt32TypeInstance,
	&Int32TypeInstance,
	&UInt64TypeInstance,
	&Int64TypeInstance,
	&FloatTypeInstance,
	&DoubleTypeInstance,
};



const char* ObjectType_GetTypeName()
{
	return "Object";
}
const Type_t ObjectType_GetType()
{
	return TYPE_OBJECT;
}


const uint32_t ObjectField_GetFieldNumber(const IField* const a) {return ((ObjectField*)a)->FieldNumber; }
const char* ObjectField_GetName(const IField* const  a)
{
	return ((ObjectField*)a)->Name;
}
const uint32_t ObjectField_GetOffset(const IField* const  a)
{
	return ((ObjectField*)a)->Offset;
}

//type部分
const char* ObjectField_GetTypeName(const IField* const a)
{
	return ObjectType_GetTypeName();
}
const IType* ObjectField_GetTypeInstance(const IField* const a)
{
	return (IType*)((((ObjectField*)a)->TypeInstance));
}

//开始填充函数数组
static const char* (* const GetTypeName_Functions[])() =
{
	UInt8Type_GetTypeName,
	Int8Type_GetTypeName,

	UInt16Type_GetTypeName,
	Int16Type_GetTypeName,

	UInt32Type_GetTypeName,
	Int32Type_GetTypeName,

	UInt64Type_GetTypeName,
	Int64Type_GetTypeName,

	FloatType_GetTypeName,
	DoubleType_GetTypeName,

	ArrayType_GetTypeName,
	ObjectType_GetTypeName,
};
const char* GetTypeName(const IType* type)
{
	return GetTypeName_Functions[type->Type]();
}

Type_t GetType(const IType* type)
{
	return type->Type;
}

//getoffset
static const uint32_t(* const GetOffset_Functions[])(const IField* const a) =
{
	UInt8Field_GetOffset,
	Int8Field_GetOffset,

	UInt16Field_GetOffset,
	Int16Field_GetOffset,

	UInt32Field_GetOffset,
	Int32Field_GetOffset,

	UInt64Field_GetOffset,
	Int64Field_GetOffset,

	FloatField_GetOffset,
	DoubleField_GetOffset,

	ArrayField_GetOffset,
	ObjectField_GetOffset,
};
const uint32_t GetOffset(const IField* const field)
{
	return GetOffset_Functions[field->Type](field);
}


//GetName
static const char* (* const GetName_Functions[])(const IField* const a) =
{
	UInt8Field_GetName,
	Int8Field_GetName,

	UInt16Field_GetName,
	Int16Field_GetName,

	UInt32Field_GetName,
	Int32Field_GetName,

	UInt64Field_GetName,
	Int64Field_GetName,

	FloatField_GetName,
	DoubleField_GetName,

	ArrayField_GetName,
	ObjectField_GetName,
};
const char* GetName(const IField* const field)
{
	return GetName_Functions[field->Type](field);
}

//IsArrayLenField
static const bool(* const IsArrayLenField_Functions[])(const IField* const a) =
{
	UInt8Field_IsArrayLenField,
	Int8Field_IsArrayLenField,

	UInt16Field_IsArrayLenField,
	Int16Field_IsArrayLenField,

	UInt32Field_IsArrayLenField,
	Int32Field_IsArrayLenField,

	UInt64Field_IsArrayLenField,
	Int64Field_IsArrayLenField,

	FloatField_IsArrayLenField,
	DoubleField_IsArrayLenField,

	nullptr,
	nullptr,
};
const bool IsArrayLenField(const IField* const field)
{
	return IsArrayLenField_Functions[field->Type](field);
}

//GetArrayLenField
static const IField* (* const GetArrayLenField_Functions[])(const IField* const a) =
{
	nullptr,
	nullptr,

	nullptr,
	nullptr,

	nullptr,
	nullptr,

	nullptr,
	nullptr,

	nullptr,
	nullptr,

	ArrayField_GetArrayLenField,
	nullptr,
};
const IField* GetArrayLenField(const IField* const field)
{
	return GetArrayLenField_Functions[field->Type](field);
}


//IsFixed
static const bool(* const IsFixed_Functions[])(const IField* const a) =
{
	nullptr,
	nullptr,

	nullptr,
	nullptr,

	nullptr,
	nullptr,

	nullptr,
	nullptr,

	nullptr,
	nullptr,

	ArrayField_IsFixed,
	nullptr,
};
const bool IsFixed(const IField* const field)
{
	return IsFixed_Functions[field->Type](field);
}


//GetFieldNumber
static const uint32_t(* const GetFieldNumber_Functions[])(const IField* const a) =
{
	UInt8Field_GetFieldNumber,
	Int8Field_GetFieldNumber,

	UInt16Field_GetFieldNumber,
	Int16Field_GetFieldNumber,

	UInt32Field_GetFieldNumber,
	Int32Field_GetFieldNumber,

	UInt64Field_GetFieldNumber,
	Int64Field_GetFieldNumber,

	FloatField_GetFieldNumber,
	DoubleField_GetFieldNumber,

	ArrayField_GetFieldNumber,
	ObjectField_GetFieldNumber,
};
const uint32_t GetFieldNumber(const IField* const field)
{
	return GetFieldNumber_Functions[field->Type](field);
}

//GetTypeName
static const char* (* const GetTypeName_Field_Functions[])(const IField* const a) =
{
	UInt8Field_GetTypeName,
	Int8Field_GetTypeName,

	UInt16Field_GetTypeName,
	Int16Field_GetTypeName,

	UInt32Field_GetTypeName,
	Int32Field_GetTypeName,

	UInt64Field_GetTypeName,
	Int64Field_GetTypeName,

	FloatField_GetTypeName,
	DoubleField_GetTypeName,

	ArrayField_GetTypeName,
	ObjectField_GetTypeName,
};
const char* GetTypeName(const IField* const field)
{
	return GetTypeName_Field_Functions[field->Type](field);
}

//GetTypeInstance
static const IType* (* const GetTypeInstance_Functions[])(const IField* const a) =
{
	UInt8Field_GetTypeInstance,
	Int8Field_GetTypeInstance,

	UInt16Field_GetTypeInstance,
	Int16Field_GetTypeInstance,

	UInt32Field_GetTypeInstance,
	Int32Field_GetTypeInstance,

	UInt64Field_GetTypeInstance,
	Int64Field_GetTypeInstance,

	FloatField_GetTypeInstance,
	DoubleField_GetTypeInstance,

	ArrayField_GetTypeInstance,
	ObjectField_GetTypeInstance,
};
const IType* GetTypeInstance(const IField* const field)
{
	return GetTypeInstance_Functions[field->Type](field);
}
//到这里函数数组填充完毕

uint32_t GetSum(uint8_t* d, uint32_t len)
{
	uint32_t sum = 0;
	for (uint32_t i = 0; i < len; i++)
	{
		sum += d[i];
	}
	return sum;
}

#define ViewCharFromBuffer(ch,offset)	\
		if(BlockBufferProvider!=nullptr)\
		{\
			BlockBufferProvider->ViewChar(&ch,offset);\
		}\
		else\
		{\
			ch = Buf[Index + offset];\
		}
#define FuntionReturn(r)	if(r==false){return false;}
void SerializationManager::SerializeKey(uint32_t  FieldNumber, Type_t   Field)
{
	/*Buf[Index++] = FieldNumber;
	Buf[Index++] = Field;
	EmbedSerializationShowMessage("SerializationManager","SerializeKey FieldNumber:%d,Type:%s\n", FieldNumber, TypeString[Field]);*/
	EmbedSerializationShowMessage("SerializationManager", "SerializeKey FieldNumber:%d,Type:%s\n", FieldNumber, TypeString[Field]);
	uint32_t shiftfn = 0;
	uint32_t next_shiftfn = 0;
	if ((FieldNumber >> 3) != 0)
	{
		Buf[Index++] = (uint8_t)(0x80 | (FieldNumber << 4) | Field);
	}
	else
	{
		Buf[Index++] = (uint8_t)((FieldNumber << 4) | Field);
	}
	shiftfn = FieldNumber >> 3;
	while (shiftfn != 0)
	{
		next_shiftfn = shiftfn >> 7;
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
void SerializationManager::SerializeLen(uint32_t  Len)
{
	EmbedSerializationShowMessage("SerializationManager", "SerializeLen:%d\n", Len);
	uint32_t next_shiftlen = 0;
	do
	{
		next_shiftlen = Len >> 7;
		if (next_shiftlen == 0)
		{
			Buf[Index++] = Len & 0x7f;
			break;
		}
		else
		{
			Buf[Index++] = (uint8_t)(Len | 0x80);
		}

		Len = Len >> 7;
	} while (true);
}
void SerializationManager::SerializeEndFlag()
{
	Buf[Index++] = 0x7F;
	EmbedSerializationShowMessage("SerializationManager", "SerializeEnd\n");
}
bool SerializationManager::IsEnd()
{
	if (BlockBufferProvider != nullptr)
	{
		uint8_t end = 0;
		BlockBufferProvider->ViewChar(&end, 0);
		return end == 0x7f;
	}
	else
	{
		uint8_t* pbuf = Buf;
		return pbuf[Index] == 0x7F;
	}

}
void SerializationManager::SerializeArrayElementFlag(uint8_t flag)
{
	Buf[Index] = flag;
	Index += 1;
	EmbedSerializationShowMessage("SerializationManager", "SerializeArrayElementFlag:0x%x\n", flag);
}
void SerializationManager::SerializeValue(uint8_t  Len, void* v)//base value used
{
	Memcpy(&Buf[Index], v, Len);
	EmbedSerializationShowMessage("SerializationManager", "SerializeValue:\n");
	for (size_t i = Index; i < Len + Index; i++)
	{
		EmbedSerializationShowMessage("SerializationManager", "%d,", Buf[i]);
		if ((i - Index + 1) % 10 == 0)
		{
			EmbedSerializationShowMessage("SerializationManager", "\n");
		}
	}
	EmbedSerializationShowMessage("SerializationManager", "\n");
	Index += Len;
}
bool SerializationManager::Pop(uint8_t* out_buf, uint32_t len)
{

	if (len + Index > BufferLen)
	{
		return false;
	}
	if (BlockBufferProvider == nullptr)
	{
		if (out_buf != nullptr)
		{
				Memcpy(out_buf, &Buf[Index], len);
		}
			CalculateSum += GetSum(&Buf[Index], len);
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
void SerializationManager::Reset()
{
	Index = 0;
		ReferenceSum = 0;
		CalculateSum = 0;
	if (BlockBufferProvider != nullptr)
	{
		BlockBufferProvider->Reset();
	}
}
uint32_t SerializationManager::GetKeyFromSerializationManager(uint32_t* fn, Type_t* type)
{
	uint32_t f = 0;
	uint8_t used = 0;
	uint8_t temp = 0;
	uint8_t Elementvalue = 0;
	ViewCharFromBuffer(Elementvalue, 0);
	if (type != nullptr)
	{
		*type = (Type_t)(Elementvalue & 0x0F);
		EmbedSerializationAssert(*type <= TYPE_OBJECT);
	}
	temp = (Elementvalue >> 4) & 0x07;//先把最低三位保存起来
	used++;
	if ((Elementvalue & 0x80) != 0)
	{
		do
		{
			ViewCharFromBuffer(Elementvalue, used);
			f = ((Elementvalue & 0x7f) << (used - 1) * 7) | f;
			if ((Elementvalue & 0x80) == 0)
			{
				used++;
				break;
			}
			else
			{
				used++;
			}
		} while (true);
	}
	f = f << 3 | temp;
	if (fn != nullptr)
	{
		*fn = f;
	}
	return used;
}
void SerializationManager::RemoveKeyFromSerializationManager()
{
	uint32_t ind = GetKeyFromSerializationManager(nullptr, nullptr);
	if (BlockBufferProvider != nullptr)
		BlockBufferProvider->PopChars(nullptr, ind);
		else
			CalculateSum += GetSum(&Buf[Index], ind);
	Index += ind;
}
uint8_t SerializationManager::GetArrayLenFromSerializationManager(uint32_t* arrayLen)
{
	uint8_t used = 0;
	uint32_t al = 0;
	uint8_t Elementvalue = 0;
	do
	{
		ViewCharFromBuffer(Elementvalue, used);
		al = ((uint64_t)(Elementvalue & 0x7f) << (used * 7)) | al;
		if ((Elementvalue & 0x80) == 0)
		{
			used++;
			break;
		}
		else
		{
			used++;
		}
	} while (true);
	if (arrayLen != nullptr)
	{
		*arrayLen = al;
	}

	return used;
}
void SerializationManager::RemoveArrayLenFromSerializationManager()
{
	uint8_t ind = GetArrayLenFromSerializationManager(nullptr);
	if (BlockBufferProvider != nullptr)
		BlockBufferProvider->PopChars(nullptr, ind);
		else
			CalculateSum += GetSum(&Buf[Index], ind);
	Index += ind;
}
uint8_t SerializationManager::GetArrayElementFlag()
{
	uint8_t Elementvalue = 0;
	ViewCharFromBuffer(Elementvalue, 0);
	return Elementvalue;

}
void SerializationManager::RemoveArrayElementFlagFromSerializationManager()
{
	if (BlockBufferProvider != nullptr)
		BlockBufferProvider->GetChar(nullptr);
		else
			CalculateSum += GetSum(&Buf[Index], 1);
	Index++;
}
void SerializationManager::RemoveEndFlagFromSerializationManager()
{
	if (BlockBufferProvider != nullptr)
		BlockBufferProvider->GetChar(nullptr);
		else
			CalculateSum += GetSum(&Buf[Index], 1);
	Index++;
}
void SerializationManager::Serialize(const ObjectType* objectType, void* objectData, uint32_t fieldNumber)
{
	if (IsEnableMataDataEncode == true)
	{
		SerializeKey(fieldNumber, TYPE_OBJECT);
		SerializeSubField(objectType, objectData);
	}
	else
	{
		NoMataData_SerializeSubField(objectType, objectData);
	}
}
void SerializationManager::SerializeSubField(const ObjectType* objectType, void* objectData)
{
	for (uint32_t i = 0; i < objectType->FieldCount; i++)
	{
		void* fieldData = (void*)((uint8_t*)objectData + GetOffset(objectType->SubFields[i]));
		//EmbedSerializationShowMessage("SerializationManager","Serialize:%s\n", objectType->SubFields[i]->GetName());

		Type_t typeOfSubField = GetType(GetTypeInstance(objectType->SubFields[i]));

		if (typeOfSubField <= TYPE_DOUBLE && IsArrayLenField(objectType->SubFields[i]) == false)
		{
			BaseValueType* bvt = (BaseValueType*)GetTypeInstance(objectType->SubFields[i]);
			SerializeKey(GetFieldNumber(objectType->SubFields[i]), typeOfSubField);
			SerializeValue(bvt->DataWidth, fieldData);
		}
		else if (typeOfSubField == TYPE_ARRAY)
		{
			const IField* arrayfield = objectType->SubFields[i];
			ArrayType* arrayType = (ArrayType*)(GetTypeInstance(arrayfield));
			IType* arrayElementType = (IType*)(arrayType->ElementType);

			const IField* arrayLenField = GetArrayLenField(arrayfield);
			//EmbedSerializationAssert(arrayLenField->GetTypeInstance()->GetType() <= TYPE_INT64);
			uint8_t sizeOfLenField = 0; //like:1,2,4,8

			SerializeKey(GetFieldNumber(objectType->SubFields[i]), TYPE_ARRAY);

			uint32_t len = 1;

			if (arrayLenField != nullptr)
			{
				BaseValueType* bvt = (BaseValueType*)GetTypeInstance((IField*)arrayLenField);
				sizeOfLenField = bvt->DataWidth; //like:1,2,4,8
				void* voidLenPtr = (void*)((uint8_t*)objectData + GetOffset(arrayLenField));
				Memcpy(&len, voidLenPtr, sizeOfLenField);
			}

			uint8_t* ptr = (uint8_t*)fieldData;
			if (IsFixed(arrayfield) == false)
			{
				ptr = (*(uint8_t**)fieldData);
			}
			SerializeLen(len);

			uint8_t baseValueTypeFlag = 0;
			if (GetType(arrayElementType) <= TYPE_DOUBLE)
			{
				baseValueTypeFlag = (GetType(arrayElementType) << 4) | 0x01;
				SerializeArrayElementFlag(baseValueTypeFlag);
				for (uint32_t j = 0; j < len; j++)
				{
					BaseValueType* bvt = (BaseValueType*)arrayElementType;
					uint32_t dw = bvt->DataWidth;
					SerializeValue(dw,
						ptr + j * dw);
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
			ObjectType* ot = (ObjectType*)(GetTypeInstance((IField*)of));
			Serialize(ot, fieldData, GetFieldNumber((IField*)of));
		}
	}
	SerializeEndFlag();
}
void SerializationManager::NoMataData_SerializeSubField(const ObjectType* objectType, void* objectData)
{
	for (uint32_t i = 0; i < objectType->FieldCount; i++)
	{
		void* fieldData = (void*)((uint8_t*)objectData + GetOffset(objectType->SubFields[i]));
		EmbedSerializationShowMessage("SerializationManager", "Serialize:%s\n", GetName(objectType->SubFields[i]));
		Type_t typeOfSubField = GetType(GetTypeInstance(objectType->SubFields[i]));
		if (typeOfSubField <= TYPE_DOUBLE)
		{
			//SerializeKey(objectType->SubFields[i]->GetFieldNumber(), typeOfSubField);
			BaseValueType* bvt = (BaseValueType*)GetTypeInstance(objectType->SubFields[i]);
			SerializeValue(bvt->DataWidth, fieldData);
		}
		else if (typeOfSubField == TYPE_ARRAY)
		{
			const IField* arrayfield = objectType->SubFields[i];
			ArrayType* arrayType = (ArrayType*)(GetTypeInstance(arrayfield));
			IType* arrayElementType = (IType*)(arrayType->ElementType);

			const IField* arrayLenField = GetArrayLenField(arrayfield);
			//EmbedSerializationAssert(arrayLenField->GetTypeInstance()->GetType() <= TYPE_INT64);
			uint8_t sizeOfLenField = 0; //like:1,2,4,8

			//SerializeKey(objectType->SubFields[i]->GetFieldNumber(), TYPE_ARRAY);

			uint64_t len = 1;

			if (arrayLenField != nullptr)
			{
				BaseValueType* bvt = (BaseValueType*)GetTypeInstance(arrayLenField);
				sizeOfLenField = bvt->DataWidth; //like:1,2,4,8
				void* voidLenPtr = (void*)((uint8_t*)objectData + GetOffset(arrayLenField));
				Memcpy(&len, voidLenPtr, sizeOfLenField);
			}

			uint8_t* ptr = (uint8_t*)fieldData;
			if (IsFixed(arrayfield) == false)
			{
				ptr = (*(uint8_t**)fieldData);
			}
			//SerializeLen(len);

			uint8_t baseValueTypeFlag = 0;
			if (GetType(arrayElementType) <= TYPE_DOUBLE)
			{
				baseValueTypeFlag = (GetType(arrayElementType) << 4) | 0x01;
				//SerializeArrayElementFlag(baseValueTypeFlag);
				BaseValueType* bvt = (BaseValueType*)arrayElementType;
				uint32_t dw = bvt->DataWidth;
				for (uint32_t j = 0; j < len; j++)
				{
					SerializeValue(dw, ptr + j * dw);
				}
			}
			else
			{
				baseValueTypeFlag = 0x02;
				//SerializeArrayElementFlag(baseValueTypeFlag);
				ObjectType* ot = (ObjectType*)arrayElementType;

				for (uint32_t j = 0; j < len; j++)
				{
					NoMataData_SerializeSubField(ot, ptr + j * (arrayType->LengthOfSingleElement));
				}
			}
		}
		else if (typeOfSubField == TYPE_OBJECT)
		{
			const IField* of = objectType->SubFields[i];
			ObjectType* ot = (ObjectType*)(GetTypeInstance(of));
			Serialize(ot, fieldData, GetFieldNumber(of));
		}
	}
	//SerializeEndFlag();
}
bool SerializationManager::Deserialize(const ObjectType* objectType, void* objectPoint, uint32_t fieldNumber )
{
	if (IsEnableMataDataEncode == true)
	{
		uint32_t fn = 0;//Pop一次KEY 因为打包的时候是按照field打包的，所以这里要把KEY 要POP出来一次
		Type_t tp = TYPE_UINT8;
		GetKeyFromSerializationManager(&fn, &tp);
		RemoveKeyFromSerializationManager();
		EmbedSerializationAssert(tp == TYPE_OBJECT);
		EmbedSerializationAssert(fieldNumber == fn);
		if (fn == 0 && tp == TYPE_OBJECT)
		{
			return DeserializeSubField(objectType, objectPoint);//如果fieldNumber 为0 说明这是第一次进来,也就是最顶级的结构体，最顶级的结构体执行完毕后,就要退出
		}
	}
	else
	{
		return NoMataData_DeserializeSubField(objectType, objectPoint);
	}
	return false;
}
bool SerializationManager::DeserializeSubField(const ObjectType* objectType, void* objectPoint)//调用这个函数的之前 必须把结构体的Tag去掉
{
	uint32_t fn = 0;
	Type_t tp = TYPE_UINT8;
	while (!IsEnd())//并没有到结构体定界符以及 没有遍历完数据流	&& Index < BufferLen
	{
		GetKeyFromSerializationManager(&fn, &tp);
		RemoveKeyFromSerializationManager();
		if (tp <= TYPE_DOUBLE)
		{
			//const IType* typeInstance = BaseValueInfos[tp].TypeInstance;//获取到对应的TP
			void* d = nullptr;
			BaseValueType* bvt = nullptr;
			if (objectType != nullptr)
			{
				for (uint32_t i = 0; i < objectType->FieldCount; i++)
				{
					bvt = (BaseValueType*)GetTypeInstance(objectType->SubFields[i]);
					if (fn == GetFieldNumber(objectType->SubFields[i]) && tp == GetType((IType*)bvt))
					{
						d = (void*)((uint8_t*)objectPoint + GetOffset(objectType->SubFields[i]));
						EmbedSerializationShowMessage("SerializationManager", "Deserialize:%s\n", GetName(objectType->SubFields[i]));
						break;
					}
				}
			}
			FuntionReturn(Pop((uint8_t*)d, bvt->DataWidth));
		}
		else if (tp == TYPE_ARRAY)
		{
			uint32_t arraylen = 0;

			uint8_t sizeOfArrayLenInStream = GetArrayLenFromSerializationManager(&arraylen);
			RemoveArrayLenFromSerializationManager();

			uint8_t baseValueTypeFlag = GetArrayElementFlag();
			RemoveArrayElementFlagFromSerializationManager();

			EmbedSerializationAssert((((baseValueTypeFlag & 0x0F) == 1) && ((baseValueTypeFlag >> 4) <= TYPE_DOUBLE)) || (baseValueTypeFlag == 0x02));

			void* d = nullptr;
			const IField* arrayfield = nullptr;
			ArrayType* arrayType = nullptr;
			void* ptr = d;
			IField* arrayLenField = nullptr;

			if (objectType != nullptr)
			{
				for (uint32_t i = 0; i < objectType->FieldCount; i++)
				{
					if (fn == GetFieldNumber(objectType->SubFields[i]) && tp == GetType(GetTypeInstance(objectType->SubFields[i])))
					{
						d = (void*)((uint8_t*)objectPoint + GetOffset(objectType->SubFields[i]));
						ptr = d;
						arrayfield = objectType->SubFields[i];
						arrayType = (ArrayType*)(GetTypeInstance(arrayfield));
						EmbedSerializationShowMessage("SerializationManager", "Deserialize:%s\n", GetName(objectType->SubFields[i]));
						break;
					}
				}
			}
			if (arrayfield != nullptr)
			{
				arrayLenField = (IField*)GetArrayLenField(arrayfield);
				if (arrayLenField != nullptr)//如果len字段不为null 就把len数据赋给len字段
				{
					uint8_t* arrayLenAddr = ((uint8_t*)objectPoint + GetOffset(arrayLenField));
					Memcpy(arrayLenAddr, &arraylen, sizeOfArrayLenInStream);
				}
				if (IsFixed(arrayfield) == false)
				{
					EmbedSerializationShowMessage("SerializationManager", "malloc:arrayfield:%s,", GetName(arrayfield));
					ptr = Malloc(arraylen * arrayType->LengthOfSingleElement);
					Memcpy((void*)((uint8_t*)d), &ptr, sizeof(uint8_t*));
				}
			}

			if ((baseValueTypeFlag & 0x0F) == 0x01)//base value type
			{
				Type_t aet = (Type_t)(baseValueTypeFlag >> 4 & 0x0F);
				EmbedSerializationAssert(aet <= TYPE_DOUBLE);
				const BaseValueType* bvt = BaseValueTypeInstances[aet];
				uint32_t dw = bvt->DataWidth;
				for (uint64_t j = 0; j < arraylen; j++)
				{
					if (ptr != nullptr)
					{
						FuntionReturn(Pop((uint8_t*)ptr + j * dw, dw));
					}
					else
					{
						FuntionReturn(Pop(nullptr, dw));
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
				for (uint64_t j = 0; j < arraylen; j++)
				{
					//这里没有结构体的tag 所以调用的是DeserializeSubField
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
			const IField* subObjectField = nullptr;
			ObjectType* subObjectType = nullptr;
			if (objectType != nullptr)
			{
				for (uint32_t i = 0; i < objectType->FieldCount; i++)
				{
					if (fn == GetFieldNumber(objectType->SubFields[i]) && tp == GetType(GetTypeInstance(objectType->SubFields[i])))
					{
						d = (void*)((uint8_t*)objectPoint + GetOffset(objectType->SubFields[i]));
						subObjectField = objectType->SubFields[i];
						subObjectType = (ObjectType*)GetTypeInstance(subObjectField);
						EmbedSerializationShowMessage("SerializationManager", "Deserialize:%s\n", GetName(objectType->SubFields[i]));
						break;
					}
				}
			}
			FuntionReturn(DeserializeSubField(subObjectType, d));//因为结构体的TAG已经在while循环处去掉了。
		}
	}
	RemoveEndFlagFromSerializationManager();
	return true;
}
bool SerializationManager::NoMataData_DeserializeSubField(const ObjectType* objectType, void* objectPoint)
{
	void* d = nullptr;
	Type_t tp = TYPE_UINT8;
	for (uint32_t i = 0; i < objectType->FieldCount; i++)
	{
		tp = GetType(GetTypeInstance(objectType->SubFields[i]));
		d = (void*)((uint8_t*)objectPoint + GetOffset(objectType->SubFields[i]));
		if (tp <= TYPE_DOUBLE)
		{
			EmbedSerializationShowMessage("SerializationManager", "Deserialize:%s\n", GetName(objectType->SubFields[i]));
			FuntionReturn(Pop((uint8_t*)d, BaseValueTypeInstances[tp]->DataWidth));
		}
		else if (tp == TYPE_ARRAY)
		{
			const IField* arrayfield = nullptr;
			ArrayType* arrayType = nullptr;
			void* ptr = d;
			uint32_t arraylen = 1;

			ptr = d;
			arrayfield = objectType->SubFields[i];
			arrayType = (ArrayType*)(GetTypeInstance(arrayfield));
			EmbedSerializationShowMessage("SerializationManager", "Deserialize:%s\n", GetName(objectType->SubFields[i]));

			IField* arrayLenField = (IField*)GetArrayLenField(arrayfield);
			if (arrayLenField != nullptr)//如果len字段不为null 就把len数据赋给len字段
			{
				uint8_t* arrayLenAddr = ((uint8_t*)objectPoint + GetOffset(arrayLenField));
				Memcpy(&arraylen, arrayLenAddr, BaseValueTypeInstances[GetType(GetTypeInstance(arrayLenField))]->DataWidth);
			}
			if (IsFixed(arrayfield) == false)
			{
				EmbedSerializationShowMessage("SerializationManager", "malloc:arrayfield:%s,", GetName(arrayfield));
				ptr = Malloc(arraylen * arrayType->LengthOfSingleElement);
				Memcpy((void*)((uint8_t*)d), &ptr, sizeof(uint8_t*));
			}

			Type_t aet = GetType(arrayType->ElementType);
			if (aet <= TYPE_DOUBLE)//base value type
			{
				for (uint64_t j = 0; j < arraylen; j++)
				{
					if (ptr != nullptr)
					{
						FuntionReturn(Pop((uint8_t*)ptr + j * BaseValueTypeInstances[aet]->DataWidth, BaseValueTypeInstances[aet]->DataWidth));
					}
					else
					{
						FuntionReturn(Pop(nullptr, BaseValueTypeInstances[aet]->DataWidth));
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
				for (uint64_t j = 0; j < arraylen; j++)
				{
					//这里没有结构体的tag 所以调用的是DeserializeSubField
					if (ptr != nullptr)
					{
						FuntionReturn(NoMataData_DeserializeSubField(ot, (uint8_t*)ptr + j * arrayType->LengthOfSingleElement));
					}
					else
					{
						FuntionReturn(NoMataData_DeserializeSubField(ot, nullptr));
					}
				}
			}

		}
		else if (tp == TYPE_OBJECT)
		{
			IField* subObjectField = nullptr;
			ObjectType* subObjectType = nullptr;
			subObjectField = (IField*)objectType->SubFields[i];
			subObjectType = (ObjectType*)GetTypeInstance(subObjectField);
			FuntionReturn(NoMataData_DeserializeSubField(subObjectType, d));
		}
	}
	return true;
}
void SerializationManager::FreeData(const ObjectType* objectType, void* objectData)
{
	for (uint32_t i = 0; i < objectType->FieldCount; i++)
	{
		if (GetType(GetTypeInstance(objectType->SubFields[i])) == TYPE_ARRAY)
		{
			const IField* arrayfield = objectType->SubFields[i];
			if (IsFixed(arrayfield) == false)
			{
				IField* arrayLenField = (IField*)GetArrayLenField(arrayfield);
				uint64_t len = 1;
				if (arrayLenField != nullptr)
				{
					void* voidLenPtr = (void*)((uint8_t*)objectData + GetOffset(arrayLenField));
					EmbedSerializationAssert(GetType(GetTypeInstance(arrayLenField)) <= TYPE_INT64);
					Memcpy(&len, voidLenPtr, BaseValueTypeInstances[GetType(GetTypeInstance(arrayLenField))]->DataWidth);
				}

				const ArrayType* at = (const ArrayType*)GetTypeInstance(arrayfield);
				uint8_t** arrayfieldDataPoint = (uint8_t**)((uint8_t*)objectData + GetOffset(arrayfield));
				const IType* elementType = at->ElementType;
				EmbedSerializationAssert(GetType(elementType) <= TYPE_DOUBLE || GetType(elementType) == TYPE_OBJECT);

				if (GetType(elementType) == TYPE_OBJECT)//如果数组元素是对象，就先free每个元素，最后释放指针
				{
					for (uint64_t j = 0; j < len; j++)
					{
						const ObjectType* elementObjectType = (const ObjectType*)elementType;
						FreeData(elementObjectType, (uint8_t*)(*arrayfieldDataPoint) + j * at->LengthOfSingleElement);
					}
				}
				Free((uint8_t*)(*arrayfieldDataPoint));
			}
		}
		else if (GetType(GetTypeInstance(objectType->SubFields[i])) == TYPE_OBJECT)
		{
			void* fieldData = (void*)((uint8_t*)objectData + GetOffset(objectType->SubFields[i]));
			const ObjectType* subObjectType = (const ObjectType*)GetTypeInstance(objectType->SubFields[i]);
			FreeData(subObjectType, fieldData);
		}
	}
}



static const char* FilterStrings[] = FilterStringHeader;
void EmbedSerializationShowMessage(const char* filter_string, const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	for (int i = 0; i < sizeof(FilterStrings) / sizeof(const char*); i++)
	{
		if (strcmp(FilterStrings[i], filter_string) == 0)
		{
			StringOutput(fmt, args);
		}
	}
	va_end(args);
}