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

        public string Prefix { get; set; } = "Field";
        public string Name { get; set; }
        public string StructName { get; set; }
        public UInt32 FieldNumber { get; set; }

        public bool IsArrayLenField { get; set; }

        public string ToCode()
        {
           return $"const {EmbedXrpcSerializationHelper.FieldReplaceDic[Type]} {StructName}_{Prefix}_{Name}({FieldNumber},\"{StructName}.{Name}\",offsetof({StructName},{Name}),{IsArrayLenField.ToString().ToLower()});\n";
        }
        public string ToExtern()
        {
            return $"extern const {EmbedXrpcSerializationHelper.FieldReplaceDic[Type]} {StructName}_{Prefix}_{Name};\n";
        }
    }
    public class ArrayType
    {
        public string Name { get; set; }
        public string ArrayElementType { get; set; }
        public string ArrayElementLen { get; set; }

        public string ToCode()
        {
            return $"const ArrayType {Name}_Type({ArrayElementType},{ArrayElementLen});\n";
        }
        public string ToExtern()
        {
            return $"extern const ArrayType {Name}_Type;\n";
        }
    }
    public class ArrayField:ArrayType
    {
        public bool IsFixed { get; set; }
        public string Prefix { get; set; } = "Field";
        public string StructName { get; set; }
        public TargetField LenField { get; set; }
        public UInt32 FieldNumber { get; set; }
        public new string ToCode()
        {
            var arrayLenFieldDesc = LenField == null ? "nullptr" : $"&{StructName}_{Prefix}_{LenField.Name}";
            return $"const ArrayField {StructName}_{Prefix}_{Name}({FieldNumber},\"{StructName}.{Name}\",{IsFixed.ToString().ToLower()},&{ArrayElementType},{ArrayElementLen},offsetof({StructName},{Name}),{arrayLenFieldDesc});\n";
        }
        public new string ToExtern()
        {
            return $"extern const ArrayField {StructName}_{Prefix}_{Name};\n";
        }
    }
    public class ObjectType
    {
        public string Name { get; set; }
        public string FieldCount { get; set; }
        public string FieldDesc { get; set; }
        public string ToCode()
        {
            return $"const ObjectType {Name}({FieldCount},{FieldDesc});\n";
        }
        public string ToExtern()
        {
            return $"extern const ObjectType {Name};\n";
        }
    }
    public class ObjectField:ObjectType
    {
        public string Prefix { get; set; } = "Field";
        public string StructName { get; set; }
        public UInt32 FieldNumber { get; set; }
        public new string ToCode()
        {
            return $"const ObjectField {StructName}_{Prefix}_{Name}({FieldNumber},\"{StructName}.{Name}\",{FieldCount},{FieldDesc},offsetof({StructName},{Name}));\n";
        }
        public new string ToExtern()
        {
            return $"extern const ObjectField {StructName}_{Prefix}_{Name};\n";
        }
    }
    public class EmbedXrpcSerializationHelper
    {
        internal static Dictionary<string, string> FieldReplaceDic = new Dictionary<string, string>();
        internal static Dictionary<string, string> TypeReplaceDic = new Dictionary<string, string>();
        static EmbedXrpcSerializationHelper()
        {
            FieldReplaceDic.Add("bool", "UInt8Field");
            FieldReplaceDic.Add("Boolean", "UInt8Field");
            FieldReplaceDic.Add("byte", "UInt8Field");
            FieldReplaceDic.Add("Byte", "UInt8Field");
            FieldReplaceDic.Add("sbyte", "Int8Field");
            FieldReplaceDic.Add("SByte", "Int8Field");
            FieldReplaceDic.Add("UInt16", "UInt16Field");
            FieldReplaceDic.Add("Int16", "Int16Field");
            FieldReplaceDic.Add("UInt32", "UInt32Field");
            FieldReplaceDic.Add("Int32", "Int32Field");
            FieldReplaceDic.Add("UInt64", "UInt64Field");
            FieldReplaceDic.Add("Int64", "Int64Field");
            FieldReplaceDic.Add("float", "FloatField");
            FieldReplaceDic.Add("double", "DoubleField");

            TypeReplaceDic.Add("bool", "UInt8TypeInstance");
            TypeReplaceDic.Add("Boolean", "UInt8TypeInstance");
            TypeReplaceDic.Add("byte", "UInt8TypeInstance");
            TypeReplaceDic.Add("Byte", "UInt8TypeInstance");
            TypeReplaceDic.Add("sbyte", "Int8TypeInstance");
            TypeReplaceDic.Add("SByte", "Int8TypeInstance");
            TypeReplaceDic.Add("UInt16", "UInt16TypeInstance");
            TypeReplaceDic.Add("Int16", "Int16TypeInstance");
            TypeReplaceDic.Add("UInt32", "UInt32TypeInstance");
            TypeReplaceDic.Add("Int32", "Int32TypeInstance");
            TypeReplaceDic.Add("UInt64", "UInt64TypeInstance");
            TypeReplaceDic.Add("Int64", "Int64TypeInstance");
            TypeReplaceDic.Add("float", "FloatTypeInstance");
            TypeReplaceDic.Add("double", "DoubleTypeInstance");
        }
        public static StringBuilder EmitIFieldsArray(string name,IList<string> FieldDesc)
        {
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.Append($"const IField* {name} []=\n");
            stringBuilder.Append("{\n");
            foreach (var fd in FieldDesc)
            {
                stringBuilder.Append("&" + fd + ",\n");
            }
            stringBuilder.Append("};\n");
            return stringBuilder;
        }
        public static void EmitStruct(TargetStruct targetStruct, StreamWriter cfilewriter, StreamWriter hfilewriter)
        {
            StringBuilder cfilestringBuilder = new StringBuilder();
            StringBuilder hfilestringBuilder = new StringBuilder();
            List<string> FieldsDesc = new List<string>();
            foreach (var field in targetStruct.TargetFields)
            {
                FieldsDesc.Add($"{targetStruct.Name}_Field_{field.Name}");
                if (field.IsArray==false)
                {                   
                    if (FieldReplaceDic.ContainsKey(field.BitsType== BitsType.NoBits? field.SourceCodeType: field.BitsType.ToString()) == true)
                    {
                        BaseValueField baseValueField = new BaseValueField();
                        baseValueField.Type = field.BitsType == BitsType.NoBits ? field.SourceCodeType : field.BitsType.ToString();//field.IdlType;
                        baseValueField.StructName = targetStruct.Name;
                        baseValueField.Name = $"{field.Name}";
                        baseValueField.FieldNumber = field.FieldNumberAttr.Number;
                        baseValueField.IsArrayLenField = field.IsArrayLenField;
                        cfilestringBuilder.Append(baseValueField.ToCode());
                        hfilestringBuilder.Append(baseValueField.ToExtern());
                    }
                    else
                    {
                        if(field.Enum==null)
                        {
                            ObjectField objectField = new ObjectField();
                            objectField.Name = $"{field.Name}";
                            objectField.StructName = targetStruct.Name;
                            objectField.FieldCount = $"sizeof({field.SourceCodeType}Desc)/sizeof(IField*)";
                            objectField.FieldDesc = $"{field.SourceCodeType}Desc";
                            objectField.FieldNumber = field.FieldNumberAttr.Number;
                            cfilestringBuilder.Append(objectField.ToCode());
                            hfilestringBuilder.Append(objectField.ToExtern());
                        }
                        else
                        {
                            BaseValueField baseValueField = new BaseValueField();
                            baseValueField.Type = field.Enum.SourceCodeType;
                            baseValueField.Name = $"{field.Name}";
                            baseValueField.StructName = targetStruct.Name;
                            baseValueField.FieldNumber = field.FieldNumberAttr.Number;
                            baseValueField.IsArrayLenField = field.IsArrayLenField;
                            cfilestringBuilder.Append(baseValueField.ToCode());
                            hfilestringBuilder.Append(baseValueField.ToExtern());
                        }
                    }
                }
                else
                {
                    string x= TypeReplaceDic.ContainsKey(CppCodeGenerater.GetSourceCodeTypeArrayElementType(field)) == false ?
                        CppCodeGenerater.GetSourceCodeTypeArrayElementType(field)+"_Type" : TypeReplaceDic[CppCodeGenerater.GetSourceCodeTypeArrayElementType(field)];

                    var arrayLenField = IdlInfo.GetArrayLenField(targetStruct.TargetFields, field);
                    

                    ArrayField arrayField = new ArrayField();
                    arrayField.Name = $"{field.Name}";
                    arrayField.StructName = targetStruct.Name;
                    arrayField.IsFixed = field.MaxCountAttribute.IsFixed;
                    arrayField.LenField = arrayLenField;
                    arrayField.ArrayElementType = x;
                    arrayField.ArrayElementLen = $"sizeof({ CppCodeGenerater.GetSourceCodeTypeArrayElementType(field)})";
                    arrayField.FieldNumber = field.FieldNumberAttr.Number;
                    cfilestringBuilder.Append(arrayField.ToCode());

                    hfilestringBuilder.Append(arrayField.ToExtern());
                    
                }
            }
            if(FieldsDesc.Count>0)
            {
                cfilestringBuilder.Append(EmitIFieldsArray($"{targetStruct.Name}Desc", FieldsDesc));
                hfilestringBuilder.Append($"extern const IField* {targetStruct.Name}Desc [{FieldsDesc.Count}];\n");
            }
            

            ObjectType objectType = new ObjectType();
            objectType.Name = $"{targetStruct.Name}_Type";
            objectType.FieldCount= FieldsDesc.Count == 0 ? "0":$"sizeof({targetStruct.Name}Desc)/sizeof(IField*)";
            objectType.FieldDesc = FieldsDesc.Count==0?"nullptr":$"{targetStruct.Name}Desc";

            cfilestringBuilder.Append(objectType.ToCode());
            hfilestringBuilder.Append(objectType.ToExtern());

            cfilewriter.WriteLine(cfilestringBuilder.ToString());
            hfilewriter.WriteLine(hfilestringBuilder.ToString());
        }
        
    }
    
}
