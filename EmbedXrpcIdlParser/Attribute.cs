using System;
using System.Collections.Generic;
using System.Text;

namespace EmbedXrpcIdlParser
{
#if false
    [AttributeUsage(AttributeTargets.Field|AttributeTargets.Parameter, Inherited = false, AllowMultiple = true)]
    public sealed class FieldIndexAttribute : Attribute
    {
        public FieldIndexAttribute(int index)
        {
            Index = index;
        }
        // This is a named argument
        public int Index { get; set; }
    }
#endif   
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
    [AttributeUsage(AttributeTargets.Struct, Inherited = false, AllowMultiple = true)]
    public sealed class BitsAttribute : Attribute
    {
        public BitsType BitsType { get; set; }
        public BitsAttribute(BitsType ut)
        {
            BitsType = ut;
        }
    }
    [AttributeUsage(AttributeTargets.Field, Inherited = false, AllowMultiple = true)]
    public sealed class BitsFieldLengthAttribute : Attribute
    {
        public int Length { get; set; }
        public BitsFieldLengthAttribute(int len)
        {
            Length = len;
        }
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
    [AttributeUsage(AttributeTargets.Method|AttributeTargets.Delegate, Inherited = false, AllowMultiple = true)]
    public class ServiceIdAttribute : Attribute
    {
        public ServiceIdAttribute(int serviceId)
        {
            this.ServiceId = serviceId;
        }
        public int ServiceId { get; set; }
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
        TYPE_OBJECT,  /*object*/
    };
    [AttributeUsage(AttributeTargets.Field, Inherited = false, AllowMultiple = true)]
    public sealed class FieldNumberAttribute : Attribute
    { 
        public FieldNumberAttribute(int number)
        {
            Number = number;
        }

        // This is a named argument
        public int Number { get; set; }
    }


}
