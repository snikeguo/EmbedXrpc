using System;
using System.Collections.Generic;
using System.Text;
using RazorLight;
using System.IO;
namespace EmbedXrpcIdlParser
{
    public class CsCodeGenerater : ICodeGenerater
    {
        public IdlInfo IdlInfo { get; set; }
        public GenType GenType { get; set; }
        public void CodeGen(IdlInfo idlInfo, GenType genType,string outputpath)
        {
            this.IdlInfo = idlInfo;
            this.GenType = genType;
            var engine = new RazorLightEngineBuilder()
                .UseFileSystemProject(Directory.GetCurrentDirectory())
                .UseMemoryCachingProvider()
                .Build();
            var result = engine.CompileRenderStringAsync("templateKey", File.ReadAllText("CSharpTemplate.cshtml"), this).Result;
            result = result.Replace("<h1", string.Empty);
            result = result.Replace("/>", string.Empty);
            File.WriteAllText(outputpath+
                idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".gen.cs", result);
        }
    }
}
/*System.NullReferenceException
  HResult=0x80004003
  Message=Object reference not set to an instance of an object.
  Source=ux5agk2j.x3u
  StackTrace:
   at RazorLight.CompiledTemplates.GeneratedTemplate.<ExecuteAsync>d__0.MoveNext() in templateKey:line 14

 */
