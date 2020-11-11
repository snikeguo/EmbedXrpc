
using CSScriptLibrary;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
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
    public enum TargetType_t
    {
        TYPE_UINT8,
        TYPE_INT8,

        TYPE_UINT16,
        TYPE_INT16,

        TYPE_UINT32,
        TYPE_INT32,

        TYPE_UINT64,
        TYPE_INT64,

        TYPE_FLOAT,

        TYPE_DOUBLE,

        TYPE_ENUM,

        TYPE_ARRAY,   /*array*/

        TYPE_OBJECT,  /*object*/
    }
    public interface ITargetType
    {
        TargetType_t TargetType { get;  }
        string TypeName { get; set; }
    }
    public class BaseType_TargetType:ITargetType
    {
        public TargetType_t TargetType { get; private set; }
        public string TypeName { get; set; }

        public static Dictionary<TargetType_t, string> TypeReplaceDic = new Dictionary<TargetType_t, string>();
        public BaseType_TargetType(TargetType_t tt)
        {
            TargetType = tt;
            TypeName = TypeReplaceDic[tt];

        }
        static BaseType_TargetType()
        {
            TypeReplaceDic.Add(TargetType_t.TYPE_UINT8, "UInt8");
            TypeReplaceDic.Add(TargetType_t.TYPE_INT8, "Int8");
            TypeReplaceDic.Add(TargetType_t.TYPE_UINT16, "UInt16");
            TypeReplaceDic.Add(TargetType_t.TYPE_INT16, "Int16");
            TypeReplaceDic.Add(TargetType_t.TYPE_UINT32, "UInt32");
            TypeReplaceDic.Add(TargetType_t.TYPE_INT32, "Int32");
            TypeReplaceDic.Add(TargetType_t.TYPE_UINT64, "UInt64");
            TypeReplaceDic.Add(TargetType_t.TYPE_INT64, "Int64");
            TypeReplaceDic.Add(TargetType_t.TYPE_FLOAT, "Float");
            TypeReplaceDic.Add(TargetType_t.TYPE_DOUBLE, "Double");
        }
    }
    public class EnumType_TargetType: ITargetType
    {
        //public string TypeName { get; set; }
        public TargetType_t NumberType { get; set; }

        public Dictionary< string, int> KeyValue { get; set; } = new Dictionary< string, int>();

        public TargetType_t TargetType { get; set; } = TargetType_t.TYPE_ENUM;

        public string TypeName { get; set; }

    }
    public class ArrayType_TargetType:ITargetType
    {
        public TargetType_t TargetType { get;private set; } = TargetType_t.TYPE_ARRAY;
        public string TypeName { get; set; }
        public ITargetType ElementType { get; set; }
    }

    public class ObjectType_TargetType:ITargetType
    {
        public TargetType_t TargetType { get;private set; } = TargetType_t.TYPE_OBJECT;
        public string TypeName { get; set; }
        public List<ITargetField> TargetFields { get; set; } = new List<ITargetField>();
        public Base_TargetField GetArrayLenField(Array_TargetField arrayField)
        {
            foreach (var f in TargetFields)
            {
                if (f.FieldName == arrayField.MaxCountAttribute.LenFieldName)
                {
                    var v= f as Base_TargetField;
                    v.IsArrayLenField = true;//更新字段Base_TargetField的IsArrayLenField标志量
                    return v;
                }
            }
            return null;
        }
    }

    public interface ITargetField
    {
        ITargetType TargetType { get; set; }
        string FieldName { get; set; }
        FieldNumberAttribute FieldNumberAttr { get; set; }
    }

    public class Base_TargetField : ITargetField
    {
        public ITargetType TargetType { get; set; }

        public string FieldName { get; set; }

        public FieldNumberAttribute FieldNumberAttr { get; set; }

        public bool IsArrayLenField { get; set; } = false;//是否是数组长度字段
        
    }
    public class Enum_TargetField: Base_TargetField
    {
               
    }

    public class Array_TargetField : ITargetField
    {
        public ITargetType TargetType { get; set; }
        public FieldNumberAttribute FieldNumberAttr { get; set; }

        public string FieldName { get; set; }

        public MaxCountAttribute MaxCountAttribute { get; set; }

        public Base_TargetField ArrayLenField { get; set; }
    }
    public class Object_TargetField : ITargetField
    {
        public ITargetType TargetType { get; set; }

        public string FieldName { get; set; }

        public FieldNumberAttribute FieldNumberAttr { get; set; }
    }

    [Serializable]
    public class TargetService
    {
        //public TargetField ReturnValue { get; set; }
        public ObjectType_TargetType ReturnStructType { get; set; } = new ObjectType_TargetType();
        public string ServiceName { get; set; }

        public string FullName { get; set; }
        //public List<TargetField> TargetFields { get; set; } = new List<TargetField>();//参数
        public ObjectType_TargetType ParameterStructType { get; set; } = new ObjectType_TargetType();
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
        public ObjectType_TargetType ParameterStructType { get; set; } = new ObjectType_TargetType();
        
    }
    /// <summary>
    /// 本文件中所有的资源
    /// </summary>
    public class FileIdlInfo
    {
        public string FileName { get; set; } = string.Empty;
        public List<EnumType_TargetType> TargetEnums { get; set; } = new List<EnumType_TargetType>();
        public List<ArrayType_TargetType> TargetArrayTypes { get; set; } = new List<ArrayType_TargetType>();
        public List<ObjectType_TargetType> TargetStructs { get; set; } = new List<ObjectType_TargetType>();
        public List<TargetInterface> TargetInterfaces { get; set; } = new List<TargetInterface>();
        public List<TargetDelegate> TargetDelegates { get; set; } = new List<TargetDelegate>();
 
        public GenerationOption GenerationOption = null;

        public EnumType_TargetType GetTargetEnum(string name)
        {
            foreach (var e in TargetEnums)
            {
                if (name == e.TypeName)
                {
                    return e;
                }
            }
            return null;
        }
        public ArrayType_TargetType GetTargetArray(string typeName)
        {
            foreach (var e in TargetArrayTypes)
            {
                if (typeName == e.TypeName)
                {
                    return e;
                }
            }
            return null;
        }
        public ObjectType_TargetType GetTargetStruct(string typeName)
        {
            foreach (var e in TargetStructs)
            {
                if (typeName == e.TypeName)
                {
                    return e;
                }
            }
            return null;
        }
    }
    public class IdlInfo
    {          
        private TargetType_t ClrBaseValueTypeToTargetType_t(Type t)
        {
            if (t == typeof(bool))
            {
                return TargetType_t.TYPE_UINT8;
            }
            if (t == typeof(byte))
            {
                return TargetType_t.TYPE_UINT8;                
            }
            if (t == typeof(sbyte))
            {
                return TargetType_t.TYPE_INT8;
                
            }
            if (t == typeof(UInt16))
            {
                return TargetType_t.TYPE_UINT16;
  
            }
            if (t == typeof(Int16))
            {
                return TargetType_t.TYPE_INT16;

            }
            if (t == typeof(UInt32))
            {
                return TargetType_t.TYPE_UINT32;

            }
            if (t == typeof(Int32))
            {
                return TargetType_t.TYPE_INT32;

            }
            if (t == typeof(UInt64))
            {
                return TargetType_t.TYPE_UINT64;

            }
            if (t == typeof(Int64))
            {
                return TargetType_t.TYPE_INT64;

            }
            throw new Exception();
        }
        private  bool IsNumberType(Type t)
        {
            bool r = false;
            if(t==typeof(bool))
            {
                return true;
            }
            if (t==typeof(byte))
            {
                
                r = true;
            }
            if (t == typeof(sbyte))
            {
               
                r = true;
            }
            if (t == typeof(UInt16))
            {
                
                r = true;
            }
            if (t == typeof(Int16))
            {
               
                r = true;
            }
            if (t == typeof(UInt32))
            {
                
                r = true;
            }
            if (t == typeof(Int32))
            {
                
                r = true;
            }
            if (t == typeof(UInt64))
            {
                
                r = true;
            }
            if (t == typeof(Int64))
            {
                
                r = true;
            }
            return r;
        }

        public bool IsEnumType(Type t)
        {
            if(t.IsEnum==true)
            {
                return true;
            }
            return false;
        }
        public List< FileIdlInfo> ParsedFiles { get; private set; } = new List< FileIdlInfo>();//已经被Parse过的file

        private void StructTypeParse(Type object_type, FileIdlInfo fileIdlInfo)
        {
            ObjectType_TargetType targetStruct = new ObjectType_TargetType();
            targetStruct.TypeName = object_type.Name;
            var fields = object_type.GetFields(BindingFlags.NonPublic | BindingFlags.Instance);
            foreach (var field in fields)
            {
                var FieldNumberAttr = field.GetCustomAttribute<FieldNumberAttribute>();
                if (FieldNumberAttr == null)
                {
                    FieldNumberAttr = new FieldNumberAttribute(0);//如果没有fieldNumber说明用户并不打算使用TTLV编码
                                                                  //throw new NotImplementedException($"the {field.Name}'s FieldNumberAttr is null");
                }
                if (IsNumberType(field.FieldType) == true)
                {
                    Base_TargetField targetfield = new Base_TargetField();
                    targetfield.TargetType = new BaseType_TargetType(ClrBaseValueTypeToTargetType_t(field.FieldType));
                    targetfield.FieldName = field.Name;
                    targetfield.FieldNumberAttr = FieldNumberAttr;
                    targetStruct.TargetFields.Add(targetfield);
                }
                else if (IsEnumType(field.FieldType) == true)
                {
                    Enum_TargetField enumField = new Enum_TargetField();
                    enumField.FieldName = field.Name;
                    enumField.FieldNumberAttr = FieldNumberAttr;
                    enumField.TargetType = fileIdlInfo.GetTargetEnum(field.FieldType.Name);
                    targetStruct.TargetFields.Add(enumField);

                }
                else if (field.FieldType.IsArray == true)
                {
                    ArrayType_TargetType attt = fileIdlInfo.GetTargetArray(field.FieldType.Name);
                    if (attt == null)
                    {
                        attt = new ArrayType_TargetType();
                        attt.TypeName = field.FieldType.Name;
                        if(IsNumberType(field.FieldType.GetElementType()) == true)
                        {
                            attt.ElementType = new BaseType_TargetType(ClrBaseValueTypeToTargetType_t(field.FieldType.GetElementType()));
                        }
                        else
                        {
                            ObjectType_TargetType ottt = fileIdlInfo.GetTargetStruct(field.FieldType.GetElementType().Name);
                            if (ottt == null)
                            {
                                StructTypeParse(field.FieldType.GetElementType(), fileIdlInfo);//如果list中没有targetstruct 那就去parse
                            }
                            ottt = fileIdlInfo.GetTargetStruct(field.FieldType.GetElementType().Name);
                            attt.ElementType = ottt;
                        }
                        fileIdlInfo.TargetArrayTypes.Add(attt);
                    }
                    Array_TargetField arrayField = new Array_TargetField();
                    arrayField.TargetType = attt;
                    arrayField.FieldNumberAttr = FieldNumberAttr;
                    arrayField.FieldName = field.Name;
                    var arratt = field.GetCustomAttribute<MaxCountAttribute>();
                    arrayField.MaxCountAttribute = arratt == null ? new MaxCountAttribute() { IsFixed = true, MaxCount = 1, LenFieldName = "" } : arratt;
                    arrayField.ArrayLenField = targetStruct.GetArrayLenField(arrayField);
                    targetStruct.TargetFields.Add(arrayField);
                }
                else if (!field.FieldType.IsPrimitive && !field.FieldType.IsEnum && field.FieldType.IsValueType)//struct
                {
                    ObjectType_TargetType ottt = fileIdlInfo.GetTargetStruct(field.FieldType.Name);
                    if (ottt == null)
                    {
                        StructTypeParse(field.FieldType, fileIdlInfo);//如果list中没有targetstruct 那就去parse
                    }
                    ottt = fileIdlInfo.GetTargetStruct(field.FieldType.Name);
                    Object_TargetField objectFiled = new Object_TargetField();
                    objectFiled.TargetType = ottt;
                    objectFiled.FieldName = field.Name;
                    objectFiled.FieldNumberAttr = FieldNumberAttr;
                    targetStruct.TargetFields.Add(objectFiled);
                }
                else
                {
                    throw new NotSupportedException();
                }
            }
            fileIdlInfo.TargetStructs.Add(targetStruct);
        }
        private void ParameterTypeParse(ParameterInfo[]parameters,ObjectType_TargetType targetStruct, FileIdlInfo fileIdlInfo)
        {
            uint fieldNumber = 1;
            foreach (var parameter in parameters)
            {
                var FieldNumberAttr = new FieldNumberAttribute(fieldNumber);//如果没有fieldNumber说明用户并不打算使用TTLV编码
                                                                            //throw new NotImplementedException($"the {field.Name}'s FieldNumberAttr is null");
                fieldNumber++;
                if (IsNumberType(parameter.ParameterType) == true)
                {
                    Base_TargetField targetfield = new Base_TargetField();
                    targetfield.TargetType = new BaseType_TargetType(ClrBaseValueTypeToTargetType_t(parameter.ParameterType));
                    targetfield.FieldName = parameter.Name;
                    targetfield.FieldNumberAttr = FieldNumberAttr;
                    targetStruct.TargetFields.Add(targetfield);
                }
                else if (IsEnumType(parameter.ParameterType) == true)
                {
                    Enum_TargetField enumField = new Enum_TargetField();
                    enumField.FieldName = parameter.Name;
                    enumField.FieldNumberAttr = FieldNumberAttr;
                    enumField.TargetType = fileIdlInfo.GetTargetEnum(parameter.ParameterType.Name);
                    targetStruct.TargetFields.Add(enumField);

                }
                else if (parameter.ParameterType.IsArray == true)
                {
                    ArrayType_TargetType attt = fileIdlInfo.GetTargetArray(parameter.ParameterType.Name);
                    if (attt == null)
                    {
                        attt = new ArrayType_TargetType();
                        attt.TypeName = parameter.ParameterType.Name;
                        if (IsNumberType(parameter.ParameterType.GetElementType()) == true)
                        {
                            attt.ElementType = new BaseType_TargetType(ClrBaseValueTypeToTargetType_t(parameter.ParameterType.GetElementType()));
                        }
                        else
                        {
                            ObjectType_TargetType ottt = fileIdlInfo.GetTargetStruct(parameter.ParameterType.GetElementType().Name);
                            if (ottt == null)
                            {
                                StructTypeParse(parameter.ParameterType.GetElementType(), fileIdlInfo);//如果list中没有targetstruct 那就去parse
                            }
                            ottt = fileIdlInfo.GetTargetStruct(parameter.ParameterType.GetElementType().Name);
                            attt.ElementType = ottt;
                        }
                    }
                    Array_TargetField arrayField = new Array_TargetField();
                    arrayField.TargetType = attt;
                    arrayField.FieldNumberAttr = FieldNumberAttr;
                    arrayField.FieldName = parameter.Name;
                    var arratt = parameter.GetCustomAttribute<MaxCountAttribute>();
                    arrayField.MaxCountAttribute = arratt == null ? new MaxCountAttribute() { IsFixed = true, MaxCount = 1, LenFieldName = "" } : arratt;
                    arrayField.ArrayLenField = targetStruct.GetArrayLenField(arrayField);
                    targetStruct.TargetFields.Add(arrayField);
                }
                else if (!parameter.ParameterType.IsPrimitive && !parameter.ParameterType.IsEnum && parameter.ParameterType.IsValueType)//struct
                {
                    ObjectType_TargetType ottt = fileIdlInfo.GetTargetStruct(parameter.ParameterType.Name);
                    if (ottt == null)
                    {
                        StructTypeParse(parameter.ParameterType, fileIdlInfo);//如果list中没有targetstruct 那就去parse
                    }
                    ottt = fileIdlInfo.GetTargetStruct(parameter.ParameterType.Name);
                    Object_TargetField objectFiled = new Object_TargetField();
                    objectFiled.TargetType = ottt;
                    objectFiled.FieldName = parameter.Name;
                    objectFiled.FieldNumberAttr = FieldNumberAttr;
                    targetStruct.TargetFields.Add(objectFiled);
                }
                else
                {
                    throw new NotSupportedException();
                }
            }
            
        }
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
                        EnumType_TargetType te = new EnumType_TargetType();
                        te.TypeName = type.Name;//类型名称
                        te.NumberType = ClrBaseValueTypeToTargetType_t(type.GetEnumUnderlyingType());
                        foreach (var vsv in vs)
                        {
                            te.KeyValue.Add( type.GetEnumName(vsv), Convert.ToInt32(vsv));
                        }
                        fileIdlInfo.TargetEnums.Add(te);
                        //Console.WriteLine(te.ToString());
                    }
                    else
                    {
                        StructTypeParse(type, fileIdlInfo);
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

                        ObjectType_TargetType returnStructType = new ObjectType_TargetType();
                        returnStructType.TypeName= type.Name + "_" + targetService.ServiceName + "_Return";

                        Enum_TargetField RequestResponseStatefield = new Enum_TargetField();
                        var ettt= new EnumType_TargetType();
                        ettt.TypeName = "RequestResponseState";
                        ettt.NumberType = TargetType_t.TYPE_UINT8;
                        //ettt.KeyValue这里在runtime中定义,不需要加
                        RequestResponseStatefield.TargetType = ettt;
                        RequestResponseStatefield.FieldName = "State";
                        RequestResponseStatefield.FieldNumberAttr = new FieldNumberAttribute(1);//state 的Field Number为1
                        returnStructType.TargetFields.Add(RequestResponseStatefield);

                        if (rt.Name != "Void")
                        {
                            //返回值有可能是枚举、基本数据、结构体
                            //根据返回值判断
                            EnumType_TargetType rt_ettt = fileIdlInfo.GetTargetEnum(rt.Name);
                            ObjectType_TargetType rt_ottt = fileIdlInfo.GetTargetStruct(rt.Name);
                            if(rt_ettt!=null)
                            {
                                //说明返回值是枚举
                                Enum_TargetField retunValueFiled = new Enum_TargetField();
                                retunValueFiled.TargetType = rt_ettt;
                                retunValueFiled.FieldName = "ReturnValue";
                                retunValueFiled.FieldNumberAttr = new FieldNumberAttribute(2);//return Value 的Field Number为2
                                returnStructType.TargetFields.Add(retunValueFiled);
                            }
                            else if(IsNumberType(rt)==true)
                            {
                                Base_TargetField retunValueFiled = new Base_TargetField();
                                retunValueFiled.TargetType = new BaseType_TargetType(ClrBaseValueTypeToTargetType_t(rt));
                                retunValueFiled.FieldName = "ReturnValue";
                                retunValueFiled.FieldNumberAttr = new FieldNumberAttribute(2);//return Value 的Field Number为2
                                returnStructType.TargetFields.Add(retunValueFiled);
                            }
                            else if(rt_ottt!=null)
                            {
                                Object_TargetField objectFiled = new Object_TargetField();
                                objectFiled.TargetType = rt_ottt;
                                objectFiled.FieldName = "ReturnValue";
                                objectFiled.FieldNumberAttr = new FieldNumberAttribute(2);//return Value 的Field Number为2
                                returnStructType.TargetFields.Add(objectFiled);
                            }
                        }

                        //处理函数参数
                        ObjectType_TargetType ParameterStructType = new ObjectType_TargetType();
                        ParameterStructType.TypeName = type.Name + "_" + targetService.ServiceName + "_Parameter";
                        var pars = mt.GetParameters();
                        ParameterTypeParse(pars, ParameterStructType, fileIdlInfo);
                        targetService.ReturnStructType = returnStructType;
                        targetService.ParameterStructType = ParameterStructType;
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

                    //处理函数参数
                    ObjectType_TargetType ParameterStructType = new ObjectType_TargetType();
                    ParameterStructType.TypeName = targetDelegate.MethodName + "_Parameter";

                    var pars = invokemi.GetParameters();
                    ParameterTypeParse(pars, ParameterStructType, fileIdlInfo);
                    targetDelegate.ParameterStructType = ParameterStructType;
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

