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

        public static Dictionary<Type, TypeString> TargetTypeStrings = new Dictionary<Type, TypeString>();

        public class TypeString
        {
            public string CppBaseTargetTypeDefineString { get; set; }
            public string CppBaseTargetTypeSerializeString { get; set; }
            public string CsBaseTargetTypeDefineString { get; set; }
            public string CsBaseTargetTypeSerializeString { get; set; }
        }

        static CppCsNanoSerializer()
        {
            TargetTypeStrings.Add(typeof(bool), new TypeString() { CppBaseTargetTypeDefineString = "bool", CppBaseTargetTypeSerializeString = "uint8_t", CsBaseTargetTypeDefineString = "bool" , CsBaseTargetTypeSerializeString = "bool" });
            TargetTypeStrings.Add(typeof(byte), new TypeString() { CppBaseTargetTypeDefineString = "uint8_t", CppBaseTargetTypeSerializeString = "uint8_t", CsBaseTargetTypeDefineString = "byte", CsBaseTargetTypeSerializeString = "byte" });
            TargetTypeStrings.Add(typeof(sbyte), new TypeString() { CppBaseTargetTypeDefineString = "int8_t", CppBaseTargetTypeSerializeString = "uint8_t", CsBaseTargetTypeDefineString = "sbyte", CsBaseTargetTypeSerializeString = "sbyte" });
            TargetTypeStrings.Add(typeof(UInt16), new TypeString() { CppBaseTargetTypeDefineString = "uint16_t", CppBaseTargetTypeSerializeString = "uint16_t", CsBaseTargetTypeDefineString = "UInt16", CsBaseTargetTypeSerializeString = "UInt16" });
            TargetTypeStrings.Add(typeof(Int16), new TypeString() { CppBaseTargetTypeDefineString = "uint32_t", CppBaseTargetTypeSerializeString = "uint32_t", CsBaseTargetTypeDefineString = "Int16", CsBaseTargetTypeSerializeString = "Int16" });
            TargetTypeStrings.Add(typeof(UInt32), new TypeString() { CppBaseTargetTypeDefineString = "uint32_t", CppBaseTargetTypeSerializeString = "uint32_t", CsBaseTargetTypeDefineString = "UInt32", CsBaseTargetTypeSerializeString = "UInt32" });
            TargetTypeStrings.Add(typeof(Int32), new TypeString() { CppBaseTargetTypeDefineString = "int32_t", CppBaseTargetTypeSerializeString = "int32_t", CsBaseTargetTypeDefineString = "Int32", CsBaseTargetTypeSerializeString = "Int32" });
            TargetTypeStrings.Add(typeof(UInt64), new TypeString() { CppBaseTargetTypeDefineString = "uint64_t", CppBaseTargetTypeSerializeString = "uint64_t", CsBaseTargetTypeDefineString = "UInt64", CsBaseTargetTypeSerializeString = "UInt64" });
            TargetTypeStrings.Add(typeof(Int64), new TypeString() { CppBaseTargetTypeDefineString = "int64_t", CppBaseTargetTypeSerializeString = "int64_t", CsBaseTargetTypeDefineString = "Int64", CsBaseTargetTypeSerializeString = "Int64" });
            TargetTypeStrings.Add(typeof(float), new TypeString() { CppBaseTargetTypeDefineString = "float", CppBaseTargetTypeSerializeString = "float", CsBaseTargetTypeDefineString = "float", CsBaseTargetTypeSerializeString = "float" });
            TargetTypeStrings.Add(typeof(double), new TypeString() { CppBaseTargetTypeDefineString = "double", CppBaseTargetTypeSerializeString = "double", CsBaseTargetTypeDefineString = "double", CsBaseTargetTypeSerializeString = "double" });
            TargetTypeStrings.Add(typeof(IntPtr), new TypeString() { CppBaseTargetTypeDefineString = "void *", CppBaseTargetTypeSerializeString = "void *", CsBaseTargetTypeDefineString = "IntPtr", CsBaseTargetTypeSerializeString = "IntPtr" });

            
        }
        public static string GetCsTypeDefineName(ITargetType itt)
        {
            if (IdlInfo.IsBaseType(itt.TargetType))
            {
                return TargetTypeStrings[itt.TargetType].CsBaseTargetTypeDefineString;
            }
            else
            {
                return itt.TypeName;
            }
        }
        public static string GetCsTypeSerializeName(ITargetType itt)
        {
            if (IdlInfo.IsBaseType(itt.TargetType))
            {
                return TargetTypeStrings[itt.TargetType].CsBaseTargetTypeSerializeString;
            }
            else
            {
                return itt.TypeName;
            }
        }
        public static string GetCppTypeDefineName(ITargetType itt)
        {
            if (IdlInfo.IsBaseType(itt.TargetType))
            {
                return TargetTypeStrings[itt.TargetType].CppBaseTargetTypeDefineString;
            }
            else
            {
                return itt.TypeName;
            }
        }
        public static string GetCppTypeSerializeName(ITargetType itt)
        {
            if (IdlInfo.IsBaseType(itt.TargetType))
            {
                return TargetTypeStrings[itt.TargetType].CppBaseTargetTypeSerializeString;
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
            internal string BitFieldCppTypeSerializeString
            {
                get
                {
                    return $"{ TargetTypeStrings[BitFieldType].CppBaseTargetTypeSerializeString}";
                }
            }
            internal string BitFieldCsTypeSerializeString
            {
                get
                {
                    return $"{ TargetTypeStrings[BitFieldType].CsBaseTargetTypeSerializeString}";
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
                    SerializeCodeString.Add($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue{BitFieldTempValueCount},sizeof({BitFieldCppTypeSerializeString}));");
                    CsSerializeCodeString.Add($"sm.Buf.AddRange(sm.ToBytes(bitsTempValue{BitFieldTempValueCount},typeof({BitFieldType})));");

                    SerializeCodeString.Add($"sm->Index+=sizeof({BitFieldCppTypeSerializeString});");
                    CsSerializeCodeString.Add($"sm.Index+=sizeof({BitFieldCsTypeSerializeString});");

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

                    if (BitFieldCppTypeSerializeString != string.Empty)
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


            SerializeHeaderSb.AppendLine($"void {GetCppTypeDefineName(targetStructUnion)}_Serialize(SerializationManager *sm,{GetCppTypeDefineName(targetStructUnion)} *obj);");
            SerializeCodeSb.AppendLine($"void {GetCppTypeDefineName(targetStructUnion)}_Serialize(SerializationManager *sm,{GetCppTypeDefineName(targetStructUnion)} *obj)");
            SerializeCodeSb.AppendLine("{");

            CsSerializeCodeSb.AppendLine($"public void Serialize(SerializationManager sm)");
            CsSerializeCodeSb.AppendLine("{");

            DeserializeHeaderSb.AppendLine($"void {GetCppTypeDefineName(targetStructUnion)}_Deserialize(SerializationManager *sm,{GetCppTypeDefineName(targetStructUnion)} *obj,int isIsr);");
            DeserializeCodeSb.AppendLine($"void {GetCppTypeDefineName(targetStructUnion)}_Deserialize(SerializationManager *sm,{GetCppTypeDefineName(targetStructUnion)} *obj,int isIsr)");
            DeserializeCodeSb.AppendLine("{");

            CsDeserializeCodeSb.AppendLine($"public void Deserialize(SerializationManager sm)");
            CsDeserializeCodeSb.AppendLine("{");

            FreeHeaderSb.AppendLine($"{StructFreeNote}void {GetCppTypeDefineName(targetStructUnion)}_FreeData({GetCppTypeDefineName(targetStructUnion)} *obj);");
            FreeCodeSb.AppendLine($"{StructFreeNote}void {GetCppTypeDefineName(targetStructUnion)}_FreeData({GetCppTypeDefineName(targetStructUnion)} *obj)");
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
                    SerializeCodeSb.AppendLine($"case {GetCppTypeDefineName(targetStructUnion)}_{field.FieldName}_FieldNumber:");
                    CsSerializeCodeSb.AppendLine($"case {GetCsTypeDefineName(targetStructUnion)}_{field.FieldName}_FieldNumber:");

                    DeserializeCodeSb.AppendLine($"case {GetCppTypeDefineName(targetStructUnion)}_{field.FieldName}_FieldNumber:");
                    CsDeserializeCodeSb.AppendLine($"case {GetCsTypeDefineName(targetStructUnion)}_{field.FieldName}_FieldNumber:");

                    FreeCodeSb.AppendLine($"{StructFreeNote}case {GetCppTypeDefineName(targetStructUnion)}_{field.FieldName}_FieldNumber:");
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

                            bitFieldStateMachine.SerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeSerializeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.CsSerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCsTypeSerializeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");

                            bitFieldStateMachine.DeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppTypeSerializeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.CsDeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCsTypeSerializeString} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            string sizeofStr = string.Empty;
                            string sizeofStrForCs = string.Empty;
                            sizeofStr = $"sizeof({TargetTypeStrings[bitFieldStateMachine.BitFieldType].CppBaseTargetTypeSerializeString})";
                            sizeofStrForCs = $"sizeof({TargetTypeStrings[bitFieldStateMachine.BitFieldType].CsBaseTargetTypeSerializeString})";
                            bitFieldStateMachine.DeserializeCodeString.Add($"DeserializeField((uint8_t *)&bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount},sm,{sizeofStr},isIsr);");
                            bitFieldStateMachine.CsDeserializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=({bitFieldStateMachine.BitFieldCsTypeSerializeString})sm.DeserializeField(typeof({base_TargetField.TargetType.TargetType}));");
                        }
                        bitFieldStateMachine.SerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCppTypeSerializeString})(obj->{field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");
                        bitFieldStateMachine.CsSerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCsTypeSerializeString})({field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");

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
                            sizeofStr = $"sizeof({TargetTypeStrings[enumtt.NumberType].CppBaseTargetTypeSerializeString})";
                            sizeofStr = $"sizeof({TargetTypeStrings[enumtt.NumberType].CsBaseTargetTypeSerializeString})";
                        }
                        else
                        {
                            sizeofStr = $"sizeof({TargetTypeStrings[base_TargetField.TargetType.TargetType].CppBaseTargetTypeSerializeString})";
                            sizeofStrForCs = $"sizeof({TargetTypeStrings[base_TargetField.TargetType.TargetType].CsBaseTargetTypeSerializeString})";
                        }
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{base_TargetField.FieldName},{sizeofStr});");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(sm.ToBytes({base_TargetField.FieldName},typeof({base_TargetField.TargetType.TargetType})));");

                        SerializeCodeSb.AppendLine($"sm->Index+={sizeofStr};");
                        CsSerializeCodeSb.AppendLine($"sm.Index+={sizeofStrForCs};");//sizeof({BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]}

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,{sizeofStr},isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}=({GetCsTypeDefineName(field.TargetType)})sm.DeserializeField(typeof({base_TargetField.TargetType.TargetType}));");

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
                    SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName},sizeof({TargetTypeStrings[ettt.NumberType].CppBaseTargetTypeSerializeString}));");
                    CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(sm.ToBytes({field.FieldName},typeof({ettt.NumberType})));");

                    SerializeCodeSb.AppendLine($"sm->Index+=sizeof({TargetTypeStrings[ettt.NumberType].CppBaseTargetTypeSerializeString});");
                    CsSerializeCodeSb.AppendLine($"sm.Index+=sizeof({TargetTypeStrings[ettt.NumberType].CsBaseTargetTypeSerializeString});");

                    DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,sizeof({TargetTypeStrings[ettt.NumberType].CppBaseTargetTypeSerializeString}),isIsr);");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=({GetCsTypeDefineName(field.TargetType)})sm.DeserializeField(typeof({ettt.NumberType}));");

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

                        cpp_len_type_string = $"{GetCppTypeSerializeName(arrayLenField.TargetType)}";
                        cs_len_type_string = $"{GetCsTypeSerializeName(arrayLenField.TargetType)}";
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
                            sizeofstr = $"sizeof({TargetTypeStrings[enumtt.NumberType].CppBaseTargetTypeSerializeString})";
                            sizeofStrForCs = $"sizeof({TargetTypeStrings[enumtt.NumberType].CsBaseTargetTypeSerializeString})";
                        }
                        else
                        {
                            sizeofstr = $"sizeof({GetCppTypeSerializeName(attt.ElementType)})";
                            sizeofStrForCs = $"sizeof({GetCsTypeSerializeName(attt.ElementType)})";
                        }
                        DeserializeCodeSb.AppendLine($"obj->{field.FieldName}=({GetCppTypeSerializeName(attt.ElementType)} *)El_Malloc({sizeofstr}*{lenstring});");
                        DeserializeCodeSb.AppendLine($"El_Memset(obj->{field.FieldName},0,{sizeofstr}*{lenstring});");
                    }
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=new {GetCsTypeDefineName(attt.ElementType)}[{cslenstring}];");

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
                            sizeofstr = $"sizeof({TargetTypeStrings[enumtt.NumberType].CppBaseTargetTypeSerializeString})";
                            sizeofStrForCs = $"sizeof({TargetTypeStrings[enumtt.NumberType].CsBaseTargetTypeSerializeString})";
                        }
                        else
                        {
                            sizeofstr = $"sizeof({GetCppTypeSerializeName(attt.ElementType)})";
                            sizeofStrForCs = $"sizeof({GetCsTypeSerializeName(attt.ElementType)})";
                        }
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],{sizeofstr});");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(sm.ToBytes({field.FieldName}[{field.FieldName}_index],typeof({attt.ElementType.TargetType})));");

                        SerializeCodeSb.AppendLine($"sm->Index+={sizeofstr};");
                        CsSerializeCodeSb.AppendLine($"sm.Index+={sizeofStrForCs};");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,{sizeofstr},isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=({GetCsTypeDefineName(attt.ElementType)})sm.DeserializeField(typeof({attt.ElementType.TargetType}));");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else if(attt.ElementType.TargetType==typeof(Enum))//else if (attt.ElementType.TargetType == TargetType_t.TYPE_ENUM)
                    {
                        var ettt = attt.ElementType as EnumType_TargetType;
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({TargetTypeStrings[ettt.NumberType].CppBaseTargetTypeSerializeString}));");
                        CsSerializeCodeSb.AppendLine($"sm.Buf.AddRange(sm.ToBytes({field.FieldName}[{field.FieldName}_index],typeof({ettt.NumberType})));");

                        SerializeCodeSb.AppendLine($"sm->Index+=sizeof({TargetTypeStrings[ettt.NumberType].CppBaseTargetTypeSerializeString});");
                        CsSerializeCodeSb.AppendLine($"sm.Index+=sizeof({TargetTypeStrings[ettt.NumberType].CsBaseTargetTypeSerializeString});");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,sizeof({TargetTypeStrings[ettt.NumberType].CppBaseTargetTypeSerializeString}),isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=({TargetTypeStrings[ettt.NumberType].CsBaseTargetTypeDefineString})sm.DeserializeField(typeof({ettt.NumberType}));");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else
                    {
                        SerializeCodeSb.AppendLine($"{GetCppTypeDefineName(attt.ElementType)}_Serialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");
                        CsSerializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index].Serialize(sm);");

                        DeserializeCodeSb.AppendLine($"{GetCppTypeDefineName(attt.ElementType)}_Deserialize(sm,&obj->{field.FieldName}[{field.FieldName}_index],isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=new {GetCsTypeDefineName(attt.ElementType)}();");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index].Deserialize(sm);");

                        FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}{GetCppTypeDefineName(attt.ElementType)}_FreeData(&obj->{field.FieldName}[{field.FieldName}_index]);");
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
                    SerializeCodeSb.AppendLine($"{GetCppTypeDefineName(field.TargetType)}_Serialize(sm,&obj->{field.FieldName});");
                    CsSerializeCodeSb.AppendLine($"{field.FieldName}.Serialize(sm);");

                    DeserializeCodeSb.AppendLine($"{GetCppTypeDefineName(field.TargetType)}_Deserialize(sm,&obj->{field.FieldName},isIsr);");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=new {GetCsTypeDefineName(field.TargetType)}();");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}.Deserialize(sm);");

                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{fieldNeedFreeMemoryNote}{GetCppTypeDefineName(field.TargetType)}_FreeData(&obj->{field.FieldName});");
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
