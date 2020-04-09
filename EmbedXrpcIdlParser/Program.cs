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
#if false
#if cpp
            args = new string[9];
            args[0] = "EmbedXrpcIdlParser.exe";
            args[1] = "-g";
            args[2] = "all";
            args[3] = "-l";
            args[4] = "cpp";
            args[5] = "-i";
            args[6] = "idltest1.cs";
            args[7] = "-o";
            args[8] = @"D:\VSProject\EmbedXrpcIdlParser\TestEmbeXrpc\";
#else
            args = new string[9];
            args[0] = "EmbedXrpcIdlParser.exe";
            args[1] = "-g";
            args[2] = "all";
            args[3] = "-l";
            args[4] = "cs";
            args[5] = "-i";
            args[6] = "idltest1.cs";
            args[7] = "-o";
            args[8] = @"D:\VSProject\EmbedXrpcIdlParser\EmbedXrpc.CSharp.Test\";
#endif
#endif
            try
            {
                /*var exitCode = Parser.Default.ParseArguments<CommandCppGenerater>(args)
                    .WithParsed(CommandCppGenerater.Parsed)
                    .WithNotParsed(CommandCppGenerater.NotParsed);*/

                var exitCode = Parser.Default.ParseArguments<CommandGenerater>(args)
                    .WithParsed(CommandGenerater.Parsed)
                    .WithNotParsed(CommandGenerater.NotParsed);
            }
            catch (Exception e)
            {

                Console.WriteLine(e.ToString());
            }
        }
    }
}
