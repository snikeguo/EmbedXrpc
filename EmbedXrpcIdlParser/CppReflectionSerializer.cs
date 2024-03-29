﻿#if false
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{ 
    public class CppReflectionSerializer : ICppSerializable
    {
        static List<string> GeneratedArrayTypes = new List<string>();
        private StringBuilder EmitIFieldsArray(string name, IList<string> FieldDesc)
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
        private void EmitArrayTypeAndObjectType(ITargetType at_ot, StringBuilder cfilestringBuilder, StringBuilder hfilestringBuilder)
        {
            if (GeneratedArrayTypes.Contains(at_ot.TypeName) == true)
                return;
            GeneratedArrayTypes.Add(at_ot.TypeName);
            if (at_ot.TargetType== TargetType_t.TYPE_ARRAY)
            {
                ArrayType_TargetType attt = at_ot as ArrayType_TargetType;
                ITargetType elementType = attt.ElementType;
                var code =  $"extern const ArrayType {elementType.TypeName}_Array_TypeInstance= \r\n" +  //修复C++中的const限定符导致的链接问题
                            $"{{\r\n" +
                            $"  {{ \r\n" +
                            $"    TYPE_ARRAY,\r\n" +
                            $"  }},\r\n" +
                            $"  (const IType *)(&{elementType.TypeName}_TypeInstance),\r\n" +
                            $"  sizeof({elementType.TypeName}),\r\n" +
                            $"}};\r\n";
                var Extern= $"extern const ArrayType {elementType.TypeName}_Array_TypeInstance;\r\n";//多extern几次没关系,不管他
                cfilestringBuilder.Append(code);
                hfilestringBuilder.Append(Extern);
            }
            else if(at_ot.TargetType== TargetType_t.TYPE_STRUCT)
            {
                StructType_TargetType ottt = at_ot as StructType_TargetType;
                List<string> FieldsDesc = new List<string>();
                foreach (var field in ottt.TargetFields)
                {
                    FieldsDesc.Add($"{ottt.TypeName}_Field_{field.FieldName}");
                }
                if (FieldsDesc.Count > 0)
                {
                    cfilestringBuilder.Append(EmitIFieldsArray($"{ottt.TypeName}_Type_Field_Desc", FieldsDesc));
                    hfilestringBuilder.Append($"extern const IField* {ottt.TypeName}_Type_Field_Desc [{FieldsDesc.Count}];\n");
                }
                string FieldCount = FieldsDesc.Count == 0 ? "0" : $"sizeof({ottt.TypeName}_Type_Field_Desc)/sizeof(IField*)";
                string FieldDesc= FieldsDesc.Count == 0 ? "nullptr" : $"{ottt.TypeName}_Type_Field_Desc";
                var code=   $"extern const ObjectType {ottt.TypeName}_TypeInstance=\r\n" +
                            $"{{    \r\n" +
                            $"  {{\r\n" +
                            $"    TYPE_OBJECT,\r\n" +
                            $"  }},\r\n" +
                            $"  {FieldCount},\r\n" +
                            $"  {FieldDesc},\r\n" +
                            $"}};\r\n";
                var Extern= $"extern const ObjectType {ottt.TypeName}_TypeInstance;\n";
                cfilestringBuilder.Append(code);
                hfilestringBuilder.Append(Extern);

            }
        }
        public void EmitStruct(StructType_TargetType objectType_TargetType, StreamWriter cfilewriter, StreamWriter hfilewriter)
        {
            StringBuilder cfilestringBuilder = new StringBuilder();
            StringBuilder hfilestringBuilder = new StringBuilder();
            //List<string> FieldsDesc = new List<string>();
            foreach (var field in objectType_TargetType.TargetFields)
            {
                //FieldsDesc.Add($"{targetStruct.TypeName}_Field_{field.FieldName}");
                if (field.TargetType.TargetType < TargetType_t.TYPE_ENUM) //(EmbedXrpcSerializationHelper.TypeReplaceDic.ContainsKey(field.BitsType == BitsType.NoBits ? field.SourceCodeType : field.BitsType.ToString()) == true)
                {
                    Base_TargetField base_TargetField = field as Base_TargetField;
                    var code = $"extern const {BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}Field {objectType_TargetType.TypeName}_Field_{base_TargetField.FieldName}=\r\n" +
                                $"{{ \r\n" +
                                $"  {{ \r\n" +
                                $"    TYPE_{BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType].ToString().ToUpper()},\r\n" +
                                $"  }},\r\n" +
                                $"  \"{objectType_TargetType.TypeName}.{base_TargetField.FieldName}\" ,\r\n" +
                                $"  offsetof({objectType_TargetType.TypeName},{base_TargetField.FieldName}) ,\r\n" +
                                $"  &{BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}_TypeInstance,  \r\n" +
                                $"  {base_TargetField.FieldNumberAttr.Number},\r\n" +
                                $"  {base_TargetField.IsArrayLenField.ToString().ToLower()},\r\n" +
                                $"}};\r\n";

                    cfilestringBuilder.Append(code);
                    var Extern = $"extern const {BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}Field {objectType_TargetType.TypeName}_Field_{base_TargetField.FieldName};\n";
                    hfilestringBuilder.Append(Extern);
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ENUM)
                {
                    Enum_TargetField enum_TargetField = field as Enum_TargetField;
                    EnumType_TargetType ettt = enum_TargetField.TargetType as EnumType_TargetType;
                    var code = $"extern const {BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}Field {objectType_TargetType.TypeName}_Field_{enum_TargetField.FieldName}=\r\n" +
                               $"{{ \r\n" +
                               $"  {{ \r\n" +
                               $"    TYPE_{BaseType_TargetType.TypeReplaceDic[ettt.NumberType].ToString().ToUpper()},\r\n" +
                               $"  }},\r\n" +
                               $"  \"{objectType_TargetType.TypeName}.{enum_TargetField.FieldName}\" ,\r\n" +
                               $"  offsetof({objectType_TargetType.TypeName},{enum_TargetField.FieldName}) ,\r\n" +
                               $"  &{BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}_TypeInstance,  \r\n" +
                               $"  {enum_TargetField.FieldNumberAttr.Number},\r\n" +
                               $"  {enum_TargetField.IsArrayLenField.ToString().ToLower()},\r\n" +
                               $"}};\r\n";

                    cfilestringBuilder.Append(code);
                    var Extern = $"extern const {BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}Field {objectType_TargetType.TypeName}_Field_{enum_TargetField.FieldName};\n";
                    hfilestringBuilder.Append(Extern);
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                {

                    Array_TargetField array_TargetField = field as Array_TargetField;
                    ArrayType_TargetType attt = array_TargetField.TargetType as ArrayType_TargetType;
                    EmitArrayTypeAndObjectType(attt as ITargetType, cfilestringBuilder, hfilestringBuilder);

                    var arrayLenFieldDesc = array_TargetField.ArrayLenField == null ? "nullptr" : $"(const IField*)&{objectType_TargetType.TypeName}_Field_{array_TargetField.ArrayLenField.FieldName}";
                    //return $"const ArrayField {StructName}_{Prefix}_{Name}({FieldNumber},\"{StructName}.{Name}\",{IsFixed.ToString().ToLower()},&{ArrayElementType},{ArrayElementLen},offsetof({StructName},{Name}),{arrayLenFieldDesc});\n";
                    string code = $"extern const ArrayType {attt.ElementType.TypeName}_Array_TypeInstance;\r\n";
                    code += $"extern const ArrayField {objectType_TargetType.TypeName}_Field_{array_TargetField.FieldName}=\r\n" +
                        $"{{ \r\n" +
                        $"  {{\r\n" +
                        $"    TYPE_ARRAY,\r\n" +
                        $"  }},\r\n" +
                        $"  \"{objectType_TargetType.TypeName}.{array_TargetField.FieldName}\",\r\n" +
                        $"  offsetof({objectType_TargetType.TypeName},{array_TargetField.FieldName}),\r\n" +
                        $"  {arrayLenFieldDesc},\r\n" +
                        $"  &{attt.ElementType.TypeName}_Array_TypeInstance,\r\n" +
                        $"  {array_TargetField.MaxCountAttribute.IsFixed.ToString().ToLower()},\r\n" +
                        $"  {array_TargetField.FieldNumberAttr.Number},\r\n" +
                        $"}};\r\n";
                    cfilestringBuilder.Append(code);
                    var Extern = $"extern const ArrayField {objectType_TargetType.TypeName}_Field_{array_TargetField.FieldName};\n";
                    hfilestringBuilder.Append(Extern);
                    
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_STRUCT)
                {
                    Struct_TargetField object_TargetField = field as Struct_TargetField;
                    StructType_TargetType ottt = object_TargetField.TargetType as StructType_TargetType;
                    string code = $"extern const ObjectType {ottt.TypeName}_TypeInstance;\r\n";
                    code += $"extern const ObjectField {objectType_TargetType.TypeName}_Field_{object_TargetField.FieldName}=\r\n" +
                        $"{{ \r\n" +
                        $"  {{\r\n" +
                        $"    TYPE_OBJECT,\r\n" +
                        $"  }},\r\n" +
                        $"  \"{objectType_TargetType.TypeName}.{object_TargetField.FieldName}\",\r\n" +
                        $"  offsetof({objectType_TargetType.TypeName},{object_TargetField.FieldName}),\r\n" +
                        $"  &{ottt.TypeName}_TypeInstance,\r\n" +
                        $"  {object_TargetField.FieldNumberAttr.Number},\r\n" +
                        $"}};\r\n";
                    cfilestringBuilder.Append(code);
                    var Extern= $"extern const ObjectField {objectType_TargetType.TypeName}_Field_{object_TargetField.FieldName};\n";
                    hfilestringBuilder.Append(Extern);
                }
            }
            EmitArrayTypeAndObjectType(objectType_TargetType, cfilestringBuilder, hfilestringBuilder);
            cfilewriter.WriteLine(cfilestringBuilder.ToString());
            hfilewriter.WriteLine(hfilestringBuilder.ToString());
        }

        public void EmitSerializeMacro(StructType_TargetType targetStruct, StreamWriter writer)
        {
            writer.WriteLine($"#define {targetStruct.TypeName}_Serialize(sm,objptr)    sm.Serialize(&{targetStruct.TypeName}_TypeInstance,objptr,0)");
        }

        public void EmitDeserializeMacro(StructType_TargetType targetStruct, StreamWriter writer)
        {
            writer.WriteLine($"#define {targetStruct.TypeName}_Deserialize(sm,objptr)    sm.Deserialize(&{targetStruct.TypeName}_TypeInstance,objptr)");
        }

        public void EmitFreeDataMacro(StructType_TargetType targetStruct, StreamWriter writer)
        {
            writer.WriteLine($"#define {targetStruct.TypeName}_FreeData(objptr)    SerializationManager::FreeData(&{targetStruct.TypeName}_TypeInstance,objptr)");
        }
    }
}
#endif