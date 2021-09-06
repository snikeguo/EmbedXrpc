using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public class CppNanoSerializer : ICppSerializable
    {
        public void EmitSerializeMacro(StructType_TargetType targetStructUnion, StreamWriter writer)
        {
            //writer.WriteLine($"#define {targetStruct.TypeName}_Serialize(sm,objptr)    sm.Serialize(&{targetStruct.TypeName}_TypeInstance,objptr,0)");
        }

        public void EmitDeserializeMacro(StructType_TargetType targetStructUnion, StreamWriter writer)
        {
            //writer.WriteLine($"#define {targetStruct.TypeName}_Deserialize(sm,objptr)    sm.Deserialize(&{targetStruct.TypeName}_TypeInstance,objptr)");
        }

        public void EmitFreeDataMacro(StructType_TargetType targetStructUnion, StreamWriter writer)
        {
            //writer.WriteLine($"#define {targetStruct.TypeName}_FreeData(objptr)    SerializationManager::FreeData(&{targetStruct.TypeName}_TypeInstance,objptr)");
        }
        private static Dictionary<TargetType_t, string> TargetTypeString = new Dictionary<TargetType_t, string>();
        static CppNanoSerializer()
        {
            TargetTypeString.Add(TargetType_t.TYPE_UINT8, "TYPE_UINT8");
            TargetTypeString.Add(TargetType_t.TYPE_INT8, "TYPE_INT8");
            TargetTypeString.Add(TargetType_t.TYPE_UINT16, "TYPE_UINT16");
            TargetTypeString.Add(TargetType_t.TYPE_INT16, "TYPE_INT16");
            TargetTypeString.Add(TargetType_t.TYPE_UINT32, "TYPE_UINT32");
            TargetTypeString.Add(TargetType_t.TYPE_INT32, "TYPE_INT32");
            TargetTypeString.Add(TargetType_t.TYPE_UINT64, "TYPE_UINT64");
            TargetTypeString.Add(TargetType_t.TYPE_INT64, "TYPE_INT64");
            TargetTypeString.Add(TargetType_t.TYPE_FLOAT, "TYPE_FLOAT");
            TargetTypeString.Add(TargetType_t.TYPE_DOUBLE, "TYPE_DOUBLE");
            TargetTypeString.Add(TargetType_t.TYPE_ARRAY, "TYPE_ARRAY");
            TargetTypeString.Add(TargetType_t.TYPE_STRUCT, "TYPE_OBJECT");
        }

        public void EmitStruct(StructType_TargetType targetStructUnion, StreamWriter cfilewriter, StreamWriter hfilewriter, bool isEncodeTlv)
        {
            StringBuilder SerializeCodeSb = new StringBuilder();
            StringBuilder DeserializeCodeSb = new StringBuilder();
            StringBuilder FreeCodeSb = new StringBuilder();

            StringBuilder SerializeExternSb = new StringBuilder();
            StringBuilder DeserializeExternSb = new StringBuilder();
            StringBuilder FreeExternSb = new StringBuilder();


            SerializeExternSb.AppendLine($"void {targetStructUnion.TypeName}_Serialize(SerializationManager &sm,{targetStructUnion.TypeName} *obj);");
            SerializeCodeSb.AppendLine($"void {targetStructUnion.TypeName}_Serialize(SerializationManager &sm,{targetStructUnion.TypeName} *obj)");
            SerializeCodeSb.AppendLine("{");

            DeserializeExternSb.AppendLine($"void {targetStructUnion.TypeName}_Deserialize(SerializationManager &sm,{targetStructUnion.TypeName} *obj);");
            DeserializeCodeSb.AppendLine($"void {targetStructUnion.TypeName}_Deserialize(SerializationManager &sm,{targetStructUnion.TypeName} *obj)");
            DeserializeCodeSb.AppendLine("{");

            FreeExternSb.AppendLine($"void {targetStructUnion.TypeName}_FreeData({targetStructUnion.TypeName} *obj);");
            FreeCodeSb.AppendLine($"void {targetStructUnion.TypeName}_FreeData({targetStructUnion.TypeName} *obj)");
            FreeCodeSb.AppendLine("{");

            List<ITargetField> TargetFields= targetStructUnion.TargetFields;
            ITargetField field = null;

            bool unionIfFirstFlag = true;
            /*if (isEncodeTlv == true)
            {
                SerializeCodeSb.AppendLine($"SerializeKey(0,{TargetTypeString[(int)field.TargetType.TargetType]});");
            }*/
            for (int i=0;i<TargetFields.Count;i++)
            {
                field = TargetFields[i];
                if (field.UnionFieldAttr != null)
                {
                    string elsestring = (unionIfFirstFlag == true ? "" : "else");
                    SerializeCodeSb.AppendLine($"{elsestring} if(obj->{targetStructUnion.UnionTargetTypeField.FieldName}=={targetStructUnion.TypeName}_{field.FieldName}_FieldNumber)");
                    SerializeCodeSb.AppendLine("{");

                    DeserializeCodeSb.AppendLine($"{elsestring} if(obj->{targetStructUnion.UnionTargetTypeField.FieldName}=={targetStructUnion.TypeName}_{field.FieldName}_FieldNumber)");
                    DeserializeCodeSb.AppendLine("{");

                    FreeCodeSb.AppendLine($"{elsestring} if(obj->{targetStructUnion.UnionTargetTypeField.FieldName}=={targetStructUnion.TypeName}_{field.FieldName}_FieldNumber)");
                    FreeCodeSb.AppendLine("{");
                    unionIfFirstFlag = false;
                }
                //FieldsDesc.Add($"{targetStruct.Name}_Field_{field.Name}");
                if (field.TargetType.TargetType < TargetType_t.TYPE_ENUM)
                {
                    if (isEncodeTlv == true)
                    {
                        SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    }
                    SerializeCodeSb.AppendLine($"Memcpy(&sm.Buf[sm.Index],&obj->{field.FieldName},sizeof(obj->{field.FieldName}));");
                    SerializeCodeSb.AppendLine($"sm.Index+=sizeof(obj->{field.FieldName});\r\n");

                    DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,sizeof(obj->{field.FieldName}));");

                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ENUM)
                {
                    EnumType_TargetType ettt = field.TargetType as EnumType_TargetType;
                    if (isEncodeTlv == true)
                    {
                        SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[ettt.NumberType]});");
                    }
                    SerializeCodeSb.AppendLine($"Memcpy(&sm.Buf[sm.Index],&obj->{field.FieldName},sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                    SerializeCodeSb.AppendLine($"sm.Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});\r\n");

                    DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");

                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                {
                    if (isEncodeTlv == true)
                    {
                        SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    }
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
                    if(isEncodeTlv==true)
                    {
                        //todo 添加SerializeLen函数
                        //函数原型void SerializeLen(SerializationManager &sm,UInt32 Len);
                        SerializeCodeSb.AppendLine($"SerializeLen(sm,{lenstring});");
                        if (attt.ElementType.TargetType < TargetType_t.TYPE_ENUM)
                        {
                            SerializeCodeSb.AppendLine($"SerializeArrayElementFlag(sm,{TargetTypeString[attt.ElementType.TargetType]}<<4|0x01);");
                        }
                        else
                        {
                            SerializeCodeSb.AppendLine($"SerializeArrayElementFlag(sm,{TargetTypeString[attt.ElementType.TargetType]}<<4|0x02);");
                        }
                    }
                    
                    SerializeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    SerializeCodeSb.AppendLine("{");//for begin

                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        DeserializeCodeSb.AppendLine($"obj->{field.FieldName}=({attt.ElementType.TypeName} *)Malloc(sizeof({attt.ElementType.TypeName})*{lenstring});");
                        DeserializeCodeSb.AppendLine($"Memset(obj->{field.FieldName},0,sizeof({attt.ElementType.TypeName})*{lenstring});");
                    }

                    DeserializeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    DeserializeCodeSb.AppendLine("{");//for begin

                    FreeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    FreeCodeSb.AppendLine("{");//for begin
                    if (attt.ElementType.TargetType < TargetType_t.TYPE_ENUM)
                    {
                        SerializeCodeSb.AppendLine($"Memcpy(&sm.Buf[sm.Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({attt.ElementType.TypeName}));");
                        SerializeCodeSb.AppendLine($"sm.Index+=sizeof({attt.ElementType.TypeName});\r\n");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,sizeof({attt.ElementType.TypeName}));");

                    }
                    else if (attt.ElementType.TargetType == TargetType_t.TYPE_ENUM)
                    {
                        var ettt = attt.ElementType as EnumType_TargetType;
                        SerializeCodeSb.AppendLine($"Memcpy(&sm.Buf[sm.Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                        SerializeCodeSb.AppendLine($"sm.Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});\r\n");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                    }
                    else
                    {
                        SerializeCodeSb.AppendLine($"{attt.ElementType.TypeName}_Serialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);\r\n");
                        DeserializeCodeSb.AppendLine($"{attt.ElementType.TypeName}_Deserialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);\r\n");

                        FreeCodeSb.AppendLine($"{attt.ElementType.TypeName}_FreeData(&obj->{field.FieldName}[{field.FieldName}_index]);\r\n");
                    }
                    SerializeCodeSb.AppendLine("}\r\n");//for end

                    DeserializeCodeSb.AppendLine("}\r\n");//for end

                    FreeCodeSb.AppendLine("}\r\n");//for end
                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        FreeCodeSb.AppendLine($"Free(obj->{field.FieldName});\r\n");
                    }
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_STRUCT)
                {
                    if (isEncodeTlv == true)
                    {
                        SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    }
                    SerializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Serialize(sm,&obj->{field.FieldName});\r\n");
                    DeserializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Deserialize(sm,&obj->{field.FieldName});\r\n");
                    FreeCodeSb.AppendLine($"{field.TargetType.TypeName}_FreeData(&obj->{field.FieldName});\r\n");
                }
                if (field.UnionFieldAttr != null)
                {
                    SerializeCodeSb.AppendLine("}");
                    DeserializeCodeSb.AppendLine("}");
                    FreeCodeSb.AppendLine("}");
                }
            }
            
            SerializeCodeSb.AppendLine("}\r\n");
            DeserializeCodeSb.AppendLine("}\r\n");
            FreeCodeSb.AppendLine("}\r\n");

            cfilewriter.WriteLine(SerializeCodeSb.ToString());
            cfilewriter.WriteLine(DeserializeCodeSb.ToString());
            cfilewriter.WriteLine(FreeCodeSb.ToString());

            hfilewriter.WriteLine(SerializeExternSb.ToString());
            hfilewriter.WriteLine(DeserializeExternSb.ToString());
            hfilewriter.WriteLine(FreeExternSb.ToString());
        }
    }

}
