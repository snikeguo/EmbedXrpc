using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public class CppCsNanoSerializer : ICppSerializable
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
        static CppCsNanoSerializer()
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
            internal List<string> CsSerializeCodeString = new List<string>();

            internal List<string> DeserializeCodeString = new List<string>();
            internal List<string> CsDeserializeCodeString = new List<string>();


            internal int BitFieldLeftShiftAccer = 0;
            static internal int BitFieldTempValueCount = 0;
            internal TargetType_t BitFieldCppType = TargetType_t.TYPE_STRUCT;
            internal string BitFieldCppTypeString
            {
                get
                {
                    return $"{ BaseType_TargetType.TypeReplaceDic[BitFieldCppType]}";
                }
            }
            internal void Reset()
            {
                SerializeCodeString.Clear();
                CsSerializeCodeString.Clear();

                DeserializeCodeString.Clear();
                CsDeserializeCodeString.Clear();

                BitFieldLeftShiftAccer = 0;
                //BitFieldTempValueCount++;
                BitFieldCppType = TargetType_t.TYPE_STRUCT;
            }
            internal void BitFieldProcess(StringBuilder cppSerializeString, StringBuilder cppDeserializeString,
                StringBuilder csSerializeString, StringBuilder csDeserializeString,
                ITargetField nextTargetField)
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
                        if (nextTargetField.TargetType.TargetType != BitFieldCppType)
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
                if (writeFlag == true)
                {
                    //var BitFieldCppTypeString = $"{ BaseType_TargetType.TypeReplaceDic[BitFieldCppType]}";
                    SerializeCodeString.Add($"El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue{BitFieldTempValueCount},sizeof({BitFieldCppTypeString}));");
                    CsSerializeCodeString.Add($"sm.Buf.AddRange(SerializationManager.ToBytes(bitsTempValue{BitFieldTempValueCount},Type_t.{BitFieldCppType}));");

                    SerializeCodeString.Add($"sm->Index+=sizeof({BitFieldCppTypeString});");
                    CsSerializeCodeString.Add($"sm.Index+=sizeof({BitFieldCppTypeString});");

                    foreach (var ss in SerializeCodeString)
                    {
                        cppSerializeString.AppendLine(ss);
                    }
                    foreach (var ss in CsSerializeCodeString)
                    {
                        csSerializeString.AppendLine(ss);
                    }

                    foreach (var ds in DeserializeCodeString)
                    {
                        cppDeserializeString.AppendLine(ds);
                    }
                    foreach (var ds in CsDeserializeCodeString)
                    {
                        csDeserializeString.AppendLine(ds);
                    }

                    if (BitFieldCppTypeString != string.Empty)
                    {
                        BitFieldTempValueCount++;
                    }
                    Reset();
                }
            }
        }
        public void EmitStruct(StructType_TargetType targetStructUnion, StreamWriter cfilewriter, StreamWriter hfilewriter, StreamWriter csfilewriter)
        {
            StringBuilder SerializeCodeSb = new StringBuilder();
            StringBuilder CsSerializeCodeSb = new StringBuilder();
            StringBuilder DeserializeCodeSb = new StringBuilder();
            StringBuilder CsDeserializeCodeSb = new StringBuilder();
            StringBuilder FreeCodeSb = new StringBuilder();

            StringBuilder SerializeHeaderSb = new StringBuilder();
            StringBuilder DeserializeHeaderSb = new StringBuilder();
            StringBuilder FreeHeaderSb = new StringBuilder();
            const string noNote = " ";
            string StructFreeNote = targetStructUnion.IsNeedFreeMemoryForNativeLanguage == true ? noNote : "//! ";


            SerializeHeaderSb.AppendLine($"void {targetStructUnion.TypeName}_Serialize(SerializationManager *sm,{targetStructUnion.TypeName} *obj);");
            SerializeCodeSb.AppendLine($"void {targetStructUnion.TypeName}_Serialize(SerializationManager *sm,{targetStructUnion.TypeName} *obj)");
            SerializeCodeSb.AppendLine("{");

            CsSerializeCodeSb.AppendLine($"public void Serialize(SerializationManager sm)");
            CsSerializeCodeSb.AppendLine("{");

            DeserializeHeaderSb.AppendLine($"void {targetStructUnion.TypeName}_Deserialize(SerializationManager *sm,{targetStructUnion.TypeName} *obj);");
            DeserializeCodeSb.AppendLine($"void {targetStructUnion.TypeName}_Deserialize(SerializationManager *sm,{targetStructUnion.TypeName} *obj)");
            DeserializeCodeSb.AppendLine("{");

            CsDeserializeCodeSb.AppendLine($"public void Deserialize(SerializationManager sm)");
            CsDeserializeCodeSb.AppendLine("{");

            FreeHeaderSb.AppendLine($"{StructFreeNote}void {targetStructUnion.TypeName}_FreeData({targetStructUnion.TypeName} *obj);");
            FreeCodeSb.AppendLine($"{StructFreeNote}void {targetStructUnion.TypeName}_FreeData({targetStructUnion.TypeName} *obj)");
            FreeCodeSb.AppendLine($"{StructFreeNote}{{");

            List<ITargetField> TargetFields = targetStructUnion.TargetFields;
            ITargetField field = null;

            //bool unionIfFirstFlag = true;
            /*if (isEncodeTlv == true)
            {
                SerializeCodeSb.AppendLine($"SerializeKey(0,{TargetTypeString[(int)field.TargetType.TargetType]});");
            }*/
            BitFieldStateMachine bitFieldStateMachine = new BitFieldStateMachine();
            BitFieldStateMachine.BitFieldTempValueCount = 0;
            
            for (int i = 0; i < TargetFields.Count; i++)
            {
                field = TargetFields[i];
                string fieldNeedFreeMemoryNote = field.TargetType.IsNeedFreeMemoryForNativeLanguage == true ? noNote: "//!! ";
                string noSerializeAttrNote = noNote;
                if (field.NoSerializationAttr != null && field.NoSerializationAttr.FieldName==string.Empty)
                {
                    noSerializeAttrNote = "//!!! ";
                }        
                if (field.UnionFieldAttr != null)
                {
                    //string elsestring = (unionIfFirstFlag == true ? "" : "else");
                    if (field.UnionFieldAttr.UnionIndex == UnionIndex.Start)
                    {
                        SerializeCodeSb.AppendLine($"switch(obj->{targetStructUnion.UnionTargetTypeField.FieldName})");
                        SerializeCodeSb.AppendLine("{");
                        CsSerializeCodeSb.AppendLine($"switch({targetStructUnion.UnionTargetTypeField.FieldName})");
                        CsSerializeCodeSb.AppendLine("{");

                        DeserializeCodeSb.AppendLine($"switch(obj->{targetStructUnion.UnionTargetTypeField.FieldName})");
                        DeserializeCodeSb.AppendLine("{");
                        CsDeserializeCodeSb.AppendLine($"switch({targetStructUnion.UnionTargetTypeField.FieldName})");
                        CsDeserializeCodeSb.AppendLine("{");

                        FreeCodeSb.AppendLine($"{StructFreeNote}switch(obj->{targetStructUnion.UnionTargetTypeField.FieldName})");
                        FreeCodeSb.AppendLine($"{StructFreeNote}{{");

                    }
                    if (field.MacroControlAttr != null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}" +CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                    }
                    SerializeCodeSb.AppendLine($"case {targetStructUnion.TypeName}_{field.FieldName}_FieldNumber:");
                    CsSerializeCodeSb.AppendLine($"case {targetStructUnion.TypeName}_{field.FieldName}_FieldNumber:");

                    DeserializeCodeSb.AppendLine($"case {targetStructUnion.TypeName}_{field.FieldName}_FieldNumber:");
                    CsDeserializeCodeSb.AppendLine($"case {targetStructUnion.TypeName}_{field.FieldName}_FieldNumber:");

                    FreeCodeSb.AppendLine($"{StructFreeNote}case {targetStructUnion.TypeName}_{field.FieldName}_FieldNumber:");
                }
                if (field.NoSerializationAttr != null)
                {
                    if (field.NoSerializationAttr.FieldName != string.Empty)
                    {
                        SerializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}if(obj->{field.NoSerializationAttr.FieldName}=={field.NoSerializationAttr.CompareValue})");
                        SerializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{{//{field.FieldName}");

                        CsSerializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}if({field.NoSerializationAttr.FieldName}=={field.NoSerializationAttr.CompareValue})");
                        CsSerializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{{//{field.FieldName}");

                        DeserializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}if(obj->{field.NoSerializationAttr.FieldName}=={field.NoSerializationAttr.CompareValue})");
                        DeserializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{{//{field.FieldName}");

                        CsDeserializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}if({field.NoSerializationAttr.FieldName}=={field.NoSerializationAttr.CompareValue})");
                        CsDeserializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{{");


                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{fieldNeedFreeMemoryNote}if(obj->{field.NoSerializationAttr.FieldName}=={field.NoSerializationAttr.CompareValue})");
                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{fieldNeedFreeMemoryNote}{{//{field.FieldName}");
                    }
                    else
                    {
                        SerializeCodeSb.AppendLine($"{StructFreeNote}//{field.FieldName}:NoSerialization");
                        CsSerializeCodeSb.AppendLine($"{StructFreeNote}//{field.FieldName}:NoSerialization");

                        DeserializeCodeSb.AppendLine($"{StructFreeNote}//{field.FieldName}:NoSerialization");
                        CsDeserializeCodeSb.AppendLine($"{StructFreeNote}//{field.FieldName}:NoSerialization");

                        FreeCodeSb.AppendLine($"{StructFreeNote}//{field.FieldName}:NoSerialization ");
                    }
                    
                }
                if(noSerializeAttrNote!=noNote)//如果“noSerializeAttrNote”不为空，说明需要序列化
                {
                    goto noSerialize;
                }
                if (field.TargetType.TargetType < TargetType_t.TYPE_ENUM)
                {
                    //if (isEncodeTlv == true)
                    //{
                    //    SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    //}
                    if (field.MacroControlAttr != null && field.UnionFieldAttr == null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}" + CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                    }
                    Base_TargetField base_TargetField = field as Base_TargetField;
                    if (base_TargetField == null)
                    {
                        throw new Exception();
                    }
                    if (base_TargetField.BitFieldAttribute != null)
                    {
                        if (bitFieldStateMachine.SerializeCodeString.Count == 0)
                        {
                            bitFieldStateMachine.BitFieldCppType = base_TargetField.TargetType.TargetType;//{ BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]};

                            bitFieldStateMachine.SerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.CsSerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");

                            bitFieldStateMachine.DeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.CsDeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");

                            bitFieldStateMachine.DeserializeCodeString.Add($"DeserializeField((uint8_t *)&bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount},sm,sizeof({bitFieldStateMachine.BitFieldCppTypeString}));");
                            bitFieldStateMachine.CsDeserializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=({bitFieldStateMachine.BitFieldCppTypeString})sm.DeserializeField(Type_t.{base_TargetField.TargetType.TargetType});");
                        }
                        bitFieldStateMachine.SerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCppTypeString})(obj->{field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");
                        bitFieldStateMachine.CsSerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCppTypeString})({field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");

                        bitFieldStateMachine.DeserializeCodeString.Add($"obj->{field.FieldName}=bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}>>{bitFieldStateMachine.BitFieldLeftShiftAccer};");
                        bitFieldStateMachine.CsDeserializeCodeString.Add($"{field.FieldName}=bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}>>{bitFieldStateMachine.BitFieldLeftShiftAccer};");

                        bitFieldStateMachine.BitFieldLeftShiftAccer += base_TargetField.BitFieldAttribute.BitWidthLength;
                        bitFieldStateMachine.BitFieldProcess(SerializeCodeSb, DeserializeCodeSb,
                            CsSerializeCodeSb, CsDeserializeCodeSb,
                            i + 1 >= TargetFields.Count ? null : TargetFields[i + 1]);
                    }
                    else
                    {
                        SerializeCodeSb.AppendLine($"El_Memcpy(&sm->Buf[sm->Index],&obj->{base_TargetField.FieldName},sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}));");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(SerializationManager.ToBytes({base_TargetField.FieldName},Type_t.{base_TargetField.TargetType.TargetType}));");

                        SerializeCodeSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]});");
                        CsSerializeCodeSb.AppendLine($"sm.Index+=sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]});");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}));");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}=({field.TargetType.TypeName})sm.DeserializeField(Type_t.{base_TargetField.TargetType.TargetType});");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);"); 
                    }
                    //SerializeCodeSb.AppendLine();
                    //DeserializeCodeSb.AppendLine();
                    if (field.MacroControlAttr != null && field.UnionFieldAttr == null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}" + CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                    }

                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ENUM)
                {
                    EnumType_TargetType ettt = field.TargetType as EnumType_TargetType;
                    //if (isEncodeTlv == true)
                    //{
                    //    SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[ettt.NumberType]});");
                    //}
                    if (field.MacroControlAttr != null && field.UnionFieldAttr == null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}" + CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                    }
                    SerializeCodeSb.AppendLine($"El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName},sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                    CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(SerializationManager.ToBytes({field.FieldName},Type_t.{ettt.NumberType}));");

                    SerializeCodeSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");
                    CsSerializeCodeSb.AppendLine($"sm.Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");

                    DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=({field.TargetType.TypeName})sm.DeserializeField(Type_t.{ettt.NumberType});");

                    SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");

                    if (field.MacroControlAttr != null && field.UnionFieldAttr == null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}" + CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                    }
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                {
                    //if (isEncodeTlv == true)
                    //{
                    //    SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    //}
                    if (field.MacroControlAttr != null && field.UnionFieldAttr == null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}" + CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                    }
                    Array_TargetField array_TargetField = field as Array_TargetField;
                    var arrayLenField = array_TargetField.ArrayLenField;
                    ArrayType_TargetType attt = array_TargetField.TargetType as ArrayType_TargetType;
                    string ElementTypeFreeNote = attt.IsNeedFreeMemoryForNativeLanguage == true ? noNote : "//!!!! ";
                    var lenstring = string.Empty;
                    var cslenstring = string.Empty;

                    var len_type_string = string.Empty;
                    if (arrayLenField != null)
                    {
                        lenstring = $"obj->{arrayLenField.FieldName}";
                        cslenstring = $"{arrayLenField.FieldName}";

                        len_type_string = $"{arrayLenField.TargetType.TypeName}";
                    }
                    else
                    {
                        lenstring = "1";
                        cslenstring = "1";

                        len_type_string = "Byte";
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

                    SerializeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    CsSerializeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{cslenstring};{field.FieldName}_index++)");

                    SerializeCodeSb.AppendLine("{");//for begin
                    CsSerializeCodeSb.AppendLine("{");//for begin

                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        DeserializeCodeSb.AppendLine($"obj->{field.FieldName}=({attt.ElementType.TypeName} *)El_Malloc(sizeof({attt.ElementType.TypeName})*{lenstring});");
                        DeserializeCodeSb.AppendLine($"El_Memset(obj->{field.FieldName},0,sizeof({attt.ElementType.TypeName})*{lenstring});");
                    }
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=new {attt.ElementType.TypeName}[{cslenstring}];");

                    DeserializeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    CsDeserializeCodeSb.AppendLine($"for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{cslenstring};{field.FieldName}_index++)");

                    DeserializeCodeSb.AppendLine("{");//for begin
                    CsDeserializeCodeSb.AppendLine("{");//for begin

                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}for({len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}{{");//for begin
                    if (attt.ElementType.TargetType < TargetType_t.TYPE_ENUM)
                    {
                        SerializeCodeSb.AppendLine($"El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({attt.ElementType.TypeName}));");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(SerializationManager.ToBytes({field.FieldName}[{field.FieldName}_index],Type_t.{attt.ElementType.TargetType}));");

                        SerializeCodeSb.AppendLine($"sm->Index+=sizeof({attt.ElementType.TypeName});");
                        CsSerializeCodeSb.AppendLine($"sm.Index+=sizeof({attt.ElementType.TypeName});");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,sizeof({attt.ElementType.TypeName}));");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=({attt.ElementType.TypeName})sm.DeserializeField(Type_t.{attt.ElementType.TargetType});");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else if (attt.ElementType.TargetType == TargetType_t.TYPE_ENUM)
                    {
                        var ettt = attt.ElementType as EnumType_TargetType;
                        SerializeCodeSb.AppendLine($"El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(SerializationManager.ToBytes({field.FieldName}[{field.FieldName}_index],Type_t.{ettt.NumberType}));");

                        SerializeCodeSb.AppendLine($"sm->Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");
                        CsSerializeCodeSb.AppendLine($"sm.Index+=sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]});");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,sizeof({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]}));");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=({BaseType_TargetType.TypeReplaceDic[ettt.NumberType]})sm.DeserializeField(Type_t.{ettt.NumberType});");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else
                    {
                        SerializeCodeSb.AppendLine($"{attt.ElementType.TypeName}_Serialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");
                        CsSerializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index].Serialize(sm);");

                        DeserializeCodeSb.AppendLine($"{attt.ElementType.TypeName}_Deserialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=new {attt.ElementType.TypeName}();");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index].Deserialize(sm);");

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}{attt.ElementType.TypeName}_FreeData(&obj->{field.FieldName}[{field.FieldName}_index]);");
                    }
                    SerializeCodeSb.AppendLine("}");//for end
                    CsSerializeCodeSb.AppendLine("}");//for end

                    DeserializeCodeSb.AppendLine("}");//for end
                    CsDeserializeCodeSb.AppendLine("}");//for end

                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}}}");//for end

                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}El_Free(obj->{field.FieldName});");
                        //FreeCodeSb.AppendLine($"delete[] obj->{field.FieldName};");
                    }
                    if (field.MacroControlAttr != null && field.UnionFieldAttr == null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}" +CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                    }
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_STRUCT)
                {
                    //if (isEncodeTlv == true)
                    //{
                    //    SerializeCodeSb.AppendLine($"SerializeKey({field.FieldNumberAttr.Number},{TargetTypeString[field.TargetType.TargetType]});");
                    //}
                    //string StructFieldFreeNote = field.TargetType.IsNeedFreeMemoryForNativeLanguage == true ? noNote : "//!!!!! ";
                    if (field.MacroControlAttr != null && field.UnionFieldAttr == null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{fieldNeedFreeMemoryNote}" + CppSerializableCommon.MacroControlWriteBegin(null, field.MacroControlAttr));
                    }
                    SerializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Serialize(sm,&obj->{field.FieldName});");
                    CsSerializeCodeSb.AppendLine($"{field.FieldName}.Serialize(sm);");

                    DeserializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Deserialize(sm,&obj->{field.FieldName});");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=new {field.TargetType.TypeName}();");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}.Deserialize(sm);");

                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{fieldNeedFreeMemoryNote}{field.TargetType.TypeName}_FreeData(&obj->{field.FieldName});");
                    if (field.MacroControlAttr != null && field.UnionFieldAttr == null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{fieldNeedFreeMemoryNote}" + CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                    }
                }
                noSerialize:
                if (field.NoSerializationAttr != null && field.NoSerializationAttr.FieldName != string.Empty)
                {
                    SerializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}}}");
                    CsSerializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}}}");
                    DeserializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}}}");
                    CsDeserializeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}}}");
                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{fieldNeedFreeMemoryNote}}}");
                }
                if (field.UnionFieldAttr != null)
                {
                    SerializeCodeSb.AppendLine("break;");
                    CsSerializeCodeSb.AppendLine("break;");

                    DeserializeCodeSb.AppendLine("break;");
                    CsDeserializeCodeSb.AppendLine("break;");

                    FreeCodeSb.AppendLine($"{StructFreeNote}break;");
                    if (field.MacroControlAttr != null)
                    {
                        SerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsSerializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        DeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                        CsDeserializeCodeSb.AppendLine(CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}" + CppSerializableCommon.MacroControlWriteEnd(null, field.MacroControlAttr));
                    }
                    if (field.UnionFieldAttr.UnionIndex == UnionIndex.End)
                    {
                        SerializeCodeSb.AppendLine("}");
                        CsSerializeCodeSb.AppendLine("}");

                        DeserializeCodeSb.AppendLine("}");
                        CsDeserializeCodeSb.AppendLine("}");

                        FreeCodeSb.AppendLine($"{StructFreeNote}}}");
                    }
                }                
            }
            //SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
            SerializeCodeSb.AppendLine("}");//函数闭
            CsSerializeCodeSb.AppendLine("}");//函数闭

            DeserializeCodeSb.AppendLine("}");//函数闭
            CsDeserializeCodeSb.AppendLine("}");//函数闭

            FreeCodeSb.AppendLine($"{StructFreeNote}}}");//函数闭

            //SerializeCodeSb.AppendLine("\r\n");
            //DeserializeCodeSb.AppendLine("\r\n");
            //FreeCodeSb.AppendLine("\r\n");
            if (cfilewriter != null)
            {
                cfilewriter.WriteLine(SerializeCodeSb.ToString());
                cfilewriter.WriteLine(DeserializeCodeSb.ToString());
                cfilewriter.WriteLine(FreeCodeSb.ToString());
            }


            if (hfilewriter != null)
            {
                hfilewriter.WriteLine(SerializeHeaderSb.ToString());
                hfilewriter.WriteLine(DeserializeHeaderSb.ToString());
                hfilewriter.WriteLine(FreeHeaderSb.ToString());
            }


            if (csfilewriter != null)
            {
                csfilewriter.WriteLine(CsSerializeCodeSb.ToString());
                csfilewriter.WriteLine(CsDeserializeCodeSb.ToString());
            }
        }

        public void EmitStruct(StructType_TargetType targetStructUnion, StreamWriter cfilewriter, StreamWriter hfilewriter)
        {
            EmitStruct(targetStructUnion, cfilewriter, hfilewriter, null);
        }
    }

}
