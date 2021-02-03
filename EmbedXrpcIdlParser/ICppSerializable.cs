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
        void EmitStruct(StructType_TargetType targetStructUnion, StreamWriter cfilewriter, StreamWriter hfilewriter,bool isEncodeTlv);
        void EmitSerializeMacro(StructType_TargetType targetStructUnion, StreamWriter writer);
        void EmitDeserializeMacro(StructType_TargetType targetStructUnion, StreamWriter writer);

        void EmitFreeDataMacro(StructType_TargetType targetStructUnion, StreamWriter writer);
    }
    public static class CppSerializableCommon
    {
        public static string EmitField(ITargetField field)
        {
            string cppType = field.TargetType.TypeName;
            string valueName = string.Empty;
            if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
            {
                Array_TargetField array_TargetField = field as Array_TargetField;
                ArrayType_TargetType attt = array_TargetField.TargetType as ArrayType_TargetType;
                cppType = attt.ElementType.TypeName;
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
            }
            return $"{cppType} {valueName}";
        }

        public static void EmitEnum(EnumType_TargetType targetEnum, StreamWriter CommonHsw)
        {
            CommonHsw.WriteLine("typedef enum _" + targetEnum.TypeName);
            CommonHsw.WriteLine("{");
            foreach (var ev in targetEnum.KeyValue)
            {
                CommonHsw.WriteLine(ev.Key + " = " + ev.Value.ToString() + ",");
            }
            CommonHsw.WriteLine("}" + targetEnum.TypeName + ";");
        }

        public static void EmitStruct(StructType_TargetType structType, StreamWriter CommonHsw)
        {
            /*
             CommonHsw用来定义具体的数据结构
            SerializeCsw、SerializeHsw用来定义序列化的操作、行为等
             */
            StringBuilder FieldNumberSb = new StringBuilder();
            bool isWriteUnion = false;
            bool IsUnionComplete = false;
            CommonHsw.WriteLine("typedef struct _" + structType.TypeName);
            CommonHsw.WriteLine("{");
            foreach (var field in structType.TargetFields)
            {
                if(field.UnionFieldAttr!=null&& isWriteUnion==false)
                {
                    if(IsUnionComplete==true)
                    {
                        throw new Exception();
                    }
                    CommonHsw.WriteLine("union{");
                    isWriteUnion=true;
                }
                if(field.UnionFieldAttr == null&& isWriteUnion == true && IsUnionComplete==false)
                {
                    CommonHsw.WriteLine("};//union end");
                    IsUnionComplete = true;
                }
                var baseValueField = field as Base_TargetField;
                string UnionTargetTypeString = string.Empty;
                if (baseValueField != null)
                    UnionTargetTypeString = baseValueField.IsUnionTargetTypeField == true ? "Union Target Type" : string.Empty;
                CommonHsw.WriteLine($"{EmitField(field)};   //FieldNumber:{field.FieldNumberAttr.Number}    {UnionTargetTypeString}");
                FieldNumberSb.Append($"#define {structType.TypeName}_{field.FieldName}_FieldNumber  {field.FieldNumberAttr.Number}\r\n");
            }
            if (isWriteUnion == true && IsUnionComplete == false)
            {
                CommonHsw.WriteLine("};//union end");
                IsUnionComplete = true;
            }
            CommonHsw.WriteLine("}" + structType.TypeName + ";");
            FieldNumberSb.Append("\r\n");
            CommonHsw.WriteLine(FieldNumberSb.ToString());


        }
    }
}
