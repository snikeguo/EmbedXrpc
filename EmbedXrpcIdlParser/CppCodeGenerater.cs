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
            var outputattr = idlInfo.GenerationOptionParameterAttribute;
            cppStreamWriter = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".h", false, Encoding.UTF8);
            cppStreamWriter.WriteLine("#include\"EmbedXrpcCommon.h\"");
            cppStreamWriter.WriteLine("#include\"EmbedXrpcClientObject.h\"");
            cppStreamWriter.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

            cppStreamWriter.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            cppStreamWriter.WriteLine("//自动代码生成,请不要修改本文件!\n");
            //fbsStreamWriter = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".fbs", false, Encoding.ASCII);
            //fbsStreamWriter.WriteLine("namespace EmbedXrpc;");



            SerializeCWriter = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".EmbedXrpcSerialization.cpp", false, Encoding.UTF8);

            SerializeCWriter.WriteLine($"#include\"{outputattr.OutPutFileName }.h\"");
            SerializeCWriter.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");
            SerializeCWriter.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            SerializeCWriter.WriteLine("//自动代码生成,请不要修改本文件!\n");

            SerializeHWriter = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".EmbedXrpcSerialization.h", false, Encoding.UTF8);
            SerializeHWriter.WriteLine($"#ifndef {outputattr.OutPutFileName}_EmbedXrpcSerialization_H");
            SerializeHWriter.WriteLine($"#define {outputattr.OutPutFileName}_EmbedXrpcSerialization_H");

            SerializeHWriter.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            SerializeHWriter.WriteLine("//自动代码生成,请不要修改本文件!\n");

            SerializeHWriter.WriteLine("//EmbedXrpc Code gen .By snikeguo.e-mail:snikeguo@foxmail.com");
            SerializeHWriter.WriteLine("//关于snikeguo作者:92年生人,热爱技术,底层功底扎实.深入理解C语言底层到汇编层，单片机从外设/裸机到OS均有涉猎");
            SerializeHWriter.WriteLine("//上位机专注于C# 界面库熟悉WPF.服务器专注于dotnet core");
            SerializeHWriter.WriteLine("//行业经验：汽车电子.独自完成UDS(包括FBL)协议栈  GBT27930-2015(SAE J1939)协议栈等");
            SerializeHWriter.WriteLine("//熟悉BMS业务逻辑,有一套自己稳定出货的BMS软件/硬件/上位机/服务器/USB转CAN&CANFD");
            SerializeHWriter.WriteLine("//TITLE:高级嵌入式软件工程师.软件架构师");
            SerializeHWriter.WriteLine("//微信:snikeguo.有好的职位推荐请加");

            //EmbedXrpcSerializationHFileStreamWriter.WriteLine("#include\"EmbedXrpcSerialization.h\"");
            //D:\VSProject\EmbedXrpcIdlParser\EmbedXrpcSerialization
            SerializeHWriter.WriteLine("#include\"EmbedSerialization.h\"");
            SerializeHWriter.WriteLine("#ifndef offsetof");
            SerializeHWriter.WriteLine("#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))");
            SerializeHWriter.WriteLine("#endif");
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
                EmbedXrpcSerializationHelper.EmitStruct(stru.Name, 
                    stru.TargetFields,
                    SerializeCWriter, 
                    SerializeHWriter);
                EmitStruct(stru);
            }

            foreach (var del in idlInfo.TargetDelegates)
            {
                //fbsStreamWriter.WriteLine(del.ToFbs().ToString());
                TargetStruct targetStruct = new TargetStruct();
                targetStruct.Name = del.MethodName+"Struct";
                targetStruct.TargetFields = del.TargetFields;
                EmitStruct(targetStruct);
                EmitDelegate(del);
                EmbedXrpcSerializationHelper.EmitStruct(targetStruct.Name,
                    targetStruct.TargetFields,
                    SerializeCWriter,
                    SerializeHWriter);
            }

            foreach (var ifs in idlInfo.TargetInterfaces)
            {
                //fbsStreamWriter.WriteLine(ifs.ToFbs().ToString());
                EmitClientInterface(ifs);
                
            }

            //fbsStreamWriter.WriteLine(FbsHelper.EmitPackageTable().ToString());

            cppStreamWriter.Flush();
            cppStreamWriter.Close();

            //fbsStreamWriter.Flush();
            //fbsStreamWriter.Close();
            //FbsHelper.GenerateSerializationCode(outputattr.OutPutPath+outputattr.OutPutFileName + ".fbs");

            SerializeCWriter.Flush();
            SerializeCWriter.Close();

            

            SerializeHWriter.WriteLine("\n#endif");

            SerializeHWriter.Flush();
            SerializeHWriter.Close();
        }
        private StreamWriter cppStreamWriter;
        //private StreamWriter fbsStreamWriter;
        private StreamWriter SerializeCWriter;
        private StreamWriter SerializeHWriter;
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
            foreach (var service in targetInterface.Services)
            {
                
                TargetStruct targetStructRequest = new TargetStruct();
                targetStructRequest.Name = service.ServiceName + "_Request";
                targetStructRequest.TargetFields.AddRange(service.TargetFields);
                EmbedXrpcSerializationHelper.EmitStruct(targetStructRequest.Name,
                    targetStructRequest.TargetFields,
                    SerializeCWriter,
                    SerializeHWriter);
                EmitStruct(targetStructRequest);

                if(service.ReturnValue!=null)
                {
                    TargetStruct targetStructResponse = new TargetStruct();
                    targetStructResponse.Name = service.ServiceName + "_Response";
                    TargetField returnValue = new TargetField();
                    returnValue.Name = "ReturnValue";
                    returnValue.IdlType = service.ReturnValue.IdlType;
                    returnValue.IsArray = false;
                    returnValue.MaxCountAttribute = null;
                    targetStructResponse.TargetFields.Add(returnValue);
                    EmbedXrpcSerializationHelper.EmitStruct(targetStructResponse.Name,
                        targetStructResponse.TargetFields,
                        SerializeCWriter,
                        SerializeHWriter);
                    EmitStruct(targetStructResponse);
                }
                
            }
            cppStreamWriter.WriteLine("class " + targetInterface.Name + "ClientImpl");
            cppStreamWriter.WriteLine("{\npublic:\nEmbedXrpcClientObject *RpcClientObject=nullptr;");
            cppStreamWriter.WriteLine(targetInterface.Name + "ClientImpl" + "(EmbedXrpcClientObject *rpcobj)");
            cppStreamWriter.WriteLine("{\nthis->RpcClientObject=rpcobj;");
            foreach (var service in targetInterface.Services)
            {
                cppStreamWriter.WriteLine("RpcClientObject->ServicesName.push_back(\"" + targetInterface.Name + "." + service.ServiceName + "\");");
            }
            cppStreamWriter.WriteLine("}");

            foreach (var service in targetInterface.Services)
            {
                if(service.ReturnValue!=null)
                    cppStreamWriter.Write("Option<" + service.ReturnValue.IdlType + "> " + service.ServiceName + "(");
                else
                {
                    cppStreamWriter.Write("void " + service.ServiceName + "(");
                }
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

                cppStreamWriter.WriteLine($"{service.ServiceName}_Request sendData;");
                foreach (var field in service.TargetFields)
                {
                    if(field.IsArray==true&&field.MaxCountAttribute.IsFixed==true)
                    {
                        string arrayelementtype= field.IdlType.Replace("[","").Replace("]","");
                        cppStreamWriter.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");
                    }
                    else
                    {
                        cppStreamWriter.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                    }
                    
                }
                cppStreamWriter.WriteLine($"{service.ServiceName}_Request_Type.Serialize(RpcClientObject->BufManager,0,&sendData);");
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
