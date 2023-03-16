using System;
using System.Collections.Generic;
using System.Diagnostics;
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
            public int TargetTypeSerializeWidth { get; set; }
            public string CsBaseTargetTypeDefineString { get; set; }
        }

        static CppCsNanoSerializer()
        {
            TargetTypeStrings.Add(typeof(bool), new TypeString() { CppBaseTargetTypeDefineString = "bool", TargetTypeSerializeWidth = 1, CsBaseTargetTypeDefineString = "bool" });
            TargetTypeStrings.Add(typeof(byte), new TypeString() { CppBaseTargetTypeDefineString = "uint8_t", TargetTypeSerializeWidth = 1, CsBaseTargetTypeDefineString = "byte" });
            TargetTypeStrings.Add(typeof(sbyte), new TypeString() { CppBaseTargetTypeDefineString = "int8_t", TargetTypeSerializeWidth = 1, CsBaseTargetTypeDefineString = "sbyte"});
            TargetTypeStrings.Add(typeof(UInt16), new TypeString() { CppBaseTargetTypeDefineString = "uint16_t", TargetTypeSerializeWidth = 2, CsBaseTargetTypeDefineString = "UInt16"});
            TargetTypeStrings.Add(typeof(Int16), new TypeString() { CppBaseTargetTypeDefineString = "int16_t", TargetTypeSerializeWidth = 2, CsBaseTargetTypeDefineString = "Int16" });
            TargetTypeStrings.Add(typeof(UInt32), new TypeString() { CppBaseTargetTypeDefineString = "uint32_t", TargetTypeSerializeWidth = 4, CsBaseTargetTypeDefineString = "UInt32"});
            TargetTypeStrings.Add(typeof(Int32), new TypeString() { CppBaseTargetTypeDefineString = "int32_t", TargetTypeSerializeWidth = 4, CsBaseTargetTypeDefineString = "Int32" });
            TargetTypeStrings.Add(typeof(UInt64), new TypeString() { CppBaseTargetTypeDefineString = "uint64_t", TargetTypeSerializeWidth = 8, CsBaseTargetTypeDefineString = "UInt64"});
            TargetTypeStrings.Add(typeof(Int64), new TypeString() { CppBaseTargetTypeDefineString = "int64_t", TargetTypeSerializeWidth = 8, CsBaseTargetTypeDefineString = "Int64"});
            TargetTypeStrings.Add(typeof(float), new TypeString() { CppBaseTargetTypeDefineString = "float", TargetTypeSerializeWidth = 4, CsBaseTargetTypeDefineString = "float"});
            TargetTypeStrings.Add(typeof(double), new TypeString() { CppBaseTargetTypeDefineString = "double", TargetTypeSerializeWidth =8, CsBaseTargetTypeDefineString = "double"});
            TargetTypeStrings.Add(typeof(IntPtr), new TypeString() { CppBaseTargetTypeDefineString = "void *", TargetTypeSerializeWidth = 0, CsBaseTargetTypeDefineString = "IntPtr"});

            
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
        
        internal class BitFieldStateMachine
        {
            internal List<string> SerializeCodeString = new List<string>();
            internal List<string> CsSerializeCodeString = new List<string>();

            internal List<string> DeserializeCodeString = new List<string>();
            internal List<string> CsDeserializeCodeString = new List<string>();


            internal int BitFieldLeftShiftAccer = 0;
            static internal int BitFieldTempValueCount = 0;
            internal Base_TargetField Field;
            internal Type BitFieldType => Field.TargetType.TargetType;

            public BitFieldStateMachine(Base_TargetField f)
            {
                Field = f;
            }
            internal string BitFieldCppDefineType
            {
                get
                {
                    return $"{ TargetTypeStrings[BitFieldType].CppBaseTargetTypeDefineString}";
                }
            }
            internal string BitFieldCsDefineType
            {
                get
                {
                    return $"{ TargetTypeStrings[BitFieldType].CsBaseTargetTypeDefineString}";
                }
            }
            internal bool BitFieldProcess(StringBuilder cppSerializeString, StringBuilder cppDeserializeString,
                StringBuilder csSerializeString, StringBuilder csDeserializeString,
                ITargetField nextTargetField)
            {
                bool writeFlag = false;
                bool isEnd = false;
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
                            throw new Exception($"相邻的bit字段的类型必须一样!当前字段:{Field.FieldName},下一个字段:{nextBaseTargetField.FieldName}");
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
                    var a = $"sizeof({BitFieldCppDefineType})";
                    var b = $"{TargetTypeStrings[Field.TargetType.TargetType].TargetTypeSerializeWidth}";
                    SerializeCodeString.Add($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&bitsTempValue{BitFieldTempValueCount},{a});");
                    // CsSerializeCodeString.Add($"sm.Buf.AddRange(sm.ToBytes(bitsTempValue{BitFieldTempValueCount},typeof({BitFieldType})));");
                    CsSerializeCodeString.Add($"if(sm.Buf!=null) bytes=sm.ToBytes(bitsTempValue{BitFieldTempValueCount},typeof({BitFieldType}));");
                    CsSerializeCodeString.Add($"if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);");

                    SerializeCodeString.Add($"sm->Index+={TargetTypeStrings[Field.TargetType.TargetType].TargetTypeSerializeWidth};");
                    CsSerializeCodeString.Add($"sm.Index+={TargetTypeStrings[Field.TargetType.TargetType].TargetTypeSerializeWidth};");

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

                    if (BitFieldCppDefineType != string.Empty)
                    {
                        BitFieldTempValueCount++;
                    }
                    isEnd = true;
                }
                return isEnd;
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
            CsSerializeCodeSb.AppendLine("byte[] bytes=null;");

            DeserializeHeaderSb.AppendLine($"void {GetCppTypeDefineName(targetStructUnion)}_Deserialize(SerializationManager *sm,{GetCppTypeDefineName(targetStructUnion)} *obj,int isIsr);");
            DeserializeCodeSb.AppendLine($"void {GetCppTypeDefineName(targetStructUnion)}_Deserialize(SerializationManager *sm,{GetCppTypeDefineName(targetStructUnion)} *obj,int isIsr)");
            DeserializeCodeSb.AppendLine("{");            

            CsDeserializeCodeSb.AppendLine($"public void Deserialize(SerializationManager sm)");
            CsDeserializeCodeSb.AppendLine("{");
            //CsDeserializeCodeSb.AppendLine("byte[] bytes=null;");

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
            BitFieldStateMachine bitFieldStateMachine = null;
            //BitFieldStateMachine.BitFieldTempValueCount = 0;
            
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
                    Debug.Assert(base_TargetField != null);
                    if (base_TargetField.BitFieldAttribute != null)
                    {
                        if (bitFieldStateMachine==null)
                        {
                            bitFieldStateMachine = new BitFieldStateMachine(base_TargetField);
                            //bitFieldStateMachine.BitFieldType = base_TargetField.TargetType.TargetType;//{ BaseType_TargetType.TypeReplaceDic[base_TargetField.TargetType.TargetType]};

                            bitFieldStateMachine.SerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppDefineType} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.CsSerializeCodeString.Add($"{bitFieldStateMachine.BitFieldCsDefineType} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");

                            bitFieldStateMachine.DeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCppDefineType} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            bitFieldStateMachine.CsDeserializeCodeString.Add($"{bitFieldStateMachine.BitFieldCsDefineType} bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=0;");
                            string sizeofStr = string.Empty;
                            string sizeofStrForCs = string.Empty;
                            sizeofStr = $"sizeof({TargetTypeStrings[bitFieldStateMachine.BitFieldType].CppBaseTargetTypeDefineString})";
                            sizeofStrForCs = $"sizeof({TargetTypeStrings[bitFieldStateMachine.BitFieldType].CsBaseTargetTypeDefineString})";
                            bitFieldStateMachine.DeserializeCodeString.Add($"DeserializeField((uint8_t *)&bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount},sm,{TargetTypeStrings[bitFieldStateMachine.BitFieldType].TargetTypeSerializeWidth},{sizeofStr},isIsr);");
                            bitFieldStateMachine.CsDeserializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}=({bitFieldStateMachine.BitFieldCsDefineType})sm.DeserializeField(typeof({base_TargetField.TargetType.TargetType}),{TargetTypeStrings[bitFieldStateMachine.BitFieldType].TargetTypeSerializeWidth});");
                        }
                        bitFieldStateMachine.SerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCppDefineType})(obj->{field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");
                        bitFieldStateMachine.CsSerializeCodeString.Add($"bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount} |= (({bitFieldStateMachine.BitFieldCsDefineType})({field.FieldName}))<< {bitFieldStateMachine.BitFieldLeftShiftAccer} ;");

                        bitFieldStateMachine.DeserializeCodeString.Add($"obj->{field.FieldName}=bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}>>{bitFieldStateMachine.BitFieldLeftShiftAccer};");
                        bitFieldStateMachine.CsDeserializeCodeString.Add($"{field.FieldName}=bitsTempValue{BitFieldStateMachine.BitFieldTempValueCount}>>{bitFieldStateMachine.BitFieldLeftShiftAccer};");

                        bitFieldStateMachine.BitFieldLeftShiftAccer += base_TargetField.BitFieldAttribute.BitWidthLength;
                        var isEnd=bitFieldStateMachine.BitFieldProcess(SerializeCodeSb, DeserializeCodeSb,
                            CsSerializeCodeSb, CsDeserializeCodeSb,
                            i + 1 >= TargetFields.Count ? null : TargetFields[i + 1]);
                        if (isEnd) bitFieldStateMachine = null;
                    }
                    else
                    {
                        string serializeSizeStr = string.Empty;
                        string defineSizeStr=string.Empty;
                        string defineSizeStrForCs = string.Empty;
                        if (base_TargetField.TargetType.TargetType == typeof(Enum))
                        {
                            throw new Exception();
                            
                            
                        }
                        else
                        {
                            serializeSizeStr = $"{TargetTypeStrings[base_TargetField.TargetType.TargetType].TargetTypeSerializeWidth}";

                            defineSizeStr = $"sizeof({TargetTypeStrings[base_TargetField.TargetType.TargetType].CppBaseTargetTypeDefineString})";
                            defineSizeStrForCs = $"sizeof({TargetTypeStrings[base_TargetField.TargetType.TargetType].CsBaseTargetTypeDefineString})";

                        }
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{base_TargetField.FieldName},{defineSizeStr});");
                        CsSerializeCodeSb.AppendLine($"if(sm.Buf!=null) bytes=sm.ToBytes({base_TargetField.FieldName},typeof({base_TargetField.TargetType.TargetType}));");
                        CsSerializeCodeSb.AppendLine($"if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);");

                        SerializeCodeSb.AppendLine($"sm->Index+={serializeSizeStr};");
                        CsSerializeCodeSb.AppendLine($"sm.Index+={serializeSizeStr};");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,{serializeSizeStr},{defineSizeStr},isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}=({GetCsTypeDefineName(field.TargetType)})sm.DeserializeField(typeof({base_TargetField.TargetType.TargetType}),{serializeSizeStr});");

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
                    var a = $"sizeof({field.TargetType.TypeName})";
                    var b = $"{TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth}";
                    SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName},{a}<={b}?{a}:{b});");
                    CsSerializeCodeSb.AppendLine($"if(sm.Buf!=null) bytes=sm.ToBytes({field.FieldName},typeof({ettt.NumberType}));");
                    CsSerializeCodeSb.AppendLine($"if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);");

                    SerializeCodeSb.AppendLine($"sm->Index+={TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth};");
                    CsSerializeCodeSb.AppendLine($"sm.Index+={TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth};");

                    DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName},sm,{TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth},sizeof({field.TargetType.TypeName}),isIsr);");
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=({field.TargetType.TypeName})sm.DeserializeField(typeof({ettt.NumberType}),{TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth});");

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

                        cpp_len_type_string = $"{TargetTypeStrings[arrayLenField.TargetType.TargetType].CppBaseTargetTypeDefineString}";
                        cs_len_type_string = $"{TargetTypeStrings[arrayLenField.TargetType.TargetType].CsBaseTargetTypeDefineString}";
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

                    string serializeSizeOfStr = string.Empty;
                    string defineOfStr = string.Empty;
                    string defineOfStrForCs = string.Empty;

                    
                    if (IdlInfo.IsBaseType(attt.ElementType.TargetType))
                    {
                        serializeSizeOfStr = $"{TargetTypeStrings[attt.ElementType.TargetType].TargetTypeSerializeWidth}";
                        defineOfStr = $"{TargetTypeStrings[attt.ElementType.TargetType].CppBaseTargetTypeDefineString}";
                        defineOfStrForCs = $"{TargetTypeStrings[attt.ElementType.TargetType].CsBaseTargetTypeDefineString}";
                    }
                    else if (attt.ElementType.TargetType == typeof(Enum))
                    {
                        var enumtt = attt.ElementType as EnumType_TargetType;
                        serializeSizeOfStr = $"{TargetTypeStrings[enumtt.NumberType].TargetTypeSerializeWidth}";
                        defineOfStr = $"{attt.ElementType.TypeName}";
                        defineOfStrForCs = defineOfStr;
                    }
                    else
                    {
                        defineOfStr = $"{attt.ElementType.TypeName}";
                        defineOfStrForCs = defineOfStr;
                    }
                    if (array_TargetField.MaxCountAttribute.IsFixed == false)
                    {
                        DeserializeCodeSb.AppendLine($"obj->{field.FieldName}=({defineOfStr} *)El_Malloc(sizeof({defineOfStr})*{lenstring});");
                        DeserializeCodeSb.AppendLine($"El_Memset(obj->{field.FieldName},0,sizeof({defineOfStr})*{lenstring});");
                    }
                    CsDeserializeCodeSb.AppendLine($"{field.FieldName}=new {defineOfStrForCs}[{cslenstring}];");

                    DeserializeCodeSb.AppendLine($"for({cpp_len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    CsDeserializeCodeSb.AppendLine($"for({cs_len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{cslenstring};{field.FieldName}_index++)");

                    DeserializeCodeSb.AppendLine("{");//for begin
                    CsDeserializeCodeSb.AppendLine("{");//for begin

                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}for({cpp_len_type_string} {field.FieldName}_index=0;{field.FieldName}_index<{lenstring};{field.FieldName}_index++)");
                    FreeCodeSb.AppendLine($"{StructFreeNote}{noSerializeAttrNote}{ElementTypeFreeNote}{{");//for begin
                    if(IdlInfo.IsBaseType(attt.ElementType.TargetType))//if (attt.ElementType.TargetType < TargetType_t.TYPE_ENUM)
                    {
                        //string serializeSizeOfStr = string.Empty;
                        if (attt.ElementType.TargetType == typeof(Enum))
                        {
                            throw new Exception();
                        }
                        else
                        {
                            serializeSizeOfStr = $"{TargetTypeStrings[attt.ElementType.TargetType].TargetTypeSerializeWidth}";
                        }
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],sizeof({TargetTypeStrings[attt.ElementType.TargetType].CppBaseTargetTypeDefineString}));");
                        CsSerializeCodeSb.AppendLine($"if(sm.Buf!=null) bytes=sm.ToBytes({field.FieldName}[{field.FieldName}_index],typeof({attt.ElementType.TargetType}));");
                        CsSerializeCodeSb.AppendLine($"if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);");

                        SerializeCodeSb.AppendLine($"sm->Index+={serializeSizeOfStr};");
                        CsSerializeCodeSb.AppendLine($"sm.Index+={serializeSizeOfStr};");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,{serializeSizeOfStr},sizeof({TargetTypeStrings[attt.ElementType.TargetType].CppBaseTargetTypeDefineString}),isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=({TargetTypeStrings[attt.ElementType.TargetType].CsBaseTargetTypeDefineString})sm.DeserializeField(typeof({attt.ElementType.TargetType}),{TargetTypeStrings[attt.ElementType.TargetType].TargetTypeSerializeWidth});");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else if(attt.ElementType.TargetType==typeof(Enum))//else if (attt.ElementType.TargetType == TargetType_t.TYPE_ENUM)
                    {
                        var ettt = attt.ElementType as EnumType_TargetType;
                        var a = $"sizeof({ettt.TypeName}";
                        var b = $"{TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth}";
                        SerializeCodeSb.AppendLine($"if(sm->Buf) El_Memcpy(&sm->Buf[sm->Index],&obj->{field.FieldName}[{field.FieldName}_index],{a}<={b}?{a}:{b});");
                        CsSerializeCodeSb.AppendLine($"if(sm.Buf!=null) bytes=sm.ToBytes({field.FieldName}[{field.FieldName}_index],typeof({ettt.TypeName}));");
                        CsSerializeCodeSb.AppendLine($"if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);");

                        SerializeCodeSb.AppendLine($"sm->Index+={TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth};");
                        CsSerializeCodeSb.AppendLine($"sm.Index+={TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth};");

                        DeserializeCodeSb.AppendLine($"DeserializeField((uint8_t *)&obj->{field.FieldName}[{field.FieldName}_index],sm,{TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth},sizeof({ettt.TypeName}),isIsr);");
                        CsDeserializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index]=({TargetTypeStrings[ettt.NumberType].CsBaseTargetTypeDefineString})sm.DeserializeField(typeof({ettt.NumberType}),{TargetTypeStrings[ettt.NumberType].TargetTypeSerializeWidth});");

                        SerializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                        //DeserializeCodeSb.AppendLine("El_Assert(sm->Index<=sm->BufferLen);");
                    }
                    else
                    {
                        SerializeCodeSb.AppendLine($"{GetCppTypeDefineName(attt.ElementType)}_Serialize(sm,&obj->{field.FieldName}[{field.FieldName}_index]);");
                        CsSerializeCodeSb.AppendLine($"{field.FieldName}[{field.FieldName}_index].Serialize(sm);");

                        DeserializeCodeSb.AppendLine($"{GetCppTypeDefineName(attt.ElementType)}_Deserialize(sm,&obj->{field.FieldName}[{field.FieldName}_index],isIsr);");
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
                    SerializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Serialize(sm,&obj->{field.FieldName});");
                    CsSerializeCodeSb.AppendLine($"{field.FieldName}.Serialize(sm);");

                    DeserializeCodeSb.AppendLine($"{field.TargetType.TypeName}_Deserialize(sm,&obj->{field.FieldName},isIsr);");
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
