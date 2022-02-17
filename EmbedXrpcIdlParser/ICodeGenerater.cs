﻿
using CSScriptLibrary;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{

    public static class StringExtensions
    {
        public static int GetCharCount(this string t, char a)
        {
            int cnt = 0;
            for (int i = 0; i < t.Length; i++)
            {
                if (t[i] == a)
                {
                    cnt++;
                }
            }
            return cnt;
        }
    }
    public enum TargetType_t
    {
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

        TYPE_ENUM,

        TYPE_ARRAY,   /*array*/

        TYPE_STRUCT,  /*struct*/
        //TYPE_UNION,
    }
    public interface ITargetType
    {
        TargetType_t TargetType { get;  }
        string TypeName { get; set; }
        bool IsNeedFreeMemoryForNativeLanguage { get; }
    }
    public class BaseType_TargetType:ITargetType
    {
        public TargetType_t TargetType { get; private set; }
        public string TypeName { get; set; }

        public static Dictionary<TargetType_t, string> TypeReplaceDic = new Dictionary<TargetType_t, string>();
        public static BaseType_TargetType GetInstance(Dictionary<string, ITargetType> targetTypes, TargetType_t tt)
        {
            BaseType_TargetType instance = null;
            if (targetTypes.ContainsKey(TypeReplaceDic[tt])==false)
            {
                instance = new BaseType_TargetType();
                instance.TargetType = tt;
                instance.TypeName= TypeReplaceDic[tt];
                targetTypes.Add(TypeReplaceDic[tt], instance);
            }
            else
            {
                instance = targetTypes[TypeReplaceDic[tt]] as BaseType_TargetType;
            }
            return instance;
        }
        static BaseType_TargetType()
        {
            TypeReplaceDic.Add(TargetType_t.TYPE_UINT8, "UInt8");
            TypeReplaceDic.Add(TargetType_t.TYPE_INT8, "Int8");
            TypeReplaceDic.Add(TargetType_t.TYPE_UINT16, "UInt16");
            TypeReplaceDic.Add(TargetType_t.TYPE_INT16, "Int16");
            TypeReplaceDic.Add(TargetType_t.TYPE_UINT32, "UInt32");
            TypeReplaceDic.Add(TargetType_t.TYPE_INT32, "Int32");
            TypeReplaceDic.Add(TargetType_t.TYPE_UINT64, "UInt64");
            TypeReplaceDic.Add(TargetType_t.TYPE_INT64, "Int64");
            TypeReplaceDic.Add(TargetType_t.TYPE_FLOAT, "Float");
            TypeReplaceDic.Add(TargetType_t.TYPE_DOUBLE, "Double");
        }
        public bool IsNeedFreeMemoryForNativeLanguage
        {
            get
            {
                return false;
            }
        }
    }
    public class EnumType_TargetType: ITargetType
    {
        //public string TypeName { get; set; }
        public TargetType_t NumberType { get; set; }

        public Dictionary< string, int> KeyValue { get; set; } = new Dictionary< string, int>();

        public TargetType_t TargetType { get; set; } = TargetType_t.TYPE_ENUM;

        public string TypeName { get; set; }
        public bool IsNeedFreeMemoryForNativeLanguage
        {
            get
            {
                return false;
            }
        }

    }
    public class ArrayType_TargetType:ITargetType
    {
        public TargetType_t TargetType { get;private set; } = TargetType_t.TYPE_ARRAY;
        public string TypeName { get; set; }
        public ITargetType ElementType { get; set; }
        public bool? _IsNeedFreeMemoryForNativeLanguage;
        public bool IsNeedFreeMemoryForNativeLanguage
        {
            get
            {
                if(_IsNeedFreeMemoryForNativeLanguage ==null)
                {
                    Debug.WriteLine($"{TypeName}._IsNeedFreeMemoryForNativeLanguage未缓存!");
                    _IsNeedFreeMemoryForNativeLanguage = ElementType.IsNeedFreeMemoryForNativeLanguage;
                }
                else
                {
                    Debug.WriteLine($"{TypeName}._IsNeedFreeMemoryForNativeLanguage已缓存!");
                }
                return (bool)_IsNeedFreeMemoryForNativeLanguage;
            }
        }
    }

    public class StructType_TargetType:ITargetType
    {
        public TargetType_t TargetType { get;private set; } = TargetType_t.TYPE_STRUCT;
        public string TypeName { get; set; }
        public List<ITargetField> TargetFields { get; set; } = new List<ITargetField>();
        public Base_TargetField GetArrayLenField(Array_TargetField arrayField)
        {
            foreach (var f in TargetFields)
            {
                if (f.FieldName == arrayField.MaxCountAttribute.LenFieldName)
                {
                    var v= f as Base_TargetField;
                    //v.IsArrayLenField = true;//更新字段Base_TargetField的IsArrayLenField标志量
                    return v;
                }
            }
            if (arrayField.MaxCountAttribute.LenFieldName != string.Empty)
            {
                throw new Exception($"{TypeName}结构体类型中,表示数组字段{arrayField.FieldName}元素个数的字段名未找到!"); 
            }
            return null;
        }
        public ITargetField UnionTargetTypeField { get; set; }

        public List<CppCustomMethodSignatureAttribute> CppCustomMethodSignatures { get; set; }

        public bool? _IsNeedFreeMemoryForNativeLanguage;
        public bool IsNeedFreeMemoryForNativeLanguage
        {
            get
            {
                if (_IsNeedFreeMemoryForNativeLanguage != null)
                {
                    Debug.WriteLine($"{TypeName}._IsNeedFreeMemoryForNativeLanguage已缓存!");
                    return (bool)_IsNeedFreeMemoryForNativeLanguage;
                }
                Debug.WriteLine($"{TypeName}._IsNeedFreeMemoryForNativeLanguage未缓存!");
                foreach (var targetField in TargetFields)
                {
                    if (targetField.NoSerializationAttr != null)
                        continue;
                    if (targetField is Array_TargetField)
                    {
                        Array_TargetField array_TargetField = targetField as Array_TargetField;
                        if(array_TargetField.MaxCountAttribute.IsFixed==false)
                        {
                            _IsNeedFreeMemoryForNativeLanguage = true;
                            return true;
                        }
                        ArrayType_TargetType arrayType_TargetType = array_TargetField.TargetType as ArrayType_TargetType;
                        ITargetType element = arrayType_TargetType.ElementType;
                        if(element.IsNeedFreeMemoryForNativeLanguage==true)
                        {
                            _IsNeedFreeMemoryForNativeLanguage = true;
                            return true;
                        }
                    }
                    if(targetField is Struct_TargetField)
                    {
                        Struct_TargetField struct_TargetField = targetField as Struct_TargetField;
                        if (struct_TargetField.TargetType.IsNeedFreeMemoryForNativeLanguage == true)
                        {
                            _IsNeedFreeMemoryForNativeLanguage = true;
                            return true;
                        }
                    }
                }
                _IsNeedFreeMemoryForNativeLanguage = false;
                return false;
            }
        }
    }


    public interface ITargetField
    {
        ITargetType TargetType { get; set; }
        string FieldName { get; set; }
        FieldNumberAttribute FieldNumberAttr { get; set; }
        UnionFieldAttribute UnionFieldAttr { get; set; }
        NoSerializationAttribute NoSerializationAttr { get; set; }
        MacroControlAttribute MacroControlAttr { get; set; }
        


    }

    public class Base_TargetField : ITargetField
    {
        public ITargetType TargetType { get; set; }

        public string FieldName { get; set; }

        public FieldNumberAttribute FieldNumberAttr { get; set; }

        public bool IsArrayLenField { get; set; } = false;//是否是数组长度字段
        public bool IsUnionTargetTypeField { get; set; } = false;//是否是target type field类型。
        public UnionFieldAttribute UnionFieldAttr { get; set; }
        public NoSerializationAttribute NoSerializationAttr { get; set; }
        public BitFieldAttribute BitFieldAttribute { get; set; }
        public MacroControlAttribute MacroControlAttr { get; set; }
        

    }
    public class Enum_TargetField: Base_TargetField
    {
               
    }

    public class Array_TargetField : ITargetField
    {
        public ITargetType TargetType { get; set; }
        public FieldNumberAttribute FieldNumberAttr { get; set; }

        public string FieldName { get; set; }

        public MaxCountAttribute MaxCountAttribute { get; set; }

        public Base_TargetField ArrayLenField { get; set; }
        public UnionFieldAttribute UnionFieldAttr { get; set; }
        public NoSerializationAttribute NoSerializationAttr { get; set; }
        public MacroControlAttribute MacroControlAttr { get; set; }
        

    }
    public class Struct_TargetField : ITargetField
    {
        public ITargetType TargetType { get; set; }

        public string FieldName { get; set; }

        public FieldNumberAttribute FieldNumberAttr { get; set; }
        public UnionFieldAttribute UnionFieldAttr { get; set; }
        public NoSerializationAttribute NoSerializationAttr { get; set; }
        public MacroControlAttribute MacroControlAttr { get; set; }
    }
    /*public class Union_TargetField:ITargetField
    {
        public ITargetType TargetType { get; set; }

        public string FieldName { get; set; }

        public FieldNumberAttribute FieldNumberAttr { get; set; }

        //public int TypeValue { get; set; }

    }*/
    [Serializable]
    public class TargetService
    {
        public StructType_TargetType ReturnStructType { get; set; } = new StructType_TargetType();
        public string ServiceName { get; set; }
        
        public RoleAttribute RoleAttribute { get; internal set; }
        public StructType_TargetType ParameterStructType { get; set; } = new StructType_TargetType();
        public int ServiceId { get; internal set; }

        public ExternalParameterAttribute ExternalParameter { get; internal set;}
        public MacroControlAttribute MacroControlAttribute { get; internal set; }

    }
    /// <summary>
    /// 本文件中所有的资源
    /// </summary>
    public class FileIdlInfo
    {
        public string FileName { get; set; } = string.Empty;
        public List<EnumType_TargetType> TargetEnums { get; set; } = new List<EnumType_TargetType>();
        public List<ArrayType_TargetType> TargetArrayTypes { get; set; } = new List<ArrayType_TargetType>();
        public List<StructType_TargetType> TargetStructs { get; set; } = new List<StructType_TargetType>();
        public List<TargetService> TargetServices { get; set; } = new List<TargetService>();
        public GenerationOption GenerationOption = null;
        public int ServiceId { get; set; }
    }
    public class IdlInfo
    {          
        private TargetType_t ClrBaseValueTypeToTargetType_t(Type t)
        {
            if (t == typeof(bool))
            {
                return TargetType_t.TYPE_UINT8;
            }
            if (t == typeof(byte))
            {
                return TargetType_t.TYPE_UINT8;                
            }
            if (t == typeof(sbyte))
            {
                return TargetType_t.TYPE_INT8;
                
            }
            if (t == typeof(UInt16))
            {
                return TargetType_t.TYPE_UINT16;
  
            }
            if (t == typeof(Int16))
            {
                return TargetType_t.TYPE_INT16;

            }
            if (t == typeof(UInt32))
            {
                return TargetType_t.TYPE_UINT32;

            }
            if (t == typeof(Int32))
            {
                return TargetType_t.TYPE_INT32;

            }
            if (t == typeof(UInt64))
            {
                return TargetType_t.TYPE_UINT64;

            }
            if (t == typeof(Int64))
            {
                return TargetType_t.TYPE_INT64;

            }
            throw new Exception();
        }
        private  bool IsNumberType(Type t)
        {
            bool r = false;
            if(t==typeof(bool))
            {
                return true;
            }
            if (t==typeof(byte))
            {
                
                r = true;
            }
            if (t == typeof(sbyte))
            {
               
                r = true;
            }
            if (t == typeof(UInt16))
            {
                
                r = true;
            }
            if (t == typeof(Int16))
            {
               
                r = true;
            }
            if (t == typeof(UInt32))
            {
                
                r = true;
            }
            if (t == typeof(Int32))
            {
                
                r = true;
            }
            if (t == typeof(UInt64))
            {
                
                r = true;
            }
            if (t == typeof(Int64))
            {
                
                r = true;
            }
            return r;
        }

        public bool IsEnumType(Type t)
        {
            if(t.IsEnum==true)
            {
                return true;
            }
            return false;
        }
        public List< FileIdlInfo> ParsedFiles { get; private set; } = new List< FileIdlInfo>();//已经被Parse过的file
        public Dictionary<string, ITargetType> TargetTypes = new Dictionary<string, ITargetType>();
        public  IdlInfo()
        {
            EnumType_TargetType ettt = null;
            ettt = new EnumType_TargetType();
            ettt.TypeName = "RequestResponseState";
            ettt.NumberType = TargetType_t.TYPE_UINT8;
            TargetTypes.Add(ettt.TypeName, ettt);
        }
        public FileIdlInfo GetFileIdlInfo(string fileName)
        {
            foreach (var fil in ParsedFiles)
            {
                if(fil.FileName==fileName)
                {
                    return fil;
                }
            }
            throw new NotSupportedException();
        }
        private class BitFieldStateMachine
        {

            internal ITargetType BitFieldBaseTargetType { get; set; } = null;
        }
        private StructType_TargetType StructTypeParse(Type object_type)
        {
            StructType_TargetType targetStructType = null;
            if (TargetTypes.ContainsKey(object_type.Name) == false)
            {
                targetStructType = new StructType_TargetType();
                targetStructType.TypeName = object_type.Name;
                TargetTypes.Add(targetStructType.TypeName, targetStructType);
            }
            else
            {
                targetStructType = TargetTypes[object_type.Name] as StructType_TargetType;
                return targetStructType;//如果找到了同一个类型,就不用解析了,直接返回了
            }
            var fields = object_type.GetFields(BindingFlags.NonPublic | BindingFlags.Instance);
            //var unionatt = object_type.GetCustomAttribute<UnionAttribute>();
            List<int> FieldNumbers = new List<int>();
            int fieldNumber = 1;
            BitFieldStateMachine bitFieldStateMachine = new BitFieldStateMachine();
            List<CppCustomMethodSignatureAttribute> cppCustomMethodSignatures = new List<CppCustomMethodSignatureAttribute>(object_type.GetCustomAttributes<CppCustomMethodSignatureAttribute>());
            targetStructType.CppCustomMethodSignatures = cppCustomMethodSignatures;

            bool isWriteUnion = false;
            //bool IsUnionComplete = false;

            for (int field_index=0; field_index<fields.Length;field_index++)
            {
                var field = fields[field_index];
                var FieldNumberAttr = field.GetCustomAttribute<FieldNumberAttribute>();
                ITargetField needAddField = null;
                if (FieldNumberAttr == null)
                {
                    FieldNumberAttr = new FieldNumberAttribute(fieldNumber);//如果没有fieldNumber说明用户并不打算使用TTLV编码
                    //throw new NotImplementedException($"the {field.Name}'s FieldNumberAttr is null");     
                }
                while (FieldNumbers.Contains(FieldNumberAttr.Number))
                {
                    fieldNumber++;
                    FieldNumberAttr.Number = fieldNumber;
                }
                FieldNumbers.Add(FieldNumberAttr.Number);
                if (IsNumberType(field.FieldType) == true)
                {
                    Base_TargetField targetfield = new Base_TargetField();
                    targetfield.TargetType = BaseType_TargetType.GetInstance(TargetTypes,ClrBaseValueTypeToTargetType_t(field.FieldType));
                    targetfield.FieldName = field.Name;
                    targetfield.FieldNumberAttr = FieldNumberAttr;
                    var arrayLenatt = field.GetCustomAttribute<MaxCountAttribute>();
                    if(arrayLenatt!=null)
                    {
                        targetfield.IsArrayLenField = true;
                    }
                    var unionTargetTypeAtt = field.GetCustomAttribute<UnionTargetTypeAttribute>();
                    targetfield.UnionFieldAttr= field.GetCustomAttribute<UnionFieldAttribute>();
                    targetfield.NoSerializationAttr= field.GetCustomAttribute<NoSerializationAttribute>();
                    targetfield.MacroControlAttr = field.GetCustomAttribute<MacroControlAttribute>();
                    targetfield.BitFieldAttribute = field.GetCustomAttribute<BitFieldAttribute>();
                    if(targetfield.BitFieldAttribute!=null)
                    {
                        if (bitFieldStateMachine.BitFieldBaseTargetType != null)//这不是第一个bitfield
                        {
                            if (targetfield.TargetType.TargetType != bitFieldStateMachine.BitFieldBaseTargetType.TargetType)
                            {
                                throw new Exception("所有相邻的位域字段的类型应该要一样!");
                            }
                            else
                            {
                                ;
                            }
                        }
                        else
                        {
                            ;
                        }
                        bitFieldStateMachine.BitFieldBaseTargetType = targetfield.TargetType;
                        if(bitFieldStateMachine.BitFieldBaseTargetType.TargetType!= TargetType_t.TYPE_UINT8
                            && bitFieldStateMachine.BitFieldBaseTargetType.TargetType != TargetType_t.TYPE_UINT16
                            && bitFieldStateMachine.BitFieldBaseTargetType.TargetType != TargetType_t.TYPE_UINT32
                            && bitFieldStateMachine.BitFieldBaseTargetType.TargetType != TargetType_t.TYPE_UINT64)
                        {
                            throw new Exception($"当前位域字段类型:{bitFieldStateMachine.BitFieldBaseTargetType.TargetType},位域字段只能用UINT8/UINT16/UINT32/UINT64类型");
                        }
                        if (targetfield.NoSerializationAttr != null)
                            throw new Exception("不支持BitField&NoSerialization组合");
                        if(targetfield.UnionFieldAttr!=null)
                            throw new Exception("不支持BitField&UnionField组合");
                        if (targetfield.MacroControlAttr != null)
                            throw new Exception("不支持BitField&MacroControl的组合");
                    }
                    else
                    {
                        
                        bitFieldStateMachine.BitFieldBaseTargetType = null;
                    }
                    if (unionTargetTypeAtt != null)
                    {
                        targetfield.IsUnionTargetTypeField = true;
                        targetStructType.UnionTargetTypeField = targetfield;
                        if (targetfield.MacroControlAttr != null)
                        {
                            throw new Exception("union的Type标识不支持MacroControl");
                        }
                    }
                    needAddField = targetfield;
                    //targetStruct.TargetFields.Add(targetfield);
                }
                else if (IsEnumType(field.FieldType) == true)
                {
                    Enum_TargetField enumField = new Enum_TargetField();
                    enumField.FieldName = field.Name;
                    enumField.FieldNumberAttr = FieldNumberAttr;
                    //有可能还没有把该枚举类型添加到集合里,所以要添加一下
                    var vs = field.FieldType.GetEnumValues();
                    EnumType_TargetType te = null;
                    if(TargetTypes.ContainsKey(field.FieldType.Name)==false)
                    {
                        te = new EnumType_TargetType();
                        te.TypeName = field.FieldType.Name;//类型名称
                        te.NumberType = ClrBaseValueTypeToTargetType_t(field.FieldType.GetEnumUnderlyingType());
                        foreach (var vsv in vs)
                        {
                            te.KeyValue.Add(field.FieldType.GetEnumName(vsv), Convert.ToInt32(vsv));
                        }
                        TargetTypes.Add(te.TypeName, te);
                    }
                    else
                    {
                        te = TargetTypes[field.FieldType.Name] as EnumType_TargetType;
                    }
                    
                    //添加完毕
                    enumField.TargetType = te;
                    enumField.UnionFieldAttr = field.GetCustomAttribute<UnionFieldAttribute>();
                    enumField.NoSerializationAttr = field.GetCustomAttribute<NoSerializationAttribute>();
                    enumField.MacroControlAttr = field.GetCustomAttribute<MacroControlAttribute>();
                    needAddField = enumField;
                }
                else if (field.FieldType.IsArray == true)
                {
                    
                    //new 类型
                    ArrayType_TargetType attt =null;
                    if(TargetTypes.ContainsKey(field.FieldType.Name)==false)
                    {
                        attt = new ArrayType_TargetType();
                        attt.TypeName = field.FieldType.Name;
                        if (IsNumberType(field.FieldType.GetElementType()) == true)
                        {
                            attt.ElementType = BaseType_TargetType.GetInstance(TargetTypes, ClrBaseValueTypeToTargetType_t(field.FieldType.GetElementType()));
                        }
                        else
                        {
                            attt.ElementType = StructTypeParse(field.FieldType.GetElementType());//如果list中没有targetstruct 那就去parse
                        }
                        TargetTypes.Add(attt.TypeName, attt);
                    }
                    else
                    {
                        attt = TargetTypes[field.FieldType.Name] as ArrayType_TargetType;
                    }
                    
                    //AddUsedArrayElementTypeToCollection(attt.ElementType);//添加到全局数组元素类型的集合中
                    //new完毕
                    Array_TargetField arrayField = new Array_TargetField();
                    arrayField.TargetType = attt;
                    arrayField.FieldNumberAttr = FieldNumberAttr;
                    arrayField.FieldName = field.Name;
                    var arratt = field.GetCustomAttribute<MaxCountAttribute>();
                    arrayField.MaxCountAttribute = arratt == null ? new MaxCountAttribute() { IsFixed = true, MaxCount = 1, LenFieldName = "" } : arratt;
                    arrayField.ArrayLenField = targetStructType.GetArrayLenField(arrayField);
                    arrayField.UnionFieldAttr = field.GetCustomAttribute<UnionFieldAttribute>();
                    arrayField.NoSerializationAttr = field.GetCustomAttribute<NoSerializationAttribute>();
                    arrayField.MacroControlAttr = field.GetCustomAttribute<MacroControlAttribute>();
                    needAddField = arrayField;
                    if(arrayField.UnionFieldAttr!=null)
                    {
                        throw new NotSupportedException("Union Field Type does not support Array Type!");
                    }
                    //targetStruct.TargetFields.Add(arrayField);
                }
                else if (!field.FieldType.IsPrimitive && !field.FieldType.IsEnum && field.FieldType.IsValueType)//struct
                {
                    ITargetType ittt = StructTypeParse(field.FieldType);
                    Struct_TargetField objectFiled = new Struct_TargetField();
                    objectFiled.TargetType = ittt;
                    objectFiled.FieldName = field.Name;
                    objectFiled.FieldNumberAttr = FieldNumberAttr;
                    objectFiled.UnionFieldAttr = field.GetCustomAttribute<UnionFieldAttribute>();
                    objectFiled.NoSerializationAttr = field.GetCustomAttribute<NoSerializationAttribute>();
                    objectFiled.MacroControlAttr = field.GetCustomAttribute<MacroControlAttribute>();
                    needAddField = objectFiled;
                    //targetStruct.TargetFields.Add(objectFiled);
                }
                else
                {
                    throw new NotSupportedException();
                }
                if (needAddField.UnionFieldAttr != null)
                {
                    if (isWriteUnion == false)
                    {
                        needAddField.UnionFieldAttr.UnionIndex = UnionIndex.Start;
                        isWriteUnion = true;
                    }
                    //如果下一个是null 或者 UnionFieldAttr==null 则打end
                    UnionFieldAttribute nextFieldUnionFieldAttr = null;
                    if (field_index + 1 < fields.Length)
                    {
                        nextFieldUnionFieldAttr = fields[field_index+1].GetCustomAttribute<UnionFieldAttribute>();
                    }
                    if (nextFieldUnionFieldAttr== null)
                    {
                        needAddField.UnionFieldAttr.UnionIndex = UnionIndex.End;
                        isWriteUnion = false;
                    }
                }

                targetStructType.TargetFields.Add(needAddField);
                if(needAddField.FieldNumberAttr.Number==0)
                {
                    throw new Exception("field number is not set 0!");
                }
            }
            if (targetStructType.TargetFields.Count == 0)
            {
                throw new Exception("The number of fields of struct type must be greater than 0!");
            }
            return targetStructType;
        }
        private void ParameterTypeParse(ParameterInfo[] parameters, StructType_TargetType targetStruct)
        {
            int fieldNumber = 1;
            foreach (var parameter in parameters)
            {
                var FieldNumberAttr = new FieldNumberAttribute(fieldNumber);//如果没有fieldNumber说明用户并不打算使用TTLV编码
                                                                            //throw new NotImplementedException($"the {field.Name}'s FieldNumberAttr is null");
                fieldNumber++;
                ITargetField needAddField = null;
                if (IsNumberType(parameter.ParameterType) == true)
                {
                    Base_TargetField targetfield = new Base_TargetField();
                    targetfield.TargetType = BaseType_TargetType.GetInstance(TargetTypes,ClrBaseValueTypeToTargetType_t(parameter.ParameterType));
                    targetfield.FieldName = parameter.Name;
                    targetfield.FieldNumberAttr = FieldNumberAttr;
                    needAddField = targetfield;
                    //targetStruct.TargetFields.Add(targetfield);
                }
                else if (IsEnumType(parameter.ParameterType) == true)
                {
                    Enum_TargetField enumField = new Enum_TargetField();
                    enumField.FieldName = parameter.Name;
                    enumField.FieldNumberAttr = FieldNumberAttr;
                    //有可能还没有把该枚举类型添加到集合里,所以要添加一下
                    var vs = parameter.ParameterType.GetEnumValues();
                    EnumType_TargetType te = null;
                    if(TargetTypes.ContainsKey(parameter.ParameterType.Name)==false)
                    {
                        te = new EnumType_TargetType();
                        te.TypeName = parameter.ParameterType.Name;//类型名称
                        te.NumberType = ClrBaseValueTypeToTargetType_t(parameter.ParameterType.GetEnumUnderlyingType());
                        foreach (var vsv in vs)
                        {
                            te.KeyValue.Add(parameter.ParameterType.GetEnumName(vsv), Convert.ToInt32(vsv));
                        }
                        TargetTypes.Add(te.TypeName, te);
                    }
                    else
                    {
                        te = TargetTypes[parameter.ParameterType.Name] as EnumType_TargetType;
                    }
                    
                    //添加完毕
                    enumField.TargetType = te;
                    needAddField = enumField;
                    //targetStruct.TargetFields.Add(enumField);

                }
                else if (parameter.ParameterType.IsArray == true)
                {
                    //new array 类型
                    ArrayType_TargetType attt = null;
                    if(TargetTypes.ContainsKey(parameter.ParameterType.Name)==false)
                    {
                        attt = new ArrayType_TargetType();
                        attt.TypeName = parameter.ParameterType.Name;
                        if (IsNumberType(parameter.ParameterType.GetElementType()) == true)
                        {
                            attt.ElementType = BaseType_TargetType.GetInstance(TargetTypes, ClrBaseValueTypeToTargetType_t(parameter.ParameterType.GetElementType()));
                        }
                        else
                        {
                            attt.ElementType = StructTypeParse(parameter.ParameterType.GetElementType());
                        }
                        TargetTypes.Add(attt.TypeName, attt);
                    }
                    else
                    {
                        attt = TargetTypes[parameter.ParameterType.Name] as ArrayType_TargetType;
                    }
                    
                    //AddUsedArrayElementTypeToCollection(attt.ElementType);
                    //new 完毕
                    Array_TargetField arrayField = new Array_TargetField();
                    arrayField.TargetType = attt;
                    arrayField.FieldNumberAttr = FieldNumberAttr;
                    arrayField.FieldName = parameter.Name;
                    var arratt = parameter.GetCustomAttribute<MaxCountAttribute>();
                    arrayField.MaxCountAttribute = arratt == null ? new MaxCountAttribute() { IsFixed = true, MaxCount = 1, LenFieldName = "" } : arratt;
                    arrayField.ArrayLenField = targetStruct.GetArrayLenField(arrayField);
                    needAddField = arrayField;
                    //targetStruct.TargetFields.Add(arrayField);
                }
                else if (!parameter.ParameterType.IsPrimitive && !parameter.ParameterType.IsEnum && parameter.ParameterType.IsValueType)//struct
                {
                    Struct_TargetField objectFiled = new Struct_TargetField();
                    objectFiled.TargetType = StructTypeParse(parameter.ParameterType);
                    objectFiled.FieldName = parameter.Name;
                    objectFiled.FieldNumberAttr = FieldNumberAttr;
                    needAddField = objectFiled;
                    //targetStruct.TargetFields.Add(objectFiled);
                }
                else
                {
                    throw new NotSupportedException();
                }
                targetStruct.TargetFields.Add(needAddField);
                if (needAddField.FieldNumberAttr.Number == 0)
                {
                    throw new Exception("field number is not set 0!");
                }
            }
        }
        public void Parse(string file)
        {
            CSScript.EvaluatorConfig.Engine = EvaluatorEngine.CodeDom;
            var eva = CSScript.Evaluator;
            Assembly assembly = CSScript.Evaluator.CompileCode(File.ReadAllText(file));
            var types = assembly.GetTypes();

            //本次循环用来添加ParsedFiles集合以及获取每个FileIdlInfo的GenerationOption参数
            foreach (var type in types)
            {
                var filenameattr = type.GetCustomAttribute<FileNameAttribute>();
                if (filenameattr == null)
                {
                    throw new Exception($"{filenameattr} you must add FileNameAttribute into every struct!");
                }
                //if (filenameattr.FileNameString != file)
                //{
                    //Parse(filename.FileNameString);
                //    throw new Exception("you must add file name attribute!");
                    //continue;
               // }
                if (type.GetInterface("IOptionProcess") != null)
                {
                    var process = assembly.CreateInstance(type.FullName) as IOptionProcess;
                    FileIdlInfo fileIdlInfo = null;
                    foreach (var fii in ParsedFiles)
                    {
                        if(fii.FileName== filenameattr.FileNameString)
                        {
                            fileIdlInfo = fii;
                        }
                    }
                    if(fileIdlInfo==null)
                    {
                        fileIdlInfo = new FileIdlInfo();
                        fileIdlInfo.FileName = filenameattr.FileNameString;
                        ParsedFiles.Add(fileIdlInfo);
                    }
                    fileIdlInfo.GenerationOption = process.Process();
                    fileIdlInfo.ServiceId = fileIdlInfo.GenerationOption.ServiceIdStartNumber;
                    //fileIdlInfo.ServiceId = fileIdlInfo.GenerationOption.ServiceIdStartNumber;
                }
            }

            foreach (var type in types)
            {
                var filename = type.GetCustomAttribute<FileNameAttribute>().FileNameString;
                var fileIdlInfo = GetFileIdlInfo(filename);
                if (type.IsValueType == true)
                {
                    if (type.IsEnum == true)
                    {
                        var vs = type.GetEnumValues();
                        EnumType_TargetType te = null;
                        if(TargetTypes.ContainsKey(type.Name)==false)
                        {
                            te = new EnumType_TargetType();
                            te.TypeName = type.Name;//类型名称
                            te.NumberType = ClrBaseValueTypeToTargetType_t(type.GetEnumUnderlyingType());
                            foreach (var vsv in vs)
                            {
                                te.KeyValue.Add(type.GetEnumName(vsv), Convert.ToInt32(vsv));
                            }
                            TargetTypes.Add(te.TypeName, te);
                        }
                        else
                        {
                            te = TargetTypes[type.Name] as EnumType_TargetType;
                        }
                        fileIdlInfo.TargetEnums.Add(te);

                        //Console.WriteLine(te.ToString());
                    }
                    else//struct
                    {
                        var struct_type = StructTypeParse(type);
                        fileIdlInfo.TargetStructs.Add(struct_type);
                    }

                }
                else if (type.IsInterface == true)
                { 
                
                }
                else if (type.BaseType.Name == "MulticastDelegate")//如果这个类型是委托。
                {
                    var delms = type.GetMembers();
                    MethodInfo invokemi = null;
                    foreach (var delm in delms)
                    {
                        if (delm.Name == "Invoke")
                        {
                            invokemi = delm as MethodInfo;
                            break;
                        }
                    }
                    if (invokemi == null)
                    {
                        throw new NullReferenceException($"{type.Name}:invokemi is null");
                    }
                    TargetService targetService = new TargetService();
                    var serviceIdAttr = type.GetCustomAttribute<ServiceIdAttribute>();
                    targetService.ServiceId = serviceIdAttr == null ? fileIdlInfo.ServiceId : serviceIdAttr.ServiceId;
                    fileIdlInfo.ServiceId++;
                    var externalParameterAttribute = type.GetCustomAttribute<ExternalParameterAttribute>();
                    targetService.ExternalParameter = externalParameterAttribute == null ? new ExternalParameterAttribute(false) : externalParameterAttribute;

                    var MacroControlAttribute = type.GetCustomAttribute<MacroControlAttribute>();
                    targetService.MacroControlAttribute = MacroControlAttribute;

                    targetService.RoleAttribute = type.GetCustomAttribute<RoleAttribute>();
                    if(targetService.RoleAttribute==null)
                    {
                        throw new Exception($"the {type.Name} service NO have RoleAttribute！");
                    }

                    var mt = invokemi;
                    Type rt = mt.ReturnType;
                    targetService.ServiceName = type.Name;
                    //targetService.FullName = type.Name + "_" + targetService.ServiceName;

                    StructType_TargetType returnStructType = null;
                    if(TargetTypes.ContainsKey(type.Name + "_Return")==false)
                    {
                        returnStructType = new StructType_TargetType();
                        returnStructType.TypeName = type.Name + "_Return";

                        Enum_TargetField RequestResponseStatefield = new Enum_TargetField();
                        EnumType_TargetType ettt = null;
                        ettt = TargetTypes["RequestResponseState"] as EnumType_TargetType;
                        //ettt.KeyValue这里在runtime中定义,不需要加
                        RequestResponseStatefield.TargetType = ettt;
                        RequestResponseStatefield.FieldName = "State";
                        RequestResponseStatefield.FieldNumberAttr = new FieldNumberAttribute(1);//state 的Field Number为1
                        returnStructType.TargetFields.Add(RequestResponseStatefield);

                        if (rt.Name != "Void")
                        {
                            //返回值有可能是枚举、基本数据、结构体
                            //根据返回值判断
                            //EnumType_TargetType rt_ettt = fileIdlInfo.GetTargetEnum(rt.Name);
                            //ObjectType_TargetType rt_ottt = fileIdlInfo.GetTargetStruct(rt.Name);
                            if (rt.IsEnum == true)
                            {
                                //说明返回值是枚举
                                //new 一个enum 类型
                                var vs = rt.GetEnumValues();
                                EnumType_TargetType te = null;
                                if (TargetTypes.ContainsKey(rt.Name) == false)
                                {
                                    te = new EnumType_TargetType();
                                    te.TypeName = rt.Name;//类型名称
                                    te.NumberType = ClrBaseValueTypeToTargetType_t(rt.GetEnumUnderlyingType());
                                    foreach (var vsv in vs)
                                    {
                                        te.KeyValue.Add(rt.GetEnumName(vsv), Convert.ToInt32(vsv));
                                    }
                                    TargetTypes.Add(te.TypeName, te);
                                }
                                else
                                {
                                    te = TargetTypes[rt.Name] as EnumType_TargetType;
                                }


                                Enum_TargetField retunValueFiled = new Enum_TargetField();
                                retunValueFiled.TargetType = te;
                                retunValueFiled.FieldName = "ReturnValue";
                                retunValueFiled.FieldNumberAttr = new FieldNumberAttribute(2);//return Value 的Field Number为2
                                returnStructType.TargetFields.Add(retunValueFiled);
                            }
                            else if (IsNumberType(rt) == true)
                            {
                                Base_TargetField retunValueFiled = new Base_TargetField();
                                retunValueFiled.TargetType = BaseType_TargetType.GetInstance(TargetTypes, ClrBaseValueTypeToTargetType_t(rt));
                                retunValueFiled.FieldName = "ReturnValue";
                                retunValueFiled.FieldNumberAttr = new FieldNumberAttribute(2);//return Value 的Field Number为2
                                returnStructType.TargetFields.Add(retunValueFiled);
                            }
                            else//一定是struct 不可能是数组，因为不支持返回值是数组的。
                            {
                                Struct_TargetField objectFiled = new Struct_TargetField();
                                objectFiled.TargetType = StructTypeParse(rt);
                                objectFiled.FieldName = "ReturnValue";
                                objectFiled.FieldNumberAttr = new FieldNumberAttribute(2);//return Value 的Field Number为2
                                returnStructType.TargetFields.Add(objectFiled);
                            }
                        }

                        TargetTypes.Add(returnStructType.TypeName, returnStructType);
                    }
                    else
                    {
                        returnStructType = TargetTypes[type.Name + "_Return"] as StructType_TargetType;
                    }


                    //处理函数参数
                    StructType_TargetType ParameterStructType = null;
                    if(TargetTypes.ContainsKey(type.Name + "_Parameter")==false)
                    {
                        ParameterStructType = new StructType_TargetType();
                        ParameterStructType.TypeName = type.Name + "_Parameter";
                        var pars = mt.GetParameters();
                        ParameterTypeParse(pars, ParameterStructType);
                        TargetTypes.Add(ParameterStructType.TypeName, ParameterStructType);
                    }
                    else
                    {
                        ParameterStructType = TargetTypes[type.Name + "_Parameter"] as StructType_TargetType;
                    }
                    targetService.ReturnStructType = returnStructType;
                    targetService.ParameterStructType = ParameterStructType;
                    fileIdlInfo.TargetServices.Add(targetService);
                }
            }
            
        }
    }

   
    //FileIdlInfo fileIdlInfo, GenType genType,string outputpath
    public class CSharpCodeGenParameter
    {
        public FileIdlInfo FileIdlInfo { get; set; }
        public RoleType RoleType { get; set; }
        public string OutPutPath { get; set; }
        
    }
    public class CppCodeGenParameter
    {
        public FileIdlInfo FileIdlInfo { get; set; }
        public RoleType RoleType { get; set; }
        public string OutPutPath { get; set; }

        

    }

}

