﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpc
{
    public enum Type_t
    {
        TYPE_UINT8,
        TYPE_INT8,

        TYPE_UINT16,
        TYPE_INT16,

        TYPE_UINT32,
        TYPE_INT32,

        TYPE_UINT64,
        TYPE_INT64,

        TYPE_FLOAT,
        TYPE_DOUBLE,

        TYPE_ARRAY,   /*array*/
        TYPE_STRUCT,  /*struct*/
        TYPE_UNION,     /*union*/

    };
    public interface IEmbedXrpcSerialization
    {
        void Serialize(SerializationManager sm);
        void Deserialize(SerializationManager sm);
    }
    public class SerializationManager
    {
        private List<byte> buf = new List<byte>();
        
        public List<byte> Buf
        {
            get
            {
                return buf;
            }
            set
            {
                buf = value;
            }
        }
        public int Index { get; set; } = 0;

        private Assembly Assembly;
        public SerializationManager(Assembly assembly,List<byte> data)
        {
            Assembly = assembly;
            //IsEnableMataDataEncode = isEnableMataDataEncode;
            Buf = data;
            Index = 0;
        }
        public static byte[] ToBytes( object obj,Type_t tp)
        {
            //Console.WriteLine($"u8:{d}");
            byte[] bs = null;
            if (tp == Type_t.TYPE_UINT8)
            {
                byte d = (byte)obj;
                bs = new byte[1];
                bs[0] = d;
            }
            else if (tp == Type_t.TYPE_INT8)
            {
                byte d = (byte)obj;
                bs = new byte[1];
                bs[0] = d;
            }
            else if(tp==Type_t.TYPE_UINT16)
            {
                bs = new byte[2];
                UInt16 d = (UInt16)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
            }
            else if (tp == Type_t.TYPE_INT16)
            {
                bs = new byte[2];
                Int16 d = (Int16)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
            }
            else if (tp == Type_t.TYPE_UINT32)
            {
                bs = new byte[4];
                UInt32 d = (UInt32)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
                bs[2] = (byte)(d >> 16 & 0xFF);
                bs[3] = (byte)(d >> 24 & 0xFF);
            }
            else if (tp == Type_t.TYPE_INT32)
            {
                bs = new byte[4];
                Int32 d = (Int32)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
                bs[2] = (byte)(d >> 16 & 0xFF);
                bs[3] = (byte)(d >> 24 & 0xFF);
            }
            else if (tp == Type_t.TYPE_UINT64)
            {
                bs = new byte[8];
                UInt64 d = (UInt64)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
                bs[2] = (byte)(d >> 16 & 0xFF);
                bs[3] = (byte)(d >> 24 & 0xFF);
                bs[4] = (byte)(d >> 32 & 0xFF);
                bs[5] = (byte)(d >> 40 & 0xFF);
                bs[6] = (byte)(d >> 48 & 0xFF);
                bs[7] = (byte)(d >> 56 & 0xFF);
            }
            else if (tp == Type_t.TYPE_INT64)
            {
                bs = new byte[8];
                Int64 d = (Int64)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
                bs[2] = (byte)(d >> 16 & 0xFF);
                bs[3] = (byte)(d >> 24 & 0xFF);
                bs[4] = (byte)(d >> 32 & 0xFF);
                bs[5] = (byte)(d >> 40 & 0xFF);
                bs[6] = (byte)(d >> 48 & 0xFF);
                bs[7] = (byte)(d >> 56 & 0xFF);
            }

            return bs;
            
        }
        
        
        public static byte[] ToBytes( UInt32 d)
        {
            //Console.WriteLine($"u32:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},");
            byte[] bs = new byte[4];
            bs[0]=(byte)(d & 0xFF);
            bs[1] = (byte)(d >> 8 & 0xFF);
            bs[2] = (byte)(d >> 16 & 0xFF);
            bs[3] = (byte)(d >> 24 & 0xFF);
            return bs;
        }
        public static byte[] ToBytes(Int32 d)
        {
            //Console.WriteLine($"i32:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},");
            byte[] bs = new byte[4];
            bs[0] = (byte)(d & 0xFF);
            bs[1] = (byte)(d >> 8 & 0xFF);
            bs[2] = (byte)(d >> 16 & 0xFF);
            bs[3] = (byte)(d >> 24 & 0xFF);
            return bs;
        }

        public static byte[] ToBytes( UInt64 d)
        {
            //Console.WriteLine($"u64:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},  {d>>32 & 0xFF},{d >> 40 & 0xFF},{d >> 48 & 0xFF},{d >> 56 & 0xFF},");
            byte[] bs = new byte[8];
            bs[0]=(byte)(d & 0xFF);
            bs[1] = (byte)(d >> 8 & 0xFF);
            bs[2] = (byte)(d >> 16 & 0xFF);
            bs[3] = (byte)(d >> 24 & 0xFF);
            bs[4] = (byte)(d >> 32 & 0xFF);
            bs[5] = (byte)(d >> 40 & 0xFF);
            bs[6] = (byte)(d >> 48 & 0xFF);
            bs[7] = (byte)(d >> 56 & 0xFF);
            return bs;
        }
        public static byte[] ToBytes(Int64 d)
        {
            //Console.WriteLine($"64:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},  {d >> 32 & 0xFF},{d >> 40 & 0xFF},{d >> 48 & 0xFF},{d >> 56 & 0xFF},");
            byte[] bs = new byte[8];
            bs[0] = (byte)(d & 0xFF);
            bs[1] = (byte)(d >> 8 & 0xFF);
            bs[2] = (byte)(d >> 16 & 0xFF);
            bs[3] = (byte)(d >> 24 & 0xFF);
            bs[4] = (byte)(d >> 32 & 0xFF);
            bs[5] = (byte)(d >> 40 & 0xFF);
            bs[6] = (byte)(d >> 48 & 0xFF);
            bs[7] = (byte)(d >> 56 & 0xFF);
            return bs;
        }
        
        private bool IsBaseValueType(Type vt,ref Type_t mytype)
        {
            if (vt.IsEnum == true)
            {
                vt = vt.GetEnumUnderlyingType();
            }
            if (vt == typeof(bool))
            {

                mytype = Type_t.TYPE_UINT8;
                return true;
            }
            else if (vt == typeof(byte))
            {
                mytype = Type_t.TYPE_UINT8;
                return true;
            }
            else if (vt == typeof(sbyte))
            {
                mytype = Type_t.TYPE_INT8;
                return true;
            }
            else if (vt == typeof(UInt16))
            {
                mytype =  Type_t.TYPE_UINT16;
                return true;
            }
            else if (vt == typeof(Int16))
            {
                mytype = Type_t.TYPE_INT16;
                return true;
            }
            else if (vt == typeof(UInt32))
            {
                mytype = Type_t.TYPE_UINT32;
                return true;
            }
            else if (vt == typeof(Int32))
            {
                mytype = Type_t.TYPE_INT32;
                return true;
            }
            else if (vt == typeof(UInt64))
            {
                mytype = Type_t.TYPE_UINT64;
                return true;
            }
            else if (vt == typeof(Int64))
            {
                mytype = Type_t.TYPE_INT64;
                return true;
            }
            else if(vt.IsArray==true)
            {
                mytype = Type_t.TYPE_ARRAY;
                return false;
            }
            else if (vt.IsClass == true)
            {
                mytype = Type_t.TYPE_STRUCT;
                return false;
            }
            else
            {
                throw new InvalidDataException();
            }
        }
        private void BaseValueSerialize(object v,Type vt)
        {
            //Type vt = v.GetType();
            if (vt.IsEnum == true)
            {
                vt = vt.GetEnumUnderlyingType();
            }
            if (vt == typeof(bool))
            {   
                ToBytes(Convert.ToByte(v));
            }
            else if (vt == typeof(byte))
            {
                ToBytes(Convert.ToByte(v));
            }
            else if (vt == typeof(sbyte))
            {
                ToBytes(Convert.ToSByte(v));
            }
            else if (vt == typeof(UInt16))
            {
                ToBytes(Convert.ToUInt16(v));
            }
            else if (vt == typeof(Int16))
            {
                ToBytes(Convert.ToInt16(v));
            }
            else if (vt == typeof(UInt32))
            {
                ToBytes(Convert.ToUInt32(v));
            }
            else if (vt == typeof(Int32))
            {
                ToBytes(Convert.ToInt32(v));
            }
            else if (vt == typeof(UInt64))
            {
                ToBytes(Convert.ToUInt64(v));
            }
            else if (vt == typeof(Int64))
            {
                ToBytes(Convert.ToInt64(v));
            }
            else
            {
                throw new NotSupportedException();
            }
        }
        private static object[] ConvertArray(Array arr)
        {
            if (arr == null)
                return null;
            int lb = arr.GetLowerBound(0);
            var ret = new object[arr.GetUpperBound(0) - lb + 1];
            for (int ix = 0; ix < ret.Length; ++ix)
            {
                ret[ix] = arr.GetValue(ix + lb);
            }
            return ret;
        }
        
       
        internal class BitFieldStateMachine
        {
            internal int BitFieldLeftShiftAccer = 0;
            internal UInt64 BitFieldTempValue=0;
            internal Type CurrentBitFieldType=null;//指向当前的位域字段，如果该字段为null 说明一个连续的位域字段结束了。结构体刚开始时这个为null
            internal void Reset()
            {
                BitFieldLeftShiftAccer = 0;
                BitFieldTempValue = 0;
                CurrentBitFieldType = null;
            }
        }
        
        public object DeserializeField(Type_t vt)
        {
            if (vt ==  Type_t.TYPE_UINT8)
            {
                byte v =Convert.ToByte(Buf[Index]);
                Index++;
                return v;
            }
            if (vt == Type_t.TYPE_INT8)
            {
                sbyte v = Convert.ToSByte(Buf[Index]);
                Index++;
                return v;
            }
            else if (vt == Type_t.TYPE_UINT16)
            {
                UInt16 v = Convert.ToUInt16((UInt16)Buf[Index + 1] << 8);
                v |= Convert.ToUInt16((UInt16)Buf[Index]);
                return v;
            }
            else if (vt == Type_t.TYPE_INT16)
            {
                Int16 v = Convert.ToInt16((Int16)Buf[Index + 1] << 8);
                v |= Convert.ToInt16((Int16)Buf[Index]);
                Index += 2;
                return v;
            }
            else if (vt == Type_t.TYPE_UINT32)
            {
                UInt32 v = Convert.ToUInt32((UInt32)Buf[Index + 3] << 24);
                v |= Convert.ToUInt32((UInt32)Buf[Index + 2] << 16);
                v |= Convert.ToUInt32((UInt32)Buf[Index + 1] << 8);
                v |= Convert.ToUInt32((UInt32)Buf[Index]);
                Index += 4;
                return v;
            }
            else if (vt == Type_t.TYPE_INT32)
            {
                Int32 v = Convert.ToInt32((Int32)Buf[Index + 3] << 24);
                v |= Convert.ToInt32((Int32)Buf[Index + 2] << 16);
                v |= Convert.ToInt32((Int32)Buf[Index + 1] << 8);
                v |= Convert.ToInt32((Int32)Buf[Index]);
                Index += 4;
                return v;
            }
            else if (vt == Type_t.TYPE_UINT64)
            {
                UInt64 v = Convert.ToUInt64((UInt64)Buf[Index + 7] << 56);
                v |= Convert.ToUInt64((UInt64)Buf[Index + 6] << 48);
                v |= Convert.ToUInt64((UInt64)Buf[Index + 5] << 40);
                v |= Convert.ToUInt64((UInt64)Buf[Index + 4] << 32);
                v |= Convert.ToUInt64((UInt64)Buf[Index + 3] << 24);
                v |= Convert.ToUInt64((UInt64)Buf[Index + 2] << 16);
                v |= Convert.ToUInt64((UInt64)Buf[Index + 1] << 8);
                v |= Convert.ToUInt64((UInt64)Buf[Index]);
                Index += 8;
                return v;
            }
            else if (vt == Type_t.TYPE_INT64)
            {
                Int64 v = Convert.ToInt64((Int64)Buf[Index + 7] << 56);
                v |= Convert.ToInt64((Int64)Buf[Index + 6] << 48);
                v |= Convert.ToInt64((Int64)Buf[Index + 5] << 40);
                v |= Convert.ToInt64((Int64)Buf[Index + 4] << 32);
                v |= Convert.ToInt64((Int64)Buf[Index + 3] << 24);
                v |= Convert.ToInt64((Int64)Buf[Index + 2] << 16);
                v |= Convert.ToInt64((Int64)Buf[Index + 1] << 8);
                v |= Convert.ToInt64((Int64)Buf[Index]);
                Index += 8;
                return v;
            }
            throw new InvalidDataException($"len is {vt},but len only support 1/2/4/8");
        }
    }
    

    [AttributeUsage(AttributeTargets.All, Inherited = false, AllowMultiple = true)]
    public sealed class ArrayPropertyAttribute : Attribute
    {
        public string LenFieldName { get; set; } = string.Empty;

    }
    public enum BitsType
    {
        NoBits,
        Byte,
        SByte,
        UInt16,
        Int16,
        UInt32,
        Int32,
        UInt64,
        Int64,
    }
    [AttributeUsage(AttributeTargets.Class, Inherited = false, AllowMultiple = true)]
    public sealed class BitsAttribute : Attribute
    {
        public BitsType BitsType { get; set; }
        public BitsAttribute(BitsType ut)
        {
            BitsType = ut;
        }
    }
    [AttributeUsage(AttributeTargets.Property, Inherited = false, AllowMultiple = true)]
    public sealed class FieldNumberAttribute : Attribute
    {
        public FieldNumberAttribute(Int32 number)
        {
            Number = number;
        }

        // This is a named argument
        public Int32 Number { get; set; }
    }
    [AttributeUsage(AttributeTargets.Property, Inherited = false, AllowMultiple = true)]
    public sealed class ArrayLenFieldFlagAttribute : Attribute
    {
        public ArrayLenFieldFlagAttribute(bool f)
        {
            Flag = f;
        }

        // This is a named argument
        public bool Flag { get; set; }
    }


    [AttributeUsage(AttributeTargets.Property, Inherited = false, AllowMultiple = true)]
    public class UnionTargetTypeAttribute : Attribute
    {

    }
    [AttributeUsage(AttributeTargets.Property, Inherited = false, AllowMultiple = true)]
    public class UnionFieldAttribute : Attribute
    {

    }
    [AttributeUsage(AttributeTargets.Property, Inherited = false, AllowMultiple = true)]
    public class NoSerializationAttribute : Attribute
    {

    }
    [AttributeUsage(AttributeTargets.Property, Inherited = false, AllowMultiple = true)]
    public sealed class BitFieldAttribute : Attribute
    {
        public BitFieldAttribute(int bitFieldLength)
        {
            BitFieldLength = bitFieldLength;
        }
        public int BitFieldLength { get; private set; }
    }
}
