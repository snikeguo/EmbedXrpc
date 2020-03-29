using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{
    public enum ReceiveType_t
    {
        ReceiveType_Request,
        ReceiveType_Response,
        ReceiveType_Delegate,
    };
    public class MessageMap
    {
        public string Name { get; set; }
        public string ServiesId { get; set; }
        public ReceiveType_t ReceiveType { get; set; }

        public string IType { get; set; }

    }
    public class CppCodeGenerater : ICodeGenerater
    {

        private static Dictionary<string, string> ReplaceDic = new Dictionary<string, string>();
        private Dictionary<ReceiveType_t,List<MessageMap>> messageMaps = new Dictionary<ReceiveType_t, List<MessageMap>>();
        int ServiceId = 0x10;
        public void EmitServiceIdCode(StreamWriter sw, string defineName, ReceiveType_t receiveType, string itype)
        {

            sw.WriteLine($"#define {defineName}_ServiceId {ServiceId}");

            MessageMap m = new MessageMap();
            m.Name = defineName;
            m.ServiesId = $"{defineName}_ServiceId";
            m.ReceiveType = receiveType;
            m.IType = "&"+itype+"_Type";
            if (messageMaps.ContainsKey(receiveType) == false)
            {
                messageMaps.Add(receiveType, new List<MessageMap>());
            }
            messageMaps[receiveType].Add(m);

            ServiceId++;

            
        }
        public void CodeGen(IdlInfo idlInfo)
        {
            Console.WriteLine("cpp code gen...");
            var outputattr = idlInfo.GenerationOptionParameterAttribute;
            cpphStreamWriter = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".h", false, Encoding.UTF8);
            cpphStreamWriter.WriteLine("#include\"EmbedXrpcCommon.h\"");
            cpphStreamWriter.WriteLine("#include\"EmbedXrpcClientObject.h\"");
            cpphStreamWriter.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

            cpphStreamWriter.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            cpphStreamWriter.WriteLine("//自动代码生成,请不要修改本文件!\n");


            cppcStreamWriter = new StreamWriter(outputattr.OutPutPath + outputattr.OutPutFileName + ".cpp", false, Encoding.UTF8);
            cppcStreamWriter.WriteLine($"#include\"{outputattr.OutPutFileName}.h\"");
            cppcStreamWriter.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            cppcStreamWriter.WriteLine("//自动代码生成,请不要修改本文件!\n");

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

            ServiceId = 0x10;
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

                EmitServiceIdCode(SerializeHWriter, targetStruct.Name, ReceiveType_t.ReceiveType_Delegate, targetStruct.Name);//生成 ServiceID 宏定义

            }

            foreach (var ifs in idlInfo.TargetInterfaces)
            {
                //fbsStreamWriter.WriteLine(ifs.ToFbs().ToString());
                EmitClientInterface(ifs);
                
            }

            //fbsStreamWriter.WriteLine(FbsHelper.EmitPackageTable().ToString());

            cpphStreamWriter.Flush();
            cpphStreamWriter.Close();

            cppcStreamWriter.Flush();
            cppcStreamWriter.Close();

            //fbsStreamWriter.Flush();
            //fbsStreamWriter.Close();
            //FbsHelper.GenerateSerializationCode(outputattr.OutPutPath+outputattr.OutPutFileName + ".fbs");

            SerializeCWriter.Flush();
            SerializeCWriter.Close();

            

            SerializeHWriter.WriteLine("\n#endif");

            SerializeHWriter.Flush();
            SerializeHWriter.Close();
        }
        private StreamWriter cpphStreamWriter;
        private StreamWriter cppcStreamWriter;
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
            cpphStreamWriter.WriteLine("typedef enum _" + targetEnum.Name);
            cpphStreamWriter.WriteLine("{");
            foreach (var ev in targetEnum.TargetEnumValues)
            {
                cpphStreamWriter.WriteLine(ev.Description + " = " + ev.Value.ToString() + ",");
            }
            cpphStreamWriter.WriteLine("}" + targetEnum.Name + ";");
        }
        
        public void EmitStruct(TargetStruct targetStruct)
        {
            cpphStreamWriter.WriteLine("typedef struct _" + targetStruct.Name);
            cpphStreamWriter.WriteLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                string name = field.Name;
                string cppType = IdlType2CppType(field);

                if (field.IsArray == true && field.MaxCountAttribute.IsFixed==true)
                {
                    name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                }

                cpphStreamWriter.WriteLine(cppType + " " + name + ";");

            }
            cpphStreamWriter.WriteLine("}" + targetStruct.Name + ";");
        }

        public void EmitDelegate(TargetDelegate targetDelegate)
        {
            cpphStreamWriter.Write("typedef void (*" + targetDelegate.MethodName + ")(");
            for (int i = 0; i < targetDelegate.TargetFields.Count; i++)
            {
                var field = targetDelegate.TargetFields[i];
                string cppType = IdlType2CppType(field);
                string name = field.Name;
                if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                {
                    name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                }

                cpphStreamWriter.Write(cppType + " " + name);
                if (i + 1 < targetDelegate.TargetFields.Count)
                {
                    cpphStreamWriter.Write(",");
                }
            }
            cpphStreamWriter.WriteLine(");");

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

                EmitServiceIdCode(SerializeHWriter, targetStructRequest.Name, ReceiveType_t.ReceiveType_Request, targetStructRequest.Name);//生成 ServiceID 宏定义

                if (service.ReturnValue!=null)
                {
                    TargetStruct targetStructResponse = new TargetStruct();
                    targetStructResponse.Name = service.ServiceName + "_Response";

                    TargetField ts = new TargetField();
                    ts.Name = "State";
                    ts.IdlType = "ResponseState";
                    ts.IsArray = false;
                    ts.Enum = new TargetEnum() { IntType = "byte", Name = "ResponseState"};
                    ts.MaxCountAttribute = null;
                    targetStructResponse.TargetFields.Add(ts);

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

                    EmitServiceIdCode(SerializeHWriter, targetStructResponse.Name, ReceiveType_t.ReceiveType_Response, targetStructResponse.Name);//生成 ServiceID 宏定义
                }
                
            }

            SerializeCWriter.WriteLine($"MessageMap RequestMessages[]=");
            SerializeCWriter.WriteLine("{");
            foreach (var message in messageMaps[ReceiveType_t.ReceiveType_Request])
            {
                SerializeCWriter.Write("{");
                SerializeCWriter.Write("\"{0}\",{1},{2},{3}", message.Name, message.ServiesId, "ReceiveType_Request", message.IType);
                SerializeCWriter.WriteLine("},");
            }
            SerializeCWriter.WriteLine("};");

            SerializeHWriter.WriteLine($"extern MessageMap RequestMessages[];");

            cpphStreamWriter.WriteLine("class " + targetInterface.Name + "ClientImpl");
            cpphStreamWriter.WriteLine("{\npublic:\nEmbedXrpcClientObject *RpcClientObject=nullptr;");
            cpphStreamWriter.WriteLine(targetInterface.Name + "ClientImpl" + "(EmbedXrpcClientObject *rpcobj)");
            cpphStreamWriter.WriteLine("{\nthis->RpcClientObject=rpcobj;");

            cpphStreamWriter.WriteLine("}");

            foreach (var service in targetInterface.Services)
            {
                if(service.ReturnValue!=null)
                    cpphStreamWriter.Write($"{service.ServiceName}_Response {service.ServiceName}(");
                else
                {
                    cpphStreamWriter.Write("void " + service.ServiceName + "(");
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

                    cpphStreamWriter.Write(cppType + " " + name);
                    if (i + 1 < service.TargetFields.Count)
                    {
                        cpphStreamWriter.Write(",");
                    }
                }
                cpphStreamWriter.WriteLine(")\n{");

                cpphStreamWriter.WriteLine("//write serialization code:{0}({1})", service.ServiceName, temp_fileds);

                cpphStreamWriter.WriteLine($"{service.ServiceName}_Request sendData;");

                cpphStreamWriter.WriteLine("RpcClientObject->porter->TakeMutex(RpcClientObject->BufMutexHandle, 100);");
                cpphStreamWriter.WriteLine("RpcClientObject->BufManager.Reset();");
                foreach (var field in service.TargetFields)
                {
                    if(field.IsArray==true&&field.MaxCountAttribute.IsFixed==true)
                    {
                        string arrayelementtype= field.IdlType.Replace("[","").Replace("]","");
                        cpphStreamWriter.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");
                    }
                    else
                    {
                        cpphStreamWriter.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                    }
                    
                }
                cpphStreamWriter.WriteLine($"{service.ServiceName}_Request_Type.Serialize(RpcClientObject->BufManager,0,&sendData);");
                cpphStreamWriter.WriteLine($"RpcClientObject->Send({service.ServiceName}_Request_ServiceId,RpcClientObject->BufManager.Index,RpcClientObject->BufManager.Buf);");
                cpphStreamWriter.WriteLine("RpcClientObject->BufManager.Reset();");
                cpphStreamWriter.WriteLine("RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);");

                if (service.ReturnValue != null)
                {
                    cpphStreamWriter.WriteLine($"EmbeXrpcRawData recData;\n"+
                                                $"{service.ServiceName}_Response response;\n" +
                                               $"ResponseState result=RpcClientObject->Wait({service.ServiceName}_Request_ServiceId,&recData,&response);");
                    cpphStreamWriter.WriteLine("if(result==ResponseState_SidError)\n{");
                    cpphStreamWriter.WriteLine($"RpcClientObject->porter->Free(recData.Data);\nresponse.State=ResponseState_SidError;");
                    cpphStreamWriter.WriteLine("}");
                    cpphStreamWriter.WriteLine("else if(result==ResponseState_Ok)\n{");
                    cpphStreamWriter.WriteLine($"RpcClientObject->porter->Free(recData.Data);\nresponse.State=ResponseState_Ok;");
                    cpphStreamWriter.WriteLine("}");
                    cpphStreamWriter.WriteLine("else if(result==ResponseState_Timeout)\n{");
                    cpphStreamWriter.WriteLine($"response.State=ResponseState_Timeout;");
                    cpphStreamWriter.WriteLine("}\nreturn response;");
                }

                //todo
                //1.serialize  service.name to buf
                //2.serialize fields to buf
                //3.call rpc->send(buff);

                cpphStreamWriter.WriteLine("}");

                if (service.ReturnValue != null)
                {
                    cpphStreamWriter.WriteLine("void Free_" + service.ServiceName +
                        $"({service.ServiceName}_Response *response)");
                    cpphStreamWriter.WriteLine("{\nif(response->State==ResponseState_Ok||response->State==ResponseState_SidError)\n{");
                    cpphStreamWriter.WriteLine($"{service.ServiceName}_Response_Type.Free(response);");
                    cpphStreamWriter.WriteLine("}\n}");
                }
                
            }

            cpphStreamWriter.WriteLine("};");

        }
    }
}
