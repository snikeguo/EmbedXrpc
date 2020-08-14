using EmbedXrpc;
using System;
using System.Reflection;
using System.Collections.Generic;
// auto code gen ! DO NOT modify this file! create time 2020-08-14 11:50:25.069 ; 
namespace StudentService
{
    public class Test
    {
        [FieldNumber(10)]
        [ArrayLenFieldFlag(true)]
        public Byte IntPtrsLen { get; set; }
        [ArrayProperty(LenFieldName = "IntPtrsLen")]
        [FieldNumber(11)]
        public Int32[] IntPtr { get; set; } = new Int32[0];
    }
    public class Achievement
    {
        [FieldNumber(10)]
        [ArrayLenFieldFlag(true)]
        public Byte Arlen { get; set; }
        [ArrayProperty(LenFieldName = "Arlen")]
        [FieldNumber(8)]
        public Test[] Ar { get; set; } = new Test[0];
    }
    public class Student
    {
        [FieldNumber(4)]
        public Achievement ChineseAchievement { get; set; } = new Achievement();
    }
}
