using System;
using System.Collections.Generic;
using System.Text;

namespace EmbedXrpcIdlParser
{
 
    [AttributeUsage(AttributeTargets.Field|AttributeTargets.ReturnValue | AttributeTargets.Parameter, Inherited = false, AllowMultiple = true)]
    [Serializable]
    public sealed class MaxCountAttribute : Attribute
    {

        public string LenFieldName { get; set; } = string.Empty;
        public int MaxCount { get; set; } = 1;
        public bool IsFixed { get; set; } = false;

    }
    public enum BitsType
    {
        NoBits,
        Byte,
        SByte,
        UInt16,
        Int16,
        UInt32,
        Int32,
        UInt64,
        Int64,
    }
    public interface IOptionProcess
    {
        GenerationOption Process();
    }
    [AttributeUsage(AttributeTargets.All, Inherited = false, AllowMultiple = true)]
    public class FileNameAttribute : Attribute
    {
        public FileNameAttribute(string fn)
        {
            FileNameString = fn;
        }
        public string FileNameString
        {
            get;set;
        }

    }
    [AttributeUsage(AttributeTargets.Delegate, Inherited = false, AllowMultiple = true)]
    public class ServiceIdAttribute : Attribute
    {
        public ServiceIdAttribute(int serviceId)
        {
            this.ServiceId = serviceId;
        }
        public int ServiceId { get; set; }
    }
    [AttributeUsage(AttributeTargets.Delegate, Inherited = false, AllowMultiple = true)]
    public class NoDeserializeParameterAttribute : Attribute
    {
        public NoDeserializeParameterAttribute()
        {
        }
    }
    [AttributeUsage(AttributeTargets.Delegate, Inherited = false, AllowMultiple = true)]
    public sealed class ExternalParameterAttribute : Attribute
    {
        public ExternalParameterAttribute(bool isExternal)
        {
            IsExternal = isExternal;
        }
        public bool IsExternal { get; set; }
    }
    public class GenerationOption
    {
        /// <summary>
        /// 输出的文件名
        /// </summary>
        public string OutPutFileName { get; set; } = "idl_gen";
        /// <summary>
        /// 生成的命名空间(C#)
        /// </summary>
        public string CSharpNameSpace { get; set; } = "idl_gen";
        /// <summary>
        /// 生成CPP时用这个
        /// </summary>
        public List<string> UserIncludes { get; set; } = new List<string>();
        /// <summary>
        ///生成C#时用这个
        /// </summary>
        public List<string> UserNamespace { get; set; } = new List<string>();
        /// <summary>
        /// 用户注释
        /// </summary>
        public string UserNote { get; set; }

        public int ServiceIdStartNumber { get; set; } = 0x10;

        public string SubOutPutPath { get; set; } = string.Empty;

    }
    public enum DataType
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

        TYPE_ARRAY,   /*array*/
        TYPE_STRUCT,  /*object*/
        TYPE_UNION,
    };

    [AttributeUsage(AttributeTargets.Field, Inherited = false, AllowMultiple = true)]
    public class UnionTargetTypeAttribute : Attribute
    {

    }

    public enum UnionIndex
    {
        Mid,
        Start,
        End,
    }

    [AttributeUsage(AttributeTargets.Field, Inherited = false, AllowMultiple = true)]
    public class UnionFieldAttribute : Attribute
    {
        public UnionIndex UnionIndex { get; set; }
    }
    [AttributeUsage(AttributeTargets.Field, Inherited = false, AllowMultiple = true)]
    public sealed class FieldNumberAttribute : Attribute
    { 
        public FieldNumberAttribute(Int32 number)
        {
            Number = number;
        }

        // This is a named argument
        public Int32 Number { get; set; }
    }

    [AttributeUsage(AttributeTargets.Delegate| AttributeTargets.Field, Inherited = false, AllowMultiple = true)]
    public class MacroControlAttribute : Attribute
    {

        public MacroControlAttribute(string macroName):this(macroName, string.Empty)
        {

        }
        public MacroControlAttribute(string macroName,string enableCondition)
        {
            MacroName = macroName;
            EnableCondition = enableCondition;
        }

        public string MacroName { get; set; }
        public string EnableCondition { get; set; }

    }
    [AttributeUsage(AttributeTargets.Field, Inherited = false, AllowMultiple = true)]
    public class NoSerializationAttribute : Attribute
    {
        public NoSerializationAttribute()
        {

        }
        public NoSerializationAttribute(string field,string cmpValue)
        {
            FieldName = field;
            CompareValue = cmpValue;
        }
        public string FieldName { get; private set; } = string.Empty;
        public string CompareValue { get; private set; }
    }

    [AttributeUsage(AttributeTargets.Field, Inherited = false, AllowMultiple = true)]
    public class BitFieldAttribute : Attribute
    {
        public BitFieldAttribute(int bitWidthLength)
        {
            BitWidthLength = bitWidthLength;
        }
        public int BitWidthLength { get; private set; }
    }

    [AttributeUsage(AttributeTargets.Struct, Inherited = false, AllowMultiple = true)]
    public class CppCustomMethodSignatureAttribute : Attribute
    {
        
        readonly string signature;

        public CppCustomMethodSignatureAttribute(string sign)
        {
            signature = sign;
        }

        public string Signature
        {
            get { return signature; }
        }

    }

    public enum RoleType
    {
        Client,
        Server,
        All
    }

    [AttributeUsage(AttributeTargets.Struct, Inherited = false, AllowMultiple = true)]
    public class AlignAttribute : Attribute
    {
        
        public AlignAttribute(int v)
        {
            Value = v;   
        }
        public int Value { get; set; }
    }

    [AttributeUsage(AttributeTargets.Struct, Inherited = false, AllowMultiple = true)]
    public class ConstValueAttribute : Attribute
    {
        
    }

#if false
    [AttributeUsage(AttributeTargets.Delegate, Inherited = false, AllowMultiple = true)]
    public class RoleAttribute : Attribute
    {

        public RoleAttribute(RoleType role)
        {
            Role = role;
        }

        public RoleType Role
        {
            get;
            private set;
        }     
    }
#endif
}
