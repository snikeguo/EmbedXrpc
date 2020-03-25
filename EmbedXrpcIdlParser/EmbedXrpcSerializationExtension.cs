using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
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
                        cfilestringBuilder.Append($"{FieldReplaceDic[field.IdlType]} {name}_{field.Name}");
                        cfilestringBuilder.Append($"(\"{name}.{field.Name}\",offsetof({name},{field.Name}));\n");
                        hfilestringBuilder.Append($"extern {FieldReplaceDic[field.IdlType]} {name}_{field.Name};\n");
                    }
                    else
                    {
                        if(field.Enum==null)
                        {
                            cfilestringBuilder.Append($"ObjectField {name}_{field.Name}(\"{name}.{field.Name}\"," +
                            $"sizeof({field.IdlType}Desc/sizeof(IField*)," +
                            $"sizeof({field.IdlType}Desc," +
                            $"offsetof({name},{field.Name});\n");

                            hfilestringBuilder.Append($"extern ObjectField{name}_{field.Name};\n");
                        }
                        else
                        {
                            cfilestringBuilder.Append($"{FieldReplaceDic[field.Enum.IntType]} {name}_{field.Name}(\"{name}.{field.Name}\",offsetof({name},{field.Name}));\n");
                            hfilestringBuilder.Append($"extern {FieldReplaceDic[field.Enum.IntType]} {name}_{field.Name};\n");
                        }
                    }
                }
                else
                {
                    string x= TypeReplaceDic.ContainsKey(CppCodeGenerater.GetIdlTypeArrayElementType(field)) == false ?
                        CppCodeGenerater.GetIdlTypeArrayElementType(field)+"_Type" : TypeReplaceDic[CppCodeGenerater.GetIdlTypeArrayElementType(field)];
                    cfilestringBuilder.Append($"ArrayField {name}_{field.Name}(\"{name}.{field.Name}\"," +
                            $"{field.MaxCountAttribute.IsFixed.ToString().ToLower()}," +
                           $"&{x}," +
                           $"sizeof({CppCodeGenerater.GetIdlTypeArrayElementType(field)}),"+
                            $"offsetof({name},{field.Name})," +
                            $"&{name}_{fields[field.MaxCountAttribute.LenFieldIndex].Name});\n");
                    hfilestringBuilder.Append($"extern ArrayField {name}_{field.Name};\n");
                }
            }
            cfilestringBuilder.Append(EmitIFieldsArray($"{name}", FieldsDesc));
            cfilestringBuilder.Append($"ObjectType {name}_Type(sizeof({name}Desc)/sizeof(IField*),{name}Desc);\n");
            hfilestringBuilder.Append($"extern ObjectType {name}_Type;\n");

            cfilewriter.WriteLine(cfilestringBuilder.ToString());
            hfilewriter.WriteLine(hfilestringBuilder.ToString());
            
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
