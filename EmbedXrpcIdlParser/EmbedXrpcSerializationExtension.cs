using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public class BaseValueField
    {
        public string Type { get; set; }
        public string Name { get; set; }
        public string StructName { get; set; }

        public string ToCode()
        {
           return $"{EmbedXrpcSerializationHelper.FieldReplaceDic[Type]} {StructName}_{Name}(\"{StructName}.{Name}\",offsetof({StructName},{Name}));\n";
        }
        public string ToExtern()
        {
            return $"extern {EmbedXrpcSerializationHelper.FieldReplaceDic[Type]} {StructName}_{Name};\n";
        }
    }
    public class ArrayType
    {
        public string Name { get; set; }
        public string ArrayElementType { get; set; }
        public string ArrayElementLen { get; set; }

        public string ToCode()
        {
            return $"ArrayType {Name}_Type({ArrayElementType},{ArrayElementLen});\n";
        }
        public string ToExtern()
        {
            return $"extern ArrayType {Name}_Type;\n";
        }
    }
    public class ArrayField:ArrayType
    {
        public bool IsFixed { get; set; }
        public string StructName { get; set; }
        public string LenFieldName { get; set; }
        public new string ToCode()
        {
            return $"ArrayField {StructName}_{Name}(\"{StructName}.{Name}\",{IsFixed.ToString().ToLower()},&{ArrayElementType},{ArrayElementLen},offsetof({StructName},{Name}),{LenFieldName});\n";
        }
        public new string ToExtern()
        {
            return $"extern ArrayField {StructName}_{Name};\n";
        }
    }
    public class ObjectType
    {
        public string Name { get; set; }
        public string FieldCount { get; set; }
        public string FieldDesc { get; set; }
        public string ToCode()
        {
            return $"ObjectType {Name}({FieldCount},{FieldDesc});\n";
        }
        public string ToExtern()
        {
            return $"extern ObjectType {Name};\n";
        }
    }
    public class ObjectField:ObjectType
    {
        public string StructName { get; set; }
        public new string ToCode()
        {
            return $"ObjectField {StructName}_{Name}(\"{StructName}.{Name}\",{FieldCount},{FieldDesc},offsetof({StructName},{Name}));\n";
        }
        public new string ToExtern()
        {
            return $"extern ObjectField {StructName}_{Name};\n";
        }
    }
    public class EmbedXrpcSerializationHelper
    {
        internal static Dictionary<string, string> FieldReplaceDic = new Dictionary<string, string>();
        internal static Dictionary<string, string> TypeReplaceDic = new Dictionary<string, string>();
        static EmbedXrpcSerializationHelper()
        {
            FieldReplaceDic.Add("byte", "Uint8Field");
            FieldReplaceDic.Add("Byte", "Uint8Field");
            FieldReplaceDic.Add("sbyte", "Int8Field");
            FieldReplaceDic.Add("SByte", "Int8Field");
            FieldReplaceDic.Add("UInt16", "Uint16Field");
            FieldReplaceDic.Add("Int16", "Int16Field");
            FieldReplaceDic.Add("UInt32", "Uint32Field");
            FieldReplaceDic.Add("Int32", "Int32Field");
            FieldReplaceDic.Add("float", "FloatField");
            FieldReplaceDic.Add("double", "DoubleField");

            TypeReplaceDic.Add("byte", "Uint8TypeInstance");
            TypeReplaceDic.Add("Byte", "Uint8TypeInstance");
            TypeReplaceDic.Add("sbyte", "Int8TypeInstance");
            TypeReplaceDic.Add("SByte", "Int8TypeInstance");
            TypeReplaceDic.Add("UInt16", "Uint16TypeInstance");
            TypeReplaceDic.Add("Int16", "Int16TypeInstance");
            TypeReplaceDic.Add("UInt32", "Uint32TypeInstance");
            TypeReplaceDic.Add("Int32", "Int32TypeInstance");
            TypeReplaceDic.Add("float", "FloatTypeInstance");
            TypeReplaceDic.Add("double", "DoubleTypeInstance");
        }
        public static StringBuilder EmitIFieldsArray(string name,IList<string> FieldDesc)
        {
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.Append($"IField* {name}Desc []=\n");
            stringBuilder.Append("{\n");
            foreach (var fd in FieldDesc)
            {
                stringBuilder.Append("&" + fd + ",\n");
            }
            stringBuilder.Append("};\n");
            return stringBuilder;
        }
        public static void EmitStruct(string name, IList<TargetField> fields, StreamWriter cfilewriter, StreamWriter hfilewriter)
        {
            StringBuilder cfilestringBuilder = new StringBuilder();
            StringBuilder hfilestringBuilder = new StringBuilder();
            List<string> FieldsDesc = new List<string>();
            foreach (var field in fields)
            {
                FieldsDesc.Add($"{name}_{field.Name}");
                if (field.IsArray==false)
                {                   
                    if (FieldReplaceDic.ContainsKey(field.IdlType) == true)
                    {
                        BaseValueField baseValueField = new BaseValueField();
                        baseValueField.Type = field.IdlType;
                        baseValueField.StructName = name;
                        baseValueField.Name = field.Name;
                        cfilestringBuilder.Append(baseValueField.ToCode());
                        hfilestringBuilder.Append(baseValueField.ToExtern());
                    }
                    else
                    {
                        if(field.Enum==null)
                        {
                            ObjectField objectField = new ObjectField();
                            objectField.Name = field.Name;
                            objectField.StructName = name;
                            objectField.FieldCount = $"sizeof({field.IdlType}Desc)/sizeof(IField*)";
                            objectField.FieldDesc = $"{field.IdlType}Desc";
                            cfilestringBuilder.Append(objectField.ToCode());
                            hfilestringBuilder.Append(objectField.ToExtern());
                        }
                        else
                        {
                            BaseValueField baseValueField = new BaseValueField();
                            baseValueField.Type = field.Enum.IntType;
                            baseValueField.Name = field.Name;
                            baseValueField.StructName = name;
                            cfilestringBuilder.Append(baseValueField.ToCode());
                            hfilestringBuilder.Append(baseValueField.ToExtern());
                        }
                    }
                }
                else
                {
                    string x= TypeReplaceDic.ContainsKey(CppCodeGenerater.GetIdlTypeArrayElementType(field)) == false ?
                        CppCodeGenerater.GetIdlTypeArrayElementType(field)+"_Type" : TypeReplaceDic[CppCodeGenerater.GetIdlTypeArrayElementType(field)];

                    var arrayLenField = IdlInfo.GetArrayLenField(fields, field);
                    var arrayLenFieldDesc = arrayLenField==null?"nullptr": $"&{name}_{arrayLenField.Name}";

                    ArrayField arrayField = new ArrayField();
                    arrayField.Name = field.Name;
                    arrayField.StructName = name;
                    arrayField.IsFixed = field.MaxCountAttribute.IsFixed;
                    arrayField.LenFieldName = arrayLenFieldDesc;
                    arrayField.ArrayElementType = x;
                    arrayField.ArrayElementLen = $"sizeof({ CppCodeGenerater.GetIdlTypeArrayElementType(field)})";
                    cfilestringBuilder.Append(arrayField.ToCode());

                    hfilestringBuilder.Append(arrayField.ToExtern());
                    
                }
            }
            if(FieldsDesc.Count>0)
            {
                cfilestringBuilder.Append(EmitIFieldsArray($"{name}", FieldsDesc));
            }
            

            ObjectType objectType = new ObjectType();
            objectType.Name = $"{name}_Type";
            objectType.FieldCount= FieldsDesc.Count == 0 ? "0":$"sizeof({name}Desc)/sizeof(IField*)";
            objectType.FieldDesc = FieldsDesc.Count==0?"nullptr":$"{name}Desc";

            cfilestringBuilder.Append(objectType.ToCode());
            hfilestringBuilder.Append(objectType.ToExtern());

            cfilewriter.WriteLine(cfilestringBuilder.ToString());
            hfilewriter.WriteLine(hfilestringBuilder.ToString());
            hfilewriter.WriteLine("\n");
        }
    }
    public static class TargetStructEmbedXrpcSerializationExtension
    {
        public static void ToXrpcSerializationCode(this TargetStruct targetStruct,StreamWriter cfilewriter, StreamWriter hfilewriter)
        {
            EmbedXrpcSerializationHelper.EmitStruct(targetStruct.Name, targetStruct.TargetFields, cfilewriter,hfilewriter);
        }
    }
}
