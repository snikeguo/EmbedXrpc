using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public class CppCodeGenerater : ICodeGenerater
    {
        private static Dictionary<string, string> ReplaceDic = new Dictionary<string, string>();
        
        public void CodeGen(IdlInfo idlInfo)
        {
            Console.WriteLine("cpp code gen...");
            cppStreamWriter = new StreamWriter(idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".h", false, Encoding.UTF8);
            cppStreamWriter.WriteLine("#include\"EmbedXrpcBaseType.h\"");

            //fbsStreamWriter = new StreamWriter(idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".fbs", false, Encoding.ASCII);
            //fbsStreamWriter.WriteLine("namespace EmbedXrpc;");

            

            EmbedXrpcSerializationCFileStreamWriter = new StreamWriter(idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".EmbedXrpcSerialization.cpp", false, Encoding.UTF8);

            EmbedXrpcSerializationCFileStreamWriter.WriteLine($"#include\"{idlInfo.GenerationOptionParameterAttribute.OutPutFileName }.h\"");
            EmbedXrpcSerializationCFileStreamWriter.WriteLine($"#include\"{idlInfo.GenerationOptionParameterAttribute.OutPutFileName}.EmbedXrpcSerialization.h\"");


            EmbedXrpcSerializationHFileStreamWriter = new StreamWriter(idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".EmbedXrpcSerialization.h", false, Encoding.UTF8);
            EmbedXrpcSerializationHFileStreamWriter.WriteLine($"#ifndef {idlInfo.GenerationOptionParameterAttribute.OutPutFileName}_EmbedXrpcSerialization_H");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine($"#define {idlInfo.GenerationOptionParameterAttribute.OutPutFileName}_EmbedXrpcSerialization_H");

            EmbedXrpcSerializationHFileStreamWriter.WriteLine("//EmbedXrpc Code gen .By snikeguo.e-mail:snikeguo@foxmail.com");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("//关于snikeguo作者:92年生人,热爱技术,底层功底扎实.深入理解C语言底层到汇编层，单片机从外设/裸机到OS通吃");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("//上位机专注于C# 界面库熟悉WPF.服务器专注于dotnet core");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("//行业经验：汽车电子.独自完成UDS(包括FBL)协议栈  GBT27930-2015(SAE J1939)协议栈等");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("//熟悉BMS业务逻辑,有一套自己稳定出货的BMS软件/硬件/上位机/服务器/USB转CAN&CANFD");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("//TITLE:高级嵌入式软件工程师.软件架构师");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("//微信:snikeguo.有好的职位推荐请加");

            //EmbedXrpcSerializationHFileStreamWriter.WriteLine("#include\"EmbedXrpcSerialization.h\"");
            //D:\VSProject\EmbedXrpcIdlParser\EmbedXrpcSerialization
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("#include\"EmbedXrpcSerialization.h\"");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("#ifndef offsetof");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))");
            EmbedXrpcSerializationHFileStreamWriter.WriteLine("#endif");
            foreach (var em in idlInfo.TargetEnums)
            {
                //EmitFbsEnum(em);
                //fbsStreamWriter.WriteLine(em.ToFbs().ToString());
                EmitEnum(em);
            }
            foreach (var stru in idlInfo.TargetStructs)
            {
                //EmitFbsTable(stru);
                //fbsStreamWriter.WriteLine(stru.ToFbs().ToString());
                stru.ToXrpcSerializationCode(EmbedXrpcSerializationCFileStreamWriter, EmbedXrpcSerializationHFileStreamWriter);
                EmitStruct(stru);
            }

            foreach (var del in idlInfo.TargetDelegates)
            {
                //fbsStreamWriter.WriteLine(del.ToFbs().ToString());
                EmitDelegate(del);
            }
#if false
            foreach (var ifs in idlInfo.TargetInterfaces)
            {
                //fbsStreamWriter.WriteLine(ifs.ToFbs().ToString());
                EmitClientInterface(ifs);
            }
