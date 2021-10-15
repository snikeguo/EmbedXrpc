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
        public void CodeGen(CppCodeGenParameter cppCodeGenParameter)
        {
            var encode = Encoding.UTF8;
            codeGenParameter = cppCodeGenParameter;
            if(embedXrpcSerializationGenerator==null)
            {
                embedXrpcSerializationGenerator = new CppNanoSerializer();
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
            if(cppCodeGenParameter.FileIdlInfo.TargetDelegates.Count>0|| cppCodeGenParameter.FileIdlInfo.TargetInterfaces.Count>0)
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

            

            

            if(cppCodeGenParameter.GenType== GenType.Client|| cppCodeGenParameter.GenType == GenType.All)
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


            if (cppCodeGenParameter.GenType == GenType.Server || cppCodeGenParameter.GenType == GenType.All)
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
            foreach (var del in cppCodeGenParameter.FileIdlInfo.TargetDelegates)
            {
                //fbsStreamWriter.WriteLine(del.ToFbs().ToString());
                /*TargetStruct targetStruct = new TargetStruct();
                targetStruct.Name = del.MethodName+"Struct";
                targetStruct.TargetFields = del.TargetFields;*/
                EmitStruct(del.ParameterStructType, cppCodeGenParameter);
                EmitDelegate(del);
                /*embedXrpcSerializationGenerator.EmitStruct(del.ParameterStructType,
                    SerializeCsw,
                    SerializeHsw);*/
                AddMessageMap(del.MethodName, ReceiveType_t.ReceiveType_Delegate);
                EmitServiceIdCode(SerializeHsw, del.MethodName, del.ServiceId);//生成 ServiceID 宏定义

            }

            foreach (var ifs in cppCodeGenParameter.FileIdlInfo.TargetInterfaces)
            {
                //fbsStreamWriter.WriteLine(ifs.ToFbs().ToString());
                EmitClientInterface(ifs,cppCodeGenParameter);
                
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
            if (cppCodeGenParameter.GenType == GenType.Client || cppCodeGenParameter.GenType == GenType.All)
            {
                ClientHsw.WriteLine("\n#endif");
                ClientHsw.Flush();
                ClientHsw.Close();

                ClientCsw.Flush();
                ClientCsw.Close();
            }

            if (cppCodeGenParameter.GenType == GenType.Server || cppCodeGenParameter.GenType == GenType.All)
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
        private void MacroControlWriteBegin(StreamWriter sw, MacroControlAttribute MacroControlAttribute)
        {
            if (MacroControlAttribute != null)
            {
                string val = MacroControlAttribute.EnableCondition == string.Empty ? string.Empty : $"=={MacroControlAttribute.EnableCondition}";
                if(val!= string.Empty)
                    sw.WriteLine($"#if {MacroControlAttribute.MacroName}{val}");
                else
                    sw.WriteLine($"#ifdef {MacroControlAttribute.MacroName}");
            }
        }
        private void MacroControlWriteEnd(StreamWriter sw, MacroControlAttribute MacroControlAttribute)
        {
            if (MacroControlAttribute != null)
            {
                string val = MacroControlAttribute.EnableCondition == string.Empty ? string.Empty : $"=={MacroControlAttribute.EnableCondition}";
                if (val != string.Empty)
                    sw.WriteLine($"#endif // #if {MacroControlAttribute.MacroName}{val}");
                else
                    sw.WriteLine($"#endif // #ifdef {MacroControlAttribute.MacroName}");
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
            if (codeGenParameter.GenType == GenType.Client || codeGenParameter.GenType == GenType.All)
            {
                MacroControlWriteBegin(ClientHsw, targetDelegate.MacroControlAttribute);
                ClientHsw.WriteLine($"class {targetDelegate.MethodName}ClientImpl:public IDelegate");
                ClientHsw.WriteLine("{\npublic:");
                ClientHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", targetDelegate.MethodName);

                var dh = targetDelegate.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                ClientHsw.Write($"void {targetDelegate.MethodName}(UserDataOfTransportLayer_t* userDataOfTransportLayer{dh}");

                
                for (int i = 0; i < targetDelegate.ParameterStructType.TargetFields.Count; i++)
                {
                    var field = targetDelegate.ParameterStructType.TargetFields[i];
                    string name = field.FieldName;
                    temp_fileds += name + ",";

                    ClientHsw.Write($"{CppSerializableCommon.EmitField(field)}");
                    if (i + 1 < targetDelegate.ParameterStructType.TargetFields.Count)
                    {
                        ClientHsw.Write(",");
                    }
                }
                ClientHsw.WriteLine(");");

                //code gen invoke
                ClientHsw.WriteLine($"{targetDelegate.ParameterStructType.TypeName} request;");
                ClientHsw.WriteLine("void Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,SerializationManager *recManager);");//声明

                MacroControlWriteBegin(ClientCsw, targetDelegate.MacroControlAttribute);
                ClientCsw.WriteLine($"void {targetDelegate.MethodName}ClientImpl::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer,SerializationManager *recManager)");
                ClientCsw.WriteLine("{");
                //ClientCsw.WriteLine($"static {targetDelegate.ParameterStructType.TypeName} request;");

                //ClientCsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Deserialize(recManager,&request);");
                //ClientCsw.WriteLine($"recManager.Deserialize(&{targetDelegate.ParameterStructType.TypeName}_TypeInstance,&request);");
                ClientCsw.WriteLine($"{targetDelegate.ParameterStructType.TypeName}_Deserialize(recManager,&request);");
                ClientCsw.WriteLine("#if EmbedXrpc_CheckSumValid==1");
                ClientCsw.WriteLine($"El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));");
                ClientCsw.WriteLine("#endif");
                dh = targetDelegate.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                ClientCsw.Write($"{targetDelegate.MethodName}(userDataOfTransportLayer{dh}");
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
                //ClientCsw.WriteLine($"SerializationManager::FreeData(&{targetDelegate.ParameterStructType.TypeName}_TypeInstance,&request);");
                ClientCsw.WriteLine($"{targetDelegate.ParameterStructType.TypeName}_FreeData(&request);");//free掉request

                ClientCsw.WriteLine("}");//函数生成完毕
                MacroControlWriteEnd(ClientCsw, targetDelegate.MacroControlAttribute);

                ClientHsw.WriteLine("};");//end class

                MacroControlWriteEnd(ClientHsw, targetDelegate.MacroControlAttribute);

                ClientHsw.WriteLine(Environment.NewLine);
                ClientCsw.WriteLine(Environment.NewLine);
                //ClientCsw.WriteLine($"{targetDelegate.MethodName}ClientImpl {targetDelegate.MethodName}ClientImplc");//创建一个委托实例
            }
            if (codeGenParameter.GenType == GenType.Server || codeGenParameter.GenType == GenType.All)
            {
                //生成服务端代码
                MacroControlWriteBegin(ServerHsw, targetDelegate.MacroControlAttribute);
                MacroControlWriteBegin(ServerCsw, targetDelegate.MacroControlAttribute);
                ServerHsw.WriteLine("class " + targetDelegate.MethodName + "Delegate");
                ServerHsw.WriteLine("{\npublic:\nEmbedXrpcObject *RpcObject=nullptr;");
                ServerHsw.WriteLine(targetDelegate.MethodName + "Delegate" + "(EmbedXrpcObject *rpcobj):RpcObject(rpcobj)");
                ServerHsw.WriteLine("{}");
                ServerHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", targetDelegate.MethodName);

                ServerHsw.WriteLine($"{targetDelegate.ParameterStructType.TypeName} SendData;");
                if (targetDelegate.ExternalParameter.IsExternal==false)
                {
                    var dh = targetDelegate.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                    ServerHsw.Write($"void  Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer{dh}");
                    ServerCsw.Write($"void  {targetDelegate.MethodName }Delegate::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer{dh}");
                    temp_fileds = string.Empty;
                    for (int i = 0; i < targetDelegate.ParameterStructType.TargetFields.Count; i++)
                    {
                        var field = targetDelegate.ParameterStructType.TargetFields[i];
                        string name = field.FieldName;
                        temp_fileds += name + ",";

                        ServerHsw.Write($"{CppSerializableCommon.EmitField(field)}");
                        ServerCsw.Write($"{CppSerializableCommon.EmitField(field)}");

                        if (i + 1 < targetDelegate.ParameterStructType.TargetFields.Count)
                        {
                            ServerHsw.Write(",");
                            ServerCsw.Write(",");
                        }
                    }
                    ServerHsw.WriteLine(");");//函数名
                    ServerCsw.WriteLine(")\n{");//函数名
                }
                else
                {
                    ServerHsw.Write($"void  Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer);");
                    ServerCsw.Write($"void  {targetDelegate.MethodName }Delegate::Invoke(UserDataOfTransportLayer_t* userDataOfTransportLayer)\n{{");
                }

                //函数实现
                ServerCsw.WriteLine("//write serialization code:{0}({1})", targetDelegate.MethodName, temp_fileds);
                //ServerCsw.WriteLine($"static {targetDelegate.ParameterStructType.TypeName} SendData;");
                ServerCsw.WriteLine($"SerializationManager sm;");
                ServerCsw.WriteLine("El_Memset(&sm,0,sizeof(SerializationManager));");
                //ServerCsw.WriteLine($"sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;");
                ServerCsw.WriteLine("El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);");
                ServerCsw.WriteLine("SerializationManager_Reset(&sm);\n" +
                        "sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];\n" +
                        "sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;");

                if(targetDelegate.ExternalParameter.IsExternal==false)//Copy to SendData
                {
                    foreach (var field in targetDelegate.ParameterStructType.TargetFields)
                    {
                        //if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                        if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                        {
                            Array_TargetField array_TargetField = field as Array_TargetField;
                            var lenField = array_TargetField.ArrayLenField;
                            if (lenField != null)
                            {
                                if (array_TargetField.MaxCountAttribute.IsFixed == true)
                                {
                                    ServerCsw.WriteLine($"for({lenField.TargetType.TypeName} index=0;index<{lenField.FieldName};index++)");
                                    ServerCsw.WriteLine("{");
                                    ServerCsw.WriteLine($"  SendData.{field.FieldName}[index]={field.FieldName}[index];");
                                    ServerCsw.WriteLine("}");
                                }
                                else
                                {
                                    ServerCsw.WriteLine($"SendData.{field.FieldName}={field.FieldName};");
                                }
                            }
                            else
                            {
                                ServerCsw.WriteLine($"SendData.{field.FieldName}[0]={field.FieldName}[0];");
                            }
                            //ServerHsw.WriteLine($"memcpy(SendData.{field.Name},{field.Name},sizeof(SendData.{field.Name})/sizeof({arrayelementtype}));");

                        }
                        else
                        {
                            //ServerHsw.WriteLine($"memcpy(&SendData.{field.Name},&{field.Name},sizeof(SendData.{field.Name}));");
                            ServerCsw.WriteLine($"SendData.{field.FieldName}={field.FieldName};");
                        }
                    }
                }
                
                //ServerCsw.WriteLine($"{targetDelegate.MethodName}Struct_Type.Serialize(sm,0,&SendData);");
                //ServerCsw.WriteLine($"sm.Serialize(&{targetDelegate.ParameterStructType.TypeName}_TypeInstance,&SendData,0);");
                ServerCsw.WriteLine($"{targetDelegate.ParameterStructType.TypeName}_Serialize(&sm,&SendData);");

                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)({targetDelegate.MethodName}_ServiceId&0xff);");
                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)({targetDelegate.MethodName}_ServiceId>>8&0xff);");
                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);");
                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);");
                ServerCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Delegate)<<6);");
                ServerCsw.WriteLine($"RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);");
                ServerCsw.WriteLine("SerializationManager_Reset(&sm);");
                ServerCsw.WriteLine("El_ReleaseMutex(RpcObject->ObjectMutexHandle);");
                ServerCsw.WriteLine("}");//function end
                MacroControlWriteEnd(ServerCsw, targetDelegate.MacroControlAttribute);

                ServerHsw.WriteLine("};");//class end

                MacroControlWriteEnd(ServerHsw, targetDelegate.MacroControlAttribute);

                ServerHsw.WriteLine(Environment.NewLine);
                ServerCsw.WriteLine(Environment.NewLine);
            }

                
        }
        public void EmitClientInterface(TargetInterface targetInterface, CppCodeGenParameter cppCodeGenParameter)
        {
            foreach (var service in targetInterface.Services)
            {
                //string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                /*TargetStruct targetStructRequest = new TargetStruct();
                targetStructRequest.Name = GeneratServiceName + "_Request";
                targetStructRequest.TargetFields.AddRange(service.TargetFields);*/
                /*embedXrpcSerializationGenerator.EmitStruct(service.ParameterStructType,
                    SerializeCsw,
                    SerializeHsw);*/
                EmitStruct(service.ParameterStructType, cppCodeGenParameter);
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

                /*embedXrpcSerializationGenerator.EmitStruct(service.ReturnStructType,
                    SerializeCsw,
                    SerializeHsw);*/

                EmitStruct(service.ReturnStructType,cppCodeGenParameter);

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
                string temp_fileds = string.Empty;
                foreach (var service in targetInterface.Services)
                {
                    MacroControlWriteBegin(ClientHsw, service.MacroControlAttribute);
                    MacroControlWriteBegin(ClientCsw, service.MacroControlAttribute);
                    ClientHsw.WriteLine($"{service.ParameterStructType.TypeName} {service.ServiceName}_SendData;");
                    ClientHsw.WriteLine($"{service.ReturnStructType.TypeName} {service.ServiceName}_reqresp;");
                    //string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                    if (service.ExternalParameter.IsExternal ==false)
                    {
                        var dh = service.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                        ClientHsw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}(UserDataOfTransportLayer_t* userDataOfTransportLayer{dh}");
                        ClientCsw.Write($"{service.ReturnStructType.TypeName}& {targetInterface.Name}ClientImpl::{service.ServiceName}(UserDataOfTransportLayer_t* userDataOfTransportLayer{dh}");
                        temp_fileds = string.Empty;
                        for (int i = 0; i < service.ParameterStructType.TargetFields.Count; i++)
                        {
                            var field = service.ParameterStructType.TargetFields[i];
                            string name = field.FieldName;
                            temp_fileds += name + ",";

                            ClientHsw.Write($"{CppSerializableCommon.EmitField(field)}");
                            ClientCsw.Write($"{CppSerializableCommon.EmitField(field)}");

                            if (i + 1 < service.ParameterStructType.TargetFields.Count)
                            {
                                ClientHsw.Write(",");
                                ClientCsw.Write(",");
                            }
                        }
                        ClientHsw.WriteLine(");");
                        ClientCsw.WriteLine(")\n{");
                    }
                    else
                    {
                        ClientHsw.Write($"{service.ReturnStructType.TypeName}& {service.ServiceName}(UserDataOfTransportLayer_t* userDataOfTransportLayer);");
                        ClientCsw.Write($"{service.ReturnStructType.TypeName}& {targetInterface.Name}ClientImpl::{service.ServiceName}(UserDataOfTransportLayer_t* userDataOfTransportLayer)\n{{");
                    }

                    

                    ClientCsw.WriteLine("//write serialization code:{0}({1})", service.ServiceName, temp_fileds);

                    //ClientCsw.WriteLine($"static {service.ParameterStructType.TypeName} SendData;");
                    ClientCsw.WriteLine($"SerializationManager sm;");
                    ClientCsw.WriteLine("El_Memset(&sm,0,sizeof(SerializationManager));");
                    //ClientCsw.WriteLine($"sm.IsEnableMataDataEncode=RpcObject->IsEnableMataDataEncode;");
                    //ClientCsw.WriteLine($"static {service.ReturnStructType.TypeName} reqresp;");
                    ClientCsw.WriteLine($"auto result=false;");

                    if (service.ReturnStructType.TargetFields.Count>1)//index 0 is state. 1 is returnfield
                    {
                        ClientCsw.WriteLine($"auto waitstate=ResponseState_Timeout;");
                    }
                    ClientCsw.WriteLine("El_TakeMutex(RpcObject->ObjectMutexHandle, El_WAIT_FOREVER);");

                    ClientCsw.WriteLine("#if EmbedXrpc_UseRingBufferWhenReceiving==1");
                    //ClientCsw.WriteLine("RpcObject->ResponseBlockBufferProvider->Reset();");
                    ClientCsw.WriteLine("BlockRingBufferProvider_Reset(RpcObject->ResponseBlockBufferProvider);");
                    ClientCsw.WriteLine("#else");
                    ClientCsw.WriteLine("El_ResetQueue(RpcObject->ResponseBlockQueue);");
                    ClientCsw.WriteLine("#endif");
                    //ClientCsw.WriteLine("ResetSemaphore(RpcObject->ResponseMessageSemaphoreHandle);");
                    ClientCsw.WriteLine("SerializationManager_Reset(&sm);\n" +
                        "sm.Buf = &RpcObject->DataLinkLayoutBuffer[4];\n" +
                        "sm.BufferLen = RpcObject->DataLinkLayoutBufferLen-4;");

                    if(service.ExternalParameter.IsExternal==false)
                    {
                        foreach (var field in service.ParameterStructType.TargetFields)
                        {
                            /*if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                            {
                                string arrayelementtype = field.IdlType.Replace("[", "").Replace("]", "");
                                ClientHsw.WriteLine($"memcpy(SendData.{field.Name},{field.Name},sizeof(SendData.{field.Name})/sizeof({arrayelementtype}));");
                            }
                            else
                            {
                                ClientHsw.WriteLine($"memcpy(&SendData.{field.Name},&{field.Name},sizeof(SendData.{field.Name}));");
                            }*/

                            //if (field.IsArray == true && field.MaxCountAttribute.IsFixed == true)
                            if (field.TargetType.TargetType == TargetType_t.TYPE_ARRAY)
                            {
                                Array_TargetField array_TargetField = field as Array_TargetField;
                                var lenField = array_TargetField.ArrayLenField;
                                if (lenField != null)
                                {
                                    if (array_TargetField.MaxCountAttribute.IsFixed == true)
                                    {
                                        ClientCsw.WriteLine($"for({lenField.TargetType.TypeName} index=0;index<{lenField.FieldName};index++)");
                                        ClientCsw.WriteLine("{");
                                        ClientCsw.WriteLine($"  {service.ServiceName}_SendData.{field.FieldName}[index]={field.FieldName}[index];");
                                        ClientCsw.WriteLine("}");
                                    }
                                    else
                                    {
                                        ClientCsw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}={field.FieldName};");
                                    }
                                }
                                else
                                {
                                    ClientCsw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}[0]={field.FieldName}[0];");
                                }
                                //ServerHsw.WriteLine($"memcpy(SendData.{field.Name},{field.Name},sizeof(SendData.{field.Name})/sizeof({arrayelementtype}));");

                            }
                            else
                            {
                                //ServerHsw.WriteLine($"memcpy(&SendData.{field.Name},&{field.Name},sizeof(SendData.{field.Name}));");
                                ClientCsw.WriteLine($"{service.ServiceName}_SendData.{field.FieldName}={field.FieldName};");
                            }

                        }
                    }
                    
                    //ClientCsw.WriteLine($"{GeneratServiceName}_Request_Type.Serialize(sm,0,&SendData);");
                    //ClientCsw.WriteLine($"sm.Serialize(&{service.ParameterStructType.TypeName}_TypeInstance,&SendData,0);");
                    ClientCsw.WriteLine($"{service.ParameterStructType.TypeName}_Serialize(&sm,&{service.ServiceName}_SendData);");

                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[0]=(uint8_t)({service.FullName}_ServiceId&0xff);");
                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[1]=(uint8_t)({service.FullName}_ServiceId>>8&0xff);");
                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[2]=(uint8_t)(RpcObject->TimeOut>>0&0xff);");
                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[3]=(uint8_t)((RpcObject->TimeOut>>8&0xff)&0x3FF);");
                    ClientCsw.WriteLine($"RpcObject->DataLinkLayoutBuffer[3]|=(uint8_t)((uint8_t)(ReceiveType_Request)<<6);");
                    ClientCsw.WriteLine($"result=RpcObject->Send(userDataOfTransportLayer,RpcObject,sm.Index+4,RpcObject->DataLinkLayoutBuffer);");
                    ClientCsw.WriteLine("SerializationManager_Reset(&sm);");
                    ClientCsw.WriteLine($"if(result==false)\n{{\n{service.ServiceName}_reqresp.State=RequestState_Failed;\ngoto exi;\n}}");
                    ClientCsw.WriteLine($"else\n{{\n{service.ServiceName}_reqresp.State=RequestState_Ok;\n}}");
                    if (service.ReturnStructType.TargetFields.Count>1)
                    {
                        //ClientCsw.WriteLine($"waitstate=RpcObject->Wait({service.FullName}_ServiceId,&{service.ReturnStructType.TypeName}_TypeInstance,&reqresp);");
                        ClientCsw.WriteLine("ReceiveItemInfo recData;");
                        ClientCsw.WriteLine($"waitstate=RpcObject->Wait({service.FullName}_ServiceId,&recData);");
                        ClientCsw.WriteLine("if(waitstate == RequestResponseState::ResponseState_Ok)");
                        ClientCsw.WriteLine("{");
                        ClientCsw.WriteLine("#if  EmbedXrpc_UseRingBufferWhenReceiving==1");
                        ClientCsw.WriteLine("sm.BlockBufferProvider = RpcObject->ResponseBlockBufferProvider;");
                        ClientCsw.WriteLine("#else");
                        //ClientCsw.WriteLine("sm.IsEnableMataDataEncode = RpcObject->IsEnableMataDataEncode;");
                        ClientCsw.WriteLine("SerializationManager_Reset(&sm);");
                        ClientCsw.WriteLine("sm.BufferLen = recData.DataLen;");
                        ClientCsw.WriteLine("sm.Buf = recData.Data;");
                        ClientCsw.WriteLine("#endif");
                        ClientCsw.WriteLine("#if  EmbedXrpc_CheckSumValid==1");
                        ClientCsw.WriteLine("SerializationManager_SetCalculateSum(&sm,0);");
                        ClientCsw.WriteLine("SerializationManager_SetReferenceSum(&sm,recData.CheckSum);");
                        ClientCsw.WriteLine("#endif");
                        ClientCsw.WriteLine($"{service.ReturnStructType.TypeName}_Deserialize(&sm,&{service.ServiceName}_reqresp);");
                        ClientCsw.WriteLine("#if  EmbedXrpc_CheckSumValid==1");
                        ClientCsw.WriteLine("El_Assert(SerializationManager_GetReferenceSum(&sm)==SerializationManager_GetCalculateSum(&sm));");
                        ClientCsw.WriteLine("#endif");
                        ClientCsw.WriteLine("}");

                        ClientCsw.WriteLine("#if  EmbedXrpc_UseRingBufferWhenReceiving==0");
                        ClientCsw.WriteLine("if(waitstate != RequestResponseState::ResponseState_Timeout)");
                        ClientCsw.WriteLine("{");

                        ClientCsw.WriteLine("if (recData.DataLen > 0)");
                        ClientCsw.WriteLine("{");
                        ClientCsw.WriteLine("El_Free(recData.Data);");
                        ClientCsw.WriteLine("}");

                        ClientCsw.WriteLine("}");
                        ClientCsw.WriteLine("#endif");

                        ClientCsw.WriteLine($"{service.ServiceName}_reqresp.State=waitstate;");
                    }

                    ClientCsw.WriteLine("exi:El_ReleaseMutex(RpcObject->ObjectMutexHandle);");
                    ClientCsw.WriteLine($"return {service.ServiceName}_reqresp;");
                    ClientCsw.WriteLine("}");

                    if (service.ReturnStructType.TargetFields.Count > 1)
                    {
                        ClientHsw.WriteLine($"void Free_{service.ServiceName}({service.ReturnStructType.TypeName} *response);\n");
                        ClientCsw.WriteLine($"void {targetInterface.Name}ClientImpl::Free_{service.ServiceName}({service.ReturnStructType.TypeName} *response)");
                        ClientCsw.WriteLine("{\nif(response->State==ResponseState_Ok)\n{");
                        //ClientCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Free(response);");
                        //ClientCsw.WriteLine($"SerializationManager::FreeData(&{service.ReturnStructType.TypeName}_TypeInstance,response);");
                        ClientCsw.WriteLine($"{service.ReturnStructType.TypeName}_FreeData(response);");
                        ClientCsw.WriteLine("}\n}");
                    }
                    ClientCsw.WriteLine("\n"); //client interface end class

                    MacroControlWriteEnd(ClientCsw, service.MacroControlAttribute);
                    MacroControlWriteEnd(ClientHsw, service.MacroControlAttribute);

                    ClientCsw.WriteLine(Environment.NewLine);
                    ClientHsw.WriteLine(Environment.NewLine);
                }
                ClientHsw.WriteLine("};\n"); //client interface end class

                
            }
            foreach (var service in targetInterface.Services)
            {
                //string GeneratServiceName = targetInterface.Name + "_" + service.ServiceName;
                //生成Server端代码
                if (codeGenParameter.GenType == GenType.Server || codeGenParameter.GenType == GenType.All)
                {
                    MacroControlWriteBegin(ServerHsw, service.MacroControlAttribute);
                    MacroControlWriteBegin(ServerCsw, service.MacroControlAttribute);
                    ServerHsw.WriteLine($"class {service.FullName}Service:public IService");
                    ServerHsw.WriteLine("{\npublic:");
                    ServerHsw.WriteLine("uint16_t GetSid(){{return {0}_ServiceId;}}", service.FullName);

                    //string returnType= service.ReturnValue==null?"void":$"{service.ServiceName}_Response& ";
                    if (service.ReturnStructType.TargetFields.Count>1)
                    {
                        ServerHsw.WriteLine($"{service.ReturnStructType.TypeName} Response;");
                    }
 
                    string returnType = "void";
                    var dh = service.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                    ServerHsw.Write($"{returnType} {service.ServiceName}(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut{dh}");

                    var temp_fileds = string.Empty;
                    for (int i = 0; i < service.ParameterStructType.TargetFields.Count; i++)
                    {
                        var field = service.ParameterStructType.TargetFields[i];
                        string name = field.FieldName;
                        temp_fileds += name + ",";

                        ServerHsw.Write($"{CppSerializableCommon.EmitField(field)}");

                        if (i + 1 < service.ParameterStructType.TargetFields.Count)
                        {
                            ServerHsw.Write(",");
                        }
                    }
                    ServerHsw.WriteLine(");");

                    //code gen invoke
                    ServerHsw.WriteLine($"{service.ParameterStructType.TypeName} request;");
                    ServerHsw.WriteLine("void Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager *recManager, SerializationManager* sendManager);");
                    ServerCsw.WriteLine($"void {service.FullName}Service::Invoke(UserDataOfTransportLayer_t* request_UserDataOfTransportLayer, UserDataOfTransportLayer_t* response_UserDataOfTransportLayer,EmbedXrpcObject* rpcObject,uint16_t targetTimeOut,SerializationManager *recManager, SerializationManager* sendManager)");
                    ServerCsw.WriteLine("{");
                    //ServerCsw.WriteLine($"static {service.ParameterStructType.TypeName} request;");
                    //ServerCsw.WriteLine($"{GeneratServiceName}_Request_Type.Deserialize(recManager,&request);");
                    //ServerCsw.WriteLine($"recManager.Deserialize(&{service.ParameterStructType.TypeName}_TypeInstance,&request);");
                    ServerCsw.WriteLine($"{service.ParameterStructType.TypeName}_Deserialize(recManager,&request);");
                    ServerCsw.WriteLine("#if EmbedXrpc_CheckSumValid==1");
                    ServerCsw.WriteLine($"El_Assert(SerializationManager_GetReferenceSum(recManager)==SerializationManager_GetCalculateSum(recManager));");
                    ServerCsw.WriteLine("#endif");
                    //if (service.ReturnValue != null)
                    //    ServerHsw.Write($"{service.ServiceName}_Response& returnValue=");
                    dh = service.ParameterStructType.TargetFields.Count > 0 ? "," : "";
                    ServerCsw.Write($"{service.ServiceName}(request_UserDataOfTransportLayer,response_UserDataOfTransportLayer,rpcObject,targetTimeOut{dh}");
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
                    //ServerCsw.WriteLine($"SerializationManager::FreeData(&{service.ParameterStructType.TypeName}_TypeInstance,&request);");//free
                    ServerCsw.WriteLine($"{service.ParameterStructType.TypeName}_FreeData(&request);");//free
                    if (service.ReturnStructType.TargetFields.Count>1)
                    {
                        //ServerCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Serialize(sendManager,0,&Response);");//生成返回值序列化
                        //ServerCsw.WriteLine($"sendManager.Serialize(&{service.ReturnStructType.TypeName}_TypeInstance,&Response,0);");//生成返回值序列化
                        ServerCsw.WriteLine($"{service.ReturnStructType.TypeName}_Serialize(sendManager,&Response);");
                        //ServerCsw.WriteLine($"{GeneratServiceName}_RequestResponseContent_Type.Free(&Response);");//生成返回值序列化
                        //ServerCsw.WriteLine($"SerializationManager::FreeData(&{service.ReturnStructType.TypeName}_TypeInstance,&Response);");//生成返回值序列化

                        //ServerCsw.WriteLine($"{service.ReturnStructType.TypeName}_FreeData(&Response);");//2021.3.10 用户malloc的空间 让用户去 free,所以注释了这条语句
                        ServerCsw.WriteLine($"if(IsFreeResponse==true) {service.ReturnStructType.TypeName}_FreeData(&Response);");//2021.3.10 用户malloc的空间 可以选择free,有用户控制所以有了这条语句
                    }

                    ServerCsw.WriteLine("}");//end function


                    ServerHsw.WriteLine("};");//end class

                    MacroControlWriteEnd(ServerCsw, service.MacroControlAttribute);
                    MacroControlWriteEnd(ServerHsw, service.MacroControlAttribute);

                    ServerCsw.WriteLine(Environment.NewLine);
                    ServerHsw.WriteLine(Environment.NewLine);
                    //ServerCsw.WriteLine($"{service.FullName}Service {service.FullName}ServiceInstance;");//创建一个service实例
                }

            }
            if (codeGenParameter.GenType == GenType.Server || codeGenParameter.GenType == GenType.All)
            {
                //生成request Service 数组
                ServerHsw.WriteLine($"/*\r\nThe Requests Of {targetInterface.Name}:");
                int RequestsCount = 0;
                ServerHsw.WriteLine("name                   type\r\n");
                foreach (var message in messageMaps)
                {
                    if (message.ReceiveType == ReceiveType_t.ReceiveType_Request)
                    {
                        RequestsCount++;
                        ServerHsw.WriteLine("\"{0:-64}\",           {1:-64}Service", message.Name, message.Name);
                    }

                }
                ServerHsw.WriteLine($"\r\nRequestsCount: {RequestsCount}");
                ServerHsw.WriteLine("*/\n");
            }
            if (codeGenParameter.GenType == GenType.Client || codeGenParameter.GenType == GenType.All)
            {
                //生成Response\Delegate service 数组
                ClientHsw.WriteLine($"/*\r\nThe Delegates Of {targetInterface.Name}:");
                int DelegatesCount = 0;
                int ResponsesCount = 0;
                ClientHsw.WriteLine("name                   type\r\n");
                foreach (var message in messageMaps)
                {
                    if (message.ReceiveType == ReceiveType_t.ReceiveType_Delegate)
                    {
                        DelegatesCount++;
                        ClientHsw.WriteLine("\"{0:-64}\"        {1:-64}ClientImpl,", message.Name,message.Name);
                    }
                }
                ClientHsw.WriteLine($"\r\nDelegatesCount:{DelegatesCount}\r\n\r\n\r\n");


                ClientHsw.WriteLine($"The Responses Of {targetInterface.Name}:");
                ClientHsw.WriteLine("name                   sid\r\n");
                foreach (var message in messageMaps)
                {
                    if (message.ReceiveType == ReceiveType_t.ReceiveType_Response)
                    {
                        ResponsesCount++;
                        ClientHsw.WriteLine("\"{0:-64}\"        {1:16}", message.Name, message.ServiceId);
                    }
                }
                ClientHsw.WriteLine($"\r\nResponsesCount:{ResponsesCount}");

                ClientHsw.WriteLine("*/\n");
            }
        }

    }
}
