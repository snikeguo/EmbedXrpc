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
        internal class BitFieldStateMachine
        {
            internal List<string> SerializeCodeString = new List<string>();
            internal List<string> DeserializeCodeString = new List<string>();
            internal int BitFieldLeftShiftAccer = 0;
            static internal int BitFieldTempValueCount = 0;
            internal string BitFieldCppTypeString = string.Empty;
            internal void Reset()
            {
                SerializeCodeString.Clear();
                DeserializeCodeString.Clear();
                BitFieldLeftShiftAccer = 0;
                //BitFieldTempValueCount++;
                BitFieldCppTypeString = string.Empty;
            }
            internal void BitFieldProcess(StringBuilder s, StringBuilder d, ITargetField nextTargetField)
            {
                bool writeFlag = false;
                if (nextTargetField is Base_TargetField)
                {
                    Base_TargetField nextBaseTargetField = nextTargetField as Base_TargetField;
                    if (nextBaseTargetField.BitFieldAttribute == null)
                    {
                        writeFlag = true;
                    }
                    else
                    {
                        writeFlag = false;
                        var subBitFieldCppTypeString = $"{BaseType_TargetType.TypeReplaceDic[nextTargetField.TargetType.TargetType]}";
                        if (subBitFieldCppTypeString != this.BitFieldCppTypeString)
                        {
                            throw new Exception("相邻的bit字段的类型必须一样!");
                        }
                    }
                }
                else
                {
                    //nextTargetField ：null/enum/array/object
                    writeFlag = true;
                }
                if (writeFlag==true) 
                {
                    SerializeCodeString.Add($"El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue{BitFieldTempValueCount},sizeof({BitFieldCppTypeString}));");
                    SerializeCodeString.Add($"sm->Index+=sizeof({BitFieldCppTypeString});");
                   
                    foreach (var ss in SerializeCodeString)
                    {
                        s.AppendLine(ss);
                    }
                    s.AppendLine();
                    foreach (var ds in DeserializeCodeString)
                    {
                        d.AppendLine(ds);
                    }
                    d.AppendLine();
                    if (BitFieldCppTypeString!=string.Empty)
                    {
                        BitFieldTempValueCount++;
                    }
                    Reset();
                }
            }
        }
        
        public void EmitStruct(StructType_TargetType targetStructUnion, StreamWriter hfilewriter)
        {
            //StringBuilder SerializeCodeSb = new StringBuilder();
            //StringBuilder DeserializeCodeSb = new StringBuilder();
            //StringBuilder FreeCodeSb = new StringBuilder();

            StringBuilder SerializeHeaderSb = new StringBuilder();
            StringBuilder DeserializeHeaderSb = new StringBuilder();
            StringBuilder FreeHeaderSb = new StringBuilder();

            SerializeHeaderSb.AppendLine("template<class DTL>");
            //SerializeExternSb.AppendLine($"void {targetStructUnion.TypeName}_Serialize(SerializationManager<DTL> *sm,{targetStructUnion.TypeName} *obj);");
            SerializeHeaderSb.AppendLine($"void {targetStructUnion.TypeName}_Serialize(SerializationManager<DTL> *sm,{targetStructUnion.TypeName} *obj)");
            SerializeHeaderSb.AppendLine("{");

            //DeserializeHeaderSb.AppendLine($"void {targetStructUnion.TypeName}_Deserialize(SerializationManager *sm,{targetStructUnion.TypeName} *obj);");
            DeserializeHeaderSb.AppendLine("template<class DTL>");
            DeserializeHeaderSb.AppendLine($"void {targetStructUnion.TypeName}_Deserialize(SerializationManager<DTL> *sm,{targetStructUnion.TypeName} *obj)");
            DeserializeHeaderSb.AppendLine("{");

            //FreeHeaderSb.AppendLine($"void {targetStructUnion.TypeName}_FreeData({targetStructUnion.TypeName} *obj);");
            FreeHeaderSb.AppendLine("template<class DTL>");
            FreeHeaderSb.AppendLine($"void {targetStructUnion.TypeName}_FreeData({targetStructUnion.TypeName} *obj)");
            FreeHeaderSb.AppendLine("{");

            List<ITargetField> TargetFields= targetStructUnion.TargetFields;
            ITargetField field = null;

            bool unionIfFirstFlag = true;
            /*if (isEncodeTlv == true)
            {
                SerializeCodeSb.AppendLine($"SerializeKey(0,{TargetTypeString[(int)field.TargetType.TargetType]});");
            }*/
            BitFieldStateMachine bitFieldStateMachine = new BitFieldStateMachine();
            BitFieldStateMachine.BitFieldTempValueCount = 0;
            for (int i=0;i<TargetFields.Count;i++)
            {
                field = TargetFields[i];
                if (field.NoSerializationAttr != null)
                {
                    SerializeHeaderSb.AppendLine($"//{field.FieldName} :NoSerialization");
                    DeserializeHeaderSb.AppendLine($"//{field.FieldName} :NoSerialization");
                    FreeHeaderSb.AppendLine($"//{field.FieldName} :NoSerialization ");
                    continue;
                }
                if (field.UnionFieldAttr != null)
                {
                    string elsestring = (unionIfFirstFlag == true ? "" : "else");
                    SerializeHeaderSb.AppendLine($"{elsestring} if(obj->{targetStructUnion.UnionTargetTypeField.FieldName}=={targetStructUnion.TypeName}_{field.FieldName}_FieldNumber)");
                    SerializeHeaderSb.AppendLine("{");

                    DeserializeHeaderSb.AppendLine($"{elsestring} if(obj->{targetStructUnion.UnionTargetTypeField.FieldName}=={targetStructUnion.TypeName}_{field.FieldName}_FieldNumber)");
                    DeserializeHeaderSb.AppendLine("{");

                    FreeHeaderSb.AppendLine($"{elsestring} if(obj->{targetStructUnion.UnionTargetTypeField.FieldName}=={targetStructUnion.TypeName}_{field.FieldName}_FieldNumber)");
                    FreeHeaderSb.AppendLine("{");
                    unionIfFirstFlag = false;
                }
                //FieldsDesc.Add($"{targetStruct.Name}_Field_{field.Name}");
                if (field.TargetType.TargetType < TargetType_t.TYPE_ENUM)
                {
                    //if (isEncodeTlv == true)
                    //{
                    //    SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    //}
                    Base_TargetField base_TargetField = field as Base_TargetField;
                    if (base_TargetField == null)
                    {
                        throw new Exception();
                    }
                    if (base_TargetField.BitFieldAttribute != null)
                    {
                        var bitFieldCppTypeString = $"{BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}";
                        if(bitFieldStateMachine.SerializeCodeString.Count==0)
                        {
                            bitFieldStateMachine.BitFieldCppTypeString = bitFieldCppTypeString;

                            bitFieldStateMachine.SerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                           
                            bitFieldStateMachine.DeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.DeserializeCodeString.Add($"DeserializeField((uint8_t *)&bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount},sm,sizeof({bitFieldStateMachine.BitFieldCppTypeString}));");
                        }
                        bitFieldStateMachine.SerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCppTypeString})(obj->{field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");
                        bitFieldStateMachine.DeserializeCodeString.Add($"obj->{field.FieldName}=bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}>>{bitFieldStateMachine.BitFieldLeftShiftAccer};");
                        bitFieldStateMachine.BitFieldLeftShiftAccer += base_TargetField.BitFieldAttribute.BitWidthLength;
                        bitFieldStateMachine.BitFieldProcess(SerializeHeaderSb, DeserializeHeaderSb, i + 1 >= TargetFields.Count ? null : TargetFields[i + 1]);
                    }
                    else
                    {
                        SerializeHeaderSb.AppendLine($"El_Memcpy(&sm->Buf[sm->Index],&obj->{base_TargetField.FieldName},sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}));");
                        SerializeHeaderSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]});");
                        DeserializeHeaderSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}));");

                        SerializeHeaderSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }

                    //SerializeHeaderSb.AppendLine("\r\n");
                    //DeserializeHeaderSb.AppendLine("\r\n");

                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ENUM)
                { 
                    EnumType_TargetType ettt = field.TargetType as EnumType_TargetType;
                    //if (isEncodeTlv == true)
                    //{
                    //    SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[ettt.NumberType]});");
                    //}
                    SerializeHeaderSb.AppendLine($"El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName},sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                    SerializeHeaderSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");

                    DeserializeHeaderSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");

                    SerializeHeaderSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");

                    //SerializeHeaderSb.AppendLine("\r\n");
                    //DeserializeHeaderSb.AppendLine("\r\n");
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                {
                    //if (isEncodeTlv == true)
                    //{
                    //    SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    //}
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
                    //if(isEncodeTlv==true)
                    //{
                    //    //todo 添加SerializeLen函数
                    //    //函数原型void SerializeLen(SerializationManager &sm,UInt32 Len);
                    //    SerializeCodeSb.AppendLine($"SerializeLen(sm,{lenstring});");
                    //    if (attt.ElementType.TargetType < TargetType_t.TYPE_ENUM)
                    //    {
                    //        SerializeCodeSb.AppendLine($"SerializeArrayElementFlag(sm,{TargetTypeString[attt.ElementType.TargetType]}<<4|0x01);");
                    //    }
                    //    else
                    //    {
                    //        SerializeCodeSb.AppendLine($"SerializeArrayElementFlag(sm,{TargetTypeString[attt.ElementType.TargetType]}<<4|0x02);");
                    //    }
                    //}

                    SerializeHeaderSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    SerializeHeaderSb.AppendLine("{");//for begin

                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        DeserializeHeaderSb.AppendLine($"obj->{field.FieldName}=({attt.ElementType.TypeName} *)El_Malloc(sizeof({attt.ElementType.TypeName})*{lenstring});");
                        //DeserializeCodeSb.AppendLine($"obj->{field.FieldName}= new {attt.ElementType.TypeName}[{lenstring}];");
                        DeserializeHeaderSb.AppendLine($"El_Memset(obj->{field.FieldName},0,sizeof({attt.ElementType.TypeName})*{lenstring});");
                    }

                    DeserializeHeaderSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    DeserializeHeaderSb.AppendLine("{");//for begin

                    FreeHeaderSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    FreeHeaderSb.AppendLine("{");//for begin
                    if (attt.ElementType.TargetType < TargetType_t.TYPE_ENUM)
                    {
                        SerializeHeaderSb.AppendLine($"El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({attt.ElementType.TypeName}));");
                        SerializeHeaderSb.AppendLine($"sm->Index+=sizeof({attt.ElementType.TypeName});");

                        DeserializeHeaderSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,sizeof({attt.ElementType.TypeName}));");

                        SerializeHeaderSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else if (attt.ElementType.TargetType == TargetType_t.TYPE_ENUM)
                    {
                        var ettt = attt.ElementType as EnumType_TargetType;
                        SerializeHeaderSb.AppendLine($"El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                        SerializeHeaderSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");

                        DeserializeHeaderSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");

                        SerializeHeaderSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else
                    {
                        SerializeHeaderSb.AppendLine($"{attt.ElementType.TypeName}_Serialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");
                        DeserializeHeaderSb.AppendLine($"{attt.ElementType.TypeName}_Deserialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");

                        FreeHeaderSb.AppendLine($"{attt.ElementType.TypeName}_FreeData<DTL>(&obj->{field.FieldName}[{field.FieldName}_index]);");
                    }
                    SerializeHeaderSb.AppendLine("}");//for end

                    DeserializeHeaderSb.AppendLine("}");//for end

                    FreeHeaderSb.AppendLine("}");//for end

                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        FreeHeaderSb.AppendLine($"El_Free(obj->{field.FieldName});");
                        //FreeCodeSb.AppendLine($"delete[] obj->{field.FieldName};");
                    }
                    //SerializeHeaderSb.AppendLine("\r\n");
                    //DeserializeHeaderSb.AppendLine("\r\n");
                    //FreeHeaderSb.AppendLine("\r\n");
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_STRUCT)
                {
                    //if (isEncodeTlv == true)
                    //{
                    //    SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    //}
                    SerializeHeaderSb.AppendLine($"{field.TargetType.TypeName}_Serialize(sm,&obj->{field.FieldName});");
                    DeserializeHeaderSb.AppendLine($"{field.TargetType.TypeName}_Deserialize(sm,&obj->{field.FieldName});");
                    FreeHeaderSb.AppendLine($"{field.TargetType.TypeName}_FreeData<DTL>(&obj->{field.FieldName});");
                }
                if (field.UnionFieldAttr != null)
                {
                    SerializeHeaderSb.AppendLine("}");
                    DeserializeHeaderSb.AppendLine("}");
                    FreeHeaderSb.AppendLine("}");
                }
            }
            //SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
            SerializeHeaderSb.AppendLine("}");
            DeserializeHeaderSb.AppendLine("}");
            FreeHeaderSb.AppendLine("}");

            //SerializeHeaderSb.AppendLine("\r\n");
            //DeserializeHeaderSb.AppendLine("\r\n");
            //FreeHeaderSb.AppendLine("\r\n");

            //cfilewriter.WriteLine(SerializeHeaderSb.ToString());
            //cfilewriter.WriteLine(DeserializeHeaderSb.ToString());
            //cfilewriter.WriteLine(FreeHeaderSb.ToString());

            hfilewriter.WriteLine(SerializeHeaderSb.ToString());
            hfilewriter.WriteLine(DeserializeHeaderSb.ToString());
            hfilewriter.WriteLine(FreeHeaderSb.ToString());
        }
    }

}
