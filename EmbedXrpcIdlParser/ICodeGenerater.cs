using CSScriptLibrary;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public class TargetType
    {
        public List<TargetType> SubTargetTypes { get; set; } = new List<TargetType>();
    }
    public static class StringExtensions
    {
        public static int GetCharCount(this string t,char a)
        {
            int cnt = 0;
            for (int i = 0; i < t.Length; i++)
            {
                if(t[i]==a)
                {
                    cnt++;
                }
            }
            return cnt;
        }
    }
    public class TargetField
    {
        public string IdlType { get; set; }
        public string Name { get; set; }
        public bool IsArray { get; set; }
        public FieldIndexAttribute FieldIndexAttribute { get; set; }
        public MaxCountAttribute MaxCountAttribute { get; set; }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append( String.Format($"Field: Type:{IdlType},Name:{Name},IsArray:{IsArray},FieldIndex:{FieldIndexAttribute.Index}"));
            if(IsArray==true)
            {
                sb.Append(string.Format($",IsFixed: { MaxCountAttribute.IsFixed},MaxCount: { MaxCountAttribute.MaxCount},LenFieldIndex:{MaxCountAttribute.LenFieldIndex}"));
            }
            sb.Append("\n");
            return sb.ToString();
        }
    }
    public class TargetReturnValue
    {
        public string IdlType { get; set; }
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(String.Format($"ReturnValue: Type:{IdlType}"));
            sb.Append("\n");
            return sb.ToString();
        }
    }
    public class TargetEnumValue
    {
        public int Value { get; set; }
        public string Description { get; set; }
    }
    public class TargetEnum
    {
        /// <summary>
        /// 转换成整数后的类型
        /// </summary>
        public string IntType { get; set; }

        public string Name { get; set; }
        public List<TargetEnumValue> TargetEnumValues { get; set; } = new List<TargetEnumValue>();
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format($"Enum: Name{Name},IntType : {IntType}\n"));
            foreach (var ev in TargetEnumValues)
            {
                sb.Append(string.Format($"--------Description : {ev.Description},Value : {ev.Value}\n"));
            }
            return sb.ToString();
        }

    }
    public class TargetStruct
    {
        public List<TargetField> TargetFields { get; set; } = new List<TargetField>();

        public string Name { get; set; }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(String.Format($"Struct: Name:{Name}\n"));
            foreach (var field in TargetFields)
            {
                sb.Append(string.Format("--------" + field.ToString()+"\n"));
            }
            return sb.ToString();

        }
    }


    public class TargetService
    {
        public TargetReturnValue ReturnValue { get; set; }
        public string ServiceName { get; set; }

        public List<TargetField> TargetFields { get; set; } = new List<TargetField>();

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(String.Format($"Service: Name:{ServiceName},ReturnValue{ReturnValue.ToString()}"));
            foreach (var field in TargetFields)
            {
                sb.Append(string.Format("--------" + field.ToString() + "\n"));
            }
            return sb.ToString();
        }
    }
    public class TargetInterface
    {
        public string Name { get; set; }
        public List<TargetService> Services { get; set; } = new List<TargetService>();
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(String.Format($"Interface: Name:{Name}\n"));
            foreach (var service in Services)
            {
                sb.Append(service.ToString());
            }
            return sb.ToString();
        }
    }
    public class TargetDelegate
    {
        public string MethodName { get; set; }
        public List<TargetField> TargetFields { get; set; } = new List<TargetField>();
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(String.Format($"Delegate: Name:{MethodName}\n"));
            foreach (var field in TargetFields)
            {
                sb.Append(string.Format("--------" + field.ToString() + "\n"));
            }
            return sb.ToString();
        }
    }
    public class IdlInfo
    {
        public List<TargetEnum> TargetEnums { get; set; } = new List<TargetEnum>();
        public List<TargetStruct> TargetStructs { get; set; } = new List<TargetStruct>();
        public List<TargetInterface> TargetInterfaces { get; set; } = new List<TargetInterface>();
        public List<TargetDelegate> TargetDelegates { get; set; } = new List<TargetDelegate>();

        public GenerationOptionParameterAttribute GenerationOptionParameterAttribute = null;


        public void Parse(string file)
        {
            using (SimpleAsmProbing.For(Assembly.GetExecutingAssembly().Location.GetDirName()))
            {
                Assembly assembly=CSScript.Evaluator.CompileCode(File.ReadAllText(file));
                var types=assembly.GetTypes();
                foreach (var type in types)
                {
                    if(type.Name=="GenerationOption")
                    {
                        GenerationOptionParameterAttribute = type.GetCustomAttribute<GenerationOptionParameterAttribute>();
                    }
                    else if (type.IsValueType == true)
                    {
                        if (type.IsEnum == true)
                        {
                            var vs = type.GetEnumValues();
                            TargetEnum te = new TargetEnum();
                            te.Name = type.Name;
                            te.IntType = type.GetEnumUnderlyingType().Name;
                            foreach (var vsv in vs)
                            {
                                TargetEnumValue targetEnumValue = new TargetEnumValue();
                                targetEnumValue.Value = Convert.ToInt32(vsv);
                                targetEnumValue.Description = type.GetEnumName(vsv);
                                te.TargetEnumValues.Add(targetEnumValue); 
                            }
                            TargetEnums.Add(te);
                            Console.WriteLine(te.ToString());
                        }
                        else
                        {
                            TargetStruct targetStruct = new TargetStruct();
                            targetStruct.Name = type.Name;
                            var fields = type.GetFields(BindingFlags.NonPublic | BindingFlags.Instance);
                            foreach (var field in fields)
                            {
                                var fieldIndexAttribute = field.GetCustomAttribute<FieldIndexAttribute>();
                                TargetField targetField = new TargetField();
                                targetField.FieldIndexAttribute = fieldIndexAttribute;
                                targetField.IsArray = field.FieldType.IsArray;
                                targetField.IdlType = field.FieldType.Name;
                                targetField.Name = field.Name;
                                if (targetField.IsArray == true)
                                {
                                    targetField.MaxCountAttribute = field.GetCustomAttribute<MaxCountAttribute>();
                                }
                                targetStruct.TargetFields.Add(targetField);
                            }
                            TargetStructs.Add(targetStruct);
                            Console.WriteLine(targetStruct.ToString());
                        }
                        
                    }
                    else if(type.IsInterface==true)
                    {
                        TargetInterface targetInterface = new TargetInterface();
                        targetInterface.Name = type.Name;
                        var services = type.GetMembers();
                        foreach (var service in services)
                        {
                            var mt = (service as MethodInfo);
                            TargetReturnValue returnValue = new TargetReturnValue();
                            returnValue.IdlType = mt.ReturnType.Name;
                            if(mt.ReturnType.IsArray==true)
                            {
                                throw new Exception("return value does not support array type!");
                            }
                            TargetService targetService = new TargetService();
                            targetService.ReturnValue = returnValue;
                            targetService.ServiceName = mt.Name;
                            

                            var pars = mt.GetParameters();
                            foreach (var par in pars)
                            {
                                TargetField field = new TargetField();
                                field.FieldIndexAttribute = par.GetCustomAttribute<FieldIndexAttribute>();
                                field.IsArray = par.ParameterType.IsArray;
                                if(field.IsArray==true)
                                {
                                    field.MaxCountAttribute = par.GetCustomAttribute<MaxCountAttribute>();
                                }
                                field.IdlType = par.ParameterType.Name;
                                field.Name = par.Name;
                                targetService.TargetFields.Add(field);
                            }
                            targetInterface.Services.Add(targetService);
                        }
                        Console.WriteLine(targetInterface.ToString());
                        TargetInterfaces.Add(targetInterface);
                    }
                    else if(type.BaseType.Name=="MulticastDelegate")//如果这个类型是委托。
                    {
                        var delms = type.GetMembers();
                        MethodInfo invokemi = null;
                        foreach (var delm in delms)
                        {
                            if(delm.Name=="Invoke")
                            {
                                invokemi = delm as MethodInfo;
                                break;
                            }
                        }
                        if(invokemi ==null)
                        {
                            throw new NullReferenceException("invokemi is null");
                        }
                        TargetDelegate targetDelegate = new TargetDelegate();
                        targetDelegate.MethodName = type.Name;
                        var pars = invokemi.GetParameters();
                        foreach (var par in pars)
                        {
                            TargetField field = new TargetField();
                            field.FieldIndexAttribute = par.GetCustomAttribute<FieldIndexAttribute>();
                            field.IsArray = par.ParameterType.IsArray;
                            if (field.IsArray == true)
                            {
                                field.MaxCountAttribute = par.GetCustomAttribute<MaxCountAttribute>();
                            }
                            field.IdlType = par.ParameterType.Name;
                            field.Name = par.Name;
                            targetDelegate.TargetFields.Add(field);
                        }
                        TargetDelegates.Add(targetDelegate);
                        Console.WriteLine(targetDelegate.ToString());
                    }
                }
            }
            if(GenerationOptionParameterAttribute==null)
            {
                throw new Exception("GenerationOptionParameterAttribute is null!");
            }
        }
    }
    public interface ICodeGenerater
    {
        void CodeGen(IdlInfo idlInfo);
    }
    public class CppCodeGenerater : ICodeGenerater
    {
        private static Dictionary<string, string> ReplaceDic = new Dictionary<string, string>();
        static CppCodeGenerater()
        {
            ReplaceDic.Add("byte", "uint8_t");
            ReplaceDic.Add("sbyte", "int8_t");
            ReplaceDic.Add("UInt16", "uint16_t");
            ReplaceDic.Add("Int16", "int16_t");
            ReplaceDic.Add("UInt32", "uint32_t");
            ReplaceDic.Add("Int32", "int32_t");
            ReplaceDic.Add("float", "float");
            ReplaceDic.Add("double", "double");
        }
        public void CodeGen(IdlInfo idlInfo)
        {
            streamWriter = new StreamWriter(idlInfo.GenerationOptionParameterAttribute.OutPutFileName+".h", false, Encoding.UTF8);
            streamWriter.WriteLine("#include\"EmbedXrpcBaseType.h\"");
            foreach (var em in idlInfo.TargetEnums)
            {
                EmitEnum(em);
            }
            foreach (var stru in idlInfo.TargetStructs)
            {
                EmitStruct(stru);
            }
            foreach (var dels in idlInfo.TargetDelegates)
            {
                EmitDelegate(dels);
            }
            foreach (var ifs in idlInfo.TargetInterfaces)
            {
                EmitClientInterface(ifs);
            }
            streamWriter.Flush();
            streamWriter.Close();
        }
        private StreamWriter streamWriter;
        public void EmitEnum(TargetEnum targetEnum)
        {
            streamWriter.WriteLine("typedef enum _" + targetEnum.Name);
            streamWriter.WriteLine("{");
            foreach (var ev in targetEnum.TargetEnumValues)
            {
                streamWriter.WriteLine(ev.Description +" = "+ ev.Value.ToString()+",");
            }
            streamWriter.WriteLine("}"+ targetEnum.Name + ";");
        }
        public void EmitStruct(TargetStruct targetStruct)
        {
            streamWriter.WriteLine("typedef struct _" + targetStruct.Name);
            streamWriter.WriteLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                string cppType = field.IdlType;
                string name = field.Name;
                if(field.IsArray==true)
                {
                    if(field.MaxCountAttribute.IsFixed==false)
                    {
                        cppType = field.IdlType.Replace("[", "*");
                        cppType = cppType.Replace("]", "");
                    }
                    else
                    {
                        cppType = field.IdlType.Replace("[", "");
                        cppType = cppType.Replace("]", "");
                        name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                    }
                }

                streamWriter.WriteLine(cppType + " " + name + ";");
                
            }
            streamWriter.WriteLine("}" + targetStruct.Name+";");
        }

        public void EmitDelegate(TargetDelegate targetDelegate)
        {
            streamWriter.Write("typedef void (*" + targetDelegate.MethodName + ")(");
            for (int i = 0; i < targetDelegate.TargetFields.Count; i++)
            {
                var field = targetDelegate.TargetFields[i];
                string cppType = field.IdlType;
                string name = field.Name;
                if (field.IsArray == true)
                {                   
                    if (field.MaxCountAttribute.IsFixed == false)
                    {
                        cppType = field.IdlType.Replace("[", "*");
                        cppType = cppType.Replace("]", "");
                    }
                    else
                    {
                        cppType = field.IdlType.Replace("[", "");
                        cppType = cppType.Replace("]", "");
                        name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                    }
                }

                streamWriter.Write(cppType + " " + name);
                if(i+1<targetDelegate.TargetFields.Count)
                {
                    streamWriter.Write(",");
                }
            }
            streamWriter.WriteLine(");");

        }
        public void EmitClientInterface(TargetInterface targetInterface)
        {
            streamWriter.WriteLine("class " + targetInterface.Name+"ClientImpl");
            streamWriter.WriteLine("{\npublic:\nEmbedXrpcClientObject *RpcClientObject=nullptr;");
            streamWriter.WriteLine(targetInterface.Name + "ClientImpl" + "(EmbedXrpcClientObject *rpcobj)");
            streamWriter.WriteLine("{\nthis->RpcClientObject=rpcobj;");
            foreach (var service in targetInterface.Services)
            {
                streamWriter.WriteLine("RpcClientObject->ServicesName.Add(\""+ targetInterface.Name+"." + service.ServiceName + "\");");
            }
            streamWriter.WriteLine("}");

            foreach (var service in targetInterface.Services)
            {
                streamWriter.Write("Option<" + service.ReturnValue.IdlType + "> " + service.ServiceName + "(");
                string temp_fileds=string.Empty;
                for (int i = 0; i < service.TargetFields.Count; i++)
                {
                    var field = service.TargetFields[i];
                    string cppType = field.IdlType;
                    string name = field.Name;
                    temp_fileds += name + ",";
                    if (field.IsArray == true)
                    {
                        if (field.MaxCountAttribute.IsFixed == false)
                        {
                            cppType = field.IdlType.Replace("[", "*");
                            cppType = cppType.Replace("]", "");
                        }
                        else
                        {
                            cppType = field.IdlType.Replace("[", "");
                            cppType = cppType.Replace("]", "");
                            name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                        }
                    }

                    streamWriter.Write(cppType + " " + name);
                    if (i + 1 < service.TargetFields.Count)
                    {
                        streamWriter.Write(",");
                    }
                }
                streamWriter.WriteLine(")\n{");

                streamWriter.WriteLine("//write serialization code:{0}({1})",service.ServiceName, temp_fileds);

                //todo
                //1.serialize  service.name to buf
                //2.serialize fields to buf
                //3.call rpc->send(buff);

                streamWriter.WriteLine("}");
            }

            streamWriter.WriteLine("};");

        }
    }
}

