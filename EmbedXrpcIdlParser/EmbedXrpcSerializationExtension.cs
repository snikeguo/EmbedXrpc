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
            //return $"const {EmbedXrpcSerializationHelper.TypeReplaceDic[Type]}Field {StructName}_{Prefix}_{Name}({FieldNumber},\"{StructName}.{Name}\",offsetof({StructName},{Name}),{IsArrayLenField.ToString().ToLower()});\n";
            return $"extern const {EmbedXrpcSerializationHelper.TypeReplaceDic[Type]}Field {StructName}_{Prefix}_{Name}=\r\n" +
                $"{{ \r\n" +
                $"  {{ \r\n" +
                $"    TYPE_{EmbedXrpcSerializationHelper.TypeReplaceDic[Type].ToString().ToUpper()},\r\n" +
                $"  }},\r\n" +
                $"  \"{StructName}.{Name}\" ,\r\n" +
                $"  offsetof({StructName},{Name}) ,\r\n" +
                $"  &{EmbedXrpcSerializationHelper.TypeReplaceDic[Type]}TypeInstance,  \r\n" +
                $"  {FieldNumber},\r\n" +
                $"  {IsArrayLenField.ToString().ToLower()},\r\n" +
                $"}};\r\n";
        }
        public string ToExtern()
        {
            return $"extern const {EmbedXrpcSerializationHelper.TypeReplaceDic[Type]}Field {StructName}_{Prefix}_{Name};\n";
        }
    }
    public class ArrayType
    {
        static List<string> GeneratedTypes = new List<string>();
        public string TypeName { get; set; }
        public string ArrayElementType { get; set; }
        public string ArrayElementLen { get; set; }

        public string ToCode()
        {
            //return $"const ArrayType {Name}_Type({ArrayElementType},{ArrayElementLen});\n";
            if (GeneratedTypes.Contains($"{TypeName}_Array_Type") == true)
            {
                return string.Empty;
            }
            GeneratedTypes.Add($"{TypeName}_Array_Type");
            return $"extern const ArrayType {TypeName}_Array_Type= \r\n" +  //修复C++中的const限定符导致的链接问题
                $"{{\r\n" +
                $"  {{ \r\n" +
                $"    TYPE_ARRAY,\r\n" +
                $"  }},\r\n" +
                $"  (const IType *)(&{ArrayElementType}),\r\n" +
                $"  {ArrayElementLen},\r\n" +
                $"}};\r\n";

        }
        public string ToExtern()
        {
            return $"extern const ArrayType {TypeName}_Array_Type;\r\n";//多extern几次没关系,不管他
        }
    }
    public class ArrayField : ArrayType
    {
        public string FieldName { get; set; }
        public bool IsFixed { get; set; }
        public string Prefix { get; set; } = "Field";
        public string StructName { get; set; }
        public TargetField LenField { get; set; }
        public UInt32 FieldNumber { get; set; }
        public new string ToCode()
        {
            var arrayLenFieldDesc = LenField == null ? "nullptr" : $"(const IField*)&{StructName}_{Prefix}_{LenField.Name}";
            //return $"const ArrayField {StructName}_{Prefix}_{Name}({FieldNumber},\"{StructName}.{Name}\",{IsFixed.ToString().ToLower()},&{ArrayElementType},{ArrayElementLen},offsetof({StructName},{Name}),{arrayLenFieldDesc});\n";
            string s= $"extern const ArrayType {TypeName}_Array_Type;\r\n";
            s+= $"extern const ArrayField {StructName}_{Prefix}_{FieldName}=\r\n" +
                $"{{ \r\n" +
                $"  {{\r\n" +
                $"    TYPE_ARRAY,\r\n" +
                $"  }},\r\n" +
                $"  \"{StructName}.{FieldName}\",\r\n" +
                $"  offsetof({StructName},{FieldName}),\r\n" +
                $"  {arrayLenFieldDesc},\r\n" +
                $"  &{TypeName}_Array_Type,\r\n" +
                $"  {IsFixed.ToString().ToLower()},\r\n" +
                $"  {FieldNumber},\r\n" +
                $"}};\r\n";
            return s;
        }
        public new string ToExtern()
        {
            return $"extern const ArrayField {StructName}_{Prefix}_{FieldName};\n";
        }
    }
    public class ObjectType
    {
        static List<string> GeneratedTypes = new List<string>();
        public string TypeName { get; set; }
        public string FieldCount { get; set; }
        public string FieldDesc { get; set; }

        public string ToCode()
        {
            if (GeneratedTypes.Contains($"{TypeName}_Object_Type") == true)
            {
                return string.Empty;
            }
            GeneratedTypes.Add($"{TypeName}_Object_Type");
            //return $"const ObjectType {Name}({FieldCount},{FieldDesc});\n";
            return $"extern const ObjectType {TypeName}_Object_Type=\r\n" +
                $"{{    \r\n" +
                $"  {{\r\n" +
                $"    TYPE_OBJECT,\r\n" +
                $"  }},\r\n" +
                $"  {FieldCount},\r\n" +
                $"  {FieldDesc},\r\n" +
                $"}};\r\n";
        }
        public string ToExtern()
        {
            return $"extern const ObjectType {TypeName}_Object_Type;\n";
        }
    }
    public class ObjectField : ObjectType
    {
        public string FieldName { get; set; }
        public string Prefix { get; set; } = "Field";
        public string StructName { get; set; }
        public UInt32 FieldNumber { get; set; }
        public new string ToCode()
        {
            string s = $"extern const ObjectType {TypeName}_Object_Type;\r\n";
            s+= $"extern const ObjectField {StructName}_{Prefix}_{FieldName}=\r\n" +
                $"{{ \r\n" +
                $"  {{\r\n" +
                $"    TYPE_OBJECT,\r\n" +
                $"  }},\r\n" +
                $"  \"{StructName}.{FieldName}\",\r\n" +
                $"  offsetof({StructName},{FieldName}),\r\n" +
                $"  &{TypeName}_Object_Type,\r\n" +
                $"  {FieldNumber},\r\n" +
                $"}};\r\n";
            return s;
        }
        public new string ToExtern()
        {
            return $"extern const ObjectField {StructName}_{Prefix}_{FieldName};\n";
        }
    }
    public class EmbedXrpcSerializationHelper
    {
        internal static Dictionary<string, string> TypeReplaceDic = new Dictionary<string, string>();
        static EmbedXrpcSerializationHelper()
        {
            TypeReplaceDic.Add("bool", "UInt8");
            TypeReplaceDic.Add("Boolean", "UInt8");
            TypeReplaceDic.Add("byte", "UInt8");
            TypeReplaceDic.Add("Byte", "UInt8");
            TypeReplaceDic.Add("sbyte", "Int8");
            TypeReplaceDic.Add("SByte", "Int8");
            TypeReplaceDic.Add("UInt16", "UInt16");
            TypeReplaceDic.Add("Int16", "Int16");
            TypeReplaceDic.Add("UInt32", "UInt32");
            TypeReplaceDic.Add("Int32", "Int32");
            TypeReplaceDic.Add("UInt64", "UInt64");
            TypeReplaceDic.Add("Int64", "Int64");
            TypeReplaceDic.Add("float", "Float");
            TypeReplaceDic.Add("double", "Double");
        }
        public static StringBuilder EmitIFieldsArray(string name, IList<string> FieldDesc)
        {
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.Append($"const IField*  {name} []=\n");
            stringBuilder.Append("{\n");
            foreach (var fd in FieldDesc)
            {
                stringBuilder.Append("  (const IField*)&" + fd + ",\n");
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
                if (field.IsArray == false)
                {
                    if (TypeReplaceDic.ContainsKey(field.BitsType == BitsType.NoBits ? field.SourceCodeType : field.BitsType.ToString()) == true)
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
                        if (field.Enum == null)
                        {
                            ObjectField objectField = new ObjectField();
                            objectField.TypeName = $"{field.SourceCodeType}";
                            objectField.FieldName = $"{field.Name}";
                            objectField.StructName = targetStruct.Name;
                            objectField.FieldCount = $"sizeof({field.SourceCodeType}_Object_Type_Desc)/sizeof(IField*)";
                            objectField.FieldDesc = $"{field.SourceCodeType}_Object_Type_Desc";
                            objectField.FieldNumber = field.FieldNumberAttr.Number;
                            cfilestringBuilder.Append(((ObjectType)objectField).ToCode());
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
                    string x = TypeReplaceDic.ContainsKey(field.SourceCodeElementType) == false ?
                        field.SourceCodeElementType + "_Object_Type" : TypeReplaceDic[field.SourceCodeElementType]+"TypeInstance";
                    //没有找到基本数据类型,说明对象元素是Object类型
                    var arrayLenField = IdlInfo.GetArrayLenField(targetStruct.TargetFields, field);


                    ArrayField arrayField = new ArrayField();
                    arrayField.TypeName = $"{field.SourceCodeElementType}";
                    arrayField.FieldName = $"{field.Name}";
                    arrayField.StructName = targetStruct.Name;
                    arrayField.IsFixed = field.MaxCountAttribute.IsFixed;
                    arrayField.LenField = arrayLenField;
                    arrayField.ArrayElementType = x;
                    arrayField.ArrayElementLen = $"sizeof({field.SourceCodeElementType})";
                    arrayField.FieldNumber = field.FieldNumberAttr.Number;
                    cfilestringBuilder.Append(((ArrayType)arrayField).ToCode());
                    cfilestringBuilder.Append(arrayField.ToCode());
                    hfilestringBuilder.Append(arrayField.ToExtern());

                }
            }
            if (FieldsDesc.Count > 0)
            {
                cfilestringBuilder.Append(EmitIFieldsArray($"{targetStruct.Name}_Object_Type_Desc", FieldsDesc));
                hfilestringBuilder.Append($"extern const IField* {targetStruct.Name}_Object_Type_Desc [{FieldsDesc.Count}];\n");
            }


            ObjectType objectType = new ObjectType();
            objectType.TypeName = $"{targetStruct.Name}";
            objectType.FieldCount = FieldsDesc.Count == 0 ? "0" : $"sizeof({targetStruct.Name}_Object_Type_Desc)/sizeof(IField*)";
            objectType.FieldDesc = FieldsDesc.Count == 0 ? "nullptr" : $"{targetStruct.Name}_Object_Type_Desc";

            cfilestringBuilder.Append(objectType.ToCode());
            hfilestringBuilder.Append(objectType.ToExtern());

            cfilewriter.WriteLine(cfilestringBuilder.ToString());
            hfilewriter.WriteLine(hfilestringBuilder.ToString());
        }

    }

}
