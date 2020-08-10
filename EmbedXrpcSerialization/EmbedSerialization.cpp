#include "EmbedSerialization.h"
DefineBaseValueInstance(UInt8);
DefineBaseValueInstance(Int8);
DefineBaseValueInstance(UInt16);
DefineBaseValueInstance(Int16);
DefineBaseValueInstance(UInt32);
DefineBaseValueInstance(Int32);
DefineBaseValueInstance(UInt64);
DefineBaseValueInstance(Int64);
DefineBaseValueInstance(Float);
DefineBaseValueInstance(Double);

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
const BaseValueInfo BaseValueInfos[] =
{
	{&UInt8TypeInstance,1},
	{&Int8TypeInstance,1},
	{&UInt16TypeInstance,2},
	{&Int16TypeInstance,2},
	{&UInt32TypeInstance,4},
	{&Int32TypeInstance,4},
	{&UInt64TypeInstance,8},
	{&Int64TypeInstance,8},
	{&FloatTypeInstance,4},
	{&DoubleTypeInstance,8},
};
