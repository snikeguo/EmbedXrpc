using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.Threading.Tasks;

namespace EmbedXrpc
{
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

        public void Add(byte d)
        {
            Index++;
            data.Add(d);
        }
        public void Reset()
        {
            data.Clear();
            Index = 0;
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
    public class Serialization
    {
        private Assembly Assembly;
        public Serialization(Assembly assembly)
        {
            Assembly = assembly;
        }
        public static void ToBytes(SerializationManager sm, bool d)
        {
            //Console.WriteLine($"u8:{d}");
            ToBytes(sm, Convert.ToByte(d));
        }
        public static void ToBytes(SerializationManager sm,byte d)
        {
            //Console.WriteLine($"u8:{d}");
            sm.Add(d);
        }
        public static void ToBytes(SerializationManager sm, sbyte d)
        {
            //Console.WriteLine($"i8:{d}");
            sm.Add((byte)d);
        }
        public static void ToBytes(SerializationManager sm, Int16 d)
        {
            //Console.WriteLine($"u16:{d&0xFF},{d>>8 & 0xFF},");
            sm.Add((byte)(d&0xFF));
            sm.Add((byte)(d>>8 & 0xFF));
        }
        public static void ToBytes(SerializationManager sm, UInt16 d)
        {
            //Console.WriteLine($"i16:{d & 0xFF},{d >> 8 & 0xFF},");
            sm.Add((byte)(d & 0xFF));
            sm.Add((byte)(d >> 8 & 0xFF));
        }
        public static void ToBytes(SerializationManager sm, UInt32 d)
        {
            //Console.WriteLine($"u32:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},");
            sm.Add((byte)(d & 0xFF));
            sm.Add((byte)(d >> 8 & 0xFF));
            sm.Add((byte)(d >> 16 & 0xFF));
            sm.Add((byte)(d >> 24 & 0xFF));
        }
        public static void ToBytes(SerializationManager sm, Int32 d)
        {
            //Console.WriteLine($"i32:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},");
            sm.Add((byte)(d & 0xFF));
            sm.Add((byte)(d >> 8 & 0xFF));
            sm.Add((byte)(d >> 16 & 0xFF));
            sm.Add((byte)(d >> 24 & 0xFF));
        }

        public static void ToBytes(SerializationManager sm, UInt64 d)
        {
            //Console.WriteLine($"u64:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},  {d>>32 & 0xFF},{d >> 40 & 0xFF},{d >> 48 & 0xFF},{d >> 56 & 0xFF},");
            sm.Add((byte)(d & 0xFF));
            sm.Add((byte)(d >> 8 & 0xFF));
            sm.Add((byte)(d >> 16 & 0xFF));
            sm.Add((byte)(d >> 24 & 0xFF));
            sm.Add((byte)(d >> 32 & 0xFF));
            sm.Add((byte)(d >> 40 & 0xFF));
            sm.Add((byte)(d >> 48 & 0xFF));
            sm.Add((byte)(d >> 56 & 0xFF));
        }
        public static void ToBytes(SerializationManager sm, Int64 d)
        {
            //Console.WriteLine($"64:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},  {d >> 32 & 0xFF},{d >> 40 & 0xFF},{d >> 48 & 0xFF},{d >> 56 & 0xFF},");
            sm.Add((byte)(d & 0xFF));
            sm.Add((byte)(d >> 8 & 0xFF));
            sm.Add((byte)(d >> 16 & 0xFF));
            sm.Add((byte)(d >> 24 & 0xFF));
            sm.Add((byte)(d >> 32 & 0xFF));
            sm.Add((byte)(d >> 40 & 0xFF));
            sm.Add((byte)(d >> 48 & 0xFF));
            sm.Add((byte)(d >> 56 & 0xFF));
        }

        public static object FromBytes(SerializationManager sm,int len)
        {
            if(len==1)
            {
                var v = sm.Data[sm.Index];
                sm.Index++;
                return v;
            }
            else if(len == 2)
            {
                var v = sm.Data[sm.Index+1]<<8|sm.Data[sm.Index];
                sm.Index+=2;
                return v;
            }
            else if (len == 4)
            {
                var v = sm.Data[sm.Index + 3] << 24 | sm.Data[sm.Index+2]<<16| sm.Data[sm.Index + 1] << 8 | sm.Data[sm.Index];
                sm.Index += 4;
                return v;
            }
            else if (len == 8)
            {
                var v = sm.Data[sm.Index + 7] << 56 | sm.Data[sm.Index + 6] << 48 | sm.Data[sm.Index + 5] << 40 | sm.Data[sm.Index+4]<<32
                    | sm.Data[sm.Index + 3] << 24 | sm.Data[sm.Index + 2] << 16 | sm.Data[sm.Index + 1] << 8 | sm.Data[sm.Index];
                sm.Index += 8;
                return v;
            }
            throw new Exception($"len is {len},but len only support 1/2/4/8");
        }
        public T Deserialize<T>(SerializationManager sm)
        {
            return (T)Deserialize(typeof(T), sm);
        }
        private bool IsBaseValueType(Type t)
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
        private void BaseValueSerialize(SerializationManager sm, object v)
        {
            Type vt = v.GetType();
            if (vt.IsEnum == true)
            {
                vt = vt.GetEnumUnderlyingType();
            }
            if (vt == typeof(bool))
            {
                ToBytes(sm, (bool)v);
            }
            else if (vt == typeof(byte))
            {
                ToBytes(sm, (byte)v);
            }
            else if (vt == typeof(sbyte))
            {
                ToBytes(sm, (sbyte)v);
            }
            else if (vt == typeof(UInt16))
            {
                ToBytes(sm, (UInt16)v);
            }
            else if (vt == typeof(Int16))
            {
                ToBytes(sm, (Int16)v);
            }
            else if (vt == typeof(UInt32))
            {
                ToBytes(sm, (UInt32)v);
            }
            else if (vt == typeof(Int32))
            {
                ToBytes(sm, (Int32)v);
            }
            else if (vt == typeof(UInt64))
            {
                ToBytes(sm, (UInt64)v);
            }
            else if (vt == typeof(Int64))
            {
                ToBytes(sm, (Int64)v);
            }
            else
            {
                Serialize(sm, v);
            }
        }
        public void Serialize(SerializationManager sm, object s)
        {
            Type st = s.GetType();
            if(IsBaseValueType(st)==true)
            {
                BaseValueSerialize(sm, s);
            }
            else
            {
                var pros = st.GetProperties();
                foreach (var field in pros)
                {
                    object v = field.GetValue(s);
                    var vt = v.GetType();
                    if (vt.IsArray == false)
                    {
                        if (IsBaseValueType(vt) == true)
                        {
                            BaseValueSerialize(sm, v);
                        }
                        else
                        {
                            Serialize(sm,v);
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
                        for (Int64 i = 0; i < len; i++)
                        {
                            var aev = arrayValue[i];
                            Serialize(sm, aev);
                        }
                    }

                }
            }
            
        }
    }
}
