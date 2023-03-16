using System;
using System.Collections.Generic;
using System.Text;

using System.IO;

using System.Reflection;

namespace EmbedXrpcIdlParser
{
    public class CsCodeGenerater
    {
        public CSharpCodeGenParameter CSharpCodeGenParameter { get; set; }
        

        private void GenerateStruct(StreamWriter sw, StructType_TargetType objectType_TargetType)
        {
            sw.WriteLine($"public class {CppCsNanoSerializer.GetCsTypeDefineName(objectType_TargetType)}:IEmbedXrpcSerialization");
            sw.WriteLine("{");//class begin
            foreach (var field in objectType_TargetType.TargetFields)
            {
                if (field.MacroControlAttr != null)
                {
                    CppSerializableCommon.MacroControlWriteBegin(sw, field.MacroControlAttr);
                }

                sw.WriteLine($"public const int {CppCsNanoSerializer.GetCsTypeDefineName(objectType_TargetType)}_{field.FieldName}_FieldNumber={field.FieldNumberAttr.Number};");
                if (field.NoSerializationAttr != null)
                {
                    sw.WriteLine($"[NoSerialization]");
                }

                if(IdlInfo.IsBaseType(field.TargetType.TargetType) || field.TargetType.TargetType ==typeof(Enum))//if (field.TargetType.TargetType <= TargetType_t.TYPE_ENUM)
                {
                    Base_TargetField base_TargetField = field as Base_TargetField;
                    if (base_TargetField.IsUnionTargetTypeField == true)
                    {
                        sw.WriteLine($"[UnionTargetType]");
                    }
                    if (base_TargetField.UnionFieldAttr != null)
                    {
                        sw.WriteLine($"[UnionField]");
                    }
                    if (base_TargetField.BitFieldAttribute != null)
                    {
                        sw.WriteLine($"[BitField({base_TargetField.BitFieldAttribute.BitWidthLength})]");
                    }
                    sw.WriteLine($"[FieldNumber( {field.FieldNumberAttr.Number}) ] ");
                    sw.WriteLine($"[ArrayLenFieldFlag( {base_TargetField.IsArrayLenField.ToString().ToString().ToLower()} ) ]");
                    sw.WriteLine($"public {CppCsNanoSerializer.GetCsTypeDefineName(field.TargetType)} {field.FieldName}{{get;set;}}");
                }
                else if(field.TargetType.TargetType==typeof(Array))//else if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                {
                    Array_TargetField array_TargetField = field as Array_TargetField;
                    if (array_TargetField.UnionFieldAttr != null)
                    {
                        sw.WriteLine($"[UnionField]");
                    }
                    sw.WriteLine($"[ArrayProperty(LenFieldName = \"{array_TargetField.MaxCountAttribute.LenFieldName}\")]");
                    Base_TargetField arrayLenField = array_TargetField.ArrayLenField;
                    ArrayType_TargetType attt = array_TargetField.TargetType as ArrayType_TargetType;
                    string lenstring = arrayLenField == null ? "1" : "0";
                    sw.WriteLine($"[FieldNumber( {field.FieldNumberAttr.Number}) ] ");
                    sw.WriteLine($"public {CppCsNanoSerializer.GetCsTypeDefineName(attt.ElementType)}[] {array_TargetField.FieldName}{{get;set;}}=new {CppCsNanoSerializer.GetCsTypeDefineName(attt.ElementType)}[{lenstring}];");

                }
                else if(field.TargetType.TargetType==typeof(object))//else if (field.TargetType.TargetType == TargetType_t.TYPE_STRUCT)
                {
                    Struct_TargetField object_TargetField = field as Struct_TargetField;
                    if (object_TargetField.UnionFieldAttr != null)
                    {
                        sw.WriteLine($"[UnionField]");
                    }
                    sw.WriteLine($"[FieldNumber( {field.FieldNumberAttr.Number}) ] ");
                    sw.WriteLine($"public {CppCsNanoSerializer.GetCsTypeDefineName(field.TargetType)} {field.FieldName}{{get;set;}}=new {CppCsNanoSerializer.GetCsTypeDefineName(field.TargetType)}();");
                }
                if (field.MacroControlAttr != null)
                {
                    CppSerializableCommon.MacroControlWriteEnd(sw, field.MacroControlAttr);
                }
                sw.WriteLine("\r\n");
            }
            new CppCsNanoSerializer().EmitStruct(objectType_TargetType, null, null, sw);
            sw.WriteLine("}");//class end

        }
        /*private void MacroControlWriteBegin(StreamWriter sw, MacroControlAttribute MacroControlAttribute)
        {
            if (MacroControlAttribute != null)
            {
                string val = MacroControlAttribute.EnableCondition == string.Empty ? string.Empty : $"=={MacroControlAttribute.EnableCondition}";
                if (val != string.Empty)
                    sw.WriteLine($"#if {MacroControlAttribute.MacroName}{val}");
                else
                    sw.WriteLine($"#ifdef {MacroControlAttribute.MacroName}");
            }
        }*/
        /*private void MacroControlWriteEnd(StreamWriter sw, MacroControlAttribute MacroControlAttribute)
        {
            if (MacroControlAttribute != null)
            {
                string val = MacroControlAttribute.EnableCondition == string.Empty ? string.Empty : $"=={MacroControlAttribute.EnableCondition}";
                if (val != string.Empty)
                    sw.WriteLine($"#endif // #if {MacroControlAttribute.MacroName}{val}");
                else
                    sw.WriteLine($"#endif // #ifdef {MacroControlAttribute.MacroName}");
            }
        }*/

