using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TestCodeGenSpeed
{
    class Program
    {
        static void Main(string[] args)
        {
            StreamWriter streamWriter = new StreamWriter(@"D:\Project\VSProject\EmbedXrpc\EmbedXrpcIdlParser\bin\x86\Debug\TestSpeed.idl.cs", false);
            streamWriter.WriteLine("using System;");
            streamWriter.WriteLine("using EmbedXrpcIdlParser;");

            int maxStructCount = 100;
            for (int i = 0; i < maxStructCount; i++)
            {
                streamWriter.WriteLine("[FileName(\"TestSpeed.idl.cs\")]");
                streamWriter.WriteLine($"public struct TestStruct{i}");
                streamWriter.WriteLine("{");
                for (int j = 0; j < 10; j++)
                {
                    streamWriter.WriteLine($"int A{j};");
                }
                streamWriter.WriteLine("}");

                
                for (int j = 0; j < 100; j++)
                {
                    streamWriter.WriteLine("[FileName(\"TestSpeed.idl.cs\")]");
                    streamWriter.WriteLine("[Role(RoleType.Server)]");
                    streamWriter.WriteLine($"delegate TestStruct{j % maxStructCount} TestMethod{i*100+j}(TestStruct{j% maxStructCount} par);");
                }
            }
            streamWriter.WriteLine("" +
                    "[FileName(\"TestSpeed.idl.cs\")]\n" +
                    "public class OptionProcess : IOptionProcess\n" +
                    "{\n" +
                    "public GenerationOption Process()\n" +
                    "{\n" +
                    "GenerationOption option = new GenerationOption();\n" +
                    "option.OutPutFileName = \"TestSpeed\";\n" +
                    "option.UserIncludes.Add(\"MyInclude.h\");\n" +
                    "option.CSharpNameSpace = \"Sample1\";\n" +
                    "option.UserNote = \"这是一个注释!\";\n" +
                    "return option;\n" +
                    "}\n" +
                    "}\n");
            streamWriter.Close();
        }
    }
}
