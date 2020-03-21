using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
namespace EmbedXrpcIdlParser
{
    public class FbsField
    {
        public string Name { get; set; }
        public string Type { get; set; }
    }
    public class FbsHelper
    {
        public static StringBuilder EmitTable(string table, IList<FbsField> fbsFields)
        {
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.Append("table " + table + Environment.NewLine);
            stringBuilder.Append("{" + Environment.NewLine);
            foreach (var fbsf in fbsFields)
            {
                stringBuilder.Append($"{fbsf.Name}:{fbsf.Type};" + Environment.NewLine);
            }
            stringBuilder.Append("}" + Environment.NewLine);
            return stringBuilder;
        }
        public static StringBuilder EmitEnum(TargetEnum targetEnum)
        {
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.Append("enum " + targetEnum.Name + ":" + FbsHelper.ReplaceDic[targetEnum.IntType] + Environment.NewLine);
            stringBuilder.Append("{" + Environment.NewLine);
            foreach (var fbse in targetEnum.TargetEnumValues)
            {
                stringBuilder.Append($"{fbse.Description}={fbse.Value}," + Environment.NewLine);
            }
            stringBuilder.Append("}" + Environment.NewLine);
            return stringBuilder;
        }

        public static string IdlType2FbsType(TargetField field)
        {
            string fbsType = field.IdlType;
            if (field.IsArray == true)
            {
                fbsType = fbsType.Replace("[]", "");
                if (FbsHelper.ReplaceDic.ContainsKey(fbsType) == true)
                {
                    fbsType = "[" + FbsHelper.ReplaceDic[fbsType] + "]";
                }
                else
                {
                    fbsType = "[" + fbsType + "]";
                }
            }
            else
            {
                if (FbsHelper.ReplaceDic.ContainsKey(fbsType) == true)
                {
                    fbsType = FbsHelper.ReplaceDic[fbsType];
                }

            }
            return fbsType;
        }
        public static string IdlType2FbsType(TargetReturnValue rv)
        {
            string fbsType = rv.IdlType;
            if (FbsHelper.ReplaceDic.ContainsKey(fbsType) == true)
            {
                fbsType = FbsHelper.ReplaceDic[fbsType];
            }
            return fbsType;
        }
        internal static Dictionary<string, string> ReplaceDic = new Dictionary<string, string>();
        static FbsHelper()
        {
            ReplaceDic.Add("byte", "uint8");
            ReplaceDic.Add("Byte", "uint8");
            ReplaceDic.Add("sbyte", "int8");
            ReplaceDic.Add("SByte", "int8");
            ReplaceDic.Add("UInt16", "uint16");
            ReplaceDic.Add("Int16", "int16");
            ReplaceDic.Add("UInt32", "uint32");
            ReplaceDic.Add("Int32", "int322");
            ReplaceDic.Add("float", "float");
            ReplaceDic.Add("double", "double");
        }
        public  static void GenerateSerializationCode(string filepath)
        {
            Process exep = new Process();
            exep.StartInfo.FileName = "cmd";
            exep.StartInfo.Arguments = " flatcc.exe -a "+filepath;
            exep.StartInfo.CreateNoWindow = false;
            exep.StartInfo.UseShellExecute = false;
            //exep.StartInfo.WorkingDirectory = ".";
            exep.OutputDataReceived += (s, e) =>
              {
                  Console.WriteLine(e.Data);
              };
            exep.ErrorDataReceived += (s, e) =>
              {
                  Console.WriteLine(e.Data);
              };
            exep.Start();
            exep.StartInfo.RedirectStandardOutput = true;
            exep.StartInfo.RedirectStandardError = true;
            //exep.BeginOutputReadLine();
            //exep.BeginErrorReadLine();
            exep.WaitForExit();//关键，等待外部程序退出后才能往下执行
        }
    }
    public static class TargetEnumFbsExtension
    {
        public static StringBuilder ToFbs(this TargetEnum targetEnum)
        {
            return FbsHelper.EmitEnum(targetEnum);
        }
    }
    public static class TargetStructFbsExtension
    {
        public static StringBuilder ToFbs(this TargetStruct targetStruct)
        {
            List<FbsField> ffs = new List<FbsField>();
            foreach (var field in targetStruct.TargetFields)
            {
                FbsField ff = new FbsField();
                ff.Name = field.Name;
                ff.Type = FbsHelper.IdlType2FbsType(field);
                ffs.Add(ff);
            }
            return FbsHelper.EmitTable(targetStruct.Name, ffs);
        }
    }
    public static class TargetInterfaceFbsExtension
    {
        public static StringBuilder ToFbs(this TargetInterface targetInterface)
        {
            StringBuilder sb = new StringBuilder();
            List<FbsField> ffs = new List<FbsField>();
            foreach (var service in targetInterface.Services)
            {
                foreach (var field in service.TargetFields)
                {
                    FbsField ff = new FbsField();
                    ff.Name = field.Name;
                    ff.Type = FbsHelper.IdlType2FbsType(field);
                    ffs.Add(ff);
                }
                sb.Append(FbsHelper.EmitTable(targetInterface.Name + "_" + service.ServiceName + "_Request", ffs));
                ffs.Clear();

                var rff = new FbsField();
                rff.Name = "ReturnValue";
                rff.Type = FbsHelper.IdlType2FbsType(service.ReturnValue);
                List<FbsField> rffs = new List<FbsField>();
                rffs.Add(rff);
                sb.Append(FbsHelper.EmitTable(targetInterface.Name + "_" + service.ServiceName + "_Response", rffs));
            }
            return sb;
        }
    }
    public static class TargetDelegateFbsExtension
    {
        public static StringBuilder ToFbs(this TargetDelegate targetDelegate)
        {
            StringBuilder sb = new StringBuilder();
            List<FbsField> ffs = new List<FbsField>();
            foreach (var field in targetDelegate.TargetFields)
            {
                FbsField ff = new FbsField();
                ff.Name = field.Name;
                ff.Type = FbsHelper.IdlType2FbsType(field);
                ffs.Add(ff);
            }
            sb.Append(FbsHelper.EmitTable(targetDelegate.MethodName, ffs));
            ffs.Clear();

            return sb;
        }
    }
}