#endif
            //fbsStreamWriter.WriteLine(FbsHelper.EmitPackageTable().ToString());

            cppStreamWriter.Flush();
            cppStreamWriter.Close();

            //fbsStreamWriter.Flush();
            //fbsStreamWriter.Close();
            //FbsHelper.GenerateSerializationCode(idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".fbs");

            EmbedXrpcSerializationCFileStreamWriter.Flush();
            EmbedXrpcSerializationCFileStreamWriter.Close();

            

            EmbedXrpcSerializationHFileStreamWriter.WriteLine("\n#endif");

            EmbedXrpcSerializationHFileStreamWriter.Flush();
            EmbedXrpcSerializationHFileStreamWriter.Close();
        }
        private StreamWriter cppStreamWriter;
        //private StreamWriter fbsStreamWriter;
        private StreamWriter EmbedXrpcSerializationCFileStreamWriter;
        private StreamWriter EmbedXrpcSerializationHFileStreamWriter;
        public static string IdlType2CppType(TargetField field)
        {
            string cppType = field.IdlType;

            if (field.IsArray == true)
            {
                cppType = field.IdlType.Replace("[", "");
                cppType = cppType.Replace("]", "");
                if (field.MaxCountAttribute.IsFixed == false)
                {
                    cppType = cppType + "*";
                }
            }
            return cppType;
        }

        public static string GetIdlTypeArrayElementType(TargetField field)
        {
            string cppType = field.IdlType;

            if (field.IsArray == true)
            {
                cppType = field.IdlType.Replace("[", "");
                cppType = cppType.Replace("]", "");
            }
            return cppType;
        }

        public void EmitEnum(TargetEnum targetEnum)
        {
            cppStreamWriter.WriteLine("typedef enum _" + targetEnum.Name);
            cppStreamWriter.WriteLine("{");
            foreach (var ev in targetEnum.TargetEnumValues)
            {
                cppStreamWriter.WriteLine(ev.Description + " = " + ev.Value.ToString() + ",");
            }
            cppStreamWriter.WriteLine("}" + targetEnum.Name + ";");
        }
        public void EmitStruct(TargetStruct targetStruct)
        {
            cppStreamWriter.WriteLine("typedef struct _" + targetStruct.Name);
            cppStreamWriter.WriteLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                string name = field.Name;
                string cppType = IdlType2CppType(field);

                if (field.IsArray == true && field.MaxCountAttribute.IsFixed==true)
                {
                    name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                }

                cppStreamWriter.WriteLine(cppType + " " + name + ";");

            }
            cppStreamWriter.WriteLine("}" + targetStruct.Name + ";");
        }

        public void EmitDelegate(TargetDelegate targetDelegate)
        {
            cppStreamWriter.Write("typedef void (*" + targetDelegate.MethodName + ")(");
            for (int i = 0; i < targetDelegate.TargetFields.Count; i++)
            {
                var field = targetDelegate.TargetFields[i];
                string cppType = IdlType2CppType(field);
                string name = field.Name;
                if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                {
                    name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                }

                cppStreamWriter.Write(cppType + " " + name);
                if (i + 1 < targetDelegate.TargetFields.Count)
                {
                    cppStreamWriter.Write(",");
                }
            }
            cppStreamWriter.WriteLine(");");

        }
        public void EmitClientInterface(TargetInterface targetInterface)
        {
            cppStreamWriter.WriteLine("class " + targetInterface.Name + "ClientImpl");
            cppStreamWriter.WriteLine("{\npublic:\nEmbedXrpcClientObject *RpcClientObject=nullptr;");
            cppStreamWriter.WriteLine(targetInterface.Name + "ClientImpl" + "(EmbedXrpcClientObject *rpcobj)");
            cppStreamWriter.WriteLine("{\nthis->RpcClientObject=rpcobj;");
            foreach (var service in targetInterface.Services)
            {
                cppStreamWriter.WriteLine("RpcClientObject->ServicesName.Add(\"" + targetInterface.Name + "." + service.ServiceName + "\");");
            }
            cppStreamWriter.WriteLine("}");

            foreach (var service in targetInterface.Services)
            {
                cppStreamWriter.Write("Option<" + service.ReturnValue.IdlType + "> " + service.ServiceName + "(");
                string temp_fileds = string.Empty;
                for (int i = 0; i < service.TargetFields.Count; i++)
                {
                    var field = service.TargetFields[i];
                    string name = field.Name;
                    temp_fileds += name + ",";

                    string cppType = IdlType2CppType(field);

                    if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                    {
                        name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                    }

                    cppStreamWriter.Write(cppType + " " + name);
                    if (i + 1 < service.TargetFields.Count)
                    {
                        cppStreamWriter.Write(",");
                    }
                }
                cppStreamWriter.WriteLine(")\n{");

                cppStreamWriter.WriteLine("//write serialization code:{0}({1})", service.ServiceName, temp_fileds);

                //todo
                //1.serialize  service.name to buf
                //2.serialize fields to buf
                //3.call rpc->send(buff);

                cppStreamWriter.WriteLine("}");
            }

            cppStreamWriter.WriteLine("};");

        }
    }
}