        private void EmitCaller(TargetService service, StreamWriter sw)
        {
            CppSerializableCommon.MacroControlWriteBegin(sw, service.MacroControlAttribute);

            //sw.WriteLine($"[RequestServiceInfo(Name=\"{service.ServiceName}\",ServiceId={service.ServiceId})]");
            sw.WriteLine($"public class {service.ServiceName}_Requester<DTL>:IRequestService<DTL> where DTL:struct");
            sw.WriteLine("{");//class begin

            sw.WriteLine("private EmbedXrpcObject<DTL> XrpcObject=null;");
            sw.WriteLine($"public {service.ServiceName}_Requester(EmbedXrpcObject<DTL> xrpcObject)");
            sw.WriteLine("{");//function begin
            sw.WriteLine("XrpcObject=xrpcObject;");
            sw.WriteLine("}");//function end

            sw.WriteLine($"public static readonly UInt16 {service.ServiceName}_ServiceId={service.ServiceId};//0x{Convert.ToString(service.ServiceId, 16)}");
            sw.WriteLine($"public UInt16 GetSid(){{ return {service.ServiceName}_ServiceId;}}");

            string pars = string.Empty;
            for (int pi = 0; pi < service.ParameterStructType.TargetFields.Count; pi++)
            {
                var par = service.ParameterStructType.TargetFields[pi];
                pars += CppCsNanoSerializer.GetCsTypeDefineName(par.TargetType) + " " + par.FieldName;
                if (pi + 1 < service.ParameterStructType.TargetFields.Count)
                {
                    pars += ",";
                }
            }
            var dh = pars == "" ? "" : ",";
            sw.WriteLine($"public {CppCsNanoSerializer.GetCsTypeDefineName(service.ReturnStructType)} Invoke(DTL userDataOfTransportLayer{dh}{pars})");
            sw.WriteLine("{");//function begin
            sw.WriteLine($"{service.ReturnStructType.TypeName} reqresp=new {CppCsNanoSerializer.GetCsTypeDefineName(service.ReturnStructType)}();");
            sw.WriteLine("lock(XrpcObject.ObjectMutex) ");
            sw.WriteLine("{");//lock begin
            sw.WriteLine("XrpcObject.MessageQueueOfRequestServiceHandle.Reset();");
            sw.WriteLine($"{CppCsNanoSerializer.GetCsTypeDefineName(service.ParameterStructType)} request =new {CppCsNanoSerializer.GetCsTypeDefineName(service.ParameterStructType)}();");
            for (int pi = 0; pi < service.ParameterStructType.TargetFields.Count; pi++)
            {
                var par = service.ParameterStructType.TargetFields[pi];
                sw.WriteLine($"request.{par.FieldName}={par.FieldName};");
            }
            sw.WriteLine("SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.RequestBuffer,12);");
            sw.WriteLine("request.Serialize(sm);");
            sw.WriteLine($"XrpcObject.RequestBuffer[0]=((byte)({service.ServiceName}_ServiceId&0xff));");
            sw.WriteLine($"XrpcObject.RequestBuffer[1]=((byte)({service.ServiceName}_ServiceId>>8&0xff));");
            sw.WriteLine($"XrpcObject.RequestBuffer[2]=((byte)(XrpcObject.TimeOut&0xff));");
            sw.WriteLine($"XrpcObject.RequestBuffer[3]=((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));\n");

            sw.WriteLine("var requestObject_dataLen=sm.Index-12;");
            sw.WriteLine("XrpcObject.RequestBuffer[4]=((byte)(requestObject_dataLen&0xff));");
            sw.WriteLine("XrpcObject.RequestBuffer[5]=((byte)(requestObject_dataLen>>8&0xff));");
            sw.WriteLine("XrpcObject.RequestBuffer[6]=((byte)(requestObject_dataLen>>16&0xff));");
            sw.WriteLine("XrpcObject.RequestBuffer[7]=((byte)(requestObject_dataLen>>24&0xff));\n");

            sw.WriteLine("UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf,12,requestObject_dataLen);");
            sw.WriteLine("XrpcObject.RequestBuffer[8]=((byte)(bufcrc&0xff));");
            sw.WriteLine("XrpcObject.RequestBuffer[9]=((byte)(bufcrc>>8&0xff));");
            sw.WriteLine("XrpcObject.RequestBuffer[10]=((byte)(bufcrc>>16&0xff));");
            sw.WriteLine("XrpcObject.RequestBuffer[11]=((byte)(bufcrc>>24&0xff));");

            sw.WriteLine("if( XrpcObject.Send ( userDataOfTransportLayer,sm.Index,0,XrpcObject.RequestBuffer )==false)\n {\nreqresp.State=RequestResponseState.RequestState_Failed;\n goto exi;\n}\n" +
                "else\n{\nreqresp.State=RequestResponseState.RequestState_Ok;\n}");
            if (service.ReturnStructType.TargetFields.Count > 1)
            {
                sw.WriteLine($"var waitstate=XrpcObject.Wait<{CppCsNanoSerializer.GetCsTypeDefineName(service.ReturnStructType)}>({service.ServiceName}_ServiceId, ref reqresp);");
                sw.WriteLine("if(reqresp==null)");
                sw.WriteLine($"{{reqresp=new {CppCsNanoSerializer.GetCsTypeDefineName(service.ReturnStructType)}();}}");
                sw.WriteLine("reqresp.State=waitstate;");
                sw.WriteLine("goto exi;");
            }
            sw.WriteLine("}");//lock begin
            sw.WriteLine("exi:\r\nreturn reqresp;");

            sw.WriteLine("}");//function end

            sw.WriteLine("}");//class end

            CppSerializableCommon.MacroControlWriteEnd(sw, service.MacroControlAttribute);
        }

