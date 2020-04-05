using CommandLine;
using System;
using System.Collections.Generic;
using System.IO;
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
            //IdlInfo idlInfo = new IdlInfo();
            //idlInfo.Parse("idltest1.cs");
            /*CppCodeGenerater cpp = new CppCodeGenerater();
            cpp.CodeGen(idlInfo);*/
            CommandCSharpGenerater cs = new CommandCSharpGenerater();
            cs.InputFile = "idltest1.cs";
            CommandCSharpGenerater.Parsed(cs);
            Console.WriteLine("complete!");
            Console.ReadLine();
#else
            Console.WriteLine("example:");
            Console.WriteLine(@".\EmbedXrpcIdlParser.exe cpp  -i idltest1.cs");
            Console.WriteLine(@".\EmbedXrpcIdlParser.exe cs  -i idltest1.cs");
            try
            {
                /*var exitCode = Parser.Default.ParseArguments<CommandCppGenerater>(args)
                    .WithParsed(CommandCppGenerater.Parsed)
                    .WithNotParsed(CommandCppGenerater.NotParsed);*/

                var exitCode = Parser.Default.ParseArguments<CommandCSharpGenerater>(args)
                    .WithParsed(CommandCSharpGenerater.Parsed)
                    .WithNotParsed(CommandCSharpGenerater.NotParsed);
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
