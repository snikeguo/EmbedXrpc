using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpcIdlParser
{

    public class MessageMap
    {
        public string Name { get; set; }
        public string ServiceId { get; set; }


    }
    public class CppCodeGenerater 
    {

        private static Dictionary<string, string> ReplaceDic = new Dictionary<string, string>();
        private List<MessageMap> messageMaps = new List<MessageMap>();
        //int ServiceId = 0x10;

        public void AddMessageMap(string fullname)
        {
            MessageMap m = new MessageMap();
            m.Name = fullname;
            m.ServiceId = $"{fullname}_ServiceId";
            //m.IType = "&" + fullname + "_Type";
            messageMaps.Add(m);
        }
        public void EmitServiceIdCode(StreamWriter sw, string defineName, int ServiceId)
        {
            sw.WriteLine("#define {0}_ServiceId {1}   //0x{2:X}", defineName, ServiceId, ServiceId);
        }
        private void EmitCPlusPlusMacroBegin(StreamWriter sw)
        {
            sw.WriteLine("#ifdef  __cplusplus");
            sw.WriteLine("extern \"C\" {");
            sw.WriteLine("#endif");
            sw.WriteLine();
        }
        private void EmitCPlusPlusMacroEnd(StreamWriter sw)
        {
            sw.WriteLine("#ifdef  __cplusplus");
            sw.WriteLine("}");
            sw.WriteLine("#endif");
            sw.WriteLine();
        }
        public CppCodeGenParameter codeGenParameter;
        public ICppSerializable embedXrpcSerializationGenerator;
        public void CodeGen(CppCodeGenParameter cppCodeGenParameter)
        {
            var encode = new UTF8Encoding(false);
            codeGenParameter = cppCodeGenParameter;
            if(embedXrpcSerializationGenerator==null)
            {
                embedXrpcSerializationGenerator = new CppCsNanoSerializer();
            }
            Console.WriteLine($"cpp code gen:   {cppCodeGenParameter.FileIdlInfo.FileName}");
            var outputattr = cppCodeGenParameter.FileIdlInfo.GenerationOption;

            var VersionHsw = new StreamWriter(cppCodeGenParameter.OutPutPath + "EmbedXrpcVersion.h", false, encode);
            VersionHsw.WriteLine("#ifndef EmbedXrpcVersion_H");
            VersionHsw.WriteLine("#define EmbedXrpcVersion_H");
            string ver = Assembly.GetExecutingAssembly().GetName().Version.ToString();
            VersionHsw.WriteLine($"#define EmbedXrpcVersion  \"{ver}\"");
            VersionHsw.WriteLine("#endif");
            VersionHsw.Flush();
            VersionHsw.Close();

            CommonHsw = new StreamWriter(cppCodeGenParameter.OutPutPath + outputattr.OutPutFileName + ".h", false, encode);
            CommonHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_H");
            CommonHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_H");
            //CommonHsw.WriteLine("#include\"EmbedSerializationBaseType.h\"");
            CommonHsw.WriteLine("#include\"EmbedSerialization.h\"");
            if(cppCodeGenParameter.FileIdlInfo.TargetServices.Count>0)
                CommonHsw.WriteLine("#include\"EmbedXrpcCommon.h\"");

            foreach (var userInc in outputattr.UserIncludes)
            {
                CommonHsw.WriteLine($"#include\"{userInc}\"");
            }
            if(outputattr.UserNote!=null&& outputattr.UserNote != string.Empty)
            {
                CommonHsw.WriteLine($"/* {outputattr.UserNote} */");
            }
            

            CommonHsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            CommonHsw.WriteLine("//自动代码生成,请不要修改本文件!\n");
            EmitCPlusPlusMacroBegin(CommonHsw);

            CommonCsw = new StreamWriter(cppCodeGenParameter.OutPutPath + outputattr.OutPutFileName + ".cpp", false, encode);
            CommonCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.h\"");
            CommonCsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            CommonCsw.WriteLine("//自动代码生成,请不要修改本文件!\n");
            

            //fbsStreamWriter = new StreamWriter(outputpath+outputattr.OutPutFileName + ".fbs", false, Encoding.UTF8);
            //fbsStreamWriter.WriteLine("namespace EmbedXrpc;");



            SerializeCsw = new StreamWriter(cppCodeGenParameter.OutPutPath + outputattr.OutPutFileName + ".EmbedXrpcSerialization.cpp", false, encode);


            SerializeCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");
            SerializeCsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            SerializeCsw.WriteLine("//自动代码生成,请不要修改本文件!\n");

            SerializeHsw = new StreamWriter(cppCodeGenParameter.OutPutPath + outputattr.OutPutFileName + ".EmbedXrpcSerialization.h", false, encode);
            SerializeHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".","_")}_EmbedXrpcSerialization_H");
            SerializeHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_EmbedXrpcSerialization_H");
            SerializeHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.h\"");


            SerializeHsw.WriteLine("\n//auto code gen ! DO NOT modify this file!");
            SerializeHsw.WriteLine("//自动代码生成,请不要修改本文件!\n");
            EmitCPlusPlusMacroBegin(SerializeHsw);
            //SerializeHsw.WriteLine("//EmbedXrpc Code gen");
            //SerializeHsw.WriteLine("//关于snikeguo作者:92年生人,热爱技术,底层功底扎实.深入理解C语言底层到汇编层，单片机从外设/裸机到OS均有涉猎");
            //SerializeHsw.WriteLine("//上位机专注于C# 界面库熟悉WPF.服务器专注于dotnet core");
            //SerializeHsw.WriteLine("//行业经验：汽车电子.独自完成UDS(包括FBL)协议栈  GBT27930-2015(SAE J1939)协议栈等");
            //SerializeHsw.WriteLine("//熟悉BMS业务逻辑,有一套自己稳定出货的BMS软件/硬件/上位机/服务器/USB转CAN&CANFD");
            //SerializeHsw.WriteLine("//TITLE:高级嵌入式软件工程师.软件架构师");
            //SerializeHsw.WriteLine("//微信:snikeguo.有好的职位推荐请加");

            //EmbedXrpcSerializationHFileStreamWriter.WriteLine("#include\"EmbedXrpcSerialization.h\"");
            //D:\VSProject\EmbedXrpcIdlParser\EmbedXrpcSerialization
            
            SerializeHsw.WriteLine("#ifndef offsetof");
            SerializeHsw.WriteLine("#define offsetof(s, m) (size_t)((char*)(&((s*)0)->m))");
            SerializeHsw.WriteLine("#endif");

            

            

            if(cppCodeGenParameter.RoleType== RoleType.Client|| cppCodeGenParameter.RoleType == RoleType.All)
            {
                if (Directory.Exists(cppCodeGenParameter.OutPutPath + "Client") == false)
                    Directory.CreateDirectory(cppCodeGenParameter.OutPutPath + "Client");

                ClientHsw = new StreamWriter(cppCodeGenParameter.OutPutPath + "Client/" + outputattr.OutPutFileName + ".Client.h", false, encode);
                ClientHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_Client_H");
                ClientHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_Client_H");                
                ClientHsw.WriteLine("#include\"EmbedXrpcObject.h\"");
                ClientHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

                ClientCsw = new StreamWriter(cppCodeGenParameter.OutPutPath + "Client/" + outputattr.OutPutFileName + ".Client.cpp", false, encode);
                ClientCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.Client.h\"");
            }


            if (cppCodeGenParameter.RoleType == RoleType.Server || cppCodeGenParameter.RoleType == RoleType.All)
            {
                if (Directory.Exists(cppCodeGenParameter.OutPutPath + "Server") == false)
                    Directory.CreateDirectory(cppCodeGenParameter.OutPutPath + "Server");

                ServerHsw = new StreamWriter(cppCodeGenParameter.OutPutPath + "Server/" + outputattr.OutPutFileName + ".Server.h", false, encode);
                ServerHsw.WriteLine($"#ifndef {outputattr.OutPutFileName.Replace(".", "_")}_Server_H");
                ServerHsw.WriteLine($"#define {outputattr.OutPutFileName.Replace(".", "_")}_Server_H");
                ServerHsw.WriteLine("#include\"EmbedXrpcObject.h\"");
                ServerHsw.WriteLine($"#include\"{outputattr.OutPutFileName}.EmbedXrpcSerialization.h\"");

                ServerCsw = new StreamWriter(cppCodeGenParameter.OutPutPath + "Server/" + outputattr.OutPutFileName + ".Server.cpp", false, encode);
                ServerCsw.WriteLine($"#include\"{outputattr.OutPutFileName}.Server.h\"");
            }
            

            foreach (var em in cppCodeGenParameter.FileIdlInfo.TargetEnums)
            {
                //EmitFbsEnum(em);
                //fbsStreamWriter.WriteLine(em.ToFbs().ToString());
                CppSerializableCommon.EmitEnum(em,CommonHsw);
            }
            foreach (var stru in cppCodeGenParameter.FileIdlInfo.TargetStructs)
            {
                //EmitFbsTable(stru);
                //fbsStreamWriter.WriteLine(stru.ToFbs().ToString());
                /*embedXrpcSerializationGenerator.EmitStruct(stru,
                    SerializeCsw, 
                    SerializeHsw);*/
                EmitStruct(stru, cppCodeGenParameter);
            }
            foreach (var service in cppCodeGenParameter.FileIdlInfo.TargetServices)
            {
                EmitStruct(service.ReturnStructType, cppCodeGenParameter);
                EmitStruct(service.ParameterStructType, cppCodeGenParameter);
                EmitService(service, cppCodeGenParameter);
                AddMessageMap(service.ServiceName);
                EmitServiceIdCode(SerializeHsw, service.ServiceName, service.ServiceId);//生成 ServiceID 宏定义

            }

            //fbsStreamWriter.WriteLine(FbsHelper.EmitPackageTable().ToString());


            EmitCPlusPlusMacroEnd(CommonHsw);
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


            EmitCPlusPlusMacroEnd(SerializeHsw);
            SerializeHsw.WriteLine("\n#endif");

            SerializeHsw.Flush();
            SerializeHsw.Close();
            if (cppCodeGenParameter.RoleType == RoleType.Client || cppCodeGenParameter.RoleType == RoleType.All)
            {
                ClientHsw.WriteLine("\n#endif");
                ClientHsw.Flush();
                ClientHsw.Close();

                ClientCsw.Flush();
                ClientCsw.Close();
            }

            if (cppCodeGenParameter.RoleType == RoleType.Server || cppCodeGenParameter.RoleType == RoleType.All)
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
     
        public void EmitStruct(StructType_TargetType structType, CppCodeGenParameter cppCodeGenParameter)
        {
            /*
             CommonHsw用来定义具体的数据结构
            SerializeCsw、SerializeHsw用来定义序列化的操作、行为等
             */
            CppSerializableCommon.EmitStruct(structType, CommonHsw);
            embedXrpcSerializationGenerator.EmitStruct(structType,
                    SerializeCsw,
                    SerializeHsw);
            embedXrpcSerializationGenerator.EmitSerializeMacro(structType, SerializeHsw);
            embedXrpcSerializationGenerator.EmitDeserializeMacro(structType, SerializeHsw);
            embedXrpcSerializationGenerator.EmitFreeDataMacro(structType, SerializeHsw);
        }
        
        private void EmitCaller(TargetService service, StreamWriter hsw, StreamWriter csw)
        {
            //生成客户端代码
            string ReturnStructFreeNote = service.ReturnStructType.IsNeedFreeMemoryForNativeLanguage == true ? "" : "//!";

            var noOsCms = new MemoryStream();
            var noOsHms=new MemoryStream();
            StreamWriter noOsCsw=new StreamWriter(noOsCms);
            StreamWriter noOsHsw=new StreamWriter(noOsHms);
            noOsHsw.WriteLine("//---------No Os-----------");

            CppSerializableCommon.MacroControlWriteBegin(hsw, service.MacroControlAttribute);
            CppSerializableCommon.MacroControlWriteBegin(csw, service.MacroControlAttribute);
            hsw.WriteLine("class " + service.ServiceName + "_Requester");
            hsw.WriteLine("{\npublic:\nEmbedXrpcObject *RpcObject=nullptr;");
            hsw.WriteLine(service.ServiceName + "_Requester" + "(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)");
            hsw.WriteLine("{}");
            hsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", service.ServiceName);
            string temp_fileds = string.Empty;

            noOsHsw.WriteLine("uint32_t RequestTick;");
            if (service.ReturnStructType.TargetFields.Count > 1)
            {
                noOsHsw.WriteLine($"//NoOs_QueryServiceState接口只能放到while1执行,不能放到中断里面跑");
                noOsHsw.WriteLine($"{service.ReturnStructType.TypeName}& NoOs_QueryServiceState(RequestParameter* rp);");
            }
            hsw.WriteLine($"{service.ParameterStructType.TypeName} {service.ServiceName}_SendData;");
            hsw.WriteLine($"{service.ReturnStructType.TypeName} {service.ServiceName}_reqresp;");
            //string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
            if (service.ExternalParameter.IsExternal == false)
            {
                var dh = service.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                hsw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}(RequestParameter* rp{dh}");
                noOsHsw.WriteLine($"//NoOs_{service.ServiceName}接口只能放到while1执行,不能放到中断里面跑");
                noOsHsw.Write($"{service.ReturnStructType.TypeName}& NoOs_{service.ServiceName}(RequestParameter* rp{dh}");

                csw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}_Requester::{service.ServiceName}(RequestParameter* rp{dh}");
                noOsCsw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}_Requester::NoOs_{service.ServiceName}(RequestParameter* rp{dh}");

                temp_fileds = string.Empty;
                for (int i = 0; i < service.ParameterStructType.TargetFields.Count; i++)
                {
                    var field = service.ParameterStructType.TargetFields[i];
                    string name = field.FieldName;
                    temp_fileds += name + ",";

                    hsw.Write($"{CppSerializableCommon.EmitField(field)}");
                    noOsHsw.Write($"{CppSerializableCommon.EmitField(field)}");
                    csw.Write($"{CppSerializableCommon.EmitField(field)}");
                    noOsCsw.Write($"{CppSerializableCommon.EmitField(field)}");

                    if (i + 1 < service.ParameterStructType.TargetFields.Count)
                    {
                        hsw.Write(",");
                        noOsHsw.Write(",");
                        csw.Write(",");
                        noOsCsw.Write(",");
                    }
                }
                hsw.WriteLine(");");
                noOsHsw.WriteLine(");");
                csw.WriteLine(")\n{");
                noOsCsw.WriteLine(")\n{");
            }
            else
            {
                hsw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}(RequestParameter* rp);");
                noOsHsw.WriteLine($"//NoOs_{service.ServiceName}接口只能放到while1执行,不能放到中断里面跑");
                noOsHsw.Write($"{service.ReturnStructType.TypeName}& NoOs_{service.ServiceName}(RequestParameter* rp);");
                csw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}_Requester::{service.ServiceName}(RequestParameter* rp)\n{{");
                noOsCsw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}_Requester::NoOs_{service.ServiceName}(RequestParameter* rp)\n{{");
            }



            csw.WriteLine("//write serialization code:{0}({1})", service.ServiceName, temp_fileds);
            noOsCsw.WriteLine("//write serialization code:{0}({1})", service.ServiceName, temp_fileds);

            csw.WriteLine($"SerializationManager sm;");
            noOsCsw.WriteLine($"SerializationManager sm;");

            csw.WriteLine("El_Memset(&sm,0,sizeof(SerializationManager));");
            noOsCsw.WriteLine("El_Memset(&sm,0,sizeof(SerializationManager));");

            csw.WriteLine($"auto result=false;");
            noOsCsw.WriteLine($"auto result=false;");

            csw.WriteLine($"uint8_t* buffer = nullptr;");
            noOsCsw.WriteLine($"uint8_t* buffer = nullptr;");

            csw.WriteLine($"uint32_t bufferLen = 0;");
            noOsCsw.WriteLine($"uint32_t bufferLen = 0;");
            noOsCsw.WriteLine("BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);");

            if (service.ReturnStructType.TargetFields.Count > 1)//index 0 is state. 1 is returnfield
            {
                csw.WriteLine($"auto waitstate=ResponseState_Timeout;");
                //noOsCsw.WriteLine($"auto waitstate=ResponseState_Timeout;");
            }

            csw.WriteLine($"if (rp->IsProvideBuffer == false)");
            csw.WriteLine("{");
            noOsCsw.WriteLine($"if (rp->IsProvideBuffer == false)");
            noOsCsw.WriteLine("{");

            csw.WriteLine("if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)");
            csw.WriteLine("{");
            csw.WriteLine("El_TakeMutex(RpcObject->DataLinkBufferForRequest.MutexHandle, El_WAIT_FOREVER,rp->IsIsr);");
            csw.WriteLine("}");
            csw.WriteLine("if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)\r\n{");
            csw.WriteLine("BlockRingBufferProvider_Reset(RpcObject->BlockBufferProviderOfRequestService,rp->IsIsr);");
            csw.WriteLine("}\r\nelse\r\n{");
            csw.WriteLine("El_ResetQueue(RpcObject->MessageQueueOfRequestService,rp->IsIsr);");

            csw.WriteLine("}");

            csw.WriteLine("buffer = RpcObject->DataLinkBufferForRequest.Buffer;");
            csw.WriteLine("bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;");
            noOsCsw.WriteLine("buffer = RpcObject->DataLinkBufferForRequest.Buffer;");
            noOsCsw.WriteLine("bufferLen = RpcObject->DataLinkBufferForRequest.BufferLen;");

            csw.WriteLine("}");
            noOsCsw.WriteLine("}");

            csw.WriteLine("else");
            csw.WriteLine("{");
            csw.WriteLine("buffer = rp->Buffer;");
            csw.WriteLine("bufferLen = rp->BufferLen;");
            csw.WriteLine("}");

            noOsCsw.WriteLine("else");
            noOsCsw.WriteLine("{");
            noOsCsw.WriteLine("buffer = rp->Buffer;");
            noOsCsw.WriteLine("bufferLen = rp->BufferLen;");
            noOsCsw.WriteLine("}");


            csw.WriteLine("sm.Index=0;\n" +
                "sm.Buf = &buffer[4+4+4];\n" +
                "sm.BufferLen = bufferLen-4-4-4;");
            
            noOsCsw.WriteLine($"RpcObject->CurrentRequestSid={service.ServiceName}_ServiceId;");
            
            noOsCsw.WriteLine("sm.Index=0;\n" +
                "sm.Buf = &buffer[4+4+4];\n" +
                "sm.BufferLen = bufferLen-4-4-4;");

            if (service.ExternalParameter.IsExternal == false)
            {
                foreach (var field in service.ParameterStructType.TargetFields)
                {
                    if(field.TargetType.TargetType==typeof(Array))//if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                    {
                        Array_TargetField array_TargetField = field as Array_TargetField;
                        var lenField = array_TargetField.ArrayLenField;
                        if (lenField != null)
                        {
                            if (array_TargetField.MaxCountAttribute.IsFixed == true)
                            {
                                var index_name=CppCsNanoSerializer.GetCppTypeDefineName(lenField.TargetType);
                                csw.WriteLine($"for({index_name} index=0;index<{lenField.FieldName};index++)");
                                noOsCsw.WriteLine($"for({index_name} index=0;index<{lenField.FieldName};index++)");
                                csw.WriteLine("{");
                                noOsCsw.WriteLine("{");
                                csw.WriteLine($"  {service.ServiceName}_SendData.{field.FieldName}[index]={field.FieldName}[index];");
                                noOsCsw.WriteLine($"  {service.ServiceName}_SendData.{field.FieldName}[index]={field.FieldName}[index];");
                                csw.WriteLine("}");
                                noOsCsw.WriteLine("}");
                            }
                            else
                            {
                                csw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}={field.FieldName};");
                                noOsCsw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}={field.FieldName};");
                            }
                        }
                        else
                        {
                            if (array_TargetField.MaxCountAttribute.IsFixed == true)
                            {
                                csw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}[0]={field.FieldName}[0];");
                                noOsCsw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}[0]={field.FieldName}[0];");
                            }
                            else
                            {
                                csw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}={field.FieldName};");
                                noOsCsw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}={field.FieldName};");
                            }
                        }
                    }
                    else
                    {
                        csw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}={field.FieldName};");
                        noOsCsw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}={field.FieldName};");
                    }

                }
            }
            csw.WriteLine($"{service.ParameterStructType.TypeName}_Serialize(&sm,&{service.ServiceName}_SendData);");
            noOsCsw.WriteLine($"{service.ParameterStructType.TypeName}_Serialize(&sm,&{service.ServiceName}_SendData);");

            csw.WriteLine($"buffer[0]=(uint8_t)({service.ServiceName}_ServiceId&0xff);");
            noOsCsw.WriteLine($"buffer[0]=(uint8_t)({service.ServiceName}_ServiceId&0xff);");

            csw.WriteLine($"buffer[1]=(uint8_t)({service.ServiceName}_ServiceId>>8&0xff);");
            noOsCsw.WriteLine($"buffer[1]=(uint8_t)({service.ServiceName}_ServiceId>>8&0xff);");

            csw.WriteLine($"buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);");
            noOsCsw.WriteLine($"buffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);");

            csw.WriteLine($"buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);");
            noOsCsw.WriteLine($"buffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);");

            csw.WriteLine($"buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);\n");
            noOsCsw.WriteLine($"buffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);\n");

            csw.WriteLine($"buffer[4]=(uint8_t)(sm.Index&0xff);");
            noOsCsw.WriteLine($"buffer[4]=(uint8_t)(sm.Index&0xff);");

            csw.WriteLine($"buffer[5]=(uint8_t)(sm.Index>>8&0xff);");
            noOsCsw.WriteLine($"buffer[5]=(uint8_t)(sm.Index>>8&0xff);");

            csw.WriteLine($"buffer[6]=(uint8_t)(sm.Index>>16&0xff);");
            noOsCsw.WriteLine($"buffer[6]=(uint8_t)(sm.Index>>16&0xff);");

            csw.WriteLine($"buffer[7]=(uint8_t)(sm.Index>>24&0xff);\n");
            noOsCsw.WriteLine($"buffer[7]=(uint8_t)(sm.Index>>24&0xff);\n");


            csw.WriteLine($"uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);");
            noOsCsw.WriteLine($"uint32_t bufcrc=GetBufferCrc(sm.Index,sm.Buf);");

            csw.WriteLine($"buffer[8]=(uint8_t)(bufcrc&0xff);");
            noOsCsw.WriteLine($"buffer[8]=(uint8_t)(bufcrc&0xff);");

            csw.WriteLine($"buffer[9]=(uint8_t)(bufcrc>>8&0xff);");
            noOsCsw.WriteLine($"buffer[9]=(uint8_t)(bufcrc>>8&0xff);");

            csw.WriteLine($"buffer[10]=(uint8_t)(bufcrc>>16&0xff);");
            noOsCsw.WriteLine($"buffer[10]=(uint8_t)(bufcrc>>16&0xff);");

            csw.WriteLine($"buffer[11]=(uint8_t)(bufcrc>>24&0xff);\n");
            noOsCsw.WriteLine($"buffer[11]=(uint8_t)(bufcrc>>24&0xff);\n");

            csw.WriteLine("RequestTick=El_GetTick(rp->IsIsr);");
            noOsCsw.WriteLine("RequestTick=El_GetTick(rp->IsIsr);");

            csw.WriteLine($"result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);");
            noOsCsw.WriteLine($"result=RpcObject->Send(rp,RpcObject,sm.Index+4+4+4,buffer);");

            csw.WriteLine("sm.Index=0;");
            noOsCsw.WriteLine("sm.Index=0;");


            csw.WriteLine($"if(result==false)\n{{\n{service.ServiceName}_reqresp.State=RequestState_Failed;\n}}");
            noOsCsw.WriteLine($"if(result==false)\n{{\n{service.ServiceName}_reqresp.State=RequestState_Failed;\n}}");

            csw.WriteLine("else\n{");
            noOsCsw.WriteLine("else\n{");

            csw.WriteLine($"{service.ServiceName}_reqresp.State=RequestState_Ok;");
            noOsCsw.WriteLine($"{service.ServiceName}_reqresp.State=RequestState_Ok;");

            if (service.ReturnStructType.TargetFields.Count > 1)
            {

                csw.WriteLine("ReceiveItemInfo recData;");
                csw.WriteLine($"waitstate=RpcObject->Wait({service.ServiceName}_ServiceId,&recData,rp->IsIsr);");
                csw.WriteLine("if(waitstate == RequestResponseState::ResponseState_Ok)");
                csw.WriteLine("{");

                csw.WriteLine("if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==true)\r\n{");
                csw.WriteLine("sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;");
                csw.WriteLine("}\r\nelse\r\n{");
                csw.WriteLine("sm.Index=0;");
                csw.WriteLine("sm.BufferLen = recData.DataLen;");
                csw.WriteLine("sm.Buf = recData.Data;");
                csw.WriteLine("}");
                //csw.WriteLine($"if(RpcObject->RpcConfig.CheckSumValid==true)\r\n{{");
                //csw.WriteLine("SerializationManager_SetCalculateSum(&sm,0);");
                //csw.WriteLine("SerializationManager_SetReferenceSum(&sm,recData.CheckSum);");
                //csw.WriteLine("}");
                csw.WriteLine($"{service.ReturnStructType.TypeName}_Deserialize(&sm,&{service.ServiceName}_reqresp,rp->IsIsr);");
                //csw.WriteLine($"if(RpcObject->RpcConfig.CheckSumValid==true)\r\n{{");
                //csw.WriteLine("El_Assert(SerializationManager_GetReferenceSum(&sm)==SerializationManager_GetCalculateSum(&sm));");
                //csw.WriteLine("}");
                csw.WriteLine("}");
                csw.WriteLine("if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)\r\n{");
                //csw.WriteLine("if(waitstate != RequestResponseState::ResponseState_Timeout)");
                //csw.WriteLine("{");

                csw.WriteLine("if (recData.DataLen > 0)");
                csw.WriteLine("{");
                csw.WriteLine("El_Free(recData.Data);");
                csw.WriteLine("}");

                //csw.WriteLine("}");
                csw.WriteLine("}//if(RpcObject->RpcConfig.UseRingBufferWhenReceiving==false)");

                csw.WriteLine($"{service.ServiceName}_reqresp.State=waitstate;");
            }
            csw.WriteLine("}");
            noOsCsw.WriteLine("}");

            csw.WriteLine("if (rp->IsProvideBuffer == false)");
            csw.WriteLine("{");

            csw.WriteLine("if(RpcObject->DataLinkBufferForRequest.MutexHandle!=nullptr)");
            csw.WriteLine("{");
            csw.WriteLine("El_ReleaseMutex(RpcObject->DataLinkBufferForRequest.MutexHandle,rp->IsIsr);");
            csw.WriteLine("}");

            csw.WriteLine("}");

            csw.WriteLine($"return {service.ServiceName}_reqresp;");
            noOsCsw.WriteLine($"return {service.ServiceName}_reqresp;");

            csw.WriteLine("}");
            noOsCsw.WriteLine("}");


            if (service.ReturnStructType.TargetFields.Count > 1)
            {
                hsw.WriteLine($"void Free_{service.ServiceName}({service.ReturnStructType.TypeName} *response);\n");

                csw.WriteLine($"void {service.ServiceName}_Requester::Free_{service.ServiceName}({service.ReturnStructType.TypeName} *response)");

                csw.WriteLine("{\nif(response->State==ResponseState_Ok)\n{");

                csw.WriteLine($"{ReturnStructFreeNote}{service.ReturnStructType.TypeName}_FreeData(response);");

                csw.WriteLine("}\n}");
            }


            csw.WriteLine();
            noOsCsw.WriteLine();

            //裸机实现
            if (service.ReturnStructType.TargetFields.Count > 1)
            {
                noOsCsw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}_Requester::NoOs_QueryServiceState(RequestParameter* rp)\n{{");
                noOsCsw.WriteLine($"SerializationManager sm;");
                noOsCsw.WriteLine("El_Memset(&sm,0,sizeof(SerializationManager));");
                noOsCsw.WriteLine($"uint32_t nowTick = El_GetTick(rp->IsIsr);");
                noOsCsw.WriteLine("ReceiveItemInfo currentReceivedResponseData;");
                noOsCsw.WriteLine("memset(&currentReceivedResponseData,0,sizeof(currentReceivedResponseData));");

                noOsCsw.WriteLine("if((RequestTick+RpcObject->TimeOut)<nowTick)\n{");
                noOsCsw.WriteLine($"{service.ServiceName}_reqresp.State = RequestResponseState::ResponseState_Timeout;");
                noOsCsw.WriteLine($"return {service.ServiceName}_reqresp;");
                noOsCsw.WriteLine("}");

                noOsCsw.WriteLine("if(BlockRingBufferProvider_Receive(RpcObject->BlockBufferProviderOfRequestService, &currentReceivedResponseData,0,rp->IsIsr) == False)\n{");
                noOsCsw.WriteLine($"{service.ServiceName}_reqresp.State = RequestResponseState::ResponseState_NoReceived;");
                noOsCsw.WriteLine($"return {service.ServiceName}_reqresp;");
                noOsCsw.WriteLine("}");

                noOsCsw.WriteLine("if(currentReceivedResponseData.Sid == EmbedXrpcNoReceivedSid)\n{");
                noOsCsw.WriteLine($"{service.ServiceName}_reqresp.State = RequestResponseState::ResponseState_NoReceived;");
                noOsCsw.WriteLine("}");

                noOsCsw.WriteLine($"else if (currentReceivedResponseData.Sid == {service.ServiceName}_ServiceId)\n{{");
                noOsCsw.WriteLine($"sm.Index=0;");
                noOsCsw.WriteLine($"sm.BlockBufferProvider = RpcObject->BlockBufferProviderOfRequestService;");
                noOsCsw.WriteLine($"{service.ReturnStructType.TypeName}_Deserialize(&sm,&{service.ServiceName}_reqresp,rp->IsIsr);");
                noOsCsw.WriteLine($"{service.ServiceName}_reqresp.State = RequestResponseState::ResponseState_Ok;");
                noOsCsw.WriteLine("}");

                noOsCsw.WriteLine($"else if (currentReceivedResponseData.Sid == EmbedXrpcSuspendSid)\n{{");
                noOsCsw.WriteLine($"{service.ServiceName}_reqresp.State = RequestResponseState::ResponseState_NoReceived;");
                noOsCsw.WriteLine($"RequestTick=nowTick;");
                noOsCsw.WriteLine("}");

                noOsCsw.WriteLine("else\n{");
                noOsCsw.WriteLine($"{service.ServiceName}_reqresp.State = RequestResponseState::ResponseState_NoReceived;");
                noOsCsw.WriteLine("}");
                noOsCsw.WriteLine($"return {service.ServiceName}_reqresp;");

                noOsCsw.WriteLine("}");//NoOs_QueryServiceState
            }
            

            noOsCsw.Flush();
            noOsHsw.Flush();

            csw.Flush();
            hsw.Flush();

            noOsCsw.BaseStream.Position = 0;
            noOsHsw.BaseStream.Position = 0;
            noOsCsw.BaseStream.CopyTo(csw.BaseStream);
            noOsHsw.BaseStream.CopyTo(hsw.BaseStream);

            hsw.WriteLine("};\n");//class end

            CppSerializableCommon.MacroControlWriteEnd(csw, service.MacroControlAttribute);
            CppSerializableCommon.MacroControlWriteEnd(hsw, service.MacroControlAttribute);
        }

        private void EmitCallee(TargetService service, StreamWriter hsw, StreamWriter csw)
        {
            string ParameterStructFreeNote = service.ParameterStructType.IsNeedFreeMemoryForNativeLanguage == true ? "" : "//!";
            string ReturnStructFreeNote = service.ReturnStructType.IsNeedFreeMemoryForNativeLanguage == true ? "" : "//!";
            CppSerializableCommon.MacroControlWriteBegin(hsw, service.MacroControlAttribute);
            CppSerializableCommon.MacroControlWriteBegin(csw, service.MacroControlAttribute);
            hsw.WriteLine($"class {service.ServiceName}_Service:public IService");
            hsw.WriteLine("{\npublic:");
            hsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", service.ServiceName);
            if (service.ReturnStructType.TargetFields.Count > 1)
            {
                hsw.WriteLine($"{service.ReturnStructType.TypeName} Response;");
            }

            if(service.NoDeserializeParameterAttribute!=null)
            {
                hsw.WriteLine($"virtual void {service.ServiceName}(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager)=0;");
            }
            else
            {
                string returnType = "void";
                var dh = service.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                hsw.Write($"virtual {returnType} {service.ServiceName}(ServiceInvokeParameter * serviceInvokeParameter{dh}");

                var temp_fileds = string.Empty;
                for (int i = 0; i < service.ParameterStructType.TargetFields.Count; i++)
                {
                    var field = service.ParameterStructType.TargetFields[i];
                    string name = field.FieldName;
                    temp_fileds += name + ",";

                    hsw.Write($"{CppSerializableCommon.EmitField(field)}");

                    if (i + 1 < service.ParameterStructType.TargetFields.Count)
                    {
                        hsw.Write(",");
                    }
                }
                hsw.WriteLine(")=0;");
            }
           

            //code gen invoke
            hsw.WriteLine($"{service.ParameterStructType.TypeName} request;");
            hsw.WriteLine("void Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager);");
            csw.WriteLine($"void {service.ServiceName}_Service::Invoke(ServiceInvokeParameter * serviceInvokeParameter,SerializationManager *recManager, SerializationManager* sendManager)");
            csw.WriteLine("{");

            if(service.NoDeserializeParameterAttribute!=null)
            {
                csw.WriteLine($"{service.ServiceName}(serviceInvokeParameter,recManager);");
            }
            else
            {
                csw.WriteLine($"{service.ParameterStructType.TypeName}_Deserialize(recManager,&request,serviceInvokeParameter->IsIsr);");

                //csw.WriteLine($"if(serviceInvokeParameter->RpcObject->RpcConfig.CheckSumValid==true)\r\n{{");
                //csw.WriteLine($"El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));");
                //csw.WriteLine("}");

                var dh = service.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                csw.Write($"{service.ServiceName}(serviceInvokeParameter{dh}");
                for (int i = 0; i < service.ParameterStructType.TargetFields.Count; i++)
                {
                    var par = service.ParameterStructType.TargetFields[i];
                    csw.Write($"request.{par.FieldName}");
                    if (i + 1 < service.ParameterStructType.TargetFields.Count)
                    {
                        csw.Write(",");
                    }
                }
                csw.WriteLine(");");//生成调用目标函数。
            }
            

            if(service.NoDeserializeParameterAttribute==null)
                csw.WriteLine($"{ParameterStructFreeNote}{service.ParameterStructType.TypeName}_FreeData(&request);");//free
            if (service.ReturnStructType.TargetFields.Count > 1)
            {
                csw.WriteLine($"{service.ReturnStructType.TypeName}_Serialize(sendManager,&Response);");
                //2021.3.10 用户malloc的空间 可以选择free,有用户控制所以有了这条语句
                csw.WriteLine($"if(IsFreeResponse==true){{");
                csw.WriteLine($"{ReturnStructFreeNote}{service.ReturnStructType.TypeName}_FreeData(&Response);");
                csw.WriteLine("}");
            }

            csw.WriteLine("}");//end function
            hsw.WriteLine("};");//end class

            CppSerializableCommon.MacroControlWriteEnd(csw, service.MacroControlAttribute);
            CppSerializableCommon.MacroControlWriteEnd(hsw, service.MacroControlAttribute);

            csw.WriteLine(Environment.NewLine);
            hsw.WriteLine(Environment.NewLine);
        }
        public void EmitService(TargetService service, CppCodeGenParameter cppCodeGenParameter)
        {
            //if (service.RoleAttribute.Role == RoleType.Client)
            { 
                if(cppCodeGenParameter.RoleType== RoleType.Client)
                {
                    EmitCaller(service, ClientHsw, ClientCsw);
                }
                else if(cppCodeGenParameter.RoleType == RoleType.Server)
                {
                    EmitCallee(service, ServerHsw, ServerCsw);
                }
                else
                {
                    EmitCaller(service, ClientHsw, ClientCsw);
                    EmitCallee(service, ServerHsw, ServerCsw);
                }

            }

            //if (service.RoleAttribute.Role == RoleType.Server)
            //{
            //    if (cppCodeGenParameter.RoleType == RoleType.Client)
            //    {
            //        EmitCallee(service, ClientHsw, ClientCsw);
            //    }
            //    else if (cppCodeGenParameter.RoleType == RoleType.Server)
            //    {
            //        EmitCaller(service, ServerHsw, ServerCsw);
            //    }
            //    else
            //    {
            //        EmitCaller(service, ServerHsw, ServerCsw);
            //        EmitCallee(service, ClientHsw, ClientCsw);
            //    }
            //}
        }
    }
}
