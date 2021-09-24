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
        static Dictionary<TargetType_t, string> NumberTypeOfEnumInCShapCodeCollection = new Dictionary<TargetType_t, string>();
        static CsCodeGenerater()
        {
            //byte、sbyte、short、ushort、int、uint、long 或 ulong
            NumberTypeOfEnumInCShapCodeCollection.Add(TargetType_t.TYPE_UINT8, "byte");
            NumberTypeOfEnumInCShapCodeCollection.Add(TargetType_t.TYPE_INT8, "sbyte");
            NumberTypeOfEnumInCShapCodeCollection.Add(TargetType_t.TYPE_UINT16, "ushort");
            NumberTypeOfEnumInCShapCodeCollection.Add(TargetType_t.TYPE_INT16, "short");
            NumberTypeOfEnumInCShapCodeCollection.Add(TargetType_t.TYPE_UINT32, "uint");
            NumberTypeOfEnumInCShapCodeCollection.Add(TargetType_t.TYPE_INT32, "int");
            NumberTypeOfEnumInCShapCodeCollection.Add(TargetType_t.TYPE_UINT64, "ulong");
            NumberTypeOfEnumInCShapCodeCollection.Add(TargetType_t.TYPE_INT64, "long");
        }
        
        private void GenerateStruct(StreamWriter sw, StructType_TargetType objectType_TargetType)
        {
            sw.WriteLine($"public class {objectType_TargetType.TypeName}");
            sw.WriteLine("{");//class begin
            foreach (var field in objectType_TargetType.TargetFields)
            {
                sw.WriteLine($"public const int {field.FieldName}_FieldNumber={field.FieldNumberAttr.Number};");
                if (field.NoSerializationAttr != null)
                {
                    sw.WriteLine($"[NoSerialization]");
                }
                if (field.TargetType.TargetType <= TargetType_t.TYPE_ENUM)
                {
                    Base_TargetField base_TargetField = field as Base_TargetField;
                    if(base_TargetField.IsUnionTargetTypeField==true)
                    {
                        sw.WriteLine($"[UnionTargetType]");
                    }
                    if(base_TargetField.UnionFieldAttr!=null)
                    {
                        sw.WriteLine($"[UnionField]");
                    }
                    if(base_TargetField.BitFieldAttribute!=null)
                    {
                        sw.WriteLine($"[BitField({base_TargetField.BitFieldAttribute.BitWidthLength})]");
                    }
                    sw.WriteLine($"[FieldNumber( {field.FieldNumberAttr.Number}) ] ");
                    sw.WriteLine($"[ArrayLenFieldFlag( {base_TargetField.IsArrayLenField.ToString().ToString().ToLower()} ) ]");
                    sw.WriteLine($"public {field.TargetType.TypeName} {field.FieldName}{{get;set;}}");
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                {
                    Array_TargetField array_TargetField = field as Array_TargetField;
                    if (array_TargetField.UnionFieldAttr !=null)
                    {
                        sw.WriteLine($"[UnionField]");
                    }
                    sw.WriteLine($"[ArrayProperty(LenFieldName = \"{array_TargetField.MaxCountAttribute.LenFieldName}\")]");
                    Base_TargetField arrayLenField = array_TargetField.ArrayLenField;
                    ArrayType_TargetType attt = array_TargetField.TargetType as ArrayType_TargetType;
                    string lenstring = arrayLenField == null ? "1" : "0";
                    sw.WriteLine($"[FieldNumber( {field.FieldNumberAttr.Number}) ] ");
                    sw.WriteLine($"public {attt.ElementType.TypeName}[] {array_TargetField.FieldName}{{get;set;}}=new {attt.ElementType.TypeName}[{lenstring}];");
                }
                else if (field.TargetType.TargetType == TargetType_t.TYPE_STRUCT)
                {
                    Struct_TargetField object_TargetField = field as Struct_TargetField;
                    if (object_TargetField.UnionFieldAttr != null)
                    {
                        sw.WriteLine($"[UnionField]");
                    }
                    sw.WriteLine($"[FieldNumber( {field.FieldNumberAttr.Number}) ] ");
                    sw.WriteLine($"public {field.TargetType.TypeName} {field.FieldName}{{get;set;}}=new {field.TargetType.TypeName}();");
                }
                sw.WriteLine("\r\n");
            }
            sw.WriteLine("}");//class end

        }
        private void MacroControlWriteBegin(StreamWriter sw, MacroControlAttribute MacroControlAttribute)
        {
            if (MacroControlAttribute != null)
            {
                string val = MacroControlAttribute.EnableCondition == string.Empty ? string.Empty : $"=={MacroControlAttribute.EnableCondition}";
                if (val != string.Empty)
                    sw.WriteLine($"#if {MacroControlAttribute.MacroName}{val}");
                else
                    sw.WriteLine($"#ifdef {MacroControlAttribute.MacroName}");
            }
        }
        private void MacroControlWriteEnd(StreamWriter sw, MacroControlAttribute MacroControlAttribute)
        {
            if (MacroControlAttribute != null)
            {
                string val = MacroControlAttribute.EnableCondition == string.Empty ? string.Empty : $"=={MacroControlAttribute.EnableCondition}";
                if (val != string.Empty)
                    sw.WriteLine($"#endif // #if {MacroControlAttribute.MacroName}{val}");
                else
                    sw.WriteLine($"#endif // #ifdef {MacroControlAttribute.MacroName}");
            }
        }
        private void GenerateDelegate(StreamWriter sw, TargetDelegate del, GenType genType)
        {
            GenerateStruct(sw, del.ParameterStructType);
            if (genType == GenType.Client || genType == GenType.All)
            {
                MacroControlWriteBegin(sw, del.MacroControlAttribute);
                sw.WriteLine($"[DelegateInfo(Name=\"{del.MethodName}\")]");
                sw.WriteLine($"public partial class {del.MethodName}ClientImpl<DTL>:IDelegate<DTL> where DTL:struct");
                sw.WriteLine("{");//class begin

                sw.WriteLine($"public static readonly UInt16 {del.MethodName}_ServiceId={del.ServiceId};//0x{Convert.ToString(del.ServiceId, 16)}");
                sw.WriteLine($"public UInt16 GetSid(){{ return {del.MethodName}_ServiceId;}}");
                sw.WriteLine("public void Invoke(DTL userDataOfTransportLayer,SerializationManager recManager)");
                sw.WriteLine("{");//function begin

                sw.WriteLine($"{del.ParameterStructType.TypeName} request = recManager.Deserialize <{del.ParameterStructType.TypeName}>();");
                string pars = string.Empty;
                string externstr = string.Empty;
                for (int pi = 0; pi < del.ParameterStructType.TargetFields.Count; pi++)
                {
                    var par = del.ParameterStructType.TargetFields[pi];
                    pars += "request." + par.FieldName;
                    externstr += par.TargetType.TypeName + " " + par.FieldName;
                    if (pi + 1 < del.ParameterStructType.TargetFields.Count)
                    {
                        pars += ",";
                        externstr += ",";
                    }
                }
                var dh = pars == "" ? "" : ",";
                sw.WriteLine($"{del.MethodName}(userDataOfTransportLayer{dh}{pars});//call function!");
                sw.WriteLine("}");//function end
                sw.WriteLine($"//public void {del.MethodName}({externstr});");

                sw.WriteLine("}");//class end

                MacroControlWriteEnd(sw, del.MacroControlAttribute);
            }
            if (genType == GenType.Server || genType == GenType.All)
            {
                MacroControlWriteBegin(sw, del.MacroControlAttribute);

                sw.WriteLine($"public class {del.MethodName}Delegate<DTL> where DTL:struct");
                sw.WriteLine("{");//class begin

                sw.WriteLine("private EmbedXrpcObject<DTL> XrpcObject; ");
                sw.WriteLine($"public {del.MethodName}Delegate(EmbedXrpcObject<DTL> xrpcObject)");
                sw.WriteLine("{");//function begin
                sw.WriteLine("XrpcObject=xrpcObject;");
                sw.WriteLine("}");//function end

                sw.WriteLine($"public static readonly UInt16 {del.MethodName}_ServiceId={del.ServiceId};//0x{Convert.ToString(del.ServiceId, 16)}");
                var pars = string.Empty;
                for (int pi = 0; pi < del.ParameterStructType.TargetFields.Count; pi++)
                {
                    var par = del.ParameterStructType.TargetFields[pi];
                    pars += par.TargetType.TypeName + " " + par.FieldName;
                    if (pi + 1 < del.ParameterStructType.TargetFields.Count)
                    {
                        pars += ",";
                    }
                }
                var dh = pars == "" ? "" : ",";
                sw.WriteLine($"public void Invoke(DTL userDataOfTransportLayer{dh}{pars})");
                sw.WriteLine("{");//function begin
                sw.WriteLine($"{del.ParameterStructType.TypeName} request =new {del.ParameterStructType.TypeName}();");
                for (int pi = 0; pi < del.ParameterStructType.TargetFields.Count; pi++)
                {
                    var par = del.ParameterStructType.TargetFields[pi];
                    sw.WriteLine($"request.{par.FieldName} = {par.FieldName};");
                }
                sw.WriteLine("SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());");
                sw.WriteLine("sm.Serialize(request,0);");
                sw.WriteLine("List<byte> sendBytes = new List<byte>();");
                sw.WriteLine($"sendBytes.Add((byte)({del.MethodName}_ServiceId&0xff));");
                sw.WriteLine($"sendBytes.Add((byte)({del.MethodName}_ServiceId>>8&0xff));");
                sw.WriteLine($"sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));");
                sw.WriteLine($"sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Delegate)<<6));");
                sw.WriteLine("sendBytes.AddRange(sm.Data);");
                sw.WriteLine("XrpcObject.Send (userDataOfTransportLayer, sendBytes.Count,0,sendBytes.ToArray() );");

                sw.WriteLine("}");//function end

                sw.WriteLine("}");//class end

                MacroControlWriteEnd(sw, del.MacroControlAttribute);
            }
            sw.WriteLine(Environment.NewLine);
        }
        private void GenerateService(StreamWriter sw,string interfaceName, TargetService service, GenType genType)
        {
            GenerateStruct(sw, service.ParameterStructType);
            GenerateStruct(sw, service.ReturnStructType);
            if (genType == GenType.Server || genType == GenType.All)
            {
                MacroControlWriteBegin(sw, service.MacroControlAttribute);
                sw.WriteLine($"[ResponseServiceInfo(Name=\"{interfaceName}_{service.ServiceName}\",ServiceId={service.ServiceId})]");
                sw.WriteLine($"public partial class {interfaceName}_{service.ServiceName}Service<DTL>:IService<DTL> where DTL:struct");
                sw.WriteLine("{");//class begin
                sw.WriteLine($"public static readonly UInt16 {service.ServiceName}_ServiceId={service.ServiceId};//0x{Convert.ToString(service.ServiceId, 16)}");
                sw.WriteLine($"public UInt16 GetSid(){{ return {service.ServiceName}_ServiceId;}}");
                if (service.ReturnStructType.TargetFields.Count > 1)
                {
                    sw.WriteLine($"private {service.ReturnStructType.TypeName} Response = new {service.ReturnStructType.TypeName}();");
                }
                sw.WriteLine("public  void Invoke(DTL request_UserDataOfTransportLayer,"+
         "ref DTL response_UserDataOfTransportLayer,"+
         "EmbedXrpcObject < DTL > rpcObject,"+
         "UInt16 targetTimeOut, SerializationManager recManager, SerializationManager sendManager)");
                sw.WriteLine("{");//function begin
                sw.WriteLine($"{service.ParameterStructType.TypeName} request = recManager.Deserialize < {service.ParameterStructType.TypeName}>();");
                string pars = string.Empty;
                string externstr = string.Empty;
                for (int pi = 0; pi < service.ParameterStructType.TargetFields.Count; pi++)
                {
                    var par = service.ParameterStructType.TargetFields[pi];
                    pars += "request." + par.FieldName;
                    externstr += par.TargetType.TypeName + " " + par.FieldName;
                    if (pi + 1 < service.ParameterStructType.TargetFields.Count)
                    {
                        pars += ",";
                        externstr += ",";
                    }
                }
                var dh = pars == "" ? "" : ",";
                sw.WriteLine($"{service.ServiceName}(request_UserDataOfTransportLayer,ref response_UserDataOfTransportLayer,rpcObject,targetTimeOut{dh}{pars});");
                if (service.ReturnStructType.TargetFields.Count > 1)
                {
                    sw.WriteLine($"sendManager.Serialize(Response, 0);");
                }
                sw.WriteLine("}");//function end
                sw.WriteLine($"//public void {service.ServiceName}(request_UserDataOfTransportLayer,ref response_UserDataOfTransportLayer,rpcObject,targetTimeOut{dh}{externstr});");
                sw.WriteLine("}");//class end

                MacroControlWriteEnd(sw, service.MacroControlAttribute);
            }
            if (genType == GenType.Client || genType == GenType.All)
            {
                MacroControlWriteBegin(sw, service.MacroControlAttribute);

                sw.WriteLine($"[RequestServiceInfo(Name=\"{interfaceName}_{service.ServiceName}\",ServiceId={service.ServiceId})]");
                sw.WriteLine($"public class {interfaceName}_{service.ServiceName}<DTL>:IRequestService<DTL> where DTL:struct");
                sw.WriteLine("{");//class begin

                sw.WriteLine("private EmbedXrpcObject<DTL> XrpcObject=null;");
                sw.WriteLine($"public {interfaceName}_{service.ServiceName}(EmbedXrpcObject<DTL> xrpcObject)");
                sw.WriteLine("{");//function begin
                sw.WriteLine("XrpcObject=xrpcObject;");
                sw.WriteLine("}");//function end

                sw.WriteLine($"public static readonly UInt16 {service.ServiceName}_ServiceId={service.ServiceId};//0x{Convert.ToString(service.ServiceId, 16)}");
                sw.WriteLine($"public UInt16 GetSid(){{ return {service.ServiceName}_ServiceId;}}");

                string pars = string.Empty;
                for (int pi = 0; pi < service.ParameterStructType.TargetFields.Count; pi++)
                {
                    var par = service.ParameterStructType.TargetFields[pi];
                    pars += par.TargetType.TypeName + " " + par.FieldName;
                    if (pi + 1 < service.ParameterStructType.TargetFields.Count)
                    {
                        pars += ",";
                    }
                }
                var dh = pars == "" ? "" : ",";
                sw.WriteLine($"public {service.ReturnStructType.TypeName} Invoke(DTL userDataOfTransportLayer{dh}{pars})");
                sw.WriteLine("{");//function begin
                sw.WriteLine($"{service.ReturnStructType.TypeName} reqresp=new {service.ReturnStructType.TypeName}();");
                sw.WriteLine("lock(XrpcObject.ObjectMutex) ");
                sw.WriteLine("{");//lock begin
                sw.WriteLine("XrpcObject.ResponseMessageQueueHandle.Reset();");
                sw.WriteLine($"{service.ParameterStructType.TypeName} request =new {service.ParameterStructType.TypeName}();");
                for (int pi = 0; pi < service.ParameterStructType.TargetFields.Count; pi++)
                {
                    var par = service.ParameterStructType.TargetFields[pi];
                    sw.WriteLine($"request.{par.FieldName}={par.FieldName};");   
                }
                sw.WriteLine("SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());");
                sw.WriteLine("sm.Serialize(request,0);");
                sw.WriteLine("List<byte> sendBytes = new List<byte>();");
                sw.WriteLine($"sendBytes.Add((byte)({service.ServiceName}_ServiceId&0xff));");
                sw.WriteLine($"sendBytes.Add((byte)({service.ServiceName}_ServiceId>>8&0xff));");
                sw.WriteLine($"sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));");
                sw.WriteLine($"sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));");
                sw.WriteLine("sendBytes.AddRange(sm.Data);");
                sw.WriteLine("if( XrpcObject.Send ( userDataOfTransportLayer,sendBytes.Count,0,sendBytes.ToArray() )==false)\n {\nreqresp.State=RequestResponseState.RequestState_Failed;\n goto exi;\n}\n" +
                    "else\n{\nreqresp.State=RequestResponseState.RequestState_Ok;\n}");
                if (service.ReturnStructType.TargetFields.Count > 1)
                { 
                    sw.WriteLine($"var waitstate=XrpcObject.Wait<{service.ReturnStructType.TypeName}>({service.ServiceName}_ServiceId, out reqresp);");
                    sw.WriteLine("if(reqresp==null)");
                    sw.WriteLine($"{{reqresp=new {service.ReturnStructType.TypeName}();}}");
                    sw.WriteLine("reqresp.State=waitstate;");
                    sw.WriteLine("goto exi;");
                }
                sw.WriteLine("}");//lock begin
                sw.WriteLine("exi:\r\nreturn reqresp;");

                sw.WriteLine("}");//function end

                sw.WriteLine("}");//class end

                MacroControlWriteEnd(sw, service.MacroControlAttribute);
            }
            sw.WriteLine(Environment.NewLine);
        }
        public void CodeGen(CSharpCodeGenParameter codeGenParameter)
        {
            CSharpCodeGenParameter = codeGenParameter;
            var outputattr = codeGenParameter.FileIdlInfo.GenerationOption;
            FileIdlInfo idlInfo = CSharpCodeGenParameter.FileIdlInfo;
            GenType genType = CSharpCodeGenParameter.GenType;

            var encode = Encoding.UTF8;
            var csStreamWriter = new StreamWriter(CSharpCodeGenParameter.OutPutPath + outputattr.OutPutFileName + ".cs", false, encode);
            csStreamWriter.WriteLine("using System;");
            csStreamWriter.WriteLine("using System.Reflection;");
            csStreamWriter.WriteLine("using System.Collections.Generic;");
            csStreamWriter.WriteLine("using EmbedXrpc;");
            foreach (var userNs in idlInfo.GenerationOption.UserNamespace)
            {
                csStreamWriter.WriteLine($"using {userNs};");
            }
            csStreamWriter.WriteLine($"// auto code gen ! DO NOT modify this file! create time {DateTime.Now.ToString("yyyy - MM - dd HH: mm:ss.fff")};");
            csStreamWriter.WriteLine($"//C# Code Generater Version:{Assembly.GetExecutingAssembly().GetName().Version.ToString()}");
            csStreamWriter.WriteLine($"namespace {idlInfo.GenerationOption.CSharpNameSpace}");
            csStreamWriter.WriteLine("{");//namespace begin
            csStreamWriter.WriteLine("using UInt8 = Byte;");
            csStreamWriter.WriteLine("using Int8 = SByte;");
            foreach (var enu in idlInfo.TargetEnums)
            {
                csStreamWriter.WriteLine($"public enum {enu.TypeName}:{NumberTypeOfEnumInCShapCodeCollection[enu.NumberType]}");
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
            foreach (var del in idlInfo.TargetDelegates)
            {
                GenerateDelegate(csStreamWriter, del, genType);
            }
            foreach (var targetInterface in idlInfo.TargetInterfaces)
            {
                foreach (var service in targetInterface.Services)
                {
                    GenerateService(csStreamWriter, targetInterface.Name, service, genType);
                }
            }

            csStreamWriter.WriteLine("}");//namespace end

            csStreamWriter.Flush();
            csStreamWriter.Close();
        }
    }

}
