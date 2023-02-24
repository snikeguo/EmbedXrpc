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
        public static Dictionary<Type, string> CppBaseTargetTypeString = new Dictionary<Type, string>();
        public static Dictionary<Type, string> CsBaseTargetTypeString = new Dictionary<Type, string>();
        static CppCsNanoSerializer()
        {
            CppBaseTargetTypeString.Add(typeof(bool), "uint8_t");
            CppBaseTargetTypeString.Add(typeof(byte), "uint8_t");
            CppBaseTargetTypeString.Add(typeof(sbyte), "int8_t");
            CppBaseTargetTypeString.Add(typeof(UInt16), "uint16_t");
            CppBaseTargetTypeString.Add(typeof(Int16), "int16_t");
            CppBaseTargetTypeString.Add(typeof(UInt32), "uint32_t");
            CppBaseTargetTypeString.Add(typeof(Int32), "int32_t");
            CppBaseTargetTypeString.Add(typeof(UInt64), "uint64_t");
            CppBaseTargetTypeString.Add(typeof(Int64), "uint64_t");
            CppBaseTargetTypeString.Add(typeof(float), "float");
            CppBaseTargetTypeString.Add(typeof(double), "double");
            CppBaseTargetTypeString.Add(typeof(IntPtr), "void *");

            CsBaseTargetTypeString.Add(typeof(bool), "bool");
            CsBaseTargetTypeString.Add(typeof(byte), "byte");
            CsBaseTargetTypeString.Add(typeof(sbyte), "sbyte");
            CsBaseTargetTypeString.Add(typeof(UInt16), "UInt16");
            CsBaseTargetTypeString.Add(typeof(Int16), "Int16");
            CsBaseTargetTypeString.Add(typeof(UInt32), "UInt32");
            CsBaseTargetTypeString.Add(typeof(Int32), "Int32");
            CsBaseTargetTypeString.Add(typeof(UInt64), "UInt64");
            CsBaseTargetTypeString.Add(typeof(Int64), "Int64");
            CsBaseTargetTypeString.Add(typeof(float), "float");
            CsBaseTargetTypeString.Add(typeof(double), "double");
            CsBaseTargetTypeString.Add(typeof(IntPtr), "IntPtr");
        }
        public static string GetCsTypeName(ITargetType itt)
        {
            if (IdlInfo.IsBaseType(itt.TargetType))
            {
                return CsBaseTargetTypeString[itt.TargetType];
            }
            else
            {
                return itt.TypeName;
            }
        }
        public static string GetCppTypeName(ITargetType itt)
        {
            if (IdlInfo.IsBaseType(itt.TargetType))
            {
                return CppBaseTargetTypeString[itt.TargetType];
            }
            else
            {
                return itt.TypeName;
            }
        }

        internal class BitFieldStateMachine
        {
            internal List<string> SerializeCodeString = new List<string>();
            internal List<string> CsSerializeCodeString = new List<string>();

            internal List<string> DeserializeCodeString = new List<string>();
            internal List<string> CsDeserializeCodeString = new List<string>();


            internal int BitFieldLeftShiftAccer = 0;
            static internal int BitFieldTempValueCount = 0;
            internal Type BitFieldType = typeof(object);//TargetType_t.TYPE_STRUCT;
            internal string BitFieldCppTypeString
            {
                get
                {
                    return $"{ CppBaseTargetTypeString[BitFieldType]}";
                }
            }
            internal string BitFieldCsTypeString
            {
                get
                {
                    return $"{ CsBaseTargetTypeString[BitFieldType]}";
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
                BitFieldType = typeof(object);//TargetType_t.TYPE_STRUCT;
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
                        if (nextTargetField.TargetType.TargetType != BitFieldType)
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
                    SerializeCodeString.Add($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue{BitFieldTempValueCount},sizeof({BitFieldCppTypeString}));");
                    CsSerializeCodeString.Add($"sm.Buf.AddRange(sm.ToBytes(bitsTempValue{BitFieldTempValueCount},typeof({BitFieldType})));");

                    SerializeCodeString.Add($"sm->Index+=sizeof({BitFieldCppTypeString});");
                    CsSerializeCodeString.Add($"sm.Index+=sizeof({BitFieldCsTypeString});");

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


            SerializeHeaderSb.AppendLine($"void {GetCppTypeName(targetStructUnion)}_Serialize(SerializationManager *sm,{GetCppTypeName(targetStructUnion)} *obj);");
            SerializeCodeSb.AppendLine($"void {GetCppTypeName(targetStructUnion)}_Serialize(SerializationManager *sm,{GetCppTypeName(targetStructUnion)} *obj)");
            SerializeCodeSb.AppendLine("{");

            CsSerializeCodeSb.AppendLine($"public void Serialize(SerializationManager sm)");
            CsSerializeCodeSb.AppendLine("{");

            DeserializeHeaderSb.AppendLine($"void {GetCppTypeName(targetStructUnion)}_Deserialize(SerializationManager *sm,{GetCppTypeName(targetStructUnion)} *obj,int isIsr);");
            DeserializeCodeSb.AppendLine($"void {GetCppTypeName(targetStructUnion)}_Deserialize(SerializationManager *sm,{GetCppTypeName(targetStructUnion)} *obj,int isIsr)");
            DeserializeCodeSb.AppendLine("{");

            CsDeserializeCodeSb.AppendLine($"public void Deserialize(SerializationManager sm)");
            CsDeserializeCodeSb.AppendLine("{");

            FreeHeaderSb.AppendLine($"{StructFreeNote}void {GetCppTypeName(targetStructUnion)}_FreeData({GetCppTypeName(targetStructUnion)} *obj);");
            FreeCodeSb.AppendLine($"{StructFreeNote}void {GetCppTypeName(targetStructUnion)}_FreeData({GetCppTypeName(targetStructUnion)} *obj)");
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
                    SerializeCodeSb.AppendLine($"case {GetCppTypeName(targetStructUnion)}_{field.FieldName}_FieldNumber:");
                    CsSerializeCodeSb.AppendLine($"case {GetCsTypeName(targetStructUnion)}_{field.FieldName}_FieldNumber:");

                    DeserializeCodeSb.AppendLine($"case {GetCppTypeName(targetStructUnion)}_{field.FieldName}_FieldNumber:");
                    CsDeserializeCodeSb.AppendLine($"case {GetCsTypeName(targetStructUnion)}_{field.FieldName}_FieldNumber:");

                    FreeCodeSb.AppendLine($"{StructFreeNote}case {GetCppTypeName(targetStructUnion)}_{field.FieldName}_FieldNumber:");
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
                if(IdlInfo.IsBaseType(field.TargetType.TargetType))//if (field.TargetType.TargetType < TargetType_t.TYPE_ENUM)
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
                            bitFieldStateMachine.BitFieldType = base_TargetField.TargetType.TargetType;//{ BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]};

                            bitFieldStateMachine.SerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.CsSerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCsTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");

                            bitFieldStateMachine.DeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.CsDeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCsTypeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            string sizeofStr = string.Empty;
                            string sizeofStrForCs = string.Empty;
                            sizeofStr = $"sizeof({CppBaseTargetTypeString[bitFieldStateMachine.BitFieldType]})";
                            sizeofStrForCs = $"sizeof({CsBaseTargetTypeString[bitFieldStateMachine.BitFieldType]})";
                            bitFieldStateMachine.DeserializeCodeString.Add($"DeserializeField((uint8_t *)&bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount},sm,{sizeofStr},isIsr);");
                            bitFieldStateMachine.CsDeserializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=({bitFieldStateMachine.BitFieldCsTypeString})sm.DeserializeField(typeof({base_TargetField.TargetType.TargetType}));");
                        }
                        bitFieldStateMachine.SerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCppTypeString})(obj->{field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");
                        bitFieldStateMachine.CsSerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCsTypeString})({field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");

                        bitFieldStateMachine.DeserializeCodeString.Add($"obj->{field.FieldName}=bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}>>{bitFieldStateMachine.BitFieldLeftShiftAccer};");
                        bitFieldStateMachine.CsDeserializeCodeString.Add($"{field.FieldName}=bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}>>{bitFieldStateMachine.BitFieldLeftShiftAccer};");

                        bitFieldStateMachine.BitFieldLeftShiftAccer += base_TargetField.BitFieldAttribute.BitWidthLength;
                        bitFieldStateMachine.BitFieldProcess(SerializeCodeSb, DeserializeCodeSb,
                            CsSerializeCodeSb, CsDeserializeCodeSb,
                            i + 1 >= TargetFields.Count ? null : TargetFields[i + 1]);
                    }
                    else
                    {
                        string sizeofStr = string.Empty;
                        string sizeofStrForCs = string.Empty;
                        if (base_TargetField.TargetType.TargetType == typeof(Enum))
                        {
                            var enumtt = base_TargetField.TargetType as EnumType_TargetType;
                            sizeofStr = $"sizeof({CppBaseTargetTypeString[enumtt.NumberType]})";
                            sizeofStr = $"sizeof({CsBaseTargetTypeString[enumtt.NumberType]})";
                        }
                        else
                        {
                            sizeofStr = $"sizeof({CppBaseTargetTypeString[base_TargetField.TargetType.TargetType]})";
                            sizeofStrForCs = $"sizeof({CsBaseTargetTypeString[base_TargetField.TargetType.TargetType]})";
                        }
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{base_TargetField.FieldName},{sizeofStr});");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(sm.ToBytes({base_TargetField.FieldName},typeof({base_TargetField.TargetType.TargetType})));");

                        SerializeCodeSb.AppendLine($"sm->Index+={sizeofStr};");
                        CsSerializeCodeSb.AppendLine($"sm.Index+={sizeofStrForCs};");//sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,{sizeofStr},isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}=({GetCsTypeName(field.TargetType)})sm.DeserializeField(typeof({base_TargetField.TargetType.TargetType}));");

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
                else if(field.TargetType.TargetType==typeof(Enum))//else if (field.TargetType.TargetType == TargetType_t.TYPE_ENUM)
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
                    SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName},sizeof({CppBaseTargetTypeString[ettt.NumberType]}));");
                    CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(sm.ToBytes({field.FieldName},typeof({ettt.NumberType})));");

                    SerializeCodeSb.AppendLine($"sm->Index+=sizeof({CppBaseTargetTypeString[ettt.NumberType]});");
                    CsSerializeCodeSb.AppendLine($"sm.Index+=sizeof({CsBaseTargetTypeString[ettt.NumberType]});");

                    DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,sizeof({CppBaseTargetTypeString[ettt.NumberType]}),isIsr);");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=({GetCsTypeName(field.TargetType)})sm.DeserializeField(typeof({ettt.NumberType}));");

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
                else if(field.TargetType.TargetType==typeof(Array))//else if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
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

                    var cpp_len_type_string = string.Empty;
                    var cs_len_type_string = string.Empty;
                    if (arrayLenField != null)
                    {
                        lenstring = $"obj->{arrayLenField.FieldName}";
                        cslenstring = $"{arrayLenField.FieldName}";

                        cpp_len_type_string = $"{GetCppTypeName(arrayLenField.TargetType)}";
                        cs_len_type_string = $"{GetCsTypeName(arrayLenField.TargetType)}";
                    }
                    else
                    {
                        lenstring = "1";
                        cslenstring = "1";

                        cpp_len_type_string = $"uint32_t";
                        cs_len_type_string = $"UInt32";
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

                    SerializeCodeSb.AppendLine($"for({cpp_len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    CsSerializeCodeSb.AppendLine($"for({cs_len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{cslenstring};{field.FieldName}_index++)");

                    SerializeCodeSb.AppendLine("{");//for begin
                    CsSerializeCodeSb.AppendLine("{");//for begin

                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        string sizeofstr = string.Empty;
                        string sizeofStrForCs = string.Empty;
                        if(attt.ElementType.TargetType==typeof(Enum))
                        {
                            var enumtt = attt.ElementType as EnumType_TargetType;
                            sizeofstr = $"sizeof({CppBaseTargetTypeString[enumtt.NumberType]})";
                            sizeofStrForCs = $"sizeof({CsBaseTargetTypeString[enumtt.NumberType]})";
                        }
                        else
                        {
                            sizeofstr = $"sizeof({GetCppTypeName(attt.ElementType)})";
                            sizeofStrForCs = $"sizeof({GetCsTypeName(attt.ElementType)})";
                        }
                        DeserializeCodeSb.AppendLine($"obj->{field.FieldName}=({GetCppTypeName(attt.ElementType)} *)El_Malloc({sizeofstr}*{lenstring});");
                        DeserializeCodeSb.AppendLine($"El_Memset(obj->{field.FieldName},0,{sizeofstr}*{lenstring});");
                    }
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=new {GetCsTypeName(attt.ElementType)}[{cslenstring}];");

                    DeserializeCodeSb.AppendLine($"for({cpp_len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    CsDeserializeCodeSb.AppendLine($"for({cs_len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{cslenstring};{field.FieldName}_index++)");

                    DeserializeCodeSb.AppendLine("{");//for begin
                    CsDeserializeCodeSb.AppendLine("{");//for begin

                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}for({cpp_len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}{{");//for begin
                    if(IdlInfo.IsBaseType(attt.ElementType.TargetType))//if (attt.ElementType.TargetType < TargetType_t.TYPE_ENUM)
                    {
                        string sizeofstr = string.Empty;
                        string sizeofStrForCs = string.Empty;
                        if (attt.ElementType.TargetType == typeof(Enum))
                        {
                            var enumtt = attt.ElementType as EnumType_TargetType;
                            sizeofstr = $"sizeof({CppBaseTargetTypeString[enumtt.NumberType]})";
                            sizeofStrForCs = $"sizeof({CsBaseTargetTypeString[enumtt.NumberType]})";
                        }
                        else
                        {
                            sizeofstr = $"sizeof({GetCppTypeName(attt.ElementType)})";
                            sizeofStrForCs = $"sizeof({GetCsTypeName(attt.ElementType)})";
                        }
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],{sizeofstr});");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(sm.ToBytes({field.FieldName}[{field.FieldName}_index],typeof({attt.ElementType.TargetType})));");

                        SerializeCodeSb.AppendLine($"sm->Index+={sizeofstr};");
                        CsSerializeCodeSb.AppendLine($"sm.Index+={sizeofStrForCs};");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,{sizeofstr},isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=({GetCsTypeName(attt.ElementType)})sm.DeserializeField(typeof({attt.ElementType.TargetType}));");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else if(attt.ElementType.TargetType==typeof(Enum))//else if (attt.ElementType.TargetType == TargetType_t.TYPE_ENUM)
                    {
                        var ettt = attt.ElementType as EnumType_TargetType;
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({CppBaseTargetTypeString[ettt.NumberType]}));");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(sm.ToBytes({field.FieldName}[{field.FieldName}_index],typeof({ettt.NumberType})));");

                        SerializeCodeSb.AppendLine($"sm->Index+=sizeof({CppBaseTargetTypeString[ettt.NumberType]});");
                        CsSerializeCodeSb.AppendLine($"sm.Index+=sizeof({CsBaseTargetTypeString[ettt.NumberType]});");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,sizeof({CppBaseTargetTypeString[ettt.NumberType]}),isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=({CsBaseTargetTypeString[ettt.NumberType]})sm.DeserializeField(typeof({ettt.NumberType}));");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else
                    {
                        SerializeCodeSb.AppendLine($"{GetCppTypeName(attt.ElementType)}_Serialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");
                        CsSerializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index].Serialize(sm);");

                        DeserializeCodeSb.AppendLine($"{GetCppTypeName(attt.ElementType)}_Deserialize(sm,&obj->{field.FieldName}[{field.FieldName}_index],isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=new {GetCsTypeName(attt.ElementType)}();");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index].Deserialize(sm);");

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}{GetCppTypeName(attt.ElementType)}_FreeData(&obj->{field.FieldName}[{field.FieldName}_index]);");
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
                else if(field.TargetType.TargetType == typeof(object))//else if (field.TargetType.TargetType == TargetType_t.TYPE_STRUCT)
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
                    SerializeCodeSb.AppendLine($"{GetCppTypeName(field.TargetType)}_Serialize(sm,&obj->{field.FieldName});");
                    CsSerializeCodeSb.AppendLine($"{field.FieldName}.Serialize(sm);");

                    DeserializeCodeSb.AppendLine($"{GetCppTypeName(field.TargetType)}_Deserialize(sm,&obj->{field.FieldName},isIsr);");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=new {GetCsTypeName(field.TargetType)}();");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}.Deserialize(sm);");

                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{fieldNeedFreeMemoryNote}{GetCppTypeName(field.TargetType)}_FreeData(&obj->{field.FieldName});");
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
