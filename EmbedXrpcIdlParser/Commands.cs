using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Security;
using System.Security.Permissions;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using CommandLine;
using RazorLight;

namespace EmbedXrpcIdlParser
{
    [Verb("cpp", HelpText = "Generate cpp code")]
    public class CommandCppGenerater
    {
        [Option('i',"InputFile",HelpText ="input idl file")]
        public string InputFile { get; set; }

        public static void Parsed(CommandCppGenerater generater)
        {
            IdlInfo idlInfo = new IdlInfo();
            idlInfo.Parse(generater.InputFile);
            CppCodeGenerater cpp = new CppCodeGenerater();
            cpp.CodeGen(idlInfo);
        }
        public static void NotParsed(IEnumerable<Error> es)
        {
            foreach (var e in es)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }

    [Verb("cs", HelpText = "Generate csharp code")]
    public class CommandCSharpGenerater
    {

        [Option('i', "InputFile", HelpText = "input idl file")]
        public string InputFile { get; set; }

        public static void Parsed(CommandCSharpGenerater generater)
        {


            //TextTemplatingFileGenerator
#if false
            if (AppDomain.CurrentDomain.IsDefaultAppDomain())
            {
                // RazorEngine cannot clean up from the default appdomain...
                Console.WriteLine("Switching to secound AppDomain, for RazorEngine...");
                AppDomainSetup adSetup = new AppDomainSetup();
                adSetup.ApplicationBase = AppDomain.CurrentDomain.SetupInformation.ApplicationBase;
                var current = AppDomain.CurrentDomain;
                // You only need to add strongnames when your appdomain is not a full trust environment.
                var strongNames = new StrongName[0];

                var domain = AppDomain.CreateDomain(
                    "MyMainDomain", null,
                    current.SetupInformation, new PermissionSet(PermissionState.Unrestricted),
                    strongNames);
                var exitCode = domain.ExecuteAssembly(Assembly.GetExecutingAssembly().Location);
                // RazorEngine will cleanup. 
                AppDomain.Unload(domain);
                return ;
            }
#endif
            IdlInfo idlInfo = new IdlInfo();
            idlInfo.Parse(generater.InputFile);
#if false
            var config = new TemplateServiceConfiguration();
            config.Debug = true;
            config.EncodedStringFactory = new RawStringFactory();
            var service = RazorEngineService.Create(config);
            Engine.Razor = service;

            var result = Engine.Razor.RunCompile(File.ReadAllText("CSharpTemplate.cshtml"), "templateKey", typeof(IdlInfo), idlInfo);
#endif
            var engine = new RazorLightEngineBuilder()
                .UseFileSystemProject(Directory.GetCurrentDirectory())
                .UseMemoryCachingProvider()
                .Build();
            var result=engine.CompileRenderStringAsync("templateKey", File.ReadAllText("CSharpTemplate.cshtml"),  idlInfo).Result;
            result =result.Replace("<h1", string.Empty);
            result = result.Replace("/>", string.Empty);
            File.WriteAllText(idlInfo.GenerationOptionParameterAttribute.OutPutPath +
                idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".gen.cs", result);

        }
        public static void NotParsed(IEnumerable<Error> es)
        {
            foreach (var e in es)
            {
                Console.WriteLine(e.ToString());
            }
        }
    }
}
/*
 * 
 */