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
            CommonHsw = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".h", false, Encoding.UTF8);
            CommonHsw.WriteLine($"#ifndef {outputattr.OutPutFileName}_H");
            CommonHsw.WriteLine($"#define {outputattr.OutPutFileName}_H");
            CommonHsw.WriteLine("#include\"EmbedSerializationBaseType.h\"");
            CommonHsw.WriteLine("#include\"EmbedSerialization.h\"");
            if(idlInfo.TargetDelegates.Count>0||idlInfo.TargetInterfaces.Count>0)
                CommonHsw.WriteLine("#include\"EmbedXrpcCommon.h\"");

            CommonHsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            CommonHsw.WriteLine("//自动代码生成,请不要修改本文件!\n");


            CommonCsw = new StreamWriter(outputattr.OutPutPath + outputattr.OutPutFileName + ".cpp", false, Encoding.UTF8);
            CommonCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.h\"");
            CommonCsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            CommonCsw.WriteLine("//自动代码生成,请不要修改本文件!\n");

            //fbsStreamWriter = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".fbs", false, Encoding.ASCII);
            //fbsStreamWriter.WriteLine("namespace EmbedXrpc;");



            SerializeCsw = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".EmbedXrpcSerialization.cpp", false, Encoding.UTF8);

            SerializeCsw.WriteLine($"#include\"{outputattr.OutPutFileName }.h\"");
            SerializeCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");
            SerializeCsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            SerializeCsw.WriteLine("//自动代码生成,请不要修改本文件!\n");

            SerializeHsw = new StreamWriter(outputattr.OutPutPath+outputattr.OutPutFileName + ".EmbedXrpcSerialization.h", false, Encoding.UTF8);
            SerializeHsw.WriteLine($"#ifndef {outputattr.OutPutFileName}_EmbedXrpcSerialization_H");
            SerializeHsw.WriteLine($"#define {outputattr.OutPutFileName}_EmbedXrpcSerialization_H");

            SerializeHsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            SerializeHsw.WriteLine("//自动代码生成,请不要修改本文件!\n");

            SerializeHsw.WriteLine("//EmbedXrpc Code gen .By snikeguo.e-mail:snikeguo@foxmail.com");
            SerializeHsw.WriteLine("//关于snikeguo作者:92年生人,热爱技术,底层功底扎实.深入理解C语言底层到汇编层，单片机从外设/裸机到OS均有涉猎");
            SerializeHsw.WriteLine("//上位机专注于C# 界面库熟悉WPF.服务器专注于dotnet core");
            SerializeHsw.WriteLine("//行业经验：汽车电子.独自完成UDS(包括FBL)协议栈  GBT27930-2015(SAE J1939)协议栈等");
            SerializeHsw.WriteLine("//熟悉BMS业务逻辑,有一套自己稳定出货的BMS软件/硬件/上位机/服务器/USB转CAN&CANFD");
            SerializeHsw.WriteLine("//TITLE:高级嵌入式软件工程师.软件架构师");
            SerializeHsw.WriteLine("//微信:snikeguo.有好的职位推荐请加");

            //EmbedXrpcSerializationHFileStreamWriter.WriteLine("#include\"EmbedXrpcSerialization.h\"");
            //D:\VSProject\EmbedXrpcIdlParser\EmbedXrpcSerialization
            
            SerializeHsw.WriteLine("#ifndef offsetof");
            SerializeHsw.WriteLine("#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))");
            SerializeHsw.WriteLine("#endif");

            ClientHsw = new StreamWriter(outputattr.OutPutPath + outputattr.OutPutFileName + ".Client.h", false, Encoding.UTF8);
            ClientHsw.WriteLine($"#ifndef {outputattr.OutPutFileName}_Client_H");
            ClientHsw.WriteLine($"#define {outputattr.OutPutFileName}_Client_H");
            ClientHsw.WriteLine($"#include\"{outputattr.OutPutFileName }.h\"");
            ClientHsw.WriteLine("#include\"EmbedXrpcClientObject.h\"");
            ClientHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

            ServerHsw = new StreamWriter(outputattr.OutPutPath + outputattr.OutPutFileName + ".Server.h", false, Encoding.UTF8);
            ServerHsw.WriteLine($"#ifndef {outputattr.OutPutFileName}_Server_H");
            ServerHsw.WriteLine($"#define {outputattr.OutPutFileName}_Server_H");
            ServerHsw.WriteLine($"#include\"{outputattr.OutPutFileName }.h\"");
            ServerHsw.WriteLine("#include\"EmbedXrpcServerObject.h\"");
            ServerHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

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
                    SerializeCsw, 
                    SerializeHsw);
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
                    SerializeCsw,
                    SerializeHsw);

                EmitServiceIdCode(SerializeHsw, targetStruct.Name, ReceiveType_t.ReceiveType_Delegate, targetStruct.Name);//生成 ServiceID 宏定义

            }

            foreach (var ifs in idlInfo.TargetInterfaces)
            {
                //fbsStreamWriter.WriteLine(ifs.ToFbs().ToString());
                EmitClientInterface(ifs);
                
            }

            //fbsStreamWriter.WriteLine(FbsHelper.EmitPackageTable().ToString());
            CommonHsw.WriteLine("\n#endif");
            CommonHsw.Flush();
            CommonHsw.Close();

            CommonCsw.Flush();
            CommonCsw.Close();

            //fbsStreamWriter.Flush();
            //fbsStreamWriter.Close();
            //FbsHelper.GenerateSerializationCode(outputattr.OutPutPath+outputattr.OutPutFileName + ".fbs");

            SerializeCsw.Flush();
            SerializeCsw.Close();

            

            SerializeHsw.WriteLine("\n#endif");

            SerializeHsw.Flush();
            SerializeHsw.Close();

            ClientHsw.WriteLine("\n#endif");
            ClientHsw.Flush();
            ClientHsw.Close();

            ServerHsw.WriteLine("\n#endif");
            ServerHsw.Flush();
            ServerHsw.Close();
        }
        private StreamWriter CommonHsw;
        private StreamWriter CommonCsw;
        private StreamWriter ClientHsw;
        private StreamWriter ServerHsw;
        private StreamWriter SerializeCsw;
        private StreamWriter SerializeHsw;
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
            CommonHsw.WriteLine("typedef enum _" + targetEnum.Name);
            CommonHsw.WriteLine("{");
            foreach (var ev in targetEnum.TargetEnumValues)
            {
                CommonHsw.WriteLine(ev.Description + " = " + ev.Value.ToString() + ",");
            }
            CommonHsw.WriteLine("}" + targetEnum.Name + ";");
        }
        
        public void EmitStruct(TargetStruct targetStruct)
        {
            CommonHsw.WriteLine("typedef struct _" + targetStruct.Name);
            CommonHsw.WriteLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                string name = field.Name;
                string cppType = IdlType2CppType(field);

                if (field.IsArray == true && field.MaxCountAttribute.IsFixed==true)
                {
                    name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                }

                CommonHsw.WriteLine(cppType + " " + name + ";");

            }
            CommonHsw.WriteLine("}" + targetStruct.Name + ";");
        }

        public void EmitDelegate(TargetDelegate targetDelegate)
        {
            CommonHsw.Write("typedef void (*" + targetDelegate.MethodName + ")(");
            for (int i = 0; i < targetDelegate.TargetFields.Count; i++)
            {
                var field = targetDelegate.TargetFields[i];
                string cppType = IdlType2CppType(field);
                string name = field.Name;
                if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                {
                    name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                }

                CommonHsw.Write(cppType + " " + name);
                if (i + 1 < targetDelegate.TargetFields.Count)
                {
                    CommonHsw.Write(",");
                }
            }
            CommonHsw.WriteLine(");");

            //生成客户端 delegate代码
            ClientHsw.WriteLine($"class {targetDelegate.MethodName}ClientImpl:public IDelegate");
            ClientHsw.WriteLine("{\npublic:");
            ClientHsw.WriteLine($"uint32_t Sid=0;");//delegate 的sid保留

            ClientHsw.Write($"void {targetDelegate.MethodName}(");

            var temp_fileds = string.Empty;
            for (int i = 0; i < targetDelegate.TargetFields.Count; i++)
            {
                var field = targetDelegate.TargetFields[i];
                string name = field.Name;
                temp_fileds += name + ",";

                string cppType = IdlType2CppType(field);

                if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                {
                    name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                }

                ClientHsw.Write(cppType + " " + name);
                if (i + 1 < targetDelegate.TargetFields.Count)
                {
                    ClientHsw.Write(",");
                }
            }
            ClientHsw.WriteLine(");");

            //code gen invoke
            ClientHsw.WriteLine("void Invoke(SerializationManager &recManager)\n{");
            ClientHsw.WriteLine($"{targetDelegate.MethodName}Struct request;");
            //ClientHsw.WriteLine("RpcClientObject->porter->TakeMutex(RpcClientObject->BufMutexHandle, 100);");
            ClientHsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Deserialize(recManager,&request);");

            ClientHsw.Write($"{targetDelegate.MethodName}(");
            for (int i = 0; i < targetDelegate.TargetFields.Count; i++)
            {
                var par = targetDelegate.TargetFields[i];
                ClientHsw.Write($"request.{par.Name}");
                if (i + 1 < targetDelegate.TargetFields.Count)
                {
                    ClientHsw.Write(",");
                }
            }
            ClientHsw.WriteLine(");");//生成调用目标函数。
            ClientHsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Free(&request);");//free
            //ClientHsw.WriteLine("RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);");
            ClientHsw.WriteLine("}");//函数生成完毕

            ClientHsw.WriteLine("};");//end class

            //生成服务端代码
            ServerHsw.WriteLine("class " + targetDelegate.MethodName + "Delegate");
            ServerHsw.WriteLine("{\npublic:\nEmbedXrpcServerObject *RpcServerObject=nullptr;");
            ServerHsw.WriteLine(targetDelegate.MethodName + "Delegate" + "(EmbedXrpcServerObject *rpcobj)");
            ServerHsw.WriteLine("{\nthis->RpcServerObject=rpcobj;");
            ServerHsw.WriteLine("}");
            ServerHsw.WriteLine($"uint32_t Sid={targetDelegate.MethodName}Struct_ServiceId;");

            ServerHsw.Write($"void  Invoke(");

            temp_fileds = string.Empty;
            for (int i = 0; i < targetDelegate.TargetFields.Count; i++)
            {
                var field = targetDelegate.TargetFields[i];
                string name = field.Name;
                temp_fileds += name + ",";

                string cppType = IdlType2CppType(field);

                if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                {
                    name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                }

                ServerHsw.Write(cppType + " " + name);
                if (i + 1 < targetDelegate.TargetFields.Count)
                {
                    ServerHsw.Write(",");
                }
            }
            ServerHsw.WriteLine("){");//函数名

            //函数实现
            ServerHsw.WriteLine("//write serialization code:{0}({1})", targetDelegate.MethodName, temp_fileds);
            ServerHsw.WriteLine($"{targetDelegate.MethodName}Struct sendData;");
            ServerHsw.WriteLine("RpcServerObject->porter->TakeMutex(RpcServerObject->BufMutexHandle, 100);");
            ServerHsw.WriteLine("RpcServerObject->BufManager.Reset();");
            foreach (var field in targetDelegate.TargetFields)
            {
                if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                {
                    string arrayelementtype = field.IdlType.Replace("[", "").Replace("]", "");
                    ServerHsw.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");
                }
                else
                {
                    ServerHsw.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                }

            }
            ServerHsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Serialize(RpcServerObject->BufManager,0,&sendData);");
            ServerHsw.WriteLine($"RpcServerObject->Send({targetDelegate.MethodName}Struct_ServiceId,RpcServerObject->BufManager.Index,RpcServerObject->BufManager.Buf);");
            ServerHsw.WriteLine("RpcServerObject->BufManager.Reset();");
            ServerHsw.WriteLine("RpcServerObject->porter->ReleaseMutex(RpcServerObject->BufMutexHandle);");
            ServerHsw.WriteLine("}");//class end


            ServerHsw.WriteLine("};");//class end
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
                    SerializeCsw,
                    SerializeHsw);
                EmitStruct(targetStructRequest);

                EmitServiceIdCode(SerializeHsw, targetStructRequest.Name, ReceiveType_t.ReceiveType_Request, targetStructRequest.Name);//生成 ServiceID 宏定义

                TargetStruct targetStructResponse = new TargetStruct();
                targetStructResponse.Name = service.ServiceName + "_Response";
                if (service.ReturnValue!=null)
                {
                   

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
                        SerializeCsw,
                        SerializeHsw);

                    EmitStruct(targetStructResponse);

                    
                }
                EmitServiceIdCode(SerializeHsw, targetStructResponse.Name, ReceiveType_t.ReceiveType_Response, targetStructResponse.Name);//生成 ServiceID 宏定义

            }

            SerializeCsw.WriteLine($"MessageMap RequestMessages[]=");
            SerializeCsw.WriteLine("{");
            foreach (var message in messageMaps[ReceiveType_t.ReceiveType_Request])
            {
                SerializeCsw.Write("{");
                SerializeCsw.Write("\"{0}\",{1},{2},{3}", message.Name, message.ServiesId, "ReceiveType_Request", message.IType);
                SerializeCsw.WriteLine("},");
            }
            SerializeCsw.WriteLine("};");

            SerializeHsw.WriteLine($"extern MessageMap RequestMessages[];");

            //这里生产client 部分代码
            ClientHsw.WriteLine("class " + targetInterface.Name + "ClientImpl");
            ClientHsw.WriteLine("{\npublic:\nEmbedXrpcClientObject *RpcClientObject=nullptr;");
            ClientHsw.WriteLine(targetInterface.Name + "ClientImpl" + "(EmbedXrpcClientObject *rpcobj)");
            ClientHsw.WriteLine("{\nthis->RpcClientObject=rpcobj;");

            ClientHsw.WriteLine("}");


            foreach (var service in targetInterface.Services)
            {
                if(service.ReturnValue!=null)
                    ClientHsw.Write($"{service.ServiceName}_Response {service.ServiceName}(");
                else
                {
                    ClientHsw.Write("void " + service.ServiceName + "(");
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

                    ClientHsw.Write(cppType + " " + name);
                    if (i + 1 < service.TargetFields.Count)
                    {
                        ClientHsw.Write(",");
                    }
                }
                ClientHsw.WriteLine(")\n{");

                ClientHsw.WriteLine("//write serialization code:{0}({1})", service.ServiceName, temp_fileds);

                ClientHsw.WriteLine($"{service.ServiceName}_Request sendData;");

                ClientHsw.WriteLine("RpcClientObject->porter->TakeMutex(RpcClientObject->BufMutexHandle, 100);");
                ClientHsw.WriteLine("RpcClientObject->BufManager.Reset();");
                foreach (var field in service.TargetFields)
                {
                    if(field.IsArray==true&&field.MaxCountAttribute.IsFixed==true)
                    {
                        string arrayelementtype= field.IdlType.Replace("[","").Replace("]","");
                        ClientHsw.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");
                    }
                    else
                    {
                        ClientHsw.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                    }
                    
                }
                ClientHsw.WriteLine($"{service.ServiceName}_Request_Type.Serialize(RpcClientObject->BufManager,0,&sendData);");
                ClientHsw.WriteLine($"RpcClientObject->Send({service.ServiceName}_Request_ServiceId,RpcClientObject->BufManager.Index,RpcClientObject->BufManager.Buf);");
                ClientHsw.WriteLine("RpcClientObject->BufManager.Reset();");
                ClientHsw.WriteLine("RpcClientObject->porter->ReleaseMutex(RpcClientObject->BufMutexHandle);");

                if (service.ReturnValue != null)
                {
                    ClientHsw.WriteLine($"EmbeXrpcRawData recData;\n"+
                                                $"{service.ServiceName}_Response response;\n" +
                                               $"ResponseState result=RpcClientObject->Wait({service.ServiceName}_Request_ServiceId,&recData,&response);");
                    ClientHsw.WriteLine("if(result==ResponseState_SidError)\n{");
                    ClientHsw.WriteLine($"RpcClientObject->porter->Free(recData.Data);\nresponse.State=ResponseState_SidError;");
                    ClientHsw.WriteLine("}");
                    ClientHsw.WriteLine("else if(result==ResponseState_Ok)\n{");
                    ClientHsw.WriteLine($"RpcClientObject->porter->Free(recData.Data);\nresponse.State=ResponseState_Ok;");
                    ClientHsw.WriteLine("}");
                    ClientHsw.WriteLine("else if(result==ResponseState_Timeout)\n{");
                    ClientHsw.WriteLine($"response.State=ResponseState_Timeout;");
                    ClientHsw.WriteLine("}\nreturn response;");
                }

                ClientHsw.WriteLine("}");

                if (service.ReturnValue != null)
                {
                    ClientHsw.WriteLine("void Free_" + service.ServiceName +
                        $"({service.ServiceName}_Response *response)");
                    ClientHsw.WriteLine("{\nif(response->State==ResponseState_Ok||response->State==ResponseState_SidError)\n{");
                    ClientHsw.WriteLine($"{service.ServiceName}_Response_Type.Free(response);");
                    ClientHsw.WriteLine("}\n}");
                }

                //生成Server端代码
                ServerHsw.WriteLine($"class {service.ServiceName}Service:public IService");
                ServerHsw.WriteLine("{\npublic:");
                ServerHsw.WriteLine($"uint32_t Sid={service.ServiceName}_Response_ServiceId;");

                string returnType= service.ReturnValue==null?"void":$"{service.ServiceName}_Response";
                ServerHsw.Write($"{returnType} {service.ServiceName}(");

                temp_fileds = string.Empty;
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

                    ServerHsw.Write(cppType + " " + name);
                    if (i + 1 < service.TargetFields.Count)
                    {
                        ServerHsw.Write(",");
                    }
                }
                ServerHsw.WriteLine(");");

                //code gen invoke
                ServerHsw.WriteLine("void Invoke(SerializationManager &recManager, SerializationManager& sendManager)\n{");

                ServerHsw.WriteLine($"{service.ServiceName}_Request request;");
                ServerHsw.WriteLine($"{service.ServiceName}_Request_Type.Deserialize(recManager,&request);");

                if (service.ReturnValue != null)
                    ServerHsw.Write($"{service.ServiceName}_Response returnValue=");
                ServerHsw.Write($"{service.ServiceName}(");
                for (int i = 0; i < service.TargetFields.Count; i++)
                {
                    var par= service.TargetFields[i];
                    ServerHsw.Write($"request.{par.Name}");
                    if (i + 1 < service.TargetFields.Count)
                    {
                        ServerHsw.Write(",");
                    }
                }
                ServerHsw.WriteLine(");");//生成调用目标函数。

                ServerHsw.WriteLine($"{service.ServiceName}_Request_Type.Free(&request);");//free

                if (service.ReturnValue!=null)
                {
                    ServerHsw.WriteLine($"{service.ServiceName}_Response_Type.Serialize(sendManager,0,&returnValue);");//生成返回值序列化
                }

                ServerHsw.WriteLine("}");


                ServerHsw.WriteLine("};");
            }

            ClientHsw.WriteLine("};");

        }
    }
}
