using EmbedXrpc;
using System;
using System.Reflection;
using System.Collections.Generic;
// auto code gen ! DO NOT modify this file! create time 2020-05-04 22:48:57.629 ; 
namespace StudentService
{
    [Bits(BitsType.UInt64)]
    public class BitsTest
    {
        [BitsFieldLength(10)]
        public Int32 Field1 { get; set; }
        [BitsFieldLength(20)]
        public Int32 Field2 { get; set; }
    }
    public class StudentBitTest
    {
        public BitsTest bt { get; set; } = new BitsTest();
    }
}
