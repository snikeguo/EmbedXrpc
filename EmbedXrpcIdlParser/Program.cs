﻿using CommandLine;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{

    class Program
    {
        static void Main(string[] args)
        {
#if true
            IdlInfo idlInfo = new IdlInfo();
            idlInfo.Parse("idltest1.cs");
            CppCodeGenerater cpp = new CppCodeGenerater();
            cpp.CodeGen(idlInfo);
            Console.ReadLine();
#else
            Console.WriteLine("example:");
            Console.WriteLine(@".\EmbedXrpcIdlParser.exe cpp  -i idltest1.cs");
            Console.WriteLine("\n\n\n");
            try
            {
                var exitCode = Parser.Default.ParseArguments<CommandCppGenerater>(args).WithParsed(CommandCppGenerater.Parsed).WithNotParsed(CommandCppGenerater.NotParsed);
            }
            catch (Exception e)
            {

                Console.WriteLine(e.ToString());
            }
            //Console.ReadLine();
#endif
        }
    }
}
