using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public interface ICppSerializable
    {
        void EmitStruct(StructType_TargetType targetStructUnion, StreamWriter cfilewriter, StreamWriter hfilewriter);
        void EmitSerializeMacro(StructType_TargetType targetStructUnion, StreamWriter writer);
        void EmitDeserializeMacro(StructType_TargetType targetStructUnion, StreamWriter writer);

        void EmitFreeDataMacro(StructType_TargetType targetStructUnion, StreamWriter writer);
    }
    public static class CppSerializableCommon
    {
        public static string MacroControlWriteBegin(StreamWriter sw, MacroControlAttribute MacroControlAttribute)
        {
            string retstring = string.Empty;
            if (MacroControlAttribute != null)
            {
                string val = MacroControlAttribute.EnableCondition == string.Empty ? string.Empty : $"=={MacroControlAttribute.EnableCondition}";
                
                if (val != string.Empty)
                {
                    retstring = $"#if {MacroControlAttribute.MacroName}{val}";
                    sw?.WriteLine(retstring);
                }
                else
                {
                    retstring = $"#ifdef {MacroControlAttribute.MacroName}";
                    sw?.WriteLine(retstring);
                }
                return retstring;
            }
            return retstring;
        }
        public static string MacroControlWriteEnd(StreamWriter sw, MacroControlAttribute MacroControlAttribute)
        {
            string retstring = string.Empty;
            if (MacroControlAttribute != null)
            {
                string val = MacroControlAttribute.EnableCondition == string.Empty ? string.Empty : $"=={MacroControlAttribute.EnableCondition}";
                if (val != string.Empty)
                {
                    retstring = $"#endif // #if {MacroControlAttribute.MacroName}{val}";
                    sw?.WriteLine(retstring);
                }
                else
                {
                    retstring = $"#endif // #ifdef {MacroControlAttribute.MacroName}";
                    sw?.WriteLine(retstring);
                }
                return retstring;
            }
            return retstring;
        }
        public static string EmitField(ITargetField field)
        {
            string cppType = CppCsNanoSerializer.GetCppTypeDefineName(field.TargetType);
            string valueName = string.Empty;
            if(field.TargetType.TargetType==typeof(Array))//if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
            {
                Array_TargetField array_TargetField = field as Array_TargetField;
                ArrayType_TargetType attt = array_TargetField.TargetType as ArrayType_TargetType;
                if(IdlInfo.IsBaseType(attt.ElementType.TargetType)==true)
                {
                    cppType = CppCsNanoSerializer.GetCppTypeDefineName(attt.ElementType);
                }
                else
                {
                    cppType = attt.ElementType.TypeName;
                }
                if (array_TargetField.MaxCountAttribute.IsFixed == false)
                {
                    cppType = cppType + "*";
                    valueName = field.FieldName;
                }
                else
                {
                    valueName = $"{field.FieldName}[{array_TargetField.MaxCountAttribute.MaxCount}]";
                }
            }
            else
            {
                valueName = field.FieldName;
                if (field is Base_TargetField)
                {
                    Base_TargetField base_TargetField = field as Base_TargetField;
                    if (base_TargetField.BitFieldAttribute != null)
                    {
                        valueName = base_TargetField.FieldName + $":{base_TargetField.BitFieldAttribute.BitWidthLength}";
                    }
                }
                
            }
            string returnvalue = $"{cppType} {valueName}";
            
            return returnvalue;
        }

        public static void EmitEnum(EnumType_TargetType targetEnum, StreamWriter CommonHsw)
        {
            CommonHsw.WriteLine($"typedef enum {CppCsNanoSerializer.GetCppTypeDefineName(targetEnum)} //serialize number type:{CppCsNanoSerializer.TargetTypeStrings[targetEnum.NumberType].CsBaseTargetTypeDefineString}");
            CommonHsw.WriteLine("{");
            foreach (var ev in targetEnum.KeyValue)
            {
                CommonHsw.WriteLine(ev.Key + " = " + ev.Value.ToString() + ",");
            }
            CommonHsw.WriteLine($"}}{CppCsNanoSerializer.GetCppTypeDefineName(targetEnum)};");
        }

        public static void EmitStruct(StructType_TargetType structType, StreamWriter CommonHsw)
        {
            /*
             CommonHsw用来定义具体的数据结构
            SerializeCsw、SerializeHsw用来定义序列化的操作、行为等
             */
            StringBuilder FieldNumberSb = new StringBuilder();
            CommonHsw.WriteLine($"typedef struct {CppCsNanoSerializer.GetCppTypeDefineName(structType)}");
            CommonHsw.WriteLine("{");
            if(structType.CppCustomMethodSignatures!=null)
            {
                foreach (var customMethodSign in structType.CppCustomMethodSignatures)
                {
                    string fh = customMethodSign.Signature[customMethodSign.Signature.Length - 1] == ';' ? "" : ";";
                    CommonHsw.WriteLine(customMethodSign.Signature + fh);
                }
            }
            
            foreach (var field in structType.TargetFields)
            {
                if(field.UnionFieldAttr!=null&& field.UnionFieldAttr.UnionIndex== UnionIndex.Start)
                {
                    CommonHsw.WriteLine("union{");
                }
                
                var baseValueField = field as Base_TargetField;
                string UnionTargetTypeString = string.Empty;
                if (baseValueField != null)
                    UnionTargetTypeString = baseValueField.IsUnionTargetTypeField == true ? "Union Target Type" : string.Empty;
                string NoSerializationString=string.Empty;
                if(field.MacroControlAttr!=null)
                {
                    var macroString=MacroControlWriteBegin(CommonHsw, field.MacroControlAttr);
                    FieldNumberSb.AppendLine(macroString);

                }
                if (field.NoSerializationAttr != null)
                {
                    NoSerializationString= "NoSerialization";
                }
                CommonHsw.WriteLine($"{EmitField(field)};   //FieldNumber:{field.FieldNumberAttr.Number}    {UnionTargetTypeString} {NoSerializationString}");
                FieldNumberSb.AppendLine($"#define {CppCsNanoSerializer.GetCppTypeDefineName(structType)}_{field.FieldName}_FieldNumber  {field.FieldNumberAttr.Number}");
                if (field.MacroControlAttr != null)
                {
                    var macroString = MacroControlWriteEnd(CommonHsw, field.MacroControlAttr);
                    FieldNumberSb.AppendLine(macroString);
                }

                if (field.UnionFieldAttr != null && field.UnionFieldAttr.UnionIndex == UnionIndex.End)
                {
                    CommonHsw.WriteLine("};//union end");
                }
            }
            CommonHsw.WriteLine($"}}{CppCsNanoSerializer.GetCppTypeDefineName(structType)};");
            FieldNumberSb.Append("\r\n");
            CommonHsw.WriteLine(FieldNumberSb.ToString());


        }
    }
}
