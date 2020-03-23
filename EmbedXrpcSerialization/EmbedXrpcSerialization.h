#ifndef EmbedXrpcSerialization_H
#define EmbedXrpcSerialization_H

#include <stddef.h>
#include <stdint.h>
#include "stdio.h"
#include <cassert>


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
    uint32_t Index=0;
    uint8_t *Buf=nullptr;
    uint32_t BufferLen=0;
    void Append(uint32_t  Tag,Type_t   FieldType,size_t	Len,void *v)
    {
        //Buf[Index++]=Tag;
        //Buf[Index++]=FieldType;
        //Buf[Index++]=Len;
        memcpy(&Buf[Index],v,Len);
        Index +=Len;
    }
};
/*
class SerializationField_t
{
public:
    uint32_t		Tag;
    FieldType_t     FieldType;
    size_t			Len;
    uint8_t		    *Data;
};
*/
class IFieldType
{
public:
    virtual const char * GetTypeName()=0; 
    virtual Type_t GetFieldType()=0;
    virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v, uint64_t arrayLen)
    {
    }
    virtual void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)
    {
    }
    virtual void Deserialize(SerializationManager& manager, void* v)
    {

    }
	
    virtual void Deserialize(SerializationManager& manager, void* v, uint64_t arrayLen)
    {
    
    }
	
    //field ²¿·Ö

	virtual IFieldType* GetArrayLenField() { return nullptr; }
    virtual uint8_t GetArrayLenFieldLen() { return 0; }
	virtual uint32_t GetOffset() { return 0; }
	virtual const char* GetName() = 0;
    

};

class ArrayFieldType:public IFieldType
{
public:
    IFieldType* ArrayElementFieldType;
    uint64_t ElementTypeLen;
	const char* Name;
	uint32_t Offset;
    IFieldType* ArrayLenField;
    const char * GetTypeName()
    {
        return "array";
    }
    Type_t GetFieldType()
    {
        return TYPE_ARRAY;
    }
    void Serialize(SerializationManager &manager,uint32_t  Tag,void *v,uint64_t arrayLen)
    {
        for(int i=0;i<arrayLen;i++)
        {
            uint8_t *d=(uint8_t *)v;
            ArrayElementFieldType->Serialize(manager,Tag,(void *)(d+i* ElementTypeLen));
        }
    }
	void Deserialize(SerializationManager& manager, void* v, uint64_t arrayLen)
	{
		for (int i = 0; i < arrayLen; i++)
		{
			memcpy((void*)((uint8_t)v + i * ElementTypeLen), manager.Buf, ElementTypeLen);
            manager.Index += ElementTypeLen;
		}
		
	}
    ArrayFieldType(const char* name, IFieldType* aetd, uint64_t elementTypeLen, uint32_t offset,IFieldType* alf)
	{
        ArrayElementFieldType = aetd;
		ElementTypeLen = elementTypeLen;
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
	IFieldType* GetArrayLenField()
	{
		return ArrayLenField;
	}
    uint8_t GetArrayLenFieldLen() 
    { 
        switch (ArrayLenField->GetFieldType())
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
};

class Uint8FieldType :public IFieldType
{
public:
	const char* Name;
	uint32_t Offset;
    Uint8FieldType(const char *name,uint32_t offset)
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
	const char* GetTypeName()
	{
		return "uint8_t";
	}
	Type_t GetFieldType()
	{
		return TYPE_UINT8;
	}
	void Serialize(SerializationManager& manager, uint32_t  Tag, void* v)
	{
		manager.Append(Tag, TYPE_UINT8, 1, v);
	}
    void Deserialize(SerializationManager& manager, void* v)
	{
        memcpy(v, manager.Buf, 1);
        manager.Index++;
	}
};

class ObjectFieldType :public IFieldType
{
public:
    uint32_t FieldCount;
	const char* Name;
	uint32_t Offset;
    IFieldType**SubFieldsType;
    const char * GetTypeName()
    {
        return "Object";
    }
    Type_t GetFieldType()
    {
        return TYPE_OBJECT;
    }
    void Serialize(SerializationManager &manager,uint32_t  Tag,void *v)
    {      
        for(uint32_t i=0;i<FieldCount;i++)
        {
            void* d = (void*)((uint8_t*)v + SubFieldsType[i]->GetOffset());
            if (SubFieldsType[i]->GetFieldType() != TYPE_ARRAY)
            {
                
                SubFieldsType[i]->Serialize(manager, Tag, d);
            }
            else
            {
                IFieldType* a =(IFieldType *) SubFieldsType[i]->GetArrayLenField();
                void *voidLen=(void*)((uint8_t*)v + a->GetOffset());
                ArrayFieldType* arrayfield = (ArrayFieldType*)SubFieldsType[i];
                uint64_t len=0;
                memcpy(&len, voidLen, arrayfield->GetArrayLenFieldLen());
                SubFieldsType[i]->Serialize(manager, Tag, d, len);
            }
        }
    }
    ObjectFieldType(const char* name, uint32_t fc, IFieldType* ftds[], uint32_t offset)
	{
		Offset = offset;
		Name = name;
		FieldCount = fc;
        SubFieldsType = ftds;
	}
	
	const char* GetName()
	{
		return Name;
	}
	uint32_t GetOffset()
	{
		return Offset;
	}
};

#endif