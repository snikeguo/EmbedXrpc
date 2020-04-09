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


namespace EmbedXrpcIdlParser
{
    public class CommandGenerater
    {
        [Option('i', "InputFile", HelpText = "input idl file")]
        public string InputFile { get; set; }

        [Option('g', "GenType", HelpText = "enum:client/server/all")]
        public string GenType { get; set; }// client /server/all

        [Option('l', "GenLanguageType", HelpText = "enum:cpp/cs")]
        public string GenLanguageType { get; set; } //  cpp/cs

        [Option('o', "OutputPath", HelpText = "Output Path")]
        public string OutputPath { get; set; }

        public static void Parsed(CommandGenerater generater)
        {
            IdlInfo idlInfo = new IdlInfo();
            idlInfo.Parse(generater.InputFile);
            GenType gt = EmbedXrpcIdlParser.GenType.All;
            if(generater.GenType.ToLower()=="client")
            {
                gt = EmbedXrpcIdlParser.GenType.Client;
            }
            else if(generater.GenType.ToLower()=="server")
            {
                gt = EmbedXrpcIdlParser.GenType.Server;
            }
            var end = generater.OutputPath[generater.OutputPath.Length - 1];
            if (end!='\\'&&end!='/')
            {
                generater.OutputPath+="\\";
                Console.WriteLine($"OutputPath:{generater.OutputPath}");
            }
            if(Directory.Exists(generater.OutputPath)==false)
            {
                Directory.CreateDirectory(generater.OutputPath);
            }
            if(generater.GenLanguageType.ToLower()=="cpp")
            {
                CppCodeGenerater cpp = new CppCodeGenerater();
                cpp.CodeGen(idlInfo,gt, generater.OutputPath);
            }
            else if (generater.GenLanguageType.ToLower() == "cs")
            {
                CsCodeGenerater cs = new CsCodeGenerater();
                cs.CodeGen(idlInfo, gt, generater.OutputPath);
            }

            Console.WriteLine("complete!");

        }
        public static void NotParsed(IEnumerable<Error> es)
        {
            Console.WriteLine("example:");
            Console.WriteLine(@".\EmbedXrpcIdlParser.exe -g all -l cpp -i idltest1.cs -o D:/Test/");
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