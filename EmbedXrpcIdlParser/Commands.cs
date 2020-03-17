using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CommandLine;
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
}
