using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
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
        TYPE_OBJECT,  /*object*/


    };
    public class SerializationManager
    {
        private List<byte> data = new List<byte>();
        public SerializationManager()
        {

        }
        public List<byte> Data
        {
            get
            {
                return data;
            }
            set
            {
                data = value;
            }
        }
        public int Index { get; set; } = 0;

        /*public void Add(byte d)
        {
            Index++;
            data.Add(d);
        }*/
        public void Reset()
        {
            data.Clear();
            Index = 0;
        }
        public bool IsEnableMataData { get; set; } = false;
        void SerializeKey(UInt32 FieldNumber, Type_t Field)
        {
            /*Buf[Index++] = FieldNumber;
            Buf[Index++] = Field;
            printf("SerializeKey FieldNumber:%d,Type:%s\n", FieldNumber, TypeString[Field]);*/
            UInt32 shiftfn = 0;
            UInt32 next_shiftfn = 0;
            if ((FieldNumber >> 3) != 0)
            {
                var data= (byte)(0x80 | (FieldNumber << 4) | (byte)Field);
                Data.Add(data);
                Index++;
            }
            else
            {
                var data = (byte)((FieldNumber << 4) | (byte)Field);
                Data.Add(data);
                Index++;
            }
            shiftfn = FieldNumber >> 3;
            while (shiftfn != 0)
            {
                next_shiftfn = shiftfn >> 7;
                if (next_shiftfn == 0)
                {
                    var data  = (byte)(shiftfn & 0x7f);
                    Data.Add(data);
                    Index++;
                    break;
                }
                else
                {
                    var data = (byte)(shiftfn | 0x80);
                    Data.Add(data);
                    Index++;
                }
                shiftfn = shiftfn >> 7;
            }
        }
        void SerializeLen(Int64 Len)
        {
            Int64 next_shiftlen = 0;
            do
            {
                next_shiftlen = Len >> 7;
                if (next_shiftlen == 0)
                {
                    var data = (byte)(Len & 0x7f);
                    Data.Add(data);
                    Index++;
                    break;
                }
                else
                {
                    var data = (byte)(Len | 0x80);
                    Data.Add(data);
                    Index++;
                }

                Len = Len >> 7;
            } while (true);
        }
        void SerializeEndFlag()
        {
            byte data = 0x7F;
            Data.Add(data);
            Index++;
            //printf("SerializeEnd\n");
        }
        bool IsEnd()
        {
            return Data[Index] == 0x7F;
        }
        void SerializeArrayElementFlag(byte flag)
        {
            Data.Add(flag);
            Index += 1;
            //printf("SerializeArrayElementFlag:0x%x\n", flag);
        }
        void SerializeValue(byte[] v)//base value used
        {
            Data.AddRange(v);            
            Index += v.Length;
        }
        /*bool Pop(byte[] out_buf, UInt32 len)
        {
            if (len + Index > Data.Count)
            {
                throw new OverflowException("len + Index > Data.Count!");
                return false;
            }
            if (out_buf != null)
            {
                MEMCPY(out_buf, &Buf[Index], len);
                Array.Copy
            }
            Index += len;
            return true;
        }*/
        private Assembly Assembly;
        public SerializationManager(Assembly assembly)
        {
            Assembly = assembly;
        }
        public void ToBytes( bool d)
        {
            //Console.WriteLine($"u8:{d}");
            ToBytes(Convert.ToByte(d));
        }
        public void ToBytes(byte d)
        {
            //Console.WriteLine($"u8:{d}");
            Data.Add(d);
            Index += 1;
        }
        public void ToBytes(sbyte d)
        {
            //Console.WriteLine($"i8:{d}");
            Data.Add((byte)d);
            Index += 1;
        }
        public void ToBytes(Int16 d)
        {
            //Console.WriteLine($"u16:{d&0xFF},{d>>8 & 0xFF},");
            Data.Add((byte)(d & 0xFF));
            Data.Add((byte)(d >> 8 & 0xFF));
            Index += 2;
        }
        public void ToBytes(UInt16 d)
        {
            //Console.WriteLine($"i16:{d & 0xFF},{d >> 8 & 0xFF},");
            Data.Add((byte)(d & 0xFF));
            Data.Add((byte)(d >> 8 & 0xFF));
            Index += 2;
        }
        public void ToBytes( UInt32 d)
        {
            //Console.WriteLine($"u32:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},");
            Data.Add((byte)(d & 0xFF));
            Data.Add((byte)(d >> 8 & 0xFF));
            Data.Add((byte)(d >> 16 & 0xFF));
            Data.Add((byte)(d >> 24 & 0xFF));
            Index += 4;
        }
        public void ToBytes(Int32 d)
        {
            //Console.WriteLine($"i32:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},");
            Data.Add((byte)(d & 0xFF));
            Data.Add((byte)(d >> 8 & 0xFF));
            Data.Add((byte)(d >> 16 & 0xFF));
            Data.Add((byte)(d >> 24 & 0xFF));
            Index += 4;
        }

        public void ToBytes( UInt64 d)
        {
            //Console.WriteLine($"u64:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},  {d>>32 & 0xFF},{d >> 40 & 0xFF},{d >> 48 & 0xFF},{d >> 56 & 0xFF},");
            Data.Add((byte)(d & 0xFF));
            Data.Add((byte)(d >> 8 & 0xFF));
            Data.Add((byte)(d >> 16 & 0xFF));
            Data.Add((byte)(d >> 24 & 0xFF));
            Data.Add((byte)(d >> 32 & 0xFF));
            Data.Add((byte)(d >> 40 & 0xFF));
            Data.Add((byte)(d >> 48 & 0xFF));
            Data.Add((byte)(d >> 56 & 0xFF));
            Index += 8;
        }
        public  void ToBytes(Int64 d)
        {
            //Console.WriteLine($"64:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},  {d >> 32 & 0xFF},{d >> 40 & 0xFF},{d >> 48 & 0xFF},{d >> 56 & 0xFF},");
            Data.Add((byte)(d & 0xFF));
            Data.Add((byte)(d >> 8 & 0xFF));
            Data.Add((byte)(d >> 16 & 0xFF));
            Data.Add((byte)(d >> 24 & 0xFF));
            Data.Add((byte)(d >> 32 & 0xFF));
            Data.Add((byte)(d >> 40 & 0xFF));
            Data.Add((byte)(d >> 48 & 0xFF));
            Data.Add((byte)(d >> 56 & 0xFF));
            Index += 8;
        }
        /*private bool IsBaseValueType(Type t)
        {
            if (t.IsEnum == true)
            {
                return true;
            }
            if (t == typeof(bool))
            {
                return true;
            }
            if (t == typeof(byte))
            {
                return true;
            }
            if (t == typeof(sbyte))
            {
                return true;
            }
            if (t == typeof(UInt16))
            {
                return true;
            }
            if (t == typeof(Int16))
            {
                return true;
            }
            if (t == typeof(UInt32))
            {
                return true;
            }
            if (t == typeof(Int32))
            {
                return true;
            }
            if (t == typeof(UInt64))
            {
                return true;
            }
            if (t == typeof(Int64))
            {
                return true;
            }
            return false;
        }*/
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
            else
            {
                return false;
            }
        }
        private void BaseValueSerialize(object v)
        {
            Type vt = v.GetType();
            if (vt.IsEnum == true)
            {
                vt = vt.GetEnumUnderlyingType();
            }
            if (vt == typeof(bool))
            {   
                ToBytes((byte)v);
            }
            else if (vt == typeof(byte))
            {
                ToBytes((byte)v);
            }
            else if (vt == typeof(sbyte))
            {
                ToBytes((sbyte)v);
            }
            else if (vt == typeof(UInt16))
            {
                ToBytes( (UInt16)v);
            }
            else if (vt == typeof(Int16))
            {
                ToBytes((Int16)v);
            }
            else if (vt == typeof(UInt32))
            {
                ToBytes((UInt32)v);
            }
            else if (vt == typeof(Int32))
            {
                ToBytes((Int32)v);
            }
            else if (vt == typeof(UInt64))
            {
                ToBytes( (UInt64)v);
            }
            else if (vt == typeof(Int64))
            {
                ToBytes((Int64)v);
            }
            else
            {
                throw new NotSupportedException();
            }
        }
        private static object[] ConvertArray(Array arr)
        {
            int lb = arr.GetLowerBound(0);
            var ret = new object[arr.GetUpperBound(0) - lb + 1];
            for (int ix = 0; ix < ret.Length; ++ix)
            {
                ret[ix] = arr.GetValue(ix + lb);
            }
            return ret;
        }
        public void Serialize(object s,UInt32 fieldNumber)
        {
            if (IsEnableMataData == true)
            {
                SerializeKey(fieldNumber,Type_t.TYPE_OBJECT);
                SerializeSubField(s);
            }
            else
            {
                NoMataData_SerializeSubField(s);
            }
        }
        void SerializeSubField(object s)
        {
            Type st = s.GetType();
            var pros = st.GetProperties();
            foreach (var field in pros)
            {
                object v = field.GetValue(s);
                var vt = v.GetType();
                FieldNumberAttribute fieldNumberAttribute = field.GetCustomAttribute<FieldNumberAttribute>();
                ArrayLenFieldFlagAttribute IsArrayLenFieldAttribute = field.GetCustomAttribute<ArrayLenFieldFlagAttribute>();
                if (vt.IsArray == false)
                {
                    Type_t lost_t = Type_t.TYPE_OBJECT;
                    if (IsBaseValueType(vt,ref lost_t) == true && IsArrayLenFieldAttribute.Flag == false)
                    {
                        SerializeKey(fieldNumberAttribute.Number, lost_t);
                        BaseValueSerialize(v);
                    }
                    else
                    {
                        Serialize(v, fieldNumberAttribute.Number);
                    }
                }
                else
                {
                    //object[] arrayValue = (object[])v;
                    object[] arrayValue = ConvertArray(v as Array);
                    ArrayPropertyAttribute att = field.GetCustomAttribute<ArrayPropertyAttribute>();
                    if (att == null)
                    {
                        throw new Exception("ArrayPropertyAttribute is null!");
                    }
                    var lenfield = from lf in pros
                                   where lf.Name == att.LenFieldName
                                   select lf;
                    Int64 len = 1;
                    if (lenfield.ToList().Count != 0)
                    {
                        len = Convert.ToInt64(lenfield.ToList()[0].GetValue(s));
                    }
                    SerializeKey(fieldNumberAttribute.Number, Type_t.TYPE_ARRAY);
                    SerializeLen(len);
                    var aet = (v as Array).GetType().GetElementType();
                    Type_t lost_t = Type_t.TYPE_OBJECT;
                    if (IsBaseValueType(aet,ref lost_t)==true)
                    {
                        SerializeArrayElementFlag((byte)((byte)lost_t << 4 | 0x01));
                        for (Int64 i = 0; i < len; i++)
                        {
                            var aev = arrayValue[i];
                            BaseValueSerialize(aev);
                        }
                    }
                    else
                    {
                        SerializeArrayElementFlag(0x02);
                        for (Int64 i = 0; i < len; i++)
                        {
                            var aev = arrayValue[i];
                            SerializeSubField(aev);
                        }
                    }
                    
                }
            }
            SerializeEndFlag();
        }
        void NoMataData_SerializeSubField(object s)
        {
            Type st = s.GetType();
            var pros = st.GetProperties();
            foreach (var field in pros)
            {
                object v = field.GetValue(s);
                var vt = v.GetType();
                FieldNumberAttribute fieldNumberAttribute = field.GetCustomAttribute<FieldNumberAttribute>();
                if (vt.IsArray == false)
                {
                    Type_t lost_t = Type_t.TYPE_OBJECT;
                    if (IsBaseValueType(vt, ref lost_t) == true)
                    {
                        //SerializeKey(fieldNumberAttribute.Number, lost_t);
                        BaseValueSerialize(v);
                    }
                    else
                    {
                        Serialize(v, fieldNumberAttribute.Number);
                    }
                }
                else
                {
                    //object[] arrayValue = (object[])v;
                    object[] arrayValue = ConvertArray(v as Array);
                    ArrayPropertyAttribute att = field.GetCustomAttribute<ArrayPropertyAttribute>();
                    if (att == null)
                    {
                        throw new Exception("ArrayPropertyAttribute is null!");
                    }
                    var lenfield = from lf in pros
                                   where lf.Name == att.LenFieldName
                                   select lf;
                    Int64 len = 1;
                    if (lenfield.ToList().Count != 0)
                    {
                        len = Convert.ToInt64(lenfield.ToList()[0].GetValue(s));
                    }
                    //SerializeKey(fieldNumberAttribute.Number, Type_t.TYPE_ARRAY);
                    //SerializeLen(len);
                    var aet = (v as Array).GetType().GetElementType();
                    Type_t lost_t = Type_t.TYPE_OBJECT;
                    if (IsBaseValueType(aet, ref lost_t) == true)
                    {
                        //SerializeArrayElementFlag((byte)((byte)lost_t << 4 | 0x01));
                        for (Int64 i = 0; i < len; i++)
                        {
                            var aev = arrayValue[i];
                            BaseValueSerialize(aev);
                        }
                    }
                    else
                    {
                        //SerializeArrayElementFlag(0x02);
                        for (Int64 i = 0; i < len; i++)
                        {
                            var aev = arrayValue[i];
                            NoMataData_SerializeSubField(aev);
                        }
                    }

                }
            }
            //SerializeEndFlag();
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
    public sealed class BitsFieldLengthAttribute : Attribute
    {
        public int Length { get; set; }
        public BitsFieldLengthAttribute(int len)
        {
            Length = len;
        }
    }
    [AttributeUsage(AttributeTargets.Property, Inherited = false, AllowMultiple = true)]
    public sealed class FieldNumberAttribute : Attribute
    {
        public FieldNumberAttribute(UInt32 number)
        {
            Number = number;
        }

        // This is a named argument
        public UInt32 Number { get; set; }
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
#if false
    public class Serialization
    {


        public T Deserialize<T>(SerializationManager sm)
        {
            return (T)Deserialize(typeof(T), sm);
        }
        
        private object BaseValueDeserialize(Type vt, SerializationManager sm)
        {
            if (vt.IsEnum == true)
            {
                vt = vt.GetEnumUnderlyingType();
            }
            if (vt == typeof(bool))
            {
                return Convert.ToBoolean(FromBytes(sm, 1));
                //field.SetValue(s, Convert.ToByte(FromBytes(sm, 1)));
            }
            else if (vt == typeof(byte))
            {
                return Convert.ToByte(FromBytes(sm, 1));
                //field.SetValue(s, Convert.ToByte(FromBytes(sm, 1)));
            }
            else if (vt == typeof(sbyte))
            {
                Convert.ToSByte(FromBytes(sm, 1));
                //field.SetValue(s, Convert.ToSByte(FromBytes(sm, 1)));
            }
            else if (vt == typeof(UInt16))
            {
                return Convert.ToUInt16(FromBytes(sm, 2));
                //field.SetValue(s, Convert.ToUInt16(FromBytes(sm, 2)));
            }
            else if (vt == typeof(Int16))
            {
                return Convert.ToInt16(FromBytes(sm, 2));
                //field.SetValue(s, Convert.ToInt16(FromBytes(sm, 2)));
            }
            else if (vt == typeof(UInt32))
            {
                return Convert.ToUInt32(FromBytes(sm, 4));
                //field.SetValue(s, Convert.ToUInt32(FromBytes(sm, 4)));
            }
            else if (vt == typeof(Int32))
            {
                return Convert.ToInt32(FromBytes(sm, 4));
                //field.SetValue(s, Convert.ToInt32(FromBytes(sm, 4)));
            }
            else if (vt == typeof(UInt64))
            {
                return Convert.ToUInt64(FromBytes(sm, 8));
                //field.SetValue(s, Convert.ToUInt64(FromBytes(sm, 8)));
            }
            else if (vt == typeof(Int64))
            {
                return Convert.ToInt64(FromBytes(sm, 8));
                //field.SetValue(s, Convert.ToInt64(FromBytes(sm, 8)));
            }
            else
            {
                throw new Exception("not support!");
                //var subobj = Assembly.CreateInstance(vt.FullName);
                //subobj = Deserialize(vt, sm);
                //field.SetValue(s, subobj);
            }
            return null;
        }
        public object Deserialize(Type st,SerializationManager sm)
        {
            if(IsBaseValueType (st)== true)
            {
                var re= BaseValueDeserialize(st, sm);
                return re;
            }
            var s = Assembly.CreateInstance(st.FullName);
            var pros = st.GetProperties();
            foreach (var field in pros)
            {
                var vt = field.PropertyType;
                if(vt.IsArray==false)
                {
                    if(IsBaseValueType(field.PropertyType) ==true)
                    {
                        field.SetValue(s, BaseValueDeserialize(vt, sm));
                    }
                    else
                    {
                        var subobj = Assembly.CreateInstance(vt.FullName);
                        subobj = Deserialize(vt, sm);
                        field.SetValue(s, subobj);
                    }
                }
                else
                {
                    ArrayPropertyAttribute att = field.GetCustomAttribute<ArrayPropertyAttribute>();
                    if (att == null)
                    {
                        throw new Exception("ArrayPropertyAttribute is null!");
                    }
                    var lenfield = from lf in pros
                                   where lf.Name == att.LenFieldName
                                   select lf;
                    Int64 len = 1;
                    if (lenfield.ToList().Count != 0)
                    {
                        //执行到这一步的时候 len字段的值必须被设置好(也就是说必须被反序列化完毕)
                        //从编写IDL文件方面来说，len字段必须放到array字段的前面;
                        len = Convert.ToInt64(lenfield.ToList()[0].GetValue(s));
                    }
                    var arrayfield = Array.CreateInstance(vt.GetElementType(), len);
                    for (Int64 i = 0; i < len; i++)
                    {
                        var arrayElementType = vt.GetElementType();
                        var item=Deserialize(arrayElementType, sm);
                        arrayfield.SetValue(item, i);
                    }
                    field.SetValue(s, arrayfield);
                }
                
            }
            return s;
        }
        
    }
#endif
}
