using System;
using System.Collections.Generic;
using System.IO;
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
        TYPE_STRUCT,  /*struct*/
        TYPE_UNION,     /*union*/

    };
    public class SerializationManager
    {
        private List<byte> data = new List<byte>();
        
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
        
        //public bool IsEnableMataDataEncode { get; set; } = false;
        void SerializeKey(Int32 FieldNumber, Type_t Field)
        {
            /*Buf[Index++] = FieldNumber;
            Buf[Index++] = Field;
            printf("SerializeKey FieldNumber:%d,Type:%s\n", FieldNumber, TypeString[Field]);*/
            //Console.WriteLine($"SerializeKey:{FieldNumber},{Field.ToString()}");
            Int32 shiftfn = 0;
            Int32 next_shiftfn = 0;
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
        void SerializeLen(UInt32 Len)
        {
            //Console.WriteLine($"SerializeLen:{Len}");
            UInt32 next_shiftlen = 0;
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
            //Console.WriteLine("SerializeEndFlag");
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
            //Console.WriteLine("SerializeArrayElementFlag:0x{0:X}",flag);
            Data.Add(flag);
            Index += 1;
            //printf("SerializeArrayElementFlag:0x%x\n", flag);
        }
        void SerializeValue(byte[] v)//base value used
        {
            //Console.WriteLine("SerializeValue");
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
                Array.Copy()
            }
            Index += len;
            return true;
        }*/
        UInt32 GetKeyFromSerializationManager(ref UInt32 fn, ref Type_t type)
        {
            UInt32 f = 0;
            byte used = 0;
            byte temp = 0;
            type = (Type_t)(Data[Index] & 0x0F);
            if(type> Type_t.TYPE_STRUCT)
            {
                throw new InvalidDataException();
            }
            temp = (byte)((Data[Index] >> 4) & 0x07);//先把最低三位保存起来
            used++;
            if ((Data[Index] & 0x80) != 0)
            {
                do
                {
                    f = (UInt32)(((UInt32)(Data[Index + used] & 0x7f) << (used - 1) * 7) | f);
                    if ((Data[Index + used] & 0x80) == 0)
                    {
                        used++;
                        break;
                    }
                    else
                    {
                        used++;
                    }
                } while (true);
            }
            f = f << 3 | temp;
            fn = f;
            return used;
        }
        void RemoveKeyFromSerializationManager()
        {
            UInt32 fn=0;
            Type_t t = Type_t.TYPE_STRUCT;
            UInt32 ind = GetKeyFromSerializationManager(ref fn, ref t);
            Index += (int)ind;
        }
        byte GetArrayLenFromSerializationManager(ref UInt32 arrayLen)
        {
            byte used = 0;
            UInt32 al = 0;
            do
            {
                al = ((UInt32)(Data[Index + used] & 0x7f) << (used * 7)) | al;
                if ((Data[Index + used] & 0x80) == 0)
                {
                    used++;
                    break;
                }
                else
                {
                    used++;
                }
            } while (true);
            arrayLen = al;
            return used;
        }
        void RemoveArrayLenFromSerializationManager()
        {
            UInt32 al = 0;
            byte ind = GetArrayLenFromSerializationManager(ref al);
            Index += ind;
        }
        byte GetArrayElementFlag()
        {
            return Data[Index];
        }
        void RemoveArrayElementFlagFromSerializationManager()
        {
            Index++;
        }
        void RemoveEndFlagFromSerializationManager()
        {
            Index++;
        }
        private Assembly Assembly;
        public SerializationManager(Assembly assembly,List<byte> data)
        {
            Assembly = assembly;
            //IsEnableMataDataEncode = isEnableMataDataEncode;
            Data = data;
            Index = 0;
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
        public void Serialize(object s,Int32 fieldNumber)
        {
            if (false)//(IsEnableMataDataEncode == true)
            {
                SerializeKey(fieldNumber,Type_t.TYPE_STRUCT);
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
                object value = field.GetValue(s);
                //var vt = v.GetType();
                var vt = field.PropertyType;
                FieldNumberAttribute fieldNumberAttribute = field.GetCustomAttribute<FieldNumberAttribute>();
                ArrayLenFieldFlagAttribute IsArrayLenFieldAttribute = field.GetCustomAttribute<ArrayLenFieldFlagAttribute>();
                if (vt.IsArray == false)
                {
                    Type_t lost_t = Type_t.TYPE_STRUCT;
                    bool isBaseValueTypeFlag = IsBaseValueType(vt, ref lost_t);
                    if (isBaseValueTypeFlag == true && IsArrayLenFieldAttribute.Flag == false)
                    {
                        SerializeKey(fieldNumberAttribute.Number, lost_t);
                        BaseValueSerialize(value);
                    }
                    else if(lost_t== Type_t.TYPE_STRUCT)
                    {
                        if (value == null)
                        {
                            value = Assembly.CreateInstance(field.PropertyType.FullName);
                        }
                        Serialize(value, fieldNumberAttribute.Number);
                    }
                }
                else
                {
                    //object[] arrayValue = (object[])v;
                    object[] arrayValue = ConvertArray(value as Array);
                    ArrayPropertyAttribute att = field.GetCustomAttribute<ArrayPropertyAttribute>();
                    if (att == null)
                    {
                        throw new Exception("ArrayPropertyAttribute is null!");
                    }
                    var lenfield = from lf in pros
                                   where lf.Name == att.LenFieldName
                                   select lf;
                    UInt32 len = 1;
                    if (lenfield.ToList().Count != 0)
                    {
                        len = Convert.ToUInt32(lenfield.ToList()[0].GetValue(s));
                    }
                    SerializeKey(fieldNumberAttribute.Number, Type_t.TYPE_ARRAY);
                    SerializeLen(len);
                    var aet = field.PropertyType.GetElementType();
                    Type_t lost_t = Type_t.TYPE_STRUCT;
                    if (IsBaseValueType(aet, ref lost_t) == true)
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
            Int32 unionTargetTypeValue = -1;
            PropertyInfo field = pros[0];
            int index = 0;
            for (; index < pros.Length; index++)
            {
                field = pros[index];
                object value = field.GetValue(s);
                var vt = field.PropertyType;
                FieldNumberAttribute fieldNumberAttribute = field.GetCustomAttribute<FieldNumberAttribute>();
                UnionTargetTypeAttribute unionTargetTypeAttribute = field.GetCustomAttribute<UnionTargetTypeAttribute>();
                UnionFieldAttribute unionFieldAttribute = field.GetCustomAttribute<UnionFieldAttribute>();
                if (unionFieldAttribute != null && unionTargetTypeValue != fieldNumberAttribute.Number)
                    continue;
                if (vt.IsArray == false)
                {
                    Type_t lost_t = Type_t.TYPE_STRUCT;
                    bool isBaseValueTypeFlag = IsBaseValueType(vt, ref lost_t);
                    if (isBaseValueTypeFlag == true)
                    {
                        //SerializeKey(fieldNumberAttribute.Number, lost_t);
                        BaseValueSerialize(value);
                        
                        if(unionTargetTypeAttribute!=null)
                        {
                            unionTargetTypeValue = (Int32)value;
                        }
                    }
                    else if(lost_t== Type_t.TYPE_STRUCT)
                    {
                        if (value == null)
                        {
                            value = Assembly.CreateInstance(field.PropertyType.FullName);
                        }
                        Serialize(value, fieldNumberAttribute.Number);
                    }
                }
                else
                {
                    //object[] arrayValue = (object[])v;
                    object[] arrayValue = ConvertArray(value as Array);
                    ArrayPropertyAttribute att = field.GetCustomAttribute<ArrayPropertyAttribute>();
                    if (att == null)
                    {
                        throw new Exception("ArrayPropertyAttribute is null!");
                    }
                    var lenfield = from lf in pros
                                   where lf.Name == att.LenFieldName
                                   select lf;
                    UInt32 len = 1;
                    if (lenfield.ToList().Count != 0)
                    {
                        len = Convert.ToUInt32(lenfield.ToList()[0].GetValue(s));
                    }
                    //SerializeKey(fieldNumberAttribute.Number, Type_t.TYPE_ARRAY);
                    //SerializeLen(len);
                    var aet = field.PropertyType.GetElementType();
                    Type_t lost_t = Type_t.TYPE_STRUCT;
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
        public object FromBytes(Type_t vt)
        {
            if (vt ==  Type_t.TYPE_UINT8)
            {
                byte v = Data[Index];
                Index++;
                return v;
            }
            if (vt == Type_t.TYPE_INT8)
            {
                sbyte v = (sbyte)Data[Index];
                Index++;
                return v;
            }
            else if (vt == Type_t.TYPE_UINT16)
            {
                UInt16 v = (UInt16)(Data[Index + 1] << 8 | Data[Index]);
                Index += 2;
                return v;
            }
            else if (vt == Type_t.TYPE_INT16)
            {
                Int16 v = (Int16)(Data[Index + 1] << 8 | Data[Index]);
                Index += 2;
                return v;
            }
            else if (vt == Type_t.TYPE_UINT32)
            {
                UInt32 v = (UInt32)(Data[Index + 3] << 24 | Data[Index + 2] << 16 | Data[Index + 1] << 8 | Data[Index]);
                Index += 4;
                return v;
            }
            else if (vt == Type_t.TYPE_INT32)
            {
                Int32 v = (Int32)(Data[Index + 3] << 24 | Data[Index + 2] << 16 | Data[Index + 1] << 8 | Data[Index]);
                Index += 4;
                return v;
            }
            else if (vt == Type_t.TYPE_UINT64)
            {
                UInt64 v = (UInt64)(Data[Index + 7] << 56 | Data[Index + 6] << 48 | Data[Index + 5] << 40 | Data[Index + 4] << 32
                    | Data[Index + 3] << 24 | Data[Index + 2] << 16 | Data[Index + 1] << 8 | Data[Index]);
                Index += 8;
                return v;
            }
            else if (vt == Type_t.TYPE_INT64)
            {
                Int64 v = (Int64)(Data[Index + 7] << 56 | Data[Index + 6] << 48 | Data[Index + 5] << 40 | Data[Index + 4] << 32
                    | Data[Index + 3] << 24 | Data[Index + 2] << 16 | Data[Index + 1] << 8 | Data[Index]);
                Index += 8;
                return v;
            }
            throw new InvalidDataException($"len is {vt},but len only support 1/2/4/8");
        }

        public T Deserialize<T>()
        {
            return (T)Deserialize(typeof(T));
        }
        public object Deserialize(Type st)
        {
            var s = Assembly.CreateInstance(st.FullName);
            if (false)//(IsEnableMataDataEncode == true)
            {
                UInt32 fn = 0;//Pop一次KEY 因为打包的时候是按照field打包的，所以这里要把KEY 要POP出来一次
                Type_t tp = Type_t.TYPE_UINT8;
                GetKeyFromSerializationManager(ref fn, ref tp);
                RemoveKeyFromSerializationManager();
                if (fn == 0 && tp ==  Type_t.TYPE_STRUCT)
                {
                    DeserializeSubField(st, s);//如果fieldNumber 为0 说明这是第一次进来,也就是最顶级的结构体，最顶级的结构体执行完毕后,就要退出
                }
            }
            else
            {
                NoMataData_DeserializeSubField(st, s);
            }
            return s;
        }
        public bool DeserializeSubField(Type st,object localstruct)
        {
            UInt32 fn = 0;
            Type_t tp =  Type_t.TYPE_UINT8;
            PropertyInfo[] pros = new PropertyInfo[0];
            if(st!=null)//st有可能是null 要给pros赋一个长度为0的数组
            {
                pros = st.GetProperties();
            }
            
            while (!IsEnd())//并没有到结构体定界符以及 没有遍历完数据流	&& Index < BufferLen
            {
                GetKeyFromSerializationManager(ref fn, ref tp);
                RemoveKeyFromSerializationManager();
                var targetfieldinfos = (from bs in pros where bs.GetCustomAttribute<FieldNumberAttribute>().Number == fn select bs).ToList();
                if (tp <=  Type_t.TYPE_DOUBLE)
                {
                    var fieldValue=BaseValueDeserialize(tp);
                    if (targetfieldinfos.Count>0 && localstruct!=null)
                    {
                        targetfieldinfos[0].SetValue(localstruct, fieldValue);
                    }
                }
                else if(tp== Type_t.TYPE_ARRAY)
                {
                    UInt32 arraylen = 0;

                    byte sizeOfArrayLenInStream = GetArrayLenFromSerializationManager(ref arraylen);
                    RemoveArrayLenFromSerializationManager();

                    byte baseValueTypeFlag = GetArrayElementFlag();
                    RemoveArrayElementFlagFromSerializationManager();

                    if(! ((((baseValueTypeFlag & 0x0F) == 1) && ((Type_t)(baseValueTypeFlag >> 4) <= Type_t.TYPE_DOUBLE)) || (baseValueTypeFlag == 0x02)))
                    {
                        throw new InvalidDataException();
                    }

                    Array arrayfield = null;
                    Type arrayElementTypeOfLocalType = null;
                    if (targetfieldinfos.Count>0)
                    {
                        ArrayPropertyAttribute att = targetfieldinfos[0].GetCustomAttribute<ArrayPropertyAttribute>();
                        if (att == null)
                        {
                            throw new Exception("ArrayPropertyAttribute is null!");
                        }
                        var lenfield = (from lf in pros
                                       where lf.Name == att.LenFieldName
                                       select lf).ToList();
                        //UInt64 len = 1;
                        if (lenfield.Count != 0)
                        {
                            //执行到这一步的时候 len字段的值必须被设置好(也就是说必须被反序列化完毕)
                            //从编写IDL文件方面来说，len字段必须放到array字段的前面;
                            //len = Convert.ToInt64(lenfield.ToList()[0].GetValue(s));
                            if (localstruct != null)
                            { 
                                lenfield[0].SetValue(localstruct,Convert.ChangeType(arraylen, lenfield[0].PropertyType));//设置len字段
                            }
                        }
                        arrayElementTypeOfLocalType = targetfieldinfos[0].PropertyType.GetElementType();
                        arrayfield = Array.CreateInstance(arrayElementTypeOfLocalType, (int)arraylen);
                        /*
                        arrayfield = Array.CreateInstance(aet, (int)len);
                        for (UInt64 i = 0; i < len; i++)
                        {
                            var item = Deserialize(aet);
                            arrayfield.SetValue(item, (int)i);
                        }
                        targetfields[0].SetValue(s, arrayfield);*/
                    }
                    if ((baseValueTypeFlag & 0x0F) == 0x01)//base value type
                    {
                        Type_t aet = (Type_t)(baseValueTypeFlag >> 4 & 0x0F);
                        if(aet>Type_t.TYPE_DOUBLE)
                        {
                            throw new InvalidDataException();
                        }
                        for (UInt64 j = 0; j < arraylen; j++)
                        {
                            var aev=BaseValueDeserialize(aet);
                            if(arrayfield!=null)
                            {
                                arrayfield.SetValue(aev, (int)j);
                            }
                        }
                        if(targetfieldinfos.Count>0 && localstruct != null)
                        {
                            targetfieldinfos[0].SetValue(localstruct, arrayfield);
                        }
                    }
                    else
                    {
                        
                        for (UInt64 j = 0; j < arraylen; j++)
                        {
                            if(arrayfield!=null)
                            {
                                var arrayElementObject=Assembly.CreateInstance(arrayElementTypeOfLocalType.FullName);
                                DeserializeSubField(arrayElementTypeOfLocalType, arrayElementObject);
                                arrayfield.SetValue(arrayElementObject, (int)j);
                            }
                            else
                            {
                                DeserializeSubField(null, null);
                            }
                        }
                        if (targetfieldinfos.Count > 0 && localstruct != null)
                        {
                            targetfieldinfos[0].SetValue(localstruct, arrayfield);
                        }
                    }
                }
                else if(tp== Type_t.TYPE_STRUCT)
                {
                    if (targetfieldinfos.Count > 0 && localstruct != null)
                    {
                        var sof = Assembly.CreateInstance(targetfieldinfos[0].PropertyType.FullName);
                        DeserializeSubField(targetfieldinfos[0].PropertyType, sof);
                        targetfieldinfos[0].SetValue(localstruct, sof);
                    }
                    else
                    {
                        DeserializeSubField(null, null);
                    }
                }
            }
            RemoveEndFlagFromSerializationManager();
            return true;
        }
        public bool NoMataData_DeserializeSubField(Type st, object localstruct)
        {
            PropertyInfo[] pros = new PropertyInfo[0];
            Int32 unionTargetTypeValue = -1;
            
            int index = 0;
            if (st != null)//st有可能是null 要给pros赋一个长度为0的数组
            {
                pros = st.GetProperties();
            }
            PropertyInfo fieldinfo = pros[0];
            for (;index<pros.Length;index++)
            {
                fieldinfo = pros[index];
                FieldNumberAttribute fieldNumberAttribute = fieldinfo.GetCustomAttribute<FieldNumberAttribute>();
                UnionFieldAttribute unionFieldAttribute= fieldinfo.GetCustomAttribute<UnionFieldAttribute>();
                if (unionFieldAttribute != null && unionTargetTypeValue != fieldNumberAttribute.Number)
                {
                    continue;
                }
                Type_t tp = Type_t.TYPE_STRUCT;
                IsBaseValueType(fieldinfo.PropertyType, ref tp);
                if(tp <= Type_t.TYPE_DOUBLE)
                {
                    var fieldValue = BaseValueDeserialize(tp);
                    fieldinfo.SetValue(localstruct, fieldValue);
                    UnionTargetTypeAttribute unionTargetTypeAttribute = fieldinfo.GetCustomAttribute<UnionTargetTypeAttribute>();
                    if(unionTargetTypeAttribute!=null)
                    {
                        unionTargetTypeValue = (Int32)fieldValue;
                    }
                }
                else if (tp == Type_t.TYPE_ARRAY)
                {
                    UInt32 arraylen = 1;
                    Array arrayfield = null;
                    Type arrayElementTypeOfLocalType = null;
                    ArrayPropertyAttribute att = fieldinfo.GetCustomAttribute<ArrayPropertyAttribute>();
                    if (att == null)
                    {
                        throw new Exception("ArrayPropertyAttribute is null!");
                    }
                    var lenfield = (from lf in pros
                                    where lf.Name == att.LenFieldName
                                    select lf).ToList();
                    //UInt64 len = 1;
                    if (lenfield.Count != 0)
                    {
                        //执行到这一步的时候 len字段的值必须被设置好(也就是说必须被反序列化完毕)
                        //从编写IDL文件方面来说，len字段必须放到array字段的前面;
                        arraylen = Convert.ToUInt32(lenfield.ToList()[0].GetValue(localstruct));
                    }
                    arrayElementTypeOfLocalType = fieldinfo.PropertyType.GetElementType();
                    arrayfield = Array.CreateInstance(arrayElementTypeOfLocalType, (int)arraylen);
                    Type_t aet = Type_t.TYPE_STRUCT;
                    bool isbaseValueTypeOfArrayElementType = IsBaseValueType(arrayElementTypeOfLocalType, ref aet);
                    if (isbaseValueTypeOfArrayElementType)//base value type
                    {
                        if (aet > Type_t.TYPE_DOUBLE)
                        {
                            throw new InvalidDataException();
                        }
                        for (UInt64 j = 0; j < arraylen; j++)
                        {
                            var aev = BaseValueDeserialize(aet);
                            if (arrayfield != null)
                            {
                                arrayfield.SetValue(aev, (int)j);
                            }
                        }
                        fieldinfo.SetValue(localstruct, arrayfield);
                    }
                    else
                    {

                        for (UInt64 j = 0; j < arraylen; j++)
                        {
                            if (arrayfield != null)
                            {
                                var arrayElementObject = Assembly.CreateInstance(arrayElementTypeOfLocalType.FullName);
                                NoMataData_DeserializeSubField(arrayElementTypeOfLocalType, arrayElementObject);
                                arrayfield.SetValue(arrayElementObject, (int)j);
                            }
                            else
                            {
                                NoMataData_DeserializeSubField(null, null);
                            }
                        }
                        fieldinfo.SetValue(localstruct, arrayfield);
                    }
                }
                else if (tp == Type_t.TYPE_STRUCT)
                {
                    var sof = Assembly.CreateInstance(fieldinfo.PropertyType.FullName);
                    NoMataData_DeserializeSubField(fieldinfo.PropertyType, sof);
                    fieldinfo.SetValue(localstruct, sof);
                }
            }
            return true;
        }
        private object BaseValueDeserialize(Type_t vt)
        {
            if (vt ==  Type_t.TYPE_UINT8)
            {
                return Convert.ToByte(FromBytes(vt));
                //field.SetValue(s, Convert.ToByte(FromBytes( 1)));
            }
            else if(vt == Type_t.TYPE_INT8)
            {
                return Convert.ToSByte(FromBytes(vt));
            }
            else if (vt == Type_t.TYPE_UINT16)
            {
                return Convert.ToUInt16(FromBytes(vt));
            }
            else if (vt == Type_t.TYPE_INT16)
            {
                return Convert.ToInt16(FromBytes(vt));
            }
            else if (vt == Type_t.TYPE_UINT32)
            {
                return Convert.ToUInt32(FromBytes(vt));
                //field.SetValue(s, Convert.ToInt16(FromBytes( 2)));
            }
            else if (vt == Type_t.TYPE_INT32)
            {
                return Convert.ToInt32(FromBytes(vt));
            }
            else if (vt == Type_t.TYPE_FLOAT)
            {
                return Convert.ToDouble(FromBytes(vt));
                //field.SetValue(s, Convert.ToInt16(FromBytes( 2)));
            }
            else if (vt == Type_t.TYPE_UINT64)
            {
                return Convert.ToUInt64(FromBytes(vt));
                //field.SetValue(s, Convert.ToUInt32(FromBytes( 4)));
            }
            else if (vt == Type_t.TYPE_INT64)
            {
                return Convert.ToInt64(FromBytes(vt));
                //field.SetValue(s, Convert.ToUInt32(FromBytes( 4)));
            }
            else if (vt == Type_t.TYPE_DOUBLE)
            {
                return Convert.ToDouble(FromBytes(vt));
                //field.SetValue(s, Convert.ToUInt32(FromBytes( 4)));
            }
            else
            {
                throw new Exception("not support!");
                //var subobj = Assembly.CreateInstance(vt.FullName);
                //subobj = Deserialize(vt, sm);
                //field.SetValue(s, subobj);
            }
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
}
