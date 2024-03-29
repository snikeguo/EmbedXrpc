﻿using System;
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

        [Option('l', "GenLanguageType", HelpText = "enum:cpp-ref:need a small runtime\r\n/cpp-nano:no need runtime\r\n/cs")]
        public string GenLanguageType { get; set; } //  cpp/cs

        [Option('o', "OutputPath", HelpText = "Output Path")]
        public string OutputPath { get; set; }

        [Option('e', "Encode TTLV", HelpText = "Encode TTLV")]
        public bool IsEncodeTlv { get; set; }

        public static void Parsed(CommandGenerater generater)
        {
            IdlInfo idlInfo = new IdlInfo();
            FileInfo fi = new FileInfo(generater.InputFile);
            idlInfo.Parse(fi.FullName);//这里不写generater.InputFile是因为 generater.InputFile有可能是这样的 .\\myidl.cs 而我们内部需要的是myidl.cs 所以在外面通过fileinfo做下处理
            RoleType gt = EmbedXrpcIdlParser.RoleType.All;
            if (generater.GenType.ToLower() == "client")
            {
                gt = EmbedXrpcIdlParser.RoleType.Client;
            }
            else if (generater.GenType.ToLower() == "server")
            {
                gt = EmbedXrpcIdlParser.RoleType.Server;
            }
            else if (generater.GenType.ToLower() == "all")
            {
                gt = EmbedXrpcIdlParser.RoleType.All;
            }
            else
            {
                throw new NotSupportedException("NotSupported the generater type! please use -g=client or -g=server or -g=all");
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
            if(generater.GenLanguageType.ToLower()=="cpp-ref")
            {
                throw new NotSupportedException("Not support cpp-ref!");
                /*EmbedXrpcCodeGenerater cpp = new EmbedXrpcCodeGenerater();
                for (int i = 0; i < idlInfo.ParsedFiles.Count; i++)
                {
                    CppCodeGenParameter parameter = new CppCodeGenParameter()
                    {
                        FileIdlInfo = idlInfo.ParsedFiles[i],
                        GenType = gt,
                        OutPutPath = generater.OutputPath,
                        IsEncodeTlv= generater.IsEncodeTlv,
                    };
                    cpp.CodeGen(parameter);
                }*/

            }
            else if (generater.GenLanguageType.ToLower() == "cpp-nano")
            {
                CppCodeGenerater cpp = new CppCodeGenerater();
                for (int i = 0; i < idlInfo.ParsedFiles.Count; i++)
                {
                    var fileIdlInfo = idlInfo.ParsedFiles[i];
                    var subendPath = generater.OutputPath+ fileIdlInfo.GenerationOption.SubOutPutPath;
                    var subendchar = subendPath[subendPath.Length - 1];
                    if (subendchar != '\\' && subendchar != '/')
                    {
                        subendPath += "\\";
                        Console.WriteLine($"sub OutputPath:{subendPath}");
                    }
                    if (Directory.Exists(subendPath) == false)
                    {
                        Directory.CreateDirectory(subendPath);
                    }
                    if (fi.Name != fileIdlInfo.FileName)
                        continue;
                    CppCodeGenParameter parameter = new CppCodeGenParameter()
                    {
                        FileIdlInfo = fileIdlInfo,
                        RoleType = gt,
                        OutPutPath = subendPath,
                    };
                    cpp.CodeGen(parameter);
                }

            }
            else if (generater.GenLanguageType.ToLower() == "cs")
            {
                CsCodeGenerater cs = new CsCodeGenerater();
                for (int i = 0; i < idlInfo.ParsedFiles.Count; i++)
                {
                    var fileIdlInfo = idlInfo.ParsedFiles[i];
                    var subendPath = generater.OutputPath + fileIdlInfo.GenerationOption.SubOutPutPath;
                    var subendchar = subendPath[subendPath.Length - 1];
                    if (subendchar != '\\' && subendchar != '/')
                    {
                        subendPath += "\\";
                        Console.WriteLine($"sub OutputPath:{subendPath}");
                    }
                    if (Directory.Exists(subendPath) == false)
                    {
                        Directory.CreateDirectory(subendPath);
                    }

                    CSharpCodeGenParameter parameter = new CSharpCodeGenParameter()
                    {
                        FileIdlInfo = fileIdlInfo,
                        RoleType = gt,
                        OutPutPath = subendPath,
                        //IsEnableMataDataEncode = generater.IsEnableMataDataEncode
                    };
                    if (fi.Name != fileIdlInfo.FileName)
                        continue;
                    cs.CodeGen(parameter);
                }   
            }
            else
            {
                throw new NotSupportedException("NotSupported the language type!please use -l=cpp-ref or -l=cpp-nano or -l=cs");
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