using System;
using System.Collections.Generic;
using System.Text;
using RazorEngine;
using RazorEngine.Templating; // For extension methods.
using System.IO;
using RazorEngine.Configuration;
using RazorEngine.Text;

namespace EmbedXrpcIdlParser
{
    public class CsCodeGenerater : ICodeGenerater
    {
        public CodeGenParameter CodeGenParameter { get; set; }
        public void CodeGen(CodeGenParameter codeGenParameter)
        {
            CodeGenParameter = codeGenParameter;
#if false
            var engine = new RazorLightEngineBuilder()
                .UseFileSystemProject(Directory.GetCurrentDirectory())
                .UseMemoryCachingProvider()
                .Build();
            var result = engine.CompileRenderStringAsync("templateKey", File.ReadAllText("CSharpTemplate.cshtml"), this).Result;
#endif
            foreach (var str in codeGenParameter.FileIdlInfo.TargetStructs)
            {
                if(str.BitsAttribute!=null)
                {
                    throw new Exception("生成C#代码不支持位域! 位域功能仅C++支持!");
                }
            }
            var config = new TemplateServiceConfiguration();
            config.EncodedStringFactory = new RawStringFactory(); // Raw string encoding.
            var service = RazorEngineService.Create(config);
            Engine.Razor = service;
            var result = Engine.Razor.RunCompile(File.ReadAllText(AppContext.BaseDirectory+"/CSharpTemplate.cshtml"), "templateKey", GetType(), this);

            result = result.Replace("<h1", string.Empty);
            result = result.Replace("/>", string.Empty);
            File.WriteAllText(codeGenParameter.OutPutPath+
                codeGenParameter.FileIdlInfo.GenerationOption.OutPutFileName + ".gen.cs", result);
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
