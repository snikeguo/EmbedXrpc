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
    public sealed class MaxCountAttribute : Attribute
    {

        public string LenFieldName { get; set; } = string.Empty;
        public int MaxCount { get; set; } = 1;
        public bool IsFixed { get; set; } = false;

    }

    [AttributeUsage(AttributeTargets.Struct, Inherited = false, AllowMultiple = false)]
    public sealed class GenerationOptionParameterAttribute : Attribute
    {
        public string OutPutFileName { get; set; } = "idl_gen";
        public string OutPutPath { get; set; } = "";
        public GenerationOptionParameterAttribute()
        {
           
        } 
    } 


}
