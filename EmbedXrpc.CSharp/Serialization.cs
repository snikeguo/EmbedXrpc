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
    public class Serialization
    {
        public static void ToBytes(SerializationManager sm,byte d)
        {
            Console.WriteLine($"u8:{d}");
            sm.Add(d);
        }
        public static void ToBytes(SerializationManager sm, sbyte d)
        {
            Console.WriteLine($"i8:{d}");
            sm.Add((byte)d);
        }
        public static void ToBytes(SerializationManager sm, Int16 d)
        {
            Console.WriteLine($"u16:{d&0xFF},{d>>8 & 0xFF},");
            sm.Add((byte)(d&0xFF));
            sm.Add((byte)(d>>8 & 0xFF));
        }
        public static void ToBytes(SerializationManager sm, UInt16 d)
        {
            Console.WriteLine($"i16:{d & 0xFF},{d >> 8 & 0xFF},");
            sm.Add((byte)(d & 0xFF));
            sm.Add((byte)(d >> 8 & 0xFF));
        }
        public static void ToBytes(SerializationManager sm, UInt32 d)
        {
            Console.WriteLine($"u32:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},");
            sm.Add((byte)(d & 0xFF));
            sm.Add((byte)(d >> 8 & 0xFF));
            sm.Add((byte)(d >> 16 & 0xFF));
            sm.Add((byte)(d >> 24 & 0xFF));
        }
        public static void ToBytes(SerializationManager sm, Int32 d)
        {
            Console.WriteLine($"i32:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},");
            sm.Add((byte)(d & 0xFF));
            sm.Add((byte)(d >> 8 & 0xFF));
            sm.Add((byte)(d >> 16 & 0xFF));
            sm.Add((byte)(d >> 24 & 0xFF));
        }

        public static void ToBytes(SerializationManager sm, UInt64 d)
        {
            Console.WriteLine($"u64:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},  {d>>32 & 0xFF},{d >> 40 & 0xFF},{d >> 48 & 0xFF},{d >> 56 & 0xFF},");
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
            Console.WriteLine($"64:{d & 0xFF},{d >> 8 & 0xFF},{d >> 16 & 0xFF},{d >> 24 & 0xFF},  {d >> 32 & 0xFF},{d >> 40 & 0xFF},{d >> 48 & 0xFF},{d >> 56 & 0xFF},");
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
        public static T Deserialize<T>(SerializationManager sm)
        {
            return (T)Deserialize(typeof(T), sm);
        }
        public static object Deserialize(Type st,SerializationManager sm)
        {
            var s=Assembly.GetExecutingAssembly().CreateInstance(st.FullName);
            var pros = st.GetProperties();
            foreach (var field in pros)
            {
                var vt = field.PropertyType;
                if(vt.IsArray==false)
                {
                    if (vt==typeof(byte))
                    {
                        field.SetValue(s, Convert.ToByte(FromBytes(sm, 1)));
                    }
                    else if (vt == typeof(sbyte))
                    {
                        field.SetValue(s, Convert.ToSByte(FromBytes(sm, 1)));
                    }
                    else if (vt == typeof(UInt16) )
                    {
                        field.SetValue(s,Convert.ToUInt16(FromBytes(sm, 2)) );
                    }
                    else if (vt == typeof(Int16))
                    {
                        field.SetValue(s,Convert.ToInt16(FromBytes(sm, 2)));
                    }
                    else if (vt == typeof(UInt32) )
                    {
                        field.SetValue(s, Convert.ToUInt32(FromBytes(sm, 4)));
                    }
                    else if (vt == typeof(Int32))
                    {
                        field.SetValue(s, Convert.ToInt32(FromBytes(sm, 4)));
                    }
                    else if (vt == typeof(UInt64) )
                    {
                        field.SetValue(s,Convert.ToUInt64(FromBytes(sm, 8)));
                    }
                    else if (vt == typeof(Int64))
                    {
                        field.SetValue(s,Convert.ToInt64(FromBytes(sm, 8)) );
                    }
                    else
                    {
                        var subobj=Assembly.GetExecutingAssembly().CreateInstance(vt.FullName);
                        subobj=Deserialize(vt, sm);
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
                        var item=Assembly.GetExecutingAssembly().CreateInstance(vt.GetElementType().FullName);
                        item=Deserialize(vt.GetElementType(),sm);
                        arrayfield.SetValue(item, i);
                    }
                    field.SetValue(s, arrayfield);
                }
                
            }
            return s;
        }
        public static void Serialize(SerializationManager sm, object s)
        {
            Type st = s.GetType();
            var pros = st.GetProperties();
            foreach (var field in pros)
            {
                object v = field.GetValue(s);
                var vt = v.GetType();
                if(vt.IsArray==false)
                {
                    if (vt==typeof(byte))
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
                else
                {
                    object[] arrayValue = (object[])v;
                    ArrayPropertyAttribute att= field.GetCustomAttribute<ArrayPropertyAttribute>();
                    if(att==null)
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
                    for(Int64 i=0;i<len;i++)
                    {
                        var aev = arrayValue[i];
                        Serialize(sm, aev);
                    }
                }
                
            }
        }
    }
}
