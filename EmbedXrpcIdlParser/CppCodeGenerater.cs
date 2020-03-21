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
            cppStreamWriter = new StreamWriter(idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".h", false, Encoding.UTF8);
            cppStreamWriter.WriteLine("#include\"EmbedXrpcBaseType.h\"");

            fbsStreamWriter = new StreamWriter(idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".fbs", false, Encoding.ASCII);
            fbsStreamWriter.WriteLine("namespace EmbedXrpc;");
            foreach (var em in idlInfo.TargetEnums)
            {
                //EmitFbsEnum(em);
                fbsStreamWriter.WriteLine(em.ToFbs().ToString());
                EmitEnum(em);
            }
            foreach (var stru in idlInfo.TargetStructs)
            {
                //EmitFbsTable(stru);
                fbsStreamWriter.WriteLine(stru.ToFbs().ToString());
                EmitStruct(stru);
            }
            foreach (var del in idlInfo.TargetDelegates)
            {
                fbsStreamWriter.WriteLine(del.ToFbs().ToString());
                EmitDelegate(del);
            }
            foreach (var ifs in idlInfo.TargetInterfaces)
            {
                fbsStreamWriter.WriteLine(ifs.ToFbs().ToString());
                EmitClientInterface(ifs);
            }
            cppStreamWriter.Flush();
            cppStreamWriter.Close();

            fbsStreamWriter.Flush();
            fbsStreamWriter.Close();

            //FbsHelper.GenerateSerializationCode(idlInfo.GenerationOptionParameterAttribute.OutPutFileName + ".fbs");
        }
        private StreamWriter cppStreamWriter;
        private StreamWriter fbsStreamWriter;

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
        private void EmitFbsMethodTable(TargetService targetService)
        {
            fbsStreamWriter.WriteLine("table " + targetService.ServiceName);
            fbsStreamWriter.WriteLine("{");
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
