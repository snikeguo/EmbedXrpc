
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

    public static class StringExtensions
    {
        public static int GetCharCount(this string t, char a)
        {
            int cnt = 0;
            for (int i = 0; i < t.Length; i++)
            {
                if (t[i] == a)
                {
                    cnt++;
                }
            }
            return cnt;
        }
    }
    [Serializable]
    public class TargetField
    {
        public string SourceCodeType { get; set; }
        public string Name { get; set; }
        public bool IsArray { get; set; }

        public bool IsBaseValueType { get; set; }

        public bool IsArrayLenField { get; set; }//是否是Arraylen字段 
        public string SourceCodeElementType { get; set; }

        public BitsFieldLengthAttribute BitsFieldLengthAttribute { get; set; }
        public BitsType BitsType { get; set; } = BitsType.NoBits;
        public TargetEnum Enum { get; set; }
        //public FieldIndexAttribute FieldIndexAttribute { get; set; }
        public MaxCountAttribute MaxCountAttribute { get; set; }

        public FieldNumberAttribute FieldNumberAttr { get; set; }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(String.Format($"Field: Type:{SourceCodeType},Name:{Name},IsArray:{IsArray}"));
            if (IsArray == true)
            {
                sb.Append(string.Format($",IsFixed: { MaxCountAttribute.IsFixed},MaxCount: { MaxCountAttribute.MaxCount},LenFieldName:{MaxCountAttribute.LenFieldName}"));
            }
            sb.Append("\n");
            return sb.ToString();
        }
        
    }
    /*[Serializable]
    public class TargetReturnValue
    {
        public FieldNumberAttribute FieldNumberAttr { get; set; } = new FieldNumberAttribute(1);//先state 后返回字段
        public string SourceCodeType { get; set; }
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(String.Format($"ReturnValue: Type:{SourceCodeType}"));
            sb.Append("\n");
            return sb.ToString();
        }
    }*/
    [Serializable]
    public class TargetEnumValue
    {
        public int Value { get; set; }
        public string Description { get; set; }
    }
    [Serializable]
    public class TargetEnum
    {
        /// <summary>
        /// 转换成整数后的类型
        /// </summary>
        public string SourceCodeType { get; set; }

        public string Name { get; set; }
        public List<TargetEnumValue> TargetEnumValues { get; set; } = new List<TargetEnumValue>();
        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(string.Format($"Enum: Name{Name},IntType : {SourceCodeType}\n"));
            foreach (var ev in TargetEnumValues)
            {
                sb.Append(string.Format($"--------Description : {ev.Description},Value : {ev.Value}\n"));
            }
            return sb.ToString();
        }
    }
    [Serializable]
    public class TargetStruct
    {
        public List<TargetField> TargetFields { get; set; } = new List<TargetField>();
        public BitsAttribute BitsAttribute { get; set; }

        public string Name { get; set; }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(String.Format($"Struct: Name:{Name}\n"));
            foreach (var field in TargetFields)
            {
                sb.Append(string.Format("--------" + field.ToString() + "\n"));
            }
            return sb.ToString();

        }
    }

    [Serializable]
    public class TargetService
    {
        //public TargetField ReturnValue { get; set; }
        public TargetStruct ReturnStruct { get; set; } = new TargetStruct();
        public string ServiceName { get; set; }

        public string FullName { get; set; }
        //public List<TargetField> TargetFields { get; set; } = new List<TargetField>();//参数
        public TargetStruct ParameterStruct { get; set; } = new TargetStruct();
        public int ServiceId { get; internal set; }

    }
    [Serializable]
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
    [Serializable]
    public class TargetDelegate
    {
        public int ServiceId { get; internal set; }
        public string MethodName { get; set; }
        //public List<TargetField> TargetFields { get; set; } = new List<TargetField>();
        public TargetStruct ParameterStruct { get; set; } = new TargetStruct();
        
    }
    /// <summary>
    /// 本文件中所有的资源
    /// </summary>
    public class FileIdlInfo
    {
        public string FileName { get; set; } = string.Empty;
        public List<TargetEnum> TargetEnums { get; set; } = new List<TargetEnum>();
        public List<TargetStruct> TargetStructs { get; set; } = new List<TargetStruct>();
        public List<TargetInterface> TargetInterfaces { get; set; } = new List<TargetInterface>();
        public List<TargetDelegate> TargetDelegates { get; set; } = new List<TargetDelegate>();

        public GenerationOption GenerationOption = null;
    }
    [Serializable]
    public class IdlInfo
    {
       

        public TargetEnum GetTargetEnum(string name,FileIdlInfo fileIdlInfo)
        {
            foreach (var e in fileIdlInfo.TargetEnums)
            {
                if (name == e.Name)
                {
                    return e;
                }
            }
            return null;
        }
        public static TargetField GetArrayLenField(IList<TargetField> fields,TargetField arrayField)
        {
            foreach (var f in fields)
            {
                if(f.Name== arrayField.MaxCountAttribute.LenFieldName)
                {
                    return f;
                }
            }
            return null;
        }
        private  bool IsBaseValueType(Type t)
        {
            if(t.IsEnum==true)
            {
                return true;
            }
            if(t==typeof(byte))
            {
                return true;
            }
            if (t == typeof(sbyte))
            {
                return true;
            }
            if (t == typeof(UInt16))
            {
                return true;
            }
            if (t == typeof(Int16))
            {
                return true;
            }
            if (t == typeof(UInt32))
            {
                return true;
            }
            if (t == typeof(Int32))
            {
                return true;
            }
            if (t == typeof(UInt64))
            {
                return true;
            }
            if (t == typeof(Int64))
            {
                return true;
            }
            return false;
        }
        public List< FileIdlInfo> ParsedFiles { get; private set; } = new List< FileIdlInfo>();//已经被Parse过的file
        public void Parse(string file)
        {
            FileIdlInfo fileIdlInfo = null;
            var pfv = from pf in ParsedFiles where pf.FileName == file select pf;
            if (pfv.ToList().Count!=0)
                return;
            else
            {
                fileIdlInfo = new FileIdlInfo();
                fileIdlInfo.FileName = file;
                ParsedFiles.Add( fileIdlInfo);
            }
            int ServiceId = 0x10;
            Console.WriteLine("parse :{0}...", file);
            CSScript.EvaluatorConfig.Engine = EvaluatorEngine.CodeDom;
            var eva = CSScript.Evaluator;
            Assembly assembly = CSScript.Evaluator.CompileCode(File.ReadAllText(file));
            var types = assembly.GetTypes();

            //processing the service id
            foreach (var type in types)
            {
                var filename = type.GetCustomAttribute<FileNameAttribute>();
                if (filename == null)
                {
                    throw new Exception($"{filename} you must add FileNameAttribute into every class/struct!");
                }
                if (filename.FileNameString != file)
                {
                    Parse(filename.FileNameString);
                    continue;
                }
                if (type.GetInterface("IOptionProcess") != null)
                {
                    var process = assembly.CreateInstance(type.FullName) as IOptionProcess;
                    fileIdlInfo.GenerationOption = process.Process();
                    ServiceId = fileIdlInfo.GenerationOption.ServiceIdStartNumber;
                }
            }
            if(fileIdlInfo==null)
            {
                throw new Exception("GenerationOption is null!");
            }

            foreach (var type in types)
            {
                var filename = type.GetCustomAttribute<FileNameAttribute>();
                if (filename == null)
                {
                    throw new Exception($"{filename} you must add FileNameAttribute into every class/struct!");
                }
                if (filename.FileNameString != file)
                {
                    Parse(filename.FileNameString);
                    continue;
                }
                if (type.IsValueType == true)
                {                    
                    if (type.IsEnum == true)
                    {
                        var vs = type.GetEnumValues();
                        TargetEnum te = new TargetEnum();
                        te.Name = type.Name;
                        te.SourceCodeType = type.GetEnumUnderlyingType().Name;
                        foreach (var vsv in vs)
                        {
                            TargetEnumValue targetEnumValue = new TargetEnumValue();
                            targetEnumValue.Value = Convert.ToInt32(vsv);
                            targetEnumValue.Description = type.GetEnumName(vsv);
                            te.TargetEnumValues.Add(targetEnumValue);
                        }
                        fileIdlInfo.TargetEnums.Add(te);
                        //Console.WriteLine(te.ToString());
                    }
                    else
                    {
                        TargetStruct targetStruct = new TargetStruct();
                        targetStruct.Name = type.Name;
                        var fields = type.GetFields(BindingFlags.NonPublic | BindingFlags.Instance);
                        var BitsAttribute = type.GetCustomAttribute<BitsAttribute>();
                        targetStruct.BitsAttribute = BitsAttribute;
                        if (BitsAttribute==null)
                        {
                            foreach (var field in fields)
                            {
                                var FieldNumberAttr = field.GetCustomAttribute<FieldNumberAttribute>();
                                if (FieldNumberAttr == null)
                                {
                                    FieldNumberAttr = new FieldNumberAttribute(0);//如果没有fieldNumber说明用户并不打算使用TTLV编码
                                    //throw new NotImplementedException($"the {field.Name}'s FieldNumberAttr is null");
                                }
                                //var fieldIndexAttribute = field.GetCustomAttribute<FieldIndexAttribute>();
                                TargetField targetField = new TargetField();
                                //targetField.FieldIndexAttribute = fieldIndexAttribute;
                                targetField.IsArray = field.FieldType.IsArray;
                                targetField.SourceCodeType = field.FieldType.Name;
                                targetField.Name = field.Name;
                                targetField.IsBaseValueType = IsBaseValueType(field.FieldType);
                                if (targetField.IsArray == true)
                                {
                                    var arratt = field.GetCustomAttribute<MaxCountAttribute>();
                                    targetField.MaxCountAttribute = arratt == null ? new MaxCountAttribute() { IsFixed = true } : arratt;
                                    targetField.SourceCodeElementType = field.FieldType.GetElementType().Name;
                                }
                                if (field.FieldType.IsEnum == true)
                                {
                                    targetField.Enum = GetTargetEnum(targetField.SourceCodeType, fileIdlInfo);
                                }
                                var fieldtypelist = (from ft in types where field.FieldType == ft select ft).ToList();
                                if(fieldtypelist.Count>0)
                                {
                                    var fieldtype = fieldtypelist[0];
                                    if (fieldtype.GetCustomAttribute<BitsAttribute>() != null)
                                    {
                                        targetField.BitsType = fieldtype.GetCustomAttribute<BitsAttribute>().BitsType;//
                                        targetField.IsBaseValueType = true;
                                    }
                                }
                                targetField.FieldNumberAttr = FieldNumberAttr;
                                targetStruct.TargetFields.Add(targetField);
                                if(targetField.IsArray==true)
                                {
                                    var lenfield = GetArrayLenField(targetStruct.TargetFields, targetField);
                                    if(lenfield!=null)
                                    {
                                        lenfield.IsArrayLenField = true;
                                    }
                                    else 
                                    {
                                        if(targetField.MaxCountAttribute.IsFixed==false)
                                        {
                                            throw new Exception($"{targetField.Name}当数组不固定的时候,必须制定数组的长度字段!");
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {//bit
                            foreach (var field in fields)
                            {
                                //var fieldIndexAttribute = field.GetCustomAttribute<FieldIndexAttribute>();
                                TargetField targetField = new TargetField();
                                targetField.SourceCodeType = field.FieldType.Name;
                                targetField.Name = field.Name;
                                targetField.BitsFieldLengthAttribute = field.GetCustomAttribute<BitsFieldLengthAttribute>();
                                targetField.IsBaseValueType = true;
                                targetStruct.TargetFields.Add(targetField);  
                            }
                        }
                        
                        fileIdlInfo.TargetStructs.Add(targetStruct);
                        //Console.WriteLine(targetStruct.ToString());
                    }

                }
                else if (type.IsInterface == true)
                {
                    TargetInterface targetInterface = new TargetInterface();
                    targetInterface.Name = type.Name;
                    
                    var services = type.GetMembers();
                    foreach (var service in services)
                    {
                        TargetService targetService = new TargetService();
                        var serviceIdAttr = service.GetCustomAttribute<ServiceIdAttribute>();
                        targetService.ServiceId = serviceIdAttr==null? ServiceId: serviceIdAttr.ServiceId;
                        ServiceId++;
                        var mt = (service as MethodInfo);
                        Type rt = mt.ReturnType;
                        targetService.ServiceName = mt.Name;
                        targetService.FullName = type.Name + "_" + targetService.ServiceName;
                        targetService.ReturnStruct.Name = type.Name+"_" + targetService.ServiceName + "_Return";

                        TargetField ts = new TargetField();
                        ts.Name = "State";
                        ts.SourceCodeType = "RequestResponseState";
                        ts.IsArray = false;
                        ts.Enum = new TargetEnum() { SourceCodeType = "byte", Name = "RequestResponseState" };
                        ts.MaxCountAttribute = null;
                        ts.IsArrayLenField = false;
                        ts.IsBaseValueType = true;
                        ts.FieldNumberAttr = new FieldNumberAttribute(1);//state 的Field Number为1
                        targetService.ReturnStruct.TargetFields.Add(ts);

                        if (rt.Name != "Void")
                        {
                            TargetField returnValue = new TargetField();
                            returnValue.Name = "ReturnValue";
                            returnValue.SourceCodeType = mt.ReturnType.Name;
                            if (mt.ReturnType.IsArray == true)
                            {
                                throw new Exception($"{service.Name}:return value does not support array type!");
                            }
                            returnValue.IsArray = false;
                            returnValue.MaxCountAttribute = null;
                            returnValue.IsArrayLenField = false;
                            returnValue.IsBaseValueType = IsBaseValueType(mt.ReturnType);
                            returnValue.FieldNumberAttr = new FieldNumberAttribute(2);
                            targetService.ReturnStruct.TargetFields.Add(returnValue);
                        }


                        
                        targetService.ParameterStruct.Name = type.Name + "_" + targetService.ServiceName + "_Parameter";
                        var pars = mt.GetParameters();
                        UInt32 fieldNumber = 1;
                        foreach (var par in pars)
                        {
                            TargetField field = new TargetField();
                            //field.FieldIndexAttribute = par.GetCustomAttribute<FieldIndexAttribute>();
                            field.IsArray = par.ParameterType.IsArray;
                            if (field.IsArray == true)
                            {
                                var arratt = par.GetCustomAttribute<MaxCountAttribute>();
                                field.MaxCountAttribute = arratt == null ? new MaxCountAttribute() { IsFixed = true } : arratt;
                                field.SourceCodeElementType = par.ParameterType.GetElementType().Name;
                            }
                            field.SourceCodeType = par.ParameterType.Name;
                            field.Name = par.Name;
                            field.IsBaseValueType = IsBaseValueType(par.ParameterType);
                            if (par.ParameterType.IsEnum == true)
                            {
                                field.Enum = GetTargetEnum(field.SourceCodeType, fileIdlInfo);
                            }
                            field.FieldNumberAttr = new FieldNumberAttribute(fieldNumber);
                            fieldNumber++;
                            targetService.ParameterStruct.TargetFields.Add(field);
                            if (field.IsArray == true)
                            {   //如果数组长度字段是null 那么按照固定长度且长度为1去处理
                                //当数组长度字段不是null,按照数组长度字段结合动态或静态情况处理
                                var lenfield = GetArrayLenField(targetService.ParameterStruct.TargetFields, field);
                                if (lenfield != null)
                                {
                                    lenfield.IsArrayLenField = true;
                                }
                                else
                                {
                                    if (field.MaxCountAttribute.IsFixed == false)
                                    {
                                        throw new Exception($"{field.Name}当数组不固定的时候,必须指定数组的长度字段!");
                                    }
                                }
                            }
                        }
                        targetInterface.Services.Add(targetService);
                    }
                    //Console.WriteLine(targetInterface.ToString());
                    fileIdlInfo.TargetInterfaces.Add(targetInterface);
                }
                else if (type.BaseType.Name == "MulticastDelegate")//如果这个类型是委托。
                {
                    var delms = type.GetMembers();
                    MethodInfo invokemi = null;
                    foreach (var delm in delms)
                    {
                        if (delm.Name == "Invoke")
                        {
                            invokemi = delm as MethodInfo;
                            break;
                        }
                    }
                    if (invokemi == null)
                    {
                        throw new NullReferenceException($"{type.Name}:invokemi is null");
                    }
                    var serviceIdAttr = type.GetCustomAttribute<ServiceIdAttribute>();
                    TargetDelegate targetDelegate = new TargetDelegate();
                    targetDelegate.ServiceId = serviceIdAttr == null ? ServiceId : serviceIdAttr.ServiceId;
                    ServiceId++;
                    targetDelegate.MethodName = type.Name;
                    var pars = invokemi.GetParameters();
                    UInt32 fieldNumber = 1;
                    targetDelegate.ParameterStruct.Name = targetDelegate.MethodName+ "_Parameter";
                    foreach (var par in pars)
                    {
                        TargetField field = new TargetField();
                        //field.FieldIndexAttribute = par.GetCustomAttribute<FieldIndexAttribute>();
                        field.IsArray = par.ParameterType.IsArray;
                        if (field.IsArray == true)
                        {
                            var arratt = par.GetCustomAttribute<MaxCountAttribute>();
                            field.MaxCountAttribute = arratt == null ? new MaxCountAttribute() { IsFixed = true } : arratt;
                            field.SourceCodeElementType = par.ParameterType.GetElementType().Name;
                        }
                        field.SourceCodeType = par.ParameterType.Name;
                        field.Name = par.Name;
                        field.IsBaseValueType = IsBaseValueType(par.ParameterType);
                        if (par.ParameterType.IsEnum == true)
                        {
                            field.Enum = GetTargetEnum(field.SourceCodeType, fileIdlInfo);
                        }
                        field.FieldNumberAttr = new FieldNumberAttribute(fieldNumber);
                        fieldNumber++;
                        targetDelegate.ParameterStruct.TargetFields.Add(field);
                        if (field.IsArray == true)
                        {
                            var lenfield = GetArrayLenField(targetDelegate.ParameterStruct.TargetFields, field);
                            if (lenfield != null)
                            {
                                lenfield.IsArrayLenField = true;
                            }
                            else
                            {
                                if (field.MaxCountAttribute.IsFixed == false)
                                {
                                    throw new Exception($"{field.Name}当数组不固定的时候,必须制定数组的长度字段!");
                                }
                            }
                        }
                    }
                    fileIdlInfo.TargetDelegates.Add(targetDelegate);
                    //Console.WriteLine(targetDelegate.ToString());
                }
            }

        }
    }
    public enum GenType
    {
        Client,
        Server,
        All
    }
    //FileIdlInfo fileIdlInfo, GenType genType,string outputpath
    public class CSharpCodeGenParameter
    {
        public FileIdlInfo FileIdlInfo { get; set; }
        public GenType GenType { get; set; }
        public string OutPutPath { get; set; }
        
    }
    public class CppCodeGenParameter
    {
        public FileIdlInfo FileIdlInfo { get; set; }
        public GenType GenType { get; set; }
        public string OutPutPath { get; set; }

        public bool IsRuntimeVersion { get; set; }

    }

}

