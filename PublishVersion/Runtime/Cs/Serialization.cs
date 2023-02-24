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
        public byte[] ToBytes( object obj,Type tp)
        {
            //Console.WriteLine($"u8:{d}");
            byte[] bs = null;
            if (tp == typeof(bool))
            {
                byte d = (bool)obj==true?(byte)1:(byte)0;
                bs = new byte[1];
                bs[0] = d;
            }
            else if (tp == typeof(byte))
            {
                byte d = (byte)obj;
                bs = new byte[1];
                bs[0] = d;
            }
            else if (tp == typeof(sbyte))
            {
                byte d = (byte)obj;
                bs = new byte[1];
                bs[0] = d;
            }
            else if(tp == typeof(UInt16))
            {
                bs = new byte[2];
                UInt16 d = (UInt16)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
            }
            else if (tp == typeof(Int16))
            {
                bs = new byte[2];
                Int16 d = (Int16)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
            }
            else if (tp == typeof(UInt32))
            {
                bs = new byte[4];
                UInt32 d = (UInt32)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
                bs[2] = (byte)(d >> 16 & 0xFF);
                bs[3] = (byte)(d >> 24 & 0xFF);
            }
            else if (tp == typeof(Int32))
            {
                bs = new byte[4];
                Int32 d = (Int32)(obj);
                bs[0] = (byte)(d & 0xFF);
                bs[1] = (byte)(d >> 8 & 0xFF);
                bs[2] = (byte)(d >> 16 & 0xFF);
                bs[3] = (byte)(d >> 24 & 0xFF);
            }
            else if (tp == typeof(UInt64))
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
            else if (tp == typeof(Int64))
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
            else
            {
                throw new NotSupportedException($"NotSupported");
            }
            return bs;
            
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
        public object DeserializeField(Type vt)
        {
            if (vt == typeof(bool))
            {
                bool v = Convert.ToBoolean(Buf[Index]);
                Index++;
                return v;
            }
            else if (vt == typeof(byte))
            {
                byte v =Convert.ToByte(Buf[Index]);
                Index++;
                return v;
            }
            else if (vt == typeof(sbyte))
            {
                sbyte v = Convert.ToSByte(Buf[Index]);
                Index++;
                return v;
            }
            else if (vt == typeof(UInt16))
            {
                UInt16 v = Convert.ToUInt16((UInt16)Buf[Index + 1] << 8);
                v |= Convert.ToUInt16((UInt16)Buf[Index]);
                Index += 2;
                return v;
            }
            else if (vt == typeof(Int16))
            {
                Int16 v = Convert.ToInt16((Int16)Buf[Index + 1] << 8);
                v |= Convert.ToInt16((Int16)Buf[Index]);
                Index += 2;
                return v;
            }
            else if (vt == typeof(UInt32))
            {
                UInt32 v = Convert.ToUInt32((UInt32)Buf[Index + 3] << 24);
                v |= Convert.ToUInt32((UInt32)Buf[Index + 2] << 16);
                v |= Convert.ToUInt32((UInt32)Buf[Index + 1] << 8);
                v |= Convert.ToUInt32((UInt32)Buf[Index]);
                Index += 4;
                return v;
            }
            else if (vt == typeof(Int32))
            {
                Int32 v = Convert.ToInt32((Int32)Buf[Index + 3] << 24);
                v |= Convert.ToInt32((Int32)Buf[Index + 2] << 16);
                v |= Convert.ToInt32((Int32)Buf[Index + 1] << 8);
                v |= Convert.ToInt32((Int32)Buf[Index]);
                Index += 4;
                return v;
            }
            else if (vt == typeof(UInt64))
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
            else if (vt == typeof(Int64))
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
            else 
            {
                throw new NotSupportedException($"NotSupported");
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
