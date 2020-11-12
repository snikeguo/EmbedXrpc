using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
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

        //public string IType { get; set; }

    }
    public class CppCodeGenerater 
    {

        private static Dictionary<string, string> ReplaceDic = new Dictionary<string, string>();
        private List<MessageMap> messageMaps = new List<MessageMap>();
        //int ServiceId = 0x10;

        public void AddMessageMap(string fullname, ReceiveType_t receiveType)
        {
            MessageMap m = new MessageMap();
            m.Name = fullname;
            m.ServiceId = $"{fullname}_ServiceId";
            m.ReceiveType = receiveType;
            //m.IType = "&" + fullname + "_Type";
            messageMaps.Add(m);
        }
        public void EmitServiceIdCode(StreamWriter sw, string defineName, int ServiceId)
        {
            sw.WriteLine("#define {0}_ServiceId {1}   //0x{2:X}", defineName, ServiceId, ServiceId);
        }
        public CppCodeGenParameter codeGenParameter;
        public ICppSerializable embedXrpcSerializationGenerator;
        public void CodeGen(CppCodeGenParameter parameter)
        {
            UTF8Encoding utf8ec = new UTF8Encoding(true);
            codeGenParameter = parameter;
            if(embedXrpcSerializationGenerator==null)
            {
                if(parameter.IsRuntimeVersion==true)
                {
                    embedXrpcSerializationGenerator = new CppNeedRuntimeSerializer();
                }
                else
                {
                    //throw new NotImplementedException();
                    embedXrpcSerializationGenerator = new CppNoRuntimeSerializer();
                }
            }
            Console.WriteLine($"cpp code gen:   {parameter.FileIdlInfo.FileName}");
            var outputattr = parameter.FileIdlInfo.GenerationOption;

            var VersionHsw = new StreamWriter(parameter.OutPutPath + "EmbedXrpcVersion.h", false, utf8ec);
            VersionHsw.WriteLine("#ifndef EmbedXrpcVersion_H");
            VersionHsw.WriteLine("#define EmbedXrpcVersion_H");
            string ver = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            VersionHsw.WriteLine($"#define EmbedXrpcVersion  \"{ver}\"");
            VersionHsw.WriteLine("#endif");
            VersionHsw.Flush();
            VersionHsw.Close();

            CommonHsw = new StreamWriter(parameter.OutPutPath + outputattr.OutPutFileName + ".h", false, utf8ec);
            CommonHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_H");
            CommonHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_H");
            //CommonHsw.WriteLine("#include\"EmbedSerializationBaseType.h\"");
            CommonHsw.WriteLine("#include\"EmbedSerialization.h\"");
            if(parameter.FileIdlInfo.TargetDelegates.Count>0|| parameter.FileIdlInfo.TargetInterfaces.Count>0)
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


            CommonCsw = new StreamWriter(parameter.OutPutPath + outputattr.OutPutFileName + ".cpp", false, utf8ec);
            CommonCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.h\"");
            CommonCsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            CommonCsw.WriteLine("//自动代码生成,请不要修改本文件!\n");

            //fbsStreamWriter = new StreamWriter(outputpath+outputattr.OutPutFileName + ".fbs", false, Encoding.ASCII);
            //fbsStreamWriter.WriteLine("namespace EmbedXrpc;");



            SerializeCsw = new StreamWriter(parameter.OutPutPath + outputattr.OutPutFileName + ".EmbedXrpcSerialization.cpp", false, utf8ec);


            SerializeCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");
            SerializeCsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            SerializeCsw.WriteLine("//自动代码生成,请不要修改本文件!\n");

            SerializeHsw = new StreamWriter(parameter.OutPutPath + outputattr.OutPutFileName + ".EmbedXrpcSerialization.h", false, utf8ec);
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

            

            

            if(parameter.GenType== GenType.Client|| parameter.GenType == GenType.All)
            {
                if (Directory.Exists(parameter.OutPutPath + "Client") == false)
                    Directory.CreateDirectory(parameter.OutPutPath + "Client");

                ClientHsw = new StreamWriter(parameter.OutPutPath + "Client/" + outputattr.OutPutFileName + ".Client.h", false, utf8ec);
                ClientHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_Client_H");
                ClientHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_Client_H");                
                ClientHsw.WriteLine("#include\"EmbedXrpcObject.h\"");
                ClientHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

                ClientCsw = new StreamWriter(parameter.OutPutPath + "Client/" + outputattr.OutPutFileName + ".Client.cpp", false, utf8ec);
                ClientCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.Client.h\"");
            }


            if (parameter.GenType == GenType.Server || parameter.GenType == GenType.All)
            {
                if (Directory.Exists(parameter.OutPutPath + "Server") == false)
                    Directory.CreateDirectory(parameter.OutPutPath + "Server");

                ServerHsw = new StreamWriter(parameter.OutPutPath + "Server/" + outputattr.OutPutFileName + ".Server.h", false, utf8ec);
                ServerHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_Server_H");
                ServerHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_Server_H");
                ServerHsw.WriteLine("#include\"EmbedXrpcObject.h\"");
                ServerHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

                ServerCsw = new StreamWriter(parameter.OutPutPath + "Server/" + outputattr.OutPutFileName + ".Server.cpp", false, utf8ec);
                ServerCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.Server.h\"");
            }
            

            foreach (var em in parameter.FileIdlInfo.TargetEnums)
            {
                //EmitFbsEnum(em);
                //fbsStreamWriter.WriteLine(em.ToFbs().ToString());
                EmitEnum(em);
            }
            foreach (var stru in parameter.FileIdlInfo.TargetStructs)
            {
                //EmitFbsTable(stru);
                //fbsStreamWriter.WriteLine(stru.ToFbs().ToString());
                embedXrpcSerializationGenerator.EmitStruct(stru,
                    SerializeCsw, 
                    SerializeHsw);
                EmitStruct(stru);
            }

            foreach (var del in parameter.FileIdlInfo.TargetDelegates)
            {
                //fbsStreamWriter.WriteLine(del.ToFbs().ToString());
                /*TargetStruct targetStruct = new TargetStruct();
                targetStruct.Name = del.MethodName+"Struct";
                targetStruct.TargetFields = del.TargetFields;*/
                EmitStruct(del.ParameterStructType);
                EmitDelegate(del);
                embedXrpcSerializationGenerator.EmitStruct(del.ParameterStructType,
                    SerializeCsw,
                    SerializeHsw);
                AddMessageMap(del.MethodName, ReceiveType_t.ReceiveType_Delegate);
                EmitServiceIdCode(SerializeHsw, del.MethodName, del.ServiceId);//生成 ServiceID 宏定义

            }

            foreach (var ifs in parameter.FileIdlInfo.TargetInterfaces)
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
            if (parameter.GenType == GenType.Client || parameter.GenType == GenType.All)
            {
                ClientHsw.WriteLine("\n#endif");
                ClientHsw.Flush();
                ClientHsw.Close();

                ClientCsw.Flush();
                ClientCsw.Close();
            }

            if (parameter.GenType == GenType.Server || parameter.GenType == GenType.All)
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

        public static string EmitField(ITargetField field)
        {
            string cppType = field.TargetType.TypeName;
            string valueName = string.Empty;
            if (field.TargetType.TargetType ==  TargetType_t.TYPE_ARRAY)
            {
                Array_TargetField array_TargetField = field as Array_TargetField;
                ArrayType_TargetType attt = array_TargetField.TargetType as ArrayType_TargetType;
                cppType = attt.ElementType.TypeName;
                if (array_TargetField.MaxCountAttribute.IsFixed == false)
                {
                    cppType = cppType + "*";
                    valueName = field.FieldName;
                }
                else
                {
                    valueName = $"{field.FieldName}[{array_TargetField.MaxCountAttribute.MaxCount}]";
                }
            }
            else
            {
                valueName = field.FieldName;
            }
            return $"{cppType} {valueName}";
        }

        /*public static string GetSourceCodeTypeArrayElementType(TargetField field)
        {
            string cppType = field.SourceCodeType;

            if (field.IsArray == true)
            {
                cppType = field.SourceCodeType.Replace("[", "");
                cppType = cppType.Replace("]", "");
            }
            return cppType;
        }*/

        public void EmitEnum(EnumType_TargetType targetEnum)
        {
            CommonHsw.WriteLine("typedef enum _" + targetEnum.TypeName);
            CommonHsw.WriteLine("{");
            foreach (var ev in targetEnum.KeyValue)
            {
                CommonHsw.WriteLine(ev.Key + " = " + ev.Value.ToString() + ",");
            }
            CommonHsw.WriteLine("}" + targetEnum.TypeName + ";");
        }
        
        public void EmitStruct(ObjectType_TargetType targetStruct)
        {
            CommonHsw.WriteLine("typedef struct _" + targetStruct.TypeName);
            CommonHsw.WriteLine("{");
            foreach (var field in targetStruct.TargetFields)
            {
                CommonHsw.WriteLine($"{EmitField(field)};   //FieldNumber:" + field.FieldNumberAttr.Number);
            }
            CommonHsw.WriteLine("}" + targetStruct.TypeName + ";");

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
            if (codeGenParameter.GenType == GenType.Client || codeGenParameter.GenType == GenType.All)
            {
                ClientHsw.WriteLine($"class {targetDelegate.MethodName}ClientImpl:public IDelegate");
                ClientHsw.WriteLine("{\npublic:");
                ClientHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", targetDelegate.MethodName);

                ClientHsw.Write($"void {targetDelegate.MethodName}(");

                
                for (int i = 0; i < targetDelegate.ParameterStructType.TargetFields.Count; i++)
                {
                    var field = targetDelegate.ParameterStructType.TargetFields[i];
                    string name = field.FieldName;
                    temp_fileds += name + ",";

                    ClientHsw.Write($"{EmitField(field)}");
                    if (i + 1 < targetDelegate.ParameterStructType.TargetFields.Count)
                    {
                        ClientHsw.Write(",");
                    }
                }
                ClientHsw.WriteLine(");");

                //code gen invoke
                ClientHsw.WriteLine("void Invoke(SerializationManager &recManager);");//声明

                ClientCsw.WriteLine($"void {targetDelegate.MethodName}ClientImpl::Invoke(SerializationManager &recManager)");
                ClientCsw.WriteLine("{");
                ClientCsw.WriteLine($"static {targetDelegate.ParameterStructType.TypeName} request;");

                //ClientCsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Deserialize(recManager,&request);");
                ClientCsw.WriteLine($"recManager.Deserialize(&{targetDelegate.ParameterStructType.TypeName}_TypeInstance,&request);");
                ClientCsw.WriteLine("#if EmbedXrpc_UseRingBufferWhenReceiving==1");
                ClientCsw.WriteLine($"EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());");
                ClientCsw.WriteLine("#else");
                ClientCsw.WriteLine($"EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);");
                ClientCsw.WriteLine("#endif");
                ClientCsw.Write($"{targetDelegate.MethodName}(");
                for (int i = 0; i < targetDelegate.ParameterStructType.TargetFields.Count; i++)
                {
                    var par = targetDelegate.ParameterStructType.TargetFields[i];
                    ClientCsw.Write($"request.{par.FieldName}");
                    if (i + 1 < targetDelegate.ParameterStructType.TargetFields.Count)
                    {
                        ClientCsw.Write(",");
                    }
                }
                ClientCsw.WriteLine(");");//生成调用目标函数。
                //ClientCsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Free(&request);");//free
                ClientCsw.WriteLine($"SerializationManager::FreeData(&{targetDelegate.ParameterStructType.TypeName}_TypeInstance,&request);");

                ClientCsw.WriteLine("}");//函数生成完毕

                ClientHsw.WriteLine("};");//end class
                ClientCsw.WriteLine($"{targetDelegate.MethodName}ClientImpl {targetDelegate.MethodName}ClientImplInstance;");//创建一个委托实例
            }
            if (codeGenParameter.GenType == GenType.Server || codeGenParameter.GenType == GenType.All)
            {
                //生成服务端代码
                ServerHsw.WriteLine("class " + targetDelegate.MethodName + "Delegate");
                ServerHsw.WriteLine("{\npublic:\nEmbedXrpcObject *RpcObject=nullptr;");
                ServerHsw.WriteLine(targetDelegate.MethodName + "Delegate" + "(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)");
                ServerHsw.WriteLine("{}");
                ServerHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", targetDelegate.MethodName);

                ServerHsw.Write($"void  Invoke(");
                ServerCsw.Write($"void  {targetDelegate.MethodName }Delegate::Invoke(");
                temp_fileds = string.Empty;
                for (int i = 0; i < targetDelegate.ParameterStructType.TargetFields.Count; i++)
                {
                    var field = targetDelegate.ParameterStructType.TargetFields[i];
                    string name = field.FieldName;
                    temp_fileds += name + ",";

                    ServerHsw.Write($"{EmitField(field)}");
                    ServerCsw.Write($"{EmitField(field)}");

                    if (i + 1 < targetDelegate.ParameterStructType.TargetFields.Count)
                    {
                        ServerHsw.Write(",");
                        ServerCsw.Write(",");
                    }
                }
                ServerHsw.WriteLine(");");//函数名
                ServerCsw.WriteLine(")\n{");//函数名

                //函数实现
                ServerCsw.WriteLine("//write serialization code:{0}({1})", targetDelegate.MethodName, temp_fileds);
                ServerCsw.WriteLine($"static {targetDelegate.ParameterStructType.TypeName} sendData;");
                ServerCsw.WriteLine($"SerializationManager sm;");
                ServerCsw.WriteLine($"sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;");
                ServerCsw.WriteLine("EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);");
                ServerCsw.WriteLine("sm.Reset();\n" +
                        "sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];\n" +
                        "sm.BufferLen = EmbedXrpc_SendBufferSize-4;");
                foreach (var field in targetDelegate.ParameterStructType.TargetFields)
                {
                    //if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                    if (field.TargetType.TargetType ==  TargetType_t.TYPE_ARRAY)
                    {
                        Array_TargetField array_TargetField = field as Array_TargetField;
                        var lenField= array_TargetField.ArrayLenField;
                        if(lenField!=null)
                        {
                            if(array_TargetField.MaxCountAttribute.IsFixed == true)
                            {
                                ServerCsw.WriteLine($"for(auto index=0;index<{lenField.FieldName};index++)");
                                ServerCsw.WriteLine("{");
                                ServerCsw.WriteLine($"  sendData.{field.FieldName}[index]={field.FieldName}[index];");
                                ServerCsw.WriteLine("}");
                            }
                            else
                            {
                                ServerCsw.WriteLine($"sendData.{field.FieldName}={field.FieldName};");
                            }
                        }
                        else
                        {
                            ServerCsw.WriteLine($"sendData.{field.FieldName}[0]={field.FieldName}[0];");
                        }
                        //ServerHsw.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");
                        
                    }
                    else
                    {
                        //ServerHsw.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                        ServerCsw.WriteLine($"sendData.{field.FieldName}={field.FieldName};");
                    }

                }
                //ServerCsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Serialize(sm,0,&sendData);");
                ServerCsw.WriteLine($"sm.Serialize(&{targetDelegate.ParameterStructType.TypeName}_TypeInstance,&sendData,0);");

                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)({targetDelegate.MethodName}_ServiceId&0xff);");
                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)({targetDelegate.MethodName}_ServiceId>>8&0xff);");
                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);");
                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);");
                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);");
                ServerCsw.WriteLine($"RpcObject->Send(RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);");
                ServerCsw.WriteLine("sm.Reset();");
                ServerCsw.WriteLine("EmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);");
                ServerCsw.WriteLine("}");//function end


                ServerHsw.WriteLine("};");//class end
            }

                
        }
        public void EmitClientInterface(TargetInterface targetInterface)
        {
            foreach (var service in targetInterface.Services)
            {
                //string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                /*TargetStruct targetStructRequest = new TargetStruct();
                targetStructRequest.Name = GeneratServiceName + "_Request";
                targetStructRequest.TargetFields.AddRange(service.TargetFields);*/
                embedXrpcSerializationGenerator.EmitStruct(service.ParameterStructType,
                    SerializeCsw,
                    SerializeHsw);
                EmitStruct(service.ParameterStructType);
                AddMessageMap(service.FullName, ReceiveType_t.ReceiveType_Request);
                EmitServiceIdCode(SerializeHsw, service.FullName, service.ServiceId);//生成 ServiceID 宏定义

                /*TargetStruct targetStructResponse = new TargetStruct();
                targetStructResponse.Name = GeneratServiceName + "_RequestResponseContent";

                TargetField ts = new TargetField();
                ts.Name = "State";
                ts.SourceCodeType = "RequestResponseState";
                ts.IsArray = false;
                ts.Enum = new TargetEnum() { SourceCodeType = "byte", Name = "RequestResponseState" };
                ts.MaxCountAttribute = null;
                ts.FieldNumberAttr = new FieldNumberAttribute(1);//state 的Field Number为1
                targetStructResponse.TargetFields.Add(ts);

                if (service.ReturnValue!=null)//添加返回值
                {                               
                    TargetField returnValue = new TargetField();
                    returnValue.Name = "ReturnValue";
                    returnValue.SourceCodeType = service.ReturnValue.SourceCodeType;
                    returnValue.IsArray = false;
                    returnValue.MaxCountAttribute = null;
                    returnValue.FieldNumberAttr = new FieldNumberAttribute(2);
                    targetStructResponse.TargetFields.Add(returnValue);
                }*/

                embedXrpcSerializationGenerator.EmitStruct(service.ReturnStructType,
                    SerializeCsw,
                    SerializeHsw);

                EmitStruct(service.ReturnStructType);

                AddMessageMap(service.FullName, ReceiveType_t.ReceiveType_Response);
                //EmitServiceIdCode(SerializeHsw, targetStructResponse.Name, ReceiveType_t.ReceiveType_Response, targetStructResponse.Name);//生成 ServiceID 宏定义

            }
            if (codeGenParameter.GenType == GenType.Client || codeGenParameter.GenType == GenType.All)
            {
                //这里生产client 部分代码
                ClientHsw.WriteLine("class " + targetInterface.Name + "ClientImpl");
                ClientHsw.WriteLine("{\npublic:\nEmbedXrpcObject *RpcObject=nullptr;");
                ClientHsw.WriteLine(targetInterface.Name + "ClientImpl" + "(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)");
                ClientHsw.WriteLine("{}");

                foreach (var service in targetInterface.Services)
                {
                    //string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                    ClientHsw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}(");
                    ClientCsw.Write($"{service.ReturnStructType.TypeName}& {targetInterface.Name}ClientImpl::{service.ServiceName}(");
                    string temp_fileds = string.Empty;
                    for (int i = 0; i < service.ParameterStructType.TargetFields.Count; i++)
                    {
                        var field = service.ParameterStructType.TargetFields[i];
                        string name = field.FieldName;
                        temp_fileds += name + ",";

                        ClientHsw.Write($"{EmitField(field)}");
                        ClientCsw.Write($"{EmitField(field)}");

                        if (i + 1 < service.ParameterStructType.TargetFields.Count)
                        {
                            ClientHsw.Write(",");
                            ClientCsw.Write(",");
                        }
                    }
                    ClientHsw.WriteLine(");");
                    ClientCsw.WriteLine(")\n{");

                    ClientCsw.WriteLine("//write serialization code:{0}({1})", service.ServiceName, temp_fileds);

                    ClientCsw.WriteLine($"static {service.ParameterStructType.TypeName} sendData;");
                    ClientCsw.WriteLine($"SerializationManager sm;");
                    ClientCsw.WriteLine($"sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;");
                    ClientCsw.WriteLine($"static {service.ReturnStructType.TypeName} reqresp;");
                    ClientCsw.WriteLine($"auto result=false;");

                    if (service.ReturnStructType.TargetFields.Count>1)//index 0 is state. 1 is returnfield
                    {
                        ClientCsw.WriteLine($"auto waitstate=ResponseState_Timeout;");
                    }
                    ClientCsw.WriteLine("EmbedXrpc_TakeMutex(RpcObject->ObjectMutexHandle, EmbedXrpc_WAIT_FOREVER);");

                    ClientCsw.WriteLine("#if EmbedXrpc_UseRingBufferWhenReceiving==1");
                    ClientCsw.WriteLine("RpcObject->ResponseBlockBufferProvider->Reset();");
                    ClientCsw.WriteLine("#else");
                    ClientCsw.WriteLine("EmbedXrpc_ResetQueue(RpcObject->ResponseBlockQueue);");
                    ClientCsw.WriteLine("#endif");
                    //ClientCsw.WriteLine("ResetSemaphore(RpcObject->ResponseMessageSemaphoreHandle);");
                    ClientCsw.WriteLine("sm.Reset();\n" +
                        "sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];\n" +
                        "sm.BufferLen = EmbedXrpc_SendBufferSize-4;");

                    foreach (var field in service.ParameterStructType.TargetFields)
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
                        if (field.TargetType.TargetType ==  TargetType_t.TYPE_ARRAY)
                        {
                            Array_TargetField array_TargetField = field as Array_TargetField;
                            var lenField = array_TargetField.ArrayLenField;
                            if (lenField != null)
                            {
                                if (array_TargetField.MaxCountAttribute.IsFixed == true)
                                {
                                    ClientCsw.WriteLine($"for(auto index=0;index<{lenField.FieldName};index++)");
                                    ClientCsw.WriteLine("{");
                                    ClientCsw.WriteLine($"  sendData.{field.FieldName}[index]={field.FieldName}[index];");
                                    ClientCsw.WriteLine("}");
                                }
                                else 
                                {
                                    ClientCsw.WriteLine($"sendData.{field.FieldName}={field.FieldName};");
                                }
                            }
                            else
                            {
                                ClientCsw.WriteLine($"sendData.{field.FieldName}[0]={field.FieldName}[0];");
                            }
                            //ServerHsw.WriteLine($"memcpy(sendData.{field.Name},{field.Name},sizeof(sendData.{field.Name})/sizeof({arrayelementtype}));");

                        }
                        else
                        {
                            //ServerHsw.WriteLine($"memcpy(&sendData.{field.Name},&{field.Name},sizeof(sendData.{field.Name}));");
                            ClientCsw.WriteLine($"sendData.{field.FieldName}={field.FieldName};");
                        }

                    }
                    //ClientCsw.WriteLine($"{GeneratServiceName}_Request_Type.Serialize(sm,0,&sendData);");
                    ClientCsw.WriteLine($"sm.Serialize(&{service.ParameterStructType.TypeName}_TypeInstance,&sendData,0);");

                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)({service.FullName}_ServiceId&0xff);");
                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)({service.FullName}_ServiceId>>8&0xff);");
                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);");
                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);");
                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);");
                    ClientCsw.WriteLine($"result=RpcObject->Send(RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);");
                    ClientCsw.WriteLine("sm.Reset();");
                    ClientCsw.WriteLine("if(result==false)\n{\nreqresp.State=RequestState_Failed;\ngoto exi;\n}");
                    ClientCsw.WriteLine("else\n{\nreqresp.State=RequestState_Ok;\n}");
                    if (service.ReturnStructType.TargetFields.Count>1)
                    {
                        ClientCsw.WriteLine($"waitstate=RpcObject->Wait({service.FullName}_ServiceId,&{service.ReturnStructType.TypeName}_TypeInstance,&reqresp);");
                        ClientCsw.WriteLine("reqresp.State=waitstate;");
                    }

                    ClientCsw.WriteLine("exi:\nEmbedXrpc_ReleaseMutex(RpcObject->ObjectMutexHandle);");
                    ClientCsw.WriteLine("return reqresp;");
                    ClientCsw.WriteLine("}");

                    if (service.ReturnStructType.TargetFields.Count > 1)
                    {
                        ClientHsw.WriteLine($"void Free_{service.ServiceName}({service.ReturnStructType.TypeName} *response);\n");
                        ClientCsw.WriteLine($"void {targetInterface.Name}ClientImpl::Free_{service.ServiceName}({service.ReturnStructType.TypeName} *response)");
                        ClientCsw.WriteLine("{\nif(response->State==ResponseState_Ok||response->State==ResponseState_SidError)\n{");
                        //ClientCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Free(response);");
                        ClientCsw.WriteLine($"SerializationManager::FreeData(&{service.ReturnStructType.TypeName}_TypeInstance,response);");
                        ClientCsw.WriteLine("}\n}");
                    }
                    ClientCsw.WriteLine("\n"); //client interface end class
                }
                ClientHsw.WriteLine("};"); //client interface end class

                
            }
            foreach (var service in targetInterface.Services)
            {
                //string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                //生成Server端代码
                if (codeGenParameter.GenType == GenType.Server || codeGenParameter.GenType == GenType.All)
                {
                    ServerHsw.WriteLine($"class {service.FullName}Service:public IService");
                    ServerHsw.WriteLine("{\npublic:");
                    ServerHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", service.FullName);

                    //string returnType= service.ReturnValue==null?"void":$"{service.ServiceName}_Response& ";
                    if (service.ReturnStructType.TargetFields.Count>1)
                        ServerHsw.WriteLine($"{service.ReturnStructType.TypeName} Response;");
                    string returnType = "void";
                    ServerHsw.Write($"{returnType} {service.ServiceName}(");

                    var temp_fileds = string.Empty;
                    for (int i = 0; i < service.ParameterStructType.TargetFields.Count; i++)
                    {
                        var field = service.ParameterStructType.TargetFields[i];
                        string name = field.FieldName;
                        temp_fileds += name + ",";

                        ServerHsw.Write($"{EmitField(field)}");

                        if (i + 1 < service.ParameterStructType.TargetFields.Count)
                        {
                            ServerHsw.Write(",");
                        }
                    }
                    ServerHsw.WriteLine(");");

                    //code gen invoke
                    ServerHsw.WriteLine("void Invoke(SerializationManager &recManager, SerializationManager& sendManager);");
                    ServerCsw.WriteLine($"void {service.FullName}Service::Invoke(SerializationManager &recManager, SerializationManager& sendManager)");
                    ServerCsw.WriteLine("{");
                    ServerCsw.WriteLine($"static {service.ParameterStructType.TypeName} request;");
                    //ServerCsw.WriteLine($"{GeneratServiceName}_Request_Type.Deserialize(recManager,&request);");
                    ServerCsw.WriteLine($"recManager.Deserialize(&{service.ParameterStructType.TypeName}_TypeInstance,&request);");
                    ServerCsw.WriteLine("#if EmbedXrpc_UseRingBufferWhenReceiving==1");
                    ServerCsw.WriteLine($"EmbedSerializationAssert(recManager.BlockBufferProvider->GetReferenceSum()==recManager.BlockBufferProvider->GetCalculateSum());");
                    ServerCsw.WriteLine("#else");
                    ServerCsw.WriteLine($"EmbedSerializationAssert(recManager.ReferenceSum==recManager.CalculateSum);");
                    ServerCsw.WriteLine("#endif");
                    //if (service.ReturnValue != null)
                    //    ServerHsw.Write($"{service.ServiceName}_Response& returnValue=");
                    ServerCsw.Write($"{service.ServiceName}(");
                    for (int i = 0; i < service.ParameterStructType.TargetFields.Count; i++)
                    {
                        var par = service.ParameterStructType.TargetFields[i];
                        ServerCsw.Write($"request.{par.FieldName}");
                        if (i + 1 < service.ParameterStructType.TargetFields.Count)
                        {
                            ServerCsw.Write(",");
                        }
                    }
                    ServerCsw.WriteLine(");");//生成调用目标函数。

                    //ServerCsw.WriteLine($"{GeneratServiceName}_Request_Type.Free(&request);");//free
                    ServerCsw.WriteLine($"SerializationManager::FreeData(&{service.ParameterStructType.TypeName}_TypeInstance,&request);");//free
                    if (service.ReturnStructType.TargetFields.Count>1)
                    {
                        //ServerCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Serialize(sendManager,0,&Response);");//生成返回值序列化
                        ServerCsw.WriteLine($"sendManager.Serialize(&{service.ReturnStructType.TypeName}_TypeInstance,&Response,0);");//生成返回值序列化
                        //ServerCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Free(&Response);");//生成返回值序列化
                        ServerCsw.WriteLine($"SerializationManager::FreeData(&{service.ReturnStructType.TypeName}_TypeInstance,&Response);");//生成返回值序列化
                    }

                    ServerCsw.WriteLine("}");//end function


                    ServerHsw.WriteLine("};");//end class

                    ServerCsw.WriteLine($"{service.FullName}Service {service.FullName}ServiceInstance;");//创建一个service实例
                }

            }
            if (codeGenParameter.GenType == GenType.Server || codeGenParameter.GenType == GenType.All)
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
            if (codeGenParameter.GenType == GenType.Client || codeGenParameter.GenType == GenType.All)
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