        private void EmitCallee(TargetService service, StreamWriter sw)
        {
            CppSerializableCommon.MacroControlWriteBegin(sw, service.MacroControlAttribute);
            sw.WriteLine($"[ResponseServiceInfo(Name=\"{service.ServiceName}\",ServiceId={service.ServiceId})]");
            sw.WriteLine($"public partial class {service.ServiceName}_Service<DTL>:IService<DTL> where DTL:struct");
            sw.WriteLine("{");//class begin
            sw.WriteLine($"public static readonly UInt16 {service.ServiceName}_ServiceId={service.ServiceId};//0x{Convert.ToString(service.ServiceId, 16)}");
            sw.WriteLine($"public UInt16 GetSid(){{ return {service.ServiceName}_ServiceId;}}");
            if (service.ReturnStructType.TargetFields.Count > 1)
            {
                sw.WriteLine($"private {CppCsNanoSerializer.GetCsTypeDefineName(service.ReturnStructType)} Response = new {CppCsNanoSerializer.GetCsTypeDefineName(service.ReturnStructType)}();");
            }
            sw.WriteLine("public  void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)");
            sw.WriteLine("{");//function begin
            sw.WriteLine($"{CppCsNanoSerializer.GetCsTypeDefineName(service.ParameterStructType)} request = new {CppCsNanoSerializer.GetCsTypeDefineName(service.ParameterStructType)}();");
            sw.WriteLine($"request.Deserialize(recManager);");
            string pars = string.Empty;
            string externstr = string.Empty;
            for (int pi = 0; pi < service.ParameterStructType.TargetFields.Count; pi++)
            {
                var par = service.ParameterStructType.TargetFields[pi];
                pars += "request." + par.FieldName;
                externstr += CppCsNanoSerializer.GetCsTypeDefineName(par.TargetType) + " " + par.FieldName;
                if (pi + 1 < service.ParameterStructType.TargetFields.Count)
                {
                    pars += ",";
                    externstr += ",";
                }
            }
            var dh = pars == "" ? "" : ",";
            sw.WriteLine($"{service.ServiceName}(ref serviceInvokeParameter{dh}{pars});");
            if (service.ReturnStructType.TargetFields.Count > 1)
            {
                sw.WriteLine($"Response.Serialize(sendManager);");
            }
            sw.WriteLine("}");//function end
            sw.WriteLine($"//public void {service.ServiceName}(ref serviceInvokeParameter{dh}{externstr});");
            sw.WriteLine("}");//class end

            CppSerializableCommon.MacroControlWriteEnd(sw, service.MacroControlAttribute);
        }
        private void GenerateService(StreamWriter sw, TargetService service, RoleType genType)
        {

            //if (service.RoleAttribute.Role == RoleType.Client)
            {
                if (genType == RoleType.Client)
                {
                    EmitCaller(service, sw);
                }
                else if (genType == RoleType.Server)
                {
                    EmitCallee(service, sw);
                }
                else
                {
                    EmitCaller(service, sw);
                    EmitCallee(service, sw);
                }
            }
            //if (service.RoleAttribute.Role == RoleType.Server)
            //{
            //    if (genType == RoleType.Client)
            //    {
            //        EmitCallee(service, sw);
            //    }
            //    else if (genType == RoleType.Server)
            //    {
            //        EmitCaller(service, sw);
            //    }
            //    else
            //    {
            //        EmitCaller(service, sw);
            //        EmitCallee(service, sw);
            //    }
            //}
            sw.WriteLine(Environment.NewLine);
        }
        public void CodeGen(CSharpCodeGenParameter codeGenParameter)
        {
            CSharpCodeGenParameter = codeGenParameter;
            var outputattr = codeGenParameter.FileIdlInfo.GenerationOption;
            FileIdlInfo idlInfo = CSharpCodeGenParameter.FileIdlInfo;
            RoleType genType = CSharpCodeGenParameter.RoleType;

            var encode = new UTF8Encoding(false);
            var csStreamWriter = new StreamWriter(CSharpCodeGenParameter.OutPutPath + outputattr.OutPutFileName + ".cs", false, encode);
            csStreamWriter.WriteLine("using System;");
            csStreamWriter.WriteLine("using System.Reflection;");
            csStreamWriter.WriteLine("using System.Collections.Generic;");
            csStreamWriter.WriteLine("using EmbedXrpc;");
            foreach (var userNs in idlInfo.GenerationOption.UserNamespace)
            {
                csStreamWriter.WriteLine($"using {userNs};");
            }
            csStreamWriter.WriteLine($"// auto code gen ! DO NOT modify this file!");
            csStreamWriter.WriteLine($"//C# Code Generater Version:{Assembly.GetExecutingAssembly().GetName().Version.ToString()}");
            csStreamWriter.WriteLine($"namespace {idlInfo.GenerationOption.CSharpNameSpace}");
            csStreamWriter.WriteLine("{");//namespace begin
            csStreamWriter.WriteLine("using UInt8 = Byte;");
            csStreamWriter.WriteLine("using Int8 = SByte;");
            foreach (var enu in idlInfo.TargetEnums)
            {
                csStreamWriter.WriteLine($"public enum {CppCsNanoSerializer.GetCsTypeDefineName(enu)}:{CppCsNanoSerializer.TargetTypeStrings[enu.NumberType].CsBaseTargetTypeDefineString}");
                csStreamWriter.WriteLine("{");//enum begin
                foreach (var ev in enu.KeyValue)
                {
                    csStreamWriter.WriteLine($"{ev.Key}={ev.Value},");
                }
                csStreamWriter.WriteLine("}");//enum begin
            }

            foreach (var stru in idlInfo.TargetStructs)
            {
                GenerateStruct(csStreamWriter, stru);
            }

            foreach (var service in idlInfo.TargetServices)
            {
                GenerateStruct(csStreamWriter, service.ParameterStructType);
                GenerateStruct(csStreamWriter, service.ReturnStructType);
                GenerateService(csStreamWriter, service, genType);
            }

            csStreamWriter.WriteLine("}");//namespace end

            csStreamWriter.Flush();
            csStreamWriter.Close();
        }
    }

}
