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

    } 


}
