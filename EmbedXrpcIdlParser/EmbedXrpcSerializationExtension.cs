using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{ 
    public interface IEmbedXrpcSerializationGenerator
    {
        void EmitStruct(ObjectType_TargetType targetStruct, StreamWriter cfilewriter, StreamWriter hfilewriter);
    }
#if false
    public class EmbedXrpcNoRuntimeSerializer : IEmbedXrpcSerializationGenerator
    {
        public void EmitStruct(ObjectType_TargetType targetStruct, StreamWriter cfilewriter, StreamWriter hfilewriter)
        {
            StringBuilder CfilestringBuilder = new StringBuilder();
            StringBuilder HfilestringBuilder = new StringBuilder();
            //List<string> FieldsDesc = new List<string>();
            CfilestringBuilder.AppendLine($"void {targetStruct.Name}_Serialize(SerializationManager &sm,{targetStruct.Name} &obj)");
            CfilestringBuilder.AppendLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                //FieldsDesc.Add($"{targetStruct.Name}_Field_{field.Name}");
                if (field.IsArray == false)
                {
                    if (EmbedXrpcSerializationHelper.TypeReplaceDic.ContainsKey(field.BitsType == BitsType.NoBits ? field.SourceCodeType : field.BitsType.ToString()) == true)
                    {
                        CfilestringBuilder.AppendLine($"Memcpy(&sm.Buf[sm.Index],&obj.{field.Name},sizeof(obj.{field.Name}));");
                        CfilestringBuilder.AppendLine($"sm.Index+=sizeof(obj.{field.Name});");
                    }
                    else
                    {
                        if (field.Enum == null)//object类型
                        {
                            CfilestringBuilder.AppendLine($"{field.SourceCodeType}_Serialize(sm,obj.{field.Name});");
                        }
                        else//枚举类型
                        {
                            CfilestringBuilder.AppendLine($"Memcpy(&sm.Buf[sm.Index],&obj.{field.Name},sizeof({field.Enum.SourceCodeType}));");
                            CfilestringBuilder.AppendLine($"sm.Index+=sizeof({field.Enum.SourceCodeType});");
                        }
                    }
                }
                else
                {
                    string x = EmbedXrpcSerializationHelper.TypeReplaceDic.ContainsKey(field.SourceCodeElementType) == false ?
                        field.SourceCodeElementType + "_Object_Type" : EmbedXrpcSerializationHelper.TypeReplaceDic[field.SourceCodeElementType] + "TypeInstance";
                    //没有找到基本数据类型,说明对象元素是Object类型
                    var arrayLenField = IdlInfo.GetArrayLenField(targetStruct.TargetFields, field);
                    var lenstring = string.Empty;
                    var len_type_string = string.Empty;
                    if (arrayLenField != null)
                    {
                        lenstring = $"obj.{arrayLenField.Name}";
                        len_type_string = $"{arrayLenField.SourceCodeType}";
                    }
                    else
                    {
                        lenstring = "1";
                        len_type_string = "uint32_t";
                    }
                    CfilestringBuilder.AppendLine($"for({len_type_string} {field.Name}_index=0;{field.Name}_index<{lenstring};{field.Name}_index++)");
                    CfilestringBuilder.AppendLine("{");
                    if (field.IsBaseValueType == true)
                    {
                        CfilestringBuilder.AppendLine($"Memcpy(&sm.Buf[sm.Index],&obj.{field.Name}[{field.Name}_index],sizeof({field.SourceCodeElementType}));");
                        CfilestringBuilder.AppendLine($"sm.Index+=sizeof({field.SourceCodeElementType});");
                    }
                    else
                    {
                        CfilestringBuilder.AppendLine($"{field.SourceCodeElementType}_Serialize(sm,obj.{field.Name}[{field.Name}_index]);");
                    }
                    CfilestringBuilder.AppendLine("}");
                }
            }
            CfilestringBuilder.AppendLine("}");

            //Deserialize
            CfilestringBuilder.AppendLine($"void {targetStruct.Name}_Deserialize(SerializationManager &sm,{targetStruct.Name} &obj)");
            CfilestringBuilder.AppendLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                //FieldsDesc.Add($"{targetStruct.Name}_Field_{field.Name}");
                if (field.IsArray == false)
                {
                    if (EmbedXrpcSerializationHelper.TypeReplaceDic.ContainsKey(field.BitsType == BitsType.NoBits ? field.SourceCodeType : field.BitsType.ToString()) == true)
                    {
                        CfilestringBuilder.AppendLine($"Memcpy(&obj.{field.Name},&sm.Buf[sm.Index],sizeof(obj.{field.Name}));");
                        CfilestringBuilder.AppendLine($"sm.Index+=sizeof(obj.{field.Name});");
                    }
                    else
                    {
                        if (field.Enum == null)//object类型
                        {
                            CfilestringBuilder.AppendLine($"{field.SourceCodeType}_Deserialize(sm,obj.{field.Name});");
                        }
                        else//枚举类型
                        {
                            CfilestringBuilder.AppendLine($"Memcpy(&obj.{field.Name},&sm.Buf[sm.Index],sizeof({field.Enum.SourceCodeType}));");
                            CfilestringBuilder.AppendLine($"sm.Index+=sizeof({field.Enum.SourceCodeType});");
                        }
                    }
                }
                else
                {
                    string x = EmbedXrpcSerializationHelper.TypeReplaceDic.ContainsKey(field.SourceCodeElementType) == false ?
                        field.SourceCodeElementType + "_Object_Type" : EmbedXrpcSerializationHelper.TypeReplaceDic[field.SourceCodeElementType] + "TypeInstance";
                    //没有找到基本数据类型,说明对象元素是Object类型
                    var arrayLenField = IdlInfo.GetArrayLenField(targetStruct.TargetFields, field);
                    var lenstring = string.Empty;
                    var len_type_string = string.Empty;
                    if (arrayLenField != null)
                    {
                        lenstring = $"obj.{arrayLenField.Name}";
                        len_type_string = $"{arrayLenField.SourceCodeType}";
                        if (field.MaxCountAttribute.IsFixed == false)
                        {
                            CfilestringBuilder.AppendLine($"obj.{field.Name}=Malloc({arrayLenField.Name}*sizeof({field.SourceCodeElementType}));");
                        }
                    }
                    else
                    {
                        lenstring = "1";
                        len_type_string = "uint32_t";
                    }
                    CfilestringBuilder.AppendLine($"for({len_type_string} {field.Name}_index=0;{field.Name}_index<{lenstring};{field.Name}_index++)");
                    CfilestringBuilder.AppendLine("{");
                    if (field.IsBaseValueType == true)
                    {
                        CfilestringBuilder.AppendLine($"Memcpy(&obj.{field.Name}[{field.Name}_index],&sm.Buf[sm.Index],sizeof({field.SourceCodeElementType}));");
                        CfilestringBuilder.AppendLine($"sm.Index+=sizeof({field.SourceCodeElementType});");
                    }
                    else
                    {
                        CfilestringBuilder.AppendLine($"{field.SourceCodeElementType}_Deserialize(sm,obj.{field.Name}[{field.Name}_index]);");
                    }
                    CfilestringBuilder.AppendLine("}");
                }
            }
            CfilestringBuilder.AppendLine("}");

            //Free
            CfilestringBuilder.AppendLine($"void {targetStruct.Name}_FreeData({targetStruct.Name} &obj)");
            CfilestringBuilder.AppendLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                if (field.IsArray == false)
                {
                    if (EmbedXrpcSerializationHelper.TypeReplaceDic.ContainsKey(field.BitsType == BitsType.NoBits ? field.SourceCodeType : field.BitsType.ToString()) == true)
                    {
                        //CfilestringBuilder.AppendLine($"Memcpy(&obj.{field.Name},&sm.Buf[sm.Index],sizeof(obj.{field.Name}));");
                        //CfilestringBuilder.AppendLine($"sm.Index+=sizeof(obj.{field.Name});");
                    }
                    else
                    {
                        if (field.Enum == null)//object类型
                        {
                            CfilestringBuilder.AppendLine($"{field.SourceCodeType}_FreeData(obj.{field.Name});");
                        }
                        else//枚举类型
                        {
                            //CfilestringBuilder.AppendLine($"Memcpy(&obj.{field.Name},&sm.Buf[sm.Index],sizeof({field.Enum.SourceCodeType}));");
                            //CfilestringBuilder.AppendLine($"sm.Index+=sizeof({field.Enum.SourceCodeType});");
                        }
                    }
                }
                else
                {
                    string x = EmbedXrpcSerializationHelper.TypeReplaceDic.ContainsKey(field.SourceCodeElementType) == false ?
                        field.SourceCodeElementType + "_Object_Type" : EmbedXrpcSerializationHelper.TypeReplaceDic[field.SourceCodeElementType] + "TypeInstance";
                    //没有找到基本数据类型,说明对象元素是Object类型
                    var arrayLenField = IdlInfo.GetArrayLenField(targetStruct.TargetFields, field);
                    var lenstring = string.Empty;
                    var len_type_string = string.Empty;
                    if (arrayLenField != null)
                    {
                        lenstring = $"obj.{arrayLenField.Name}";
                        len_type_string = $"{arrayLenField.SourceCodeType}";
                    }
                    else
                    {
                        lenstring = "1";
                        len_type_string = "uint32_t";
                    }
                    CfilestringBuilder.AppendLine($"for({len_type_string} {field.Name}_index=0;{field.Name}_index<{lenstring};{field.Name}_index++)");
                    CfilestringBuilder.AppendLine("{");
                    if (field.IsBaseValueType == true)
                    {
                        //CfilestringBuilder.AppendLine($"Memcpy(&obj.{field.Name}[{field.Name}_index],&sm.Buf[sm.Index],sizeof({field.SourceCodeElementType}));");
                        //CfilestringBuilder.AppendLine($"sm.Index+=sizeof({field.SourceCodeElementType});");
                    }
                    else
                    {
                        CfilestringBuilder.AppendLine($"{field.SourceCodeElementType}_FreeData(obj.{field.Name}[{field.Name}_index]);");
                    }
                    CfilestringBuilder.AppendLine("}");

                    if (field.MaxCountAttribute.IsFixed == false)
                    {
                        CfilestringBuilder.AppendLine($"Free(obj.{field.Name}));");
                    }
                }
            }
            CfilestringBuilder.AppendLine("}");


            cfilewriter.WriteLine(CfilestringBuilder.ToString());
            hfilewriter.WriteLine(HfilestringBuilder.ToString());
        }
    }
#endif
    public class EmbedXrpcNeedRuntimeSerializer : IEmbedXrpcSerializationGenerator
    {
        static List<ITargetType> GeneratedTypes = new List<ITargetType>();
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
            if (GeneratedTypes.Contains(at_ot) == true)
                return;
            GeneratedTypes.Add(at_ot);
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
            else if(at_ot.TargetType== TargetType_t.TYPE_OBJECT)
            {
                ObjectType_TargetType ottt = at_ot as ObjectType_TargetType;
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
        public void EmitStruct(ObjectType_TargetType objectType_TargetType, StreamWriter cfilewriter, StreamWriter hfilewriter)
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
                else if (field.TargetType.TargetType == TargetType_t.TYPE_OBJECT)
                {
                    Object_TargetField object_TargetField = field as Object_TargetField;
                    ObjectType_TargetType ottt = object_TargetField.TargetType as ObjectType_TargetType;
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
    }
}
