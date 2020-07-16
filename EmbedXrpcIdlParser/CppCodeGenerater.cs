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
        public string ServiceId { get; set; }
        public ReceiveType_t ReceiveType { get; set; }

        public string IType { get; set; }

    }
    public class CppCodeGenerater : ICodeGenerater
    {

        private static Dictionary<string, string> ReplaceDic = new Dictionary<string, string>();
        private List<MessageMap> messageMaps = new List<MessageMap>();
        //int ServiceId = 0x10;

        public void AddMessageMap(string defineName, ReceiveType_t receiveType, string itype)
        {
            MessageMap m = new MessageMap();
            m.Name = defineName;
            m.ServiceId = $"{defineName}_ServiceId";
            m.ReceiveType = receiveType;
            m.IType = "&" + itype + "_Type";
            messageMaps.Add(m);
        }
        public void EmitServiceIdCode(StreamWriter sw, string defineName, int ServiceId)
        {
            sw.WriteLine($"#define {defineName}_ServiceId {ServiceId}");
        }
        public GenType genType;
        public void CodeGen(FileIdlInfo fileIdlInfo,GenType genType,string outputpath)
        {
            this.genType = genType;

            Console.WriteLine($"cpp code gen:   {fileIdlInfo.FileName}");
            var outputattr = fileIdlInfo.GenerationOption;
            CommonHsw = new StreamWriter(outputpath+outputattr.OutPutFileName + ".h", false, Encoding.UTF8);
            CommonHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_H");
            CommonHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_H");
            //CommonHsw.WriteLine("#include\"EmbedSerializationBaseType.h\"");
            CommonHsw.WriteLine("#include\"EmbedSerialization.h\"");
            if(fileIdlInfo.TargetDelegates.Count>0||fileIdlInfo.TargetInterfaces.Count>0)
                CommonHsw.WriteLine("#include\"EmbedXrpcCommon.h\"");

            foreach (var userInc in outputattr.UserIncludes)
            {
                CommonHsw.WriteLine($"#include\"{userInc}.h\"");
            }
            if(outputattr.UserNote!=null&& outputattr.UserNote != string.Empty)
            {
                CommonHsw.WriteLine($"/* {outputattr.UserNote} */");
            }
            

            CommonHsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            CommonHsw.WriteLine("//自动代码生成,请不要修改本文件!\n");


            CommonCsw = new StreamWriter(outputpath + outputattr.OutPutFileName + ".cpp", false, Encoding.UTF8);
            CommonCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.h\"");
            CommonCsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            CommonCsw.WriteLine("//自动代码生成,请不要修改本文件!\n");

            //fbsStreamWriter = new StreamWriter(outputpath+outputattr.OutPutFileName + ".fbs", false, Encoding.ASCII);
            //fbsStreamWriter.WriteLine("namespace EmbedXrpc;");



            SerializeCsw = new StreamWriter(outputpath+outputattr.OutPutFileName + ".EmbedXrpcSerialization.cpp", false, Encoding.UTF8);


            SerializeCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");
            SerializeCsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            SerializeCsw.WriteLine("//自动代码生成,请不要修改本文件!\n");

            SerializeHsw = new StreamWriter(outputpath+outputattr.OutPutFileName + ".EmbedXrpcSerialization.h", false, Encoding.UTF8);
            SerializeHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".","_")}_EmbedXrpcSerialization_H");
            SerializeHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_EmbedXrpcSerialization_H");
            SerializeHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.h\"");

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

            

            

            if(genType== GenType.Client|| genType== GenType.All)
            {
                if (Directory.Exists(outputpath + "Client") == false)
                    Directory.CreateDirectory(outputpath + "Client");

                ClientHsw = new StreamWriter(outputpath + "Client/" + outputattr.OutPutFileName + ".Client.h", false, Encoding.UTF8);
                ClientHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_Client_H");
                ClientHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_Client_H");                
                ClientHsw.WriteLine("#include\"EmbedXrpcObject.h\"");
                ClientHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

                ClientCsw = new StreamWriter(outputpath + "Client/" + outputattr.OutPutFileName + ".Client.cpp", false, Encoding.UTF8);
                ClientCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.Client.h\"");
            }


            if (genType == GenType.Server || genType == GenType.All)
            {
                if (Directory.Exists(outputpath + "Server") == false)
                    Directory.CreateDirectory(outputpath + "Server");

                ServerHsw = new StreamWriter(outputpath + "Server/" + outputattr.OutPutFileName + ".Server.h", false, Encoding.UTF8);
                ServerHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_Server_H");
                ServerHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_Server_H");
                ServerHsw.WriteLine("#include\"EmbedXrpcObject.h\"");
                ServerHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

                ServerCsw = new StreamWriter(outputpath + "Server/" + outputattr.OutPutFileName + ".Server.cpp", false, Encoding.UTF8);
                ServerCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.Server.h\"");
            }
            

            foreach (var em in fileIdlInfo.TargetEnums)
            {
                //EmitFbsEnum(em);
                //fbsStreamWriter.WriteLine(em.ToFbs().ToString());
                EmitEnum(em);
            }
            foreach (var stru in fileIdlInfo.TargetStructs)
            {
                //EmitFbsTable(stru);
                //fbsStreamWriter.WriteLine(stru.ToFbs().ToString());
                EmbedXrpcSerializationHelper.EmitStruct(stru.Name, 
                    stru.TargetFields,
                    SerializeCsw, 
                    SerializeHsw);
                EmitStruct(stru);
            }

            foreach (var del in fileIdlInfo.TargetDelegates)
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
                AddMessageMap(del.MethodName, ReceiveType_t.ReceiveType_Delegate, del.MethodName);
                EmitServiceIdCode(SerializeHsw, del.MethodName, del.ServiceId);//生成 ServiceID 宏定义

            }

            foreach (var ifs in fileIdlInfo.TargetInterfaces)
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
            //FbsHelper.GenerateSerializationCode(outputpath+outputattr.OutPutFileName + ".fbs");

            SerializeCsw.Flush();
            SerializeCsw.Close();

            

            SerializeHsw.WriteLine("\n#endif");

            SerializeHsw.Flush();
            SerializeHsw.Close();
            if (genType == GenType.Client || genType == GenType.All)
            {
                ClientHsw.WriteLine("\n#endif");
                ClientHsw.Flush();
                ClientHsw.Close();

                ClientCsw.Flush();
                ClientCsw.Close();
            }

            if (genType == GenType.Server || genType == GenType.All)
            {
                ServerHsw.WriteLine("\n#endif");
                ServerHsw.Flush();
                ServerHsw.Close();
                ServerCsw.Flush();
                ServerCsw.Close();
            }

                
        }
        private StreamWriter CommonHsw;
        private StreamWriter CommonCsw;
        private StreamWriter ClientHsw;
        private StreamWriter ClientCsw;
        private StreamWriter ServerHsw;
        private StreamWriter ServerCsw;
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
            if (targetStruct.BitsAttribute == null)
            {
                CommonHsw.WriteLine("typedef struct _" + targetStruct.Name);
                CommonHsw.WriteLine("{");
                foreach (var field in targetStruct.TargetFields)
                {
                    string name = field.Name;
                    string cppType = IdlType2CppType(field);

                    if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                    {
                        name += "[" + field.MaxCountAttribute.MaxCount.ToString() + "]";
                    }

                    CommonHsw.WriteLine(cppType + " " + name + ";");

                }
                CommonHsw.WriteLine("}" + targetStruct.Name + ";");
            }
            else
            {
                CommonHsw.WriteLine("typedef struct _" + targetStruct.Name);
                CommonHsw.WriteLine("{");
                foreach (var field in targetStruct.TargetFields)
                {
                    CommonHsw.WriteLine($"{targetStruct.BitsAttribute.BitsType} {field.Name}:{field.BitsFieldLengthAttribute.Length};");

                }
                CommonHsw.WriteLine("}" + targetStruct.Name + ";");
            }
            
        }

        public void EmitDelegate(TargetDelegate targetDelegate)
        {
            /*
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
            */
            var temp_fileds = string.Empty;
            //生成客户端 delegate代码
            if (genType == GenType.Client || genType == GenType.All)
            {
                ClientHsw.WriteLine($"class {targetDelegate.MethodName}ClientImpl:public IDelegate");
                ClientHsw.WriteLine("{\npublic:");
                ClientHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", targetDelegate.MethodName);

                ClientHsw.Write($"void {targetDelegate.MethodName}(");

                
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
                ClientHsw.WriteLine("void Invoke(SerializationManager &recManager);");//声明

                ClientCsw.WriteLine($"void {targetDelegate.MethodName}ClientImpl::Invoke(SerializationManager &recManager)");
                ClientCsw.WriteLine("{");
                ClientCsw.WriteLine($"static {targetDelegate.MethodName}Struct request;");

                ClientCsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Deserialize(recManager,&request);");
                ClientCsw.WriteLine($"EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());");
                ClientCsw.Write($"{targetDelegate.MethodName}(");
                for (int i = 0; i < targetDelegate.TargetFields.Count; i++)
                {
                    var par = targetDelegate.TargetFields[i];
                    ClientCsw.Write($"request.{par.Name}");
                    if (i + 1 < targetDelegate.TargetFields.Count)
                    {
                        ClientCsw.Write(",");
                    }
                }
                ClientCsw.WriteLine(");");//生成调用目标函数。
                ClientCsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Free(&request);");//free
                                                                                               //ClientHsw.WriteLine("RpcObject->porter->ReleaseMutex(RpcObject->ObjectMutexHandle);");
                ClientCsw.WriteLine("}");//函数生成完毕

                ClientHsw.WriteLine("};");//end class
                ClientCsw.WriteLine($"{targetDelegate.MethodName}ClientImpl {targetDelegate.MethodName}ClientImplInstance;");//创建一个委托实例
            }
            if (genType == GenType.Server || genType == GenType.All)
            {
                //生成服务端代码
                ServerHsw.WriteLine("class " + targetDelegate.MethodName + "Delegate");
                ServerHsw.WriteLine("{\npublic:\nEmbedXrpcObject *RpcObject=nullptr;");
                ServerHsw.WriteLine(targetDelegate.MethodName + "Delegate" + "(EmbedXrpcObject *rpcobj)");
                ServerHsw.WriteLine("{\nthis->RpcObject=rpcobj;");
                ServerHsw.WriteLine("}");
                ServerHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", targetDelegate.MethodName);

                ServerHsw.Write($"void  Invoke(");
                ServerCsw.Write($"void  {targetDelegate.MethodName }Delegate::Invoke(");
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
                    ServerCsw.Write(cppType + " " + name);
                    if (i + 1 < targetDelegate.TargetFields.Count)
                    {
                        ServerHsw.Write(",");
                        ServerCsw.Write(",");
                    }
                }
                ServerHsw.WriteLine(");");//函数名
                ServerCsw.WriteLine(")\n{");//函数名

                //函数实现
                ServerCsw.WriteLine("//write serialization code:{0}({1})", targetDelegate.MethodName, temp_fileds);
                ServerCsw.WriteLine($"static {targetDelegate.MethodName}Struct sendData;");
                ServerCsw.WriteLine($"SerializationManager sm;");
                ServerCsw.WriteLine("RpcObject->porter->TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);");
                ServerCsw.WriteLine("sm.Reset();\n" +
                        "sm.Buf = &RpcObject->Buffer[4];\n" +
                        "sm.BufferLen = RpcObject->BufferLen-4;");
                foreach (var field in targetDelegate.TargetFields)
                {
                    //if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                    if (field.IsArray == true)
                    {
                        var lenField=IdlInfo.GetArrayLenField(targetDelegate.TargetFields, field);
                        if(lenField!=null)
                        {
                            ServerCsw.WriteLine($"for(auto index=0;index<{lenField.Name};index++)");
                            ServerCsw.WriteLine("{");
                            ServerCsw.WriteLine($"  sendData.{field.Name}[index]={field.Name}[index];");
                            ServerCsw.WriteLine("}");
                        }
                        else
                        {
                            ServerCsw.WriteLine($"sendData.{field.Name}[0]={field.Name}[0];");
                        }
                        //ServerHsw.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");
                        
                    }
                    else
                    {
                        //ServerHsw.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                        ServerCsw.WriteLine($"sendData.{field.Name}={field.Name};");
                    }

                }
                ServerCsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Serialize(sm,0,&sendData);");

                ServerCsw.WriteLine($"RpcObject->Buffer[0]=(uint8_t)({targetDelegate.MethodName}_ServiceId&0xff);");
                ServerCsw.WriteLine($"RpcObject->Buffer[1]=(uint8_t)({targetDelegate.MethodName}_ServiceId>>8&0xff);");
                ServerCsw.WriteLine($"RpcObject->Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);");
                ServerCsw.WriteLine($"RpcObject->Buffer[3]=(uint8_t)(RpcObject->TimeOut>>8&0xff);");

                ServerCsw.WriteLine($"RpcObject->Send(RpcObject,sm.Index+4,RpcObject->Buffer);");
                ServerCsw.WriteLine("sm.Reset();");
                ServerCsw.WriteLine("RpcObject->porter->ReleaseMutex(RpcObject->ObjectMutexHandle);");
                ServerCsw.WriteLine("}");//function end


                ServerHsw.WriteLine("};");//class end
            }

                
        }
        public void EmitClientInterface(TargetInterface targetInterface)
        {
            foreach (var service in targetInterface.Services)
            {
                string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                TargetStruct targetStructRequest = new TargetStruct();
                targetStructRequest.Name = GeneratServiceName + "_Request";
                targetStructRequest.TargetFields.AddRange(service.TargetFields);
                EmbedXrpcSerializationHelper.EmitStruct( targetStructRequest.Name,
                    targetStructRequest.TargetFields,
                    SerializeCsw,
                    SerializeHsw);
                EmitStruct(targetStructRequest);
                AddMessageMap(GeneratServiceName, ReceiveType_t.ReceiveType_Request, targetStructRequest.Name);
                EmitServiceIdCode(SerializeHsw, GeneratServiceName, service.ServiceId);//生成 ServiceID 宏定义

                TargetStruct targetStructResponse = new TargetStruct();
                targetStructResponse.Name = GeneratServiceName + "_RequestResponseContent";

                TargetField ts = new TargetField();
                ts.Name = "State";
                ts.IdlType = "RequestResponseState";
                ts.IsArray = false;
                ts.Enum = new TargetEnum() { IntType = "byte", Name = "RequestResponseState" };
                ts.MaxCountAttribute = null;
                targetStructResponse.TargetFields.Add(ts);

                if (service.ReturnValue!=null)//添加返回值
                {                               
                    TargetField returnValue = new TargetField();
                    returnValue.Name = "ReturnValue";
                    returnValue.IdlType = service.ReturnValue.IdlType;
                    returnValue.IsArray = false;
                    returnValue.MaxCountAttribute = null;
                    targetStructResponse.TargetFields.Add(returnValue);
                }

                EmbedXrpcSerializationHelper.EmitStruct(targetStructResponse.Name,
                    targetStructResponse.TargetFields,
                    SerializeCsw,
                    SerializeHsw);

                EmitStruct(targetStructResponse);

                AddMessageMap(GeneratServiceName, ReceiveType_t.ReceiveType_Response, targetStructResponse.Name);
                //EmitServiceIdCode(SerializeHsw, targetStructResponse.Name, ReceiveType_t.ReceiveType_Response, targetStructResponse.Name);//生成 ServiceID 宏定义

            }
            if (genType == GenType.Client || genType == GenType.All)
            {
                //这里生产client 部分代码
                ClientHsw.WriteLine("class " + targetInterface.Name + "ClientImpl");
                ClientHsw.WriteLine("{\npublic:\nEmbedXrpcObject *RpcObject=nullptr;");
                ClientHsw.WriteLine(targetInterface.Name + "ClientImpl" + "(EmbedXrpcObject *rpcobj)");
                ClientHsw.WriteLine("{\nthis->RpcObject=rpcobj;");

                ClientHsw.WriteLine("}");

                foreach (var service in targetInterface.Services)
                {
                    string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                    ClientHsw.Write($"{GeneratServiceName}_RequestResponseContent& {service.ServiceName}(");
                    ClientCsw.Write($"{GeneratServiceName}_RequestResponseContent& {targetInterface.Name}ClientImpl::{service.ServiceName}(");
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
                        ClientCsw.Write(cppType + " " + name);
                        if (i + 1 < service.TargetFields.Count)
                        {
                            ClientHsw.Write(",");
                            ClientCsw.Write(",");
                        }
                    }
                    ClientHsw.WriteLine(");");
                    ClientCsw.WriteLine(")\n{");

                    ClientCsw.WriteLine("//write serialization code:{0}({1})", service.ServiceName, temp_fileds);

                    ClientCsw.WriteLine($"static {GeneratServiceName}_Request sendData;");
                    ClientCsw.WriteLine($"SerializationManager sm;");
                    ClientCsw.WriteLine($"static {GeneratServiceName}_RequestResponseContent reqresp;");
                    ClientCsw.WriteLine($"auto result=false;");

                    if (service.ReturnValue != null)
                    {
                        ClientCsw.WriteLine($"auto waitstate=ResponseState_Timeout;");
                    }
                    ClientCsw.WriteLine("RpcObject->porter->TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);");
                    
                    ClientCsw.WriteLine("RpcObject->ResponseBlockBufferProvider->Reset();");
                    //ClientCsw.WriteLine("RpcObject->porter->ResetSemaphore(RpcObject->ResponseMessageSemaphoreHandle);");
                    ClientCsw.WriteLine("sm.Reset();\n" +
                        "sm.Buf = &RpcObject->Buffer[4];\n" +
                        "sm.BufferLen = RpcObject->BufferLen-4;");

                    foreach (var field in service.TargetFields)
                    {
                        /*if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                        {
                            string arrayelementtype = field.IdlType.Replace("[", "").Replace("]", "");
                            ClientHsw.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");
                        }
                        else
                        {
                            ClientHsw.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                        }*/

                        //if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                        if (field.IsArray == true)
                        {
                            var lenField = IdlInfo.GetArrayLenField(service.TargetFields, field);
                            if (lenField != null)
                            {
                                ClientCsw.WriteLine($"for(auto index=0;index<{lenField.Name};index++)");
                                ClientCsw.WriteLine("{");
                                ClientCsw.WriteLine($"  sendData.{field.Name}[index]={field.Name}[index];");
                                ClientCsw.WriteLine("}");
                            }
                            else
                            {
                                ClientCsw.WriteLine($"sendData.{field.Name}[0]={field.Name}[0];");
                            }
                            //ServerHsw.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");

                        }
                        else
                        {
                            //ServerHsw.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                            ClientCsw.WriteLine($"sendData.{field.Name}={field.Name};");
                        }

                    }
                    ClientCsw.WriteLine($"{GeneratServiceName}_Request_Type.Serialize(sm,0,&sendData);");

                    ClientCsw.WriteLine($"RpcObject->Buffer[0]=(uint8_t)({GeneratServiceName}_ServiceId&0xff);");
                    ClientCsw.WriteLine($"RpcObject->Buffer[1]=(uint8_t)({GeneratServiceName}_ServiceId>>8&0xff);");
                    ClientCsw.WriteLine($"RpcObject->Buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);");
                    ClientCsw.WriteLine($"RpcObject->Buffer[3]=(uint8_t)(RpcObject->TimeOut>>8&0xff);");
                    
                    ClientCsw.WriteLine($"result=RpcObject->Send(RpcObject,sm.Index+4,RpcObject->Buffer);");
                    ClientCsw.WriteLine("sm.Reset();");
                    ClientCsw.WriteLine("if(result==false)\n{\nreqresp.State=RequestState_Failed;\ngoto exi;\n}");
                    ClientCsw.WriteLine("else\n{\nreqresp.State=RequestState_Ok;\n}");
                    if (service.ReturnValue != null)
                    {
                        ClientCsw.WriteLine($"waitstate=RpcObject->Wait({GeneratServiceName}_ServiceId,&{GeneratServiceName}_RequestResponseContent_Type,&reqresp);");
                        ClientCsw.WriteLine("reqresp.State=waitstate;");
                    }

                    ClientCsw.WriteLine("exi:\nRpcObject->porter->ReleaseMutex(RpcObject->ObjectMutexHandle);");
                    ClientCsw.WriteLine("return reqresp;");
                    ClientCsw.WriteLine("}");

                    if (service.ReturnValue != null)
                    {
                        ClientHsw.WriteLine($"void Free_{service.ServiceName}({GeneratServiceName}_RequestResponseContent *response);\n");
                        ClientCsw.WriteLine($"void {targetInterface.Name}ClientImpl::Free_{service.ServiceName}({GeneratServiceName}_RequestResponseContent *response)");
                        ClientCsw.WriteLine("{\nif(response->State==ResponseState_Ok||response->State==ResponseState_SidError)\n{");
                        ClientCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Free(response);");
                        ClientCsw.WriteLine("}\n}");
                    }
                    ClientCsw.WriteLine("\n"); //client interface end class
                }
                ClientHsw.WriteLine("};"); //client interface end class

                
            }
            foreach (var service in targetInterface.Services)
            {
                string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                //生成Server端代码
                if (genType == GenType.Server || genType == GenType.All)
                {
                    ServerHsw.WriteLine($"class {GeneratServiceName}Service:public IService");
                    ServerHsw.WriteLine("{\npublic:");
                    ServerHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", GeneratServiceName);

                    //string returnType= service.ReturnValue==null?"void":$"{service.ServiceName}_Response& ";
                    if (service.ReturnValue != null)
                        ServerHsw.WriteLine($"{GeneratServiceName}_RequestResponseContent Response;");
                    string returnType = "void";
                    ServerHsw.Write($"{returnType} {service.ServiceName}(");

                    var temp_fileds = string.Empty;
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
                    ServerHsw.WriteLine("void Invoke(SerializationManager &recManager, SerializationManager& sendManager);");
                    ServerCsw.WriteLine($"void {GeneratServiceName}Service::Invoke(SerializationManager &recManager, SerializationManager& sendManager)");
                    ServerCsw.WriteLine("{");
                    ServerCsw.WriteLine($"static {GeneratServiceName}_Request request;");
                    ServerCsw.WriteLine($"{GeneratServiceName}_Request_Type.Deserialize(recManager,&request);");
                    ServerCsw.WriteLine($"EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());");
                    //if (service.ReturnValue != null)
                    //    ServerHsw.Write($"{service.ServiceName}_Response& returnValue=");
                    ServerCsw.Write($"{service.ServiceName}(");
                    for (int i = 0; i < service.TargetFields.Count; i++)
                    {
                        var par = service.TargetFields[i];
                        ServerCsw.Write($"request.{par.Name}");
                        if (i + 1 < service.TargetFields.Count)
                        {
                            ServerCsw.Write(",");
                        }
                    }
                    ServerCsw.WriteLine(");");//生成调用目标函数。

                    ServerCsw.WriteLine($"{GeneratServiceName}_Request_Type.Free(&request);");//free

                    if (service.ReturnValue != null)
                    {
                        ServerCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Serialize(sendManager,0,&Response);");//生成返回值序列化
                        ServerCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Free(&Response);");//生成返回值序列化
                    }

                    ServerCsw.WriteLine("}");//end function


                    ServerHsw.WriteLine("};");//end class

                    ServerCsw.WriteLine($"{GeneratServiceName}Service {GeneratServiceName}ServiceInstance;");//创建一个service实例
                }

            }
            if (genType == GenType.Server || genType == GenType.All)
            {
                //生成request Service 数组
                ServerCsw.WriteLine($"RequestMessageMap {targetInterface.Name}_RequestMessages[]=");
                int RequestMessagesCount = 0;
                ServerCsw.WriteLine("{");
                foreach (var message in messageMaps)
                {
                    if (message.ReceiveType == ReceiveType_t.ReceiveType_Request)
                    {
                        RequestMessagesCount++;
                        ServerCsw.Write("{");
                        ServerCsw.Write("\"{0}\",&{1}ServiceInstance", message.Name, message.Name);
                        ServerCsw.WriteLine("},");
                    }

                }
                ServerCsw.WriteLine("};");
                ServerHsw.WriteLine($"#define {targetInterface.Name}_RequestMessages_Count  {RequestMessagesCount}");
                ServerHsw.WriteLine($"extern RequestMessageMap {targetInterface.Name}_RequestMessages[{targetInterface.Name}_RequestMessages_Count];");
            }
            if (genType == GenType.Client || genType == GenType.All)
            {
                //生成Response\Delegate service 数组
                ClientCsw.WriteLine($"ResponseDelegateMessageMap {targetInterface.Name}_ResponseDelegateMessages[]=");
                int ResponseDelegateMessagesCount = 0;
                ClientCsw.WriteLine("{");
                foreach (var message in messageMaps)
                {
                    if (message.ReceiveType == ReceiveType_t.ReceiveType_Delegate)
                    {
                        ResponseDelegateMessagesCount++;
                        ClientCsw.Write("{");
                        ClientCsw.Write("\"{0}\",{1},{2},&{3}ClientImplInstance", message.Name, message.ServiceId, "ReceiveType_Delegate", message.Name);
                        ClientCsw.WriteLine("},");
                    }
                    else if (message.ReceiveType == ReceiveType_t.ReceiveType_Response)
                    {
                        ResponseDelegateMessagesCount++;
                        ClientCsw.Write("{");
                        ClientCsw.Write("\"{0}\",{1},{2},nullptr", message.Name, message.ServiceId, "ReceiveType_Response");
                        ClientCsw.WriteLine("},");
                    }
                }
                ClientCsw.WriteLine("};");
                ClientHsw.WriteLine($"#define {targetInterface.Name}_ResponseDelegateMessages_Count {ResponseDelegateMessagesCount}");
                ClientHsw.WriteLine($"extern ResponseDelegateMessageMap {targetInterface.Name}_ResponseDelegateMessages[{targetInterface.Name}_ResponseDelegateMessages_Count];");
            }
        }

    }
}
