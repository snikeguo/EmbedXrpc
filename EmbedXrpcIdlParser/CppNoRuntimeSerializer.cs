using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public class CppNoRuntimeSerializer : ICppSerializable
    {
        public void EmitSerializeMacro(ObjectType_TargetType targetStruct, StreamWriter writer)
        {
            throw new NotImplementedException();
        }

        public void EmitStruct(ObjectType_TargetType targetStruct, StreamWriter cfilewriter, StreamWriter hfilewriter)
        {
            StringBuilder SerializeCodeSb = new StringBuilder();
            StringBuilder DeserializeCodeSb = new StringBuilder();
            StringBuilder FreeCodeSb = new StringBuilder();

            StringBuilder SerializeExternSb = new StringBuilder();
            StringBuilder DeserializeExternSb = new StringBuilder();
            StringBuilder FreeExternSb = new StringBuilder();

            SerializeExternSb.AppendLine($"void {targetStruct.TypeName}_Serialize(SerializationManager *sm,{targetStruct.TypeName} *obj);");
            SerializeCodeSb.AppendLine($"void {targetStruct.TypeName}_Serialize(SerializationManager *sm,{targetStruct.TypeName} *obj)");
            SerializeCodeSb.AppendLine("{");

            DeserializeExternSb.AppendLine($"void {targetStruct.TypeName}_Deserialize(SerializationManager *sm,{targetStruct.TypeName} *obj);");
            DeserializeCodeSb.AppendLine($"void {targetStruct.TypeName}_Deserialize(SerializationManager *sm,{targetStruct.TypeName} *obj)");
            DeserializeCodeSb.AppendLine("{");

            FreeExternSb.AppendLine($"void {targetStruct.TypeName}_FreeData({targetStruct.TypeName} *obj);");
            FreeCodeSb.AppendLine($"void {targetStruct.TypeName}_FreeData({targetStruct.TypeName} *obj)");
            FreeCodeSb.AppendLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                //FieldsDesc.Add($"{targetStruct.Name}_Field_{field.Name}");
                if (field.TargetType.TargetType < TargetType_t.TYPE_ENUM)
                {
                    SerializeCodeSb.AppendLine($"Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName},sizeof(obj->{field.FieldName}));");
                    SerializeCodeSb.AppendLine($"sm->Index+=sizeof(obj->{field.FieldName});");

                    DeserializeCodeSb.AppendLine($"Memcpy(&obj->{field.FieldName},&sm->Buf[sm->Index],sizeof(obj->{field.FieldName}));");
                    DeserializeCodeSb.AppendLine($"sm->Index+=sizeof(obj->{field.FieldName});");

                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ENUM)
                {
                    EnumType_TargetType ettt = field.TargetType as EnumType_TargetType;
                    SerializeCodeSb.AppendLine($"Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName},sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                    SerializeCodeSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");

                    DeserializeCodeSb.AppendLine($"Memcpy(&obj->{field.FieldName},&sm->Buf[sm->Index],sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                    DeserializeCodeSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                {
                    Array_TargetField array_TargetField = field as Array_TargetField;
                    var arrayLenField = array_TargetField.ArrayLenField;
                    ArrayType_TargetType attt = array_TargetField.TargetType as ArrayType_TargetType;
                    var lenstring = string.Empty;
                    var len_type_string = string.Empty;
                    if (arrayLenField != null)
                    {
                        lenstring = $"obj->{arrayLenField.FieldName}";
                        len_type_string = $"{arrayLenField.TargetType.TypeName}";
                    }
                    else
                    {
                        lenstring = "1";
                        len_type_string = "uint8_t";
                    }
                    SerializeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    SerializeCodeSb.AppendLine("{");//for begin

                    DeserializeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    DeserializeCodeSb.AppendLine("{");//for begin

                    FreeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    FreeCodeSb.AppendLine("{");//for begin
                    if (attt.ElementType.TargetType < TargetType_t.TYPE_ENUM)
                    {
                        SerializeCodeSb.AppendLine($"Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({attt.ElementType.TypeName}));");
                        SerializeCodeSb.AppendLine($"sm->Index+=sizeof({attt.ElementType.TypeName});");

                        DeserializeCodeSb.AppendLine($"Memcpy(&obj->{field.FieldName}[{field.FieldName}_index],&sm->Buf[sm->Index],sizeof({attt.ElementType.TypeName}));");
                        DeserializeCodeSb.AppendLine($"sm->Index+=sizeof({attt.ElementType.TypeName});");
                    }
                    else if (attt.ElementType.TargetType == TargetType_t.TYPE_ENUM)
                    {
                        var ettt = attt.ElementType as EnumType_TargetType;
                        SerializeCodeSb.AppendLine($"Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                        SerializeCodeSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");

                        DeserializeCodeSb.AppendLine($"Memcpy(&obj->{field.FieldName}[{field.FieldName}_index],&sm->Buf[sm->Index],sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                        DeserializeCodeSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");
                    }
                    else
                    {
                        SerializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Serialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");
                        DeserializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Deserialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");

                        FreeCodeSb.AppendLine($"{field.TargetType.TypeName}_FreeData(&obj->{field.FieldName}[{field.FieldName}_index]);");
                    }
                    SerializeCodeSb.AppendLine("}");//for end

                    DeserializeCodeSb.AppendLine("}");//for end

                    FreeCodeSb.AppendLine("}");//for end
                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        FreeCodeSb.AppendLine($"Free(obj->{field.FieldName}));");
                    }
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_OBJECT)
                {
                    SerializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Serialize(sm,&obj->{field.FieldName});");
                    DeserializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Deserialize(sm,&obj->{field.FieldName});");
                    FreeCodeSb.AppendLine($"{field.TargetType.TypeName}_FreeData(&obj->{field.FieldName});");
                }
            }
            SerializeCodeSb.AppendLine("}");
            DeserializeCodeSb.AppendLine("}");
            FreeCodeSb.AppendLine("}");

            cfilewriter.WriteLine(SerializeCodeSb.ToString());
            cfilewriter.WriteLine(DeserializeCodeSb.ToString());
            cfilewriter.WriteLine(FreeCodeSb.ToString());

            hfilewriter.WriteLine(SerializeExternSb.ToString());
            hfilewriter.WriteLine(DeserializeExternSb.ToString());
            hfilewriter.WriteLine(FreeExternSb.ToString());
        }
    }

}
