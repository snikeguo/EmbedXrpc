using System;
using System.Reflection;
using System.Collections.Generic;
using EmbedXrpc;
using System.Threading.Tasks;
// auto code gen ! DO NOT modify this file!
//C# Code Generater Version:4.5.0.0
namespace Sample1
{
    using UInt8 = Byte;
    using Int8 = SByte;
    public enum Sex : UInt64
    {
        Man = 2,
        WoMan = 9,
    }
    public class AddResult : IEmbedXrpcSerialization
    {
        public const int AddResult_boolTest_FieldNumber = 1;
        [FieldNumber(1)]
        [ArrayLenFieldFlag(false)]
        public bool boolTest { get; set; }


        public const int AddResult_Sum_FieldNumber = 2;
        [BitField(11)]
        [FieldNumber(2)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum { get; set; }


        public const int AddResult_Sum2_FieldNumber = 3;
        [BitField(19)]
        [FieldNumber(3)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum2 { get; set; }


        public const int AddResult_Sum3_FieldNumber = 4;
        [BitField(2)]
        [FieldNumber(4)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum3 { get; set; }


        public const int AddResult_dataLen_FieldNumber = 5;
        [FieldNumber(5)]
        [ArrayLenFieldFlag(false)]
        public Int32 dataLen { get; set; }


        public const int AddResult_data_FieldNumber = 6;
        [NoSerialization]
        [ArrayProperty(LenFieldName = "dataLen")]
        [FieldNumber(6)]
        public byte[] data { get; set; } = new byte[0];


        public const int AddResult_Sum4_FieldNumber = 7;
        [BitField(11)]
        [FieldNumber(7)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum4 { get; set; }


        public const int AddResult_Sum5_FieldNumber = 8;
        [BitField(19)]
        [FieldNumber(8)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum5 { get; set; }


        public const int AddResult_Sum6_FieldNumber = 9;
        [BitField(2)]
        [FieldNumber(9)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum6 { get; set; }


        public const int AddResult_Sum7_FieldNumber = 10;
        [FieldNumber(10)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum7 { get; set; }


        public const int AddResult_PtrTest1_FieldNumber = 11;
        [NoSerialization]
        [FieldNumber(11)]
        [ArrayLenFieldFlag(false)]
        public IntPtr PtrTest1 { get; set; }


        public const int AddResult_data1_FieldNumber = 12;
        [ArrayProperty(LenFieldName = "")]
        [FieldNumber(12)]
        public byte[] data1 { get; set; } = new byte[1];


        public void Serialize(SerializationManager sm)
        {
            byte[] bytes = null;
            if (sm.Buf != null) bytes = sm.ToBytes(boolTest, typeof(System.Boolean));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 1;
            UInt32 bitsTempValue0 = 0;
            bitsTempValue0 |= ((UInt32)(Sum)) << 0;
            bitsTempValue0 |= ((UInt32)(Sum2)) << 11;
            bitsTempValue0 |= ((UInt32)(Sum3)) << 30;
            if (sm.Buf != null) bytes = sm.ToBytes(bitsTempValue0, typeof(System.UInt32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(dataLen, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            //data:NoSerialization
            UInt64 bitsTempValue1 = 0;
            bitsTempValue1 |= ((UInt64)(Sum4)) << 0;
            bitsTempValue1 |= ((UInt64)(Sum5)) << 11;
            bitsTempValue1 |= ((UInt64)(Sum6)) << 30;
            if (sm.Buf != null) bytes = sm.ToBytes(bitsTempValue1, typeof(System.UInt64));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 8;
            if (sm.Buf != null) bytes = sm.ToBytes(Sum7, typeof(System.UInt32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            //PtrTest1:NoSerialization
            for (UInt32 data1_index = 0; data1_index < 1; data1_index++)
            {
                if (sm.Buf != null) bytes = sm.ToBytes(data1[data1_index], typeof(System.Byte));
                if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
                sm.Index += 1;
            }
        }

        public void Deserialize(SerializationManager sm)
        {
            boolTest = (bool)sm.DeserializeField(typeof(System.Boolean), 1);
            UInt32 bitsTempValue0 = 0;
            bitsTempValue0 = (UInt32)sm.DeserializeField(typeof(System.UInt32), 4);
            Sum = bitsTempValue0 >> 0;
            Sum2 = bitsTempValue0 >> 11;
            Sum3 = bitsTempValue0 >> 30;
            dataLen = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            //data:NoSerialization
            UInt64 bitsTempValue1 = 0;
            bitsTempValue1 = (UInt64)sm.DeserializeField(typeof(System.UInt64), 8);
            Sum4 = bitsTempValue1 >> 0;
            Sum5 = bitsTempValue1 >> 11;
            Sum6 = bitsTempValue1 >> 30;
            Sum7 = (UInt32)sm.DeserializeField(typeof(System.UInt32), 4);
            //PtrTest1:NoSerialization
            data1 = new byte[1];
            for (UInt32 data1_index = 0; data1_index < 1; data1_index++)
            {
                data1[data1_index] = (byte)sm.DeserializeField(typeof(System.Byte), 1);
            }
        }

    }
    public class Student : IEmbedXrpcSerialization
    {
#if MyMacro == true
        public const int Student_a_FieldNumber = 1;
        [NoSerialization]
        [FieldNumber(1)]
        [ArrayLenFieldFlag(false)]
        public Int32 a { get; set; }
#endif // #if MyMacro==true


        public const int Student_b_FieldNumber = 2;
        [FieldNumber(2)]
        [ArrayLenFieldFlag(false)]
        public Int32 b { get; set; }


        public const int Student_AddressType_FieldNumber = 3;
        [UnionTargetType]
        [FieldNumber(3)]
        [ArrayLenFieldFlag(false)]
        public Int32 AddressType { get; set; }


        public const int Student_u1_FieldNumber = 4;
        [UnionField]
        [FieldNumber(4)]
        [ArrayLenFieldFlag(false)]
        public byte u1 { get; set; }


#if MyMacro == true
        public const int Student_u2_FieldNumber = 5;
        [UnionField]
        [FieldNumber(5)]
        [ArrayLenFieldFlag(false)]
        public UInt64 u2 { get; set; }
#endif // #if MyMacro==true


        public const int Student_u3_FieldNumber = 6;
        [UnionField]
        [FieldNumber(6)]
        [ArrayLenFieldFlag(false)]
        public Sex u3 { get; set; }


        public const int Student_u4_FieldNumber = 7;
        [UnionField]
        [FieldNumber(7)]
        public AddResult u4 { get; set; } = new AddResult();


        public const int Student_Sum_FieldNumber = 8;
        [BitField(11)]
        [FieldNumber(8)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum { get; set; }


        public const int Student_Sum2_FieldNumber = 9;
        [BitField(19)]
        [FieldNumber(9)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum2 { get; set; }


        public const int Student_Sum3_FieldNumber = 10;
        [BitField(2)]
        [FieldNumber(10)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum3 { get; set; }


        public const int Student_uend1_FieldNumber = 100;
        [FieldNumber(100)]
        [ArrayLenFieldFlag(false)]
        public byte uend1 { get; set; }


        public const int Student_uend2_FieldNumber = 101;
        [FieldNumber(101)]
        [ArrayLenFieldFlag(false)]
        public byte uend2 { get; set; }


        public const int Student_Sum4_FieldNumber = 11;
        [BitField(11)]
        [FieldNumber(11)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum4 { get; set; }


        public const int Student_Sum5_FieldNumber = 12;
        [BitField(19)]
        [FieldNumber(12)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum5 { get; set; }


        public const int Student_Sum6_FieldNumber = 13;
        [BitField(2)]
        [FieldNumber(13)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum6 { get; set; }


        public const int Student_Sum7_FieldNumber = 14;
        [FieldNumber(14)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum7 { get; set; }


        public void Serialize(SerializationManager sm)
        {
            byte[] bytes = null;
            //a:NoSerialization
            if (sm.Buf != null) bytes = sm.ToBytes(b, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(AddressType, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            switch (AddressType)
            {
                case Student_u1_FieldNumber:
                    if (sm.Buf != null) bytes = sm.ToBytes(u1, typeof(System.Byte));
                    if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
                    sm.Index += 1;
                    break;
#if MyMacro == true
                case Student_u2_FieldNumber:
                    if (sm.Buf != null) bytes = sm.ToBytes(u2, typeof(System.UInt64));
                    if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
                    sm.Index += 8;
                    break;
#endif // #if MyMacro==true
                case Student_u3_FieldNumber:
                    if (sm.Buf != null) bytes = sm.ToBytes(u3, typeof(System.UInt64));
                    if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
                    sm.Index += 8;
                    break;
                case Student_u4_FieldNumber:
                    u4.Serialize(sm);
                    break;
            }
            UInt32 bitsTempValue2 = 0;
            bitsTempValue2 |= ((UInt32)(Sum)) << 0;
            bitsTempValue2 |= ((UInt32)(Sum2)) << 11;
            bitsTempValue2 |= ((UInt32)(Sum3)) << 30;
            if (sm.Buf != null) bytes = sm.ToBytes(bitsTempValue2, typeof(System.UInt32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(uend1, typeof(System.Byte));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 1;
            if (sm.Buf != null) bytes = sm.ToBytes(uend2, typeof(System.Byte));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 1;
            UInt64 bitsTempValue3 = 0;
            bitsTempValue3 |= ((UInt64)(Sum4)) << 0;
            bitsTempValue3 |= ((UInt64)(Sum5)) << 11;
            bitsTempValue3 |= ((UInt64)(Sum6)) << 30;
            if (sm.Buf != null) bytes = sm.ToBytes(bitsTempValue3, typeof(System.UInt64));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 8;
            if (sm.Buf != null) bytes = sm.ToBytes(Sum7, typeof(System.UInt32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
        }

        public void Deserialize(SerializationManager sm)
        {
            //a:NoSerialization
            b = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            AddressType = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            switch (AddressType)
            {
                case Student_u1_FieldNumber:
                    u1 = (byte)sm.DeserializeField(typeof(System.Byte), 1);
                    break;
#if MyMacro == true
                case Student_u2_FieldNumber:
                    u2 = (UInt64)sm.DeserializeField(typeof(System.UInt64), 8);
                    break;
#endif // #if MyMacro==true
                case Student_u3_FieldNumber:
                    u3 = (Sex)sm.DeserializeField(typeof(System.UInt64), 8);
                    break;
                case Student_u4_FieldNumber:
                    u4 = new AddResult();
                    u4.Deserialize(sm);
                    break;
            }
            UInt32 bitsTempValue2 = 0;
            bitsTempValue2 = (UInt32)sm.DeserializeField(typeof(System.UInt32), 4);
            Sum = bitsTempValue2 >> 0;
            Sum2 = bitsTempValue2 >> 11;
            Sum3 = bitsTempValue2 >> 30;
            uend1 = (byte)sm.DeserializeField(typeof(System.Byte), 1);
            uend2 = (byte)sm.DeserializeField(typeof(System.Byte), 1);
            UInt64 bitsTempValue3 = 0;
            bitsTempValue3 = (UInt64)sm.DeserializeField(typeof(System.UInt64), 8);
            Sum4 = bitsTempValue3 >> 0;
            Sum5 = bitsTempValue3 >> 11;
            Sum6 = bitsTempValue3 >> 30;
            Sum7 = (UInt32)sm.DeserializeField(typeof(System.UInt32), 4);
        }

    }
    public class Student2 : IEmbedXrpcSerialization
    {
        public const int Student2_u8_FieldNumber = 1;
        [FieldNumber(1)]
        [ArrayLenFieldFlag(false)]
        public byte u8 { get; set; }


        public const int Student2_s8_FieldNumber = 2;
        [FieldNumber(2)]
        [ArrayLenFieldFlag(false)]
        public sbyte s8 { get; set; }


        public const int Student2_u16_FieldNumber = 3;
        [FieldNumber(3)]
        [ArrayLenFieldFlag(false)]
        public UInt16 u16 { get; set; }


        public const int Student2_i16_FieldNumber = 4;
        [FieldNumber(4)]
        [ArrayLenFieldFlag(false)]
        public Int16 i16 { get; set; }


        public const int Student2_u32_FieldNumber = 5;
        [FieldNumber(5)]
        [ArrayLenFieldFlag(false)]
        public UInt32 u32 { get; set; }


        public const int Student2_i32_FieldNumber = 6;
        [FieldNumber(6)]
        [ArrayLenFieldFlag(false)]
        public Int32 i32 { get; set; }


        public const int Student2_u64_FieldNumber = 7;
        [FieldNumber(7)]
        [ArrayLenFieldFlag(false)]
        public UInt64 u64 { get; set; }


        public const int Student2_i64_FieldNumber = 8;
        [FieldNumber(8)]
        [ArrayLenFieldFlag(false)]
        public Int64 i64 { get; set; }


        public const int Student2_f_FieldNumber = 9;
        [FieldNumber(9)]
        [ArrayLenFieldFlag(false)]
        public float f { get; set; }


        public const int Student2_d_FieldNumber = 10;
        [FieldNumber(10)]
        [ArrayLenFieldFlag(false)]
        public double d { get; set; }


        public void Serialize(SerializationManager sm)
        {
            byte[] bytes = null;
            if (sm.Buf != null) bytes = sm.ToBytes(u8, typeof(System.Byte));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 1;
            if (sm.Buf != null) bytes = sm.ToBytes(s8, typeof(System.SByte));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 1;
            if (sm.Buf != null) bytes = sm.ToBytes(u16, typeof(System.UInt16));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 2;
            if (sm.Buf != null) bytes = sm.ToBytes(i16, typeof(System.Int16));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 2;
            if (sm.Buf != null) bytes = sm.ToBytes(u32, typeof(System.UInt32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(i32, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(u64, typeof(System.UInt64));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 8;
            if (sm.Buf != null) bytes = sm.ToBytes(i64, typeof(System.Int64));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 8;
            if (sm.Buf != null) bytes = sm.ToBytes(f, typeof(System.Single));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(d, typeof(System.Double));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 8;
        }

        public void Deserialize(SerializationManager sm)
        {
            u8 = (byte)sm.DeserializeField(typeof(System.Byte), 1);
            s8 = (sbyte)sm.DeserializeField(typeof(System.SByte), 1);
            u16 = (UInt16)sm.DeserializeField(typeof(System.UInt16), 2);
            i16 = (Int16)sm.DeserializeField(typeof(System.Int16), 2);
            u32 = (UInt32)sm.DeserializeField(typeof(System.UInt32), 4);
            i32 = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            u64 = (UInt64)sm.DeserializeField(typeof(System.UInt64), 8);
            i64 = (Int64)sm.DeserializeField(typeof(System.Int64), 8);
            f = (float)sm.DeserializeField(typeof(System.Single), 4);
            d = (double)sm.DeserializeField(typeof(System.Double), 8);
        }

    }
    public class DateTime_t : IEmbedXrpcSerialization
    {
        public const int DateTime_t_Year_FieldNumber = 10086;
        [FieldNumber(10086)]
        [ArrayLenFieldFlag(false)]
        public Int32 Year { get; set; }


        public const int DateTime_t_Month_FieldNumber = 2;
        [FieldNumber(2)]
        [ArrayLenFieldFlag(false)]
        public Int32 Month { get; set; }


        public const int DateTime_t_Day_FieldNumber = 3;
        [FieldNumber(3)]
        [ArrayLenFieldFlag(false)]
        public Int32 Day { get; set; }


        public const int DateTime_t_Hour_FieldNumber = 4;
        [FieldNumber(4)]
        [ArrayLenFieldFlag(false)]
        public Int32 Hour { get; set; }


        public const int DateTime_t_Min_FieldNumber = 5;
        [FieldNumber(5)]
        [ArrayLenFieldFlag(false)]
        public Int32 Min { get; set; }


        public const int DateTime_t_Sec_FieldNumber = 6;
        [FieldNumber(6)]
        [ArrayLenFieldFlag(false)]
        public Int32 Sec { get; set; }


        public const int DateTime_t_Sex_FieldNumber = 8;
        [FieldNumber(8)]
        [ArrayLenFieldFlag(false)]
        public Sex Sex { get; set; }


        public const int DateTime_t_David_FieldNumber = 7;
        [FieldNumber(7)]
        public Student David { get; set; } = new Student();


        public void Serialize(SerializationManager sm)
        {
            byte[] bytes = null;
            if (sm.Buf != null) bytes = sm.ToBytes(Year, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(Month, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(Day, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(Hour, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(Min, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(Sec, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(Sex, typeof(System.UInt64));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 8;
            David.Serialize(sm);
        }

        public void Deserialize(SerializationManager sm)
        {
            Year = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            Month = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            Day = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            Hour = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            Min = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            Sec = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            Sex = (Sex)sm.DeserializeField(typeof(System.UInt64), 8);
            David = new Student();
            David.Deserialize(sm);
        }

    }
    public class DateTimeChange_Parameter : IEmbedXrpcSerialization
    {
        public const int DateTimeChange_Parameter_now_FieldNumber = 1;
        [ArrayProperty(LenFieldName = "")]
        [FieldNumber(1)]
        public DateTime_t[] now { get; set; } = new DateTime_t[1];


        public void Serialize(SerializationManager sm)
        {
            byte[] bytes = null;
            for (UInt32 now_index = 0; now_index < 1; now_index++)
            {
                now[now_index].Serialize(sm);
            }
        }

        public void Deserialize(SerializationManager sm)
        {
            now = new DateTime_t[1];
            for (UInt32 now_index = 0; now_index < 1; now_index++)
            {
                now[now_index] = new DateTime_t();
                now[now_index].Deserialize(sm);
            }
        }

    }
    public class DateTimeChange_Return : IEmbedXrpcSerialization
    {
        public const int DateTimeChange_Return_State_FieldNumber = 1;
        [FieldNumber(1)]
        [ArrayLenFieldFlag(false)]
        public RequestResponseState State { get; set; }


        public void Serialize(SerializationManager sm)
        {
            byte[] bytes = null;
            if (sm.Buf != null) bytes = sm.ToBytes(State, typeof(System.Byte));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 1;
        }

        public void Deserialize(SerializationManager sm)
        {
            State = (RequestResponseState)sm.DeserializeField(typeof(System.Byte), 1);
        }

    }
#if MyMacro == true
    public class DateTimeChange_Requester<DTL> : IRequestService<DTL> //where DTL:struct
    {
        private EmbedXrpcObject<DTL> XrpcObject = null;
        public DateTimeChange_Requester(EmbedXrpcObject<DTL> xrpcObject)
        {
            XrpcObject = xrpcObject;
        }
        public static readonly UInt16 DateTimeChange_ServiceId = 16;//0x10
        public UInt16 GetSid() { return DateTimeChange_ServiceId; }
        public async Task<DateTimeChange_Return> Invoke(DTL userDataOfTransportLayer, DateTime_t[] now)
        {
            DateTimeChange_Return reqresp = new DateTimeChange_Return();
            XrpcObject.ObjectMutex.WaitOne();
            XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
            DateTimeChange_Parameter request = new DateTimeChange_Parameter();
            request.now = now;
            SerializationManager sm = new SerializationManager(Assembly.GetExecutingAssembly(), XrpcObject.RequestBuffer, 12);
            request.Serialize(sm);
            XrpcObject.RequestBuffer[0] = ((byte)(DateTimeChange_ServiceId & 0xff));
            XrpcObject.RequestBuffer[1] = ((byte)(DateTimeChange_ServiceId >> 8 & 0xff));
            XrpcObject.RequestBuffer[2] = ((byte)(XrpcObject.TimeOut & 0xff));
            XrpcObject.RequestBuffer[3] = ((byte)(((XrpcObject.TimeOut >> 8 & 0xff) & 0x3F) | ((byte)ReceiveType.Request) << 6));

            var requestObject_dataLen = sm.Index - 12;
            XrpcObject.RequestBuffer[4] = ((byte)(requestObject_dataLen & 0xff));
            XrpcObject.RequestBuffer[5] = ((byte)(requestObject_dataLen >> 8 & 0xff));
            XrpcObject.RequestBuffer[6] = ((byte)(requestObject_dataLen >> 16 & 0xff));
            XrpcObject.RequestBuffer[7] = ((byte)(requestObject_dataLen >> 24 & 0xff));

            UInt32 bufcrc = XrpcObject.GetBufferCrc(sm.Buf, 12, requestObject_dataLen);
            XrpcObject.RequestBuffer[8] = ((byte)(bufcrc & 0xff));
            XrpcObject.RequestBuffer[9] = ((byte)(bufcrc >> 8 & 0xff));
            XrpcObject.RequestBuffer[10] = ((byte)(bufcrc >> 16 & 0xff));
            XrpcObject.RequestBuffer[11] = ((byte)(bufcrc >> 24 & 0xff));
            var send_result = await XrpcObject.Send(userDataOfTransportLayer, sm.Index, 0, XrpcObject.RequestBuffer);
            if (send_result == false)
            {
                reqresp.State = RequestResponseState.RequestState_Failed;
                goto exi;
            }
            else
            {
                reqresp.State = RequestResponseState.RequestState_Ok;
            }
        exi:
            XrpcObject.ObjectMutex.ReleaseMutex();
            return reqresp;
        }
    }
#endif // #if MyMacro==true
#if MyMacro == true
    [ResponseServiceInfo(Name = "DateTimeChange", ServiceId = 16)]
    public partial class DateTimeChange_Service<DTL> : IService<DTL> //where DTL:struct
    {
        public static readonly UInt16 DateTimeChange_ServiceId = 16;//0x10
        public UInt16 GetSid() { return DateTimeChange_ServiceId; }
        public async Task Invoke(ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
        {
            DateTimeChange_Parameter request = new DateTimeChange_Parameter();
            request.Deserialize(recManager);
            await DateTimeChange(serviceInvokeParameter, request.now);
        }
        /*
        public partial class DateTimeChange_Service<DTL>:IService<DTL> //where DTL:struct
        {
        public async Task DateTimeChange(ServiceInvokeParameter<DTL> serviceInvokeParameter,DateTime_t[] now){}
        }
        */
    }
#endif // #if MyMacro==true


    public class Add_Parameter : IEmbedXrpcSerialization
    {
        public const int Add_Parameter_a_FieldNumber = 1;
        [FieldNumber(1)]
        [ArrayLenFieldFlag(false)]
        public Int32 a { get; set; }


        public const int Add_Parameter_b_FieldNumber = 2;
        [FieldNumber(2)]
        [ArrayLenFieldFlag(false)]
        public Int32 b { get; set; }


        public const int Add_Parameter_dataLen_FieldNumber = 3;
        [FieldNumber(3)]
        [ArrayLenFieldFlag(false)]
        public Int32 dataLen { get; set; }


        public const int Add_Parameter_data_FieldNumber = 4;
        [ArrayProperty(LenFieldName = "dataLen")]
        [FieldNumber(4)]
        public byte[] data { get; set; } = new byte[0];


        public void Serialize(SerializationManager sm)
        {
            byte[] bytes = null;
            if (sm.Buf != null) bytes = sm.ToBytes(a, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(b, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            if (sm.Buf != null) bytes = sm.ToBytes(dataLen, typeof(System.Int32));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 4;
            for (Int32 data_index = 0; data_index < dataLen; data_index++)
            {
                if (sm.Buf != null) bytes = sm.ToBytes(data[data_index], typeof(System.Byte));
                if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
                sm.Index += 1;
            }
        }

        public void Deserialize(SerializationManager sm)
        {
            a = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            b = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            dataLen = (Int32)sm.DeserializeField(typeof(System.Int32), 4);
            data = new byte[dataLen];
            for (Int32 data_index = 0; data_index < dataLen; data_index++)
            {
                data[data_index] = (byte)sm.DeserializeField(typeof(System.Byte), 1);
            }
        }

    }
    public class Add_Return : IEmbedXrpcSerialization
    {
        public const int Add_Return_State_FieldNumber = 1;
        [FieldNumber(1)]
        [ArrayLenFieldFlag(false)]
        public RequestResponseState State { get; set; }


        public const int Add_Return_ReturnValue_FieldNumber = 2;
        [FieldNumber(2)]
        [ArrayLenFieldFlag(false)]
        public Int64 ReturnValue { get; set; }


        public void Serialize(SerializationManager sm)
        {
            byte[] bytes = null;
            if (sm.Buf != null) bytes = sm.ToBytes(State, typeof(System.Byte));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 1;
            if (sm.Buf != null) bytes = sm.ToBytes(ReturnValue, typeof(System.Int64));
            if (sm.Buf != null) Array.Copy(bytes, 0, sm.Buf, sm.Index, bytes.Length);
            sm.Index += 8;
        }

        public void Deserialize(SerializationManager sm)
        {
            State = (RequestResponseState)sm.DeserializeField(typeof(System.Byte), 1);
            ReturnValue = (Int64)sm.DeserializeField(typeof(System.Int64), 8);
        }

    }
    public class Add_Requester<DTL> : IRequestService<DTL> //where DTL:struct
    {
        private EmbedXrpcObject<DTL> XrpcObject = null;
        public Add_Requester(EmbedXrpcObject<DTL> xrpcObject)
        {
            XrpcObject = xrpcObject;
        }
        public static readonly UInt16 Add_ServiceId = 17;//0x11
        public UInt16 GetSid() { return Add_ServiceId; }
        public async Task<Add_Return> Invoke(DTL userDataOfTransportLayer, Int32 a, Int32 b, Int32 dataLen, Byte[] data)
        {
            Add_Return reqresp = new Add_Return();
            XrpcObject.ObjectMutex.WaitOne();
            XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
            Add_Parameter request = new Add_Parameter();
            request.a = a;
            request.b = b;
            request.dataLen = dataLen;
            request.data = data;
            SerializationManager sm = new SerializationManager(Assembly.GetExecutingAssembly(), XrpcObject.RequestBuffer, 12);
            request.Serialize(sm);
            XrpcObject.RequestBuffer[0] = ((byte)(Add_ServiceId & 0xff));
            XrpcObject.RequestBuffer[1] = ((byte)(Add_ServiceId >> 8 & 0xff));
            XrpcObject.RequestBuffer[2] = ((byte)(XrpcObject.TimeOut & 0xff));
            XrpcObject.RequestBuffer[3] = ((byte)(((XrpcObject.TimeOut >> 8 & 0xff) & 0x3F) | ((byte)ReceiveType.Request) << 6));

            var requestObject_dataLen = sm.Index - 12;
            XrpcObject.RequestBuffer[4] = ((byte)(requestObject_dataLen & 0xff));
            XrpcObject.RequestBuffer[5] = ((byte)(requestObject_dataLen >> 8 & 0xff));
            XrpcObject.RequestBuffer[6] = ((byte)(requestObject_dataLen >> 16 & 0xff));
            XrpcObject.RequestBuffer[7] = ((byte)(requestObject_dataLen >> 24 & 0xff));

            UInt32 bufcrc = XrpcObject.GetBufferCrc(sm.Buf, 12, requestObject_dataLen);
            XrpcObject.RequestBuffer[8] = ((byte)(bufcrc & 0xff));
            XrpcObject.RequestBuffer[9] = ((byte)(bufcrc >> 8 & 0xff));
            XrpcObject.RequestBuffer[10] = ((byte)(bufcrc >> 16 & 0xff));
            XrpcObject.RequestBuffer[11] = ((byte)(bufcrc >> 24 & 0xff));
            var send_result = await XrpcObject.Send(userDataOfTransportLayer, sm.Index, 0, XrpcObject.RequestBuffer);
            if (send_result == false)
            {
                reqresp.State = RequestResponseState.RequestState_Failed;
                goto exi;
            }
            else
            {
                reqresp.State = RequestResponseState.RequestState_Ok;
            }
            var waitstate = XrpcObject.Wait<Add_Return>(Add_ServiceId, reqresp);
            if (reqresp == null)
            {
                reqresp = new Add_Return();
            }
            reqresp.State = waitstate;
            goto exi;
        exi:
            XrpcObject.ObjectMutex.ReleaseMutex();
            return reqresp;
        }
    }
    [ResponseServiceInfo(Name = "Add", ServiceId = 17)]
    public partial class Add_Service<DTL> : IService<DTL> //where DTL:struct
    {
        public static readonly UInt16 Add_ServiceId = 17;//0x11
        public UInt16 GetSid() { return Add_ServiceId; }
        private Add_Return Response = new Add_Return();
        public async Task Invoke(ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
        {
            Add_Parameter request = new Add_Parameter();
            request.Deserialize(recManager);
            await Add(serviceInvokeParameter, request.a, request.b, request.dataLen, request.data);
            Response.Serialize(sendManager);
        }
        /*
        public partial class Add_Service<DTL>:IService<DTL> //where DTL:struct
        {
        public async Task Add(ServiceInvokeParameter<DTL> serviceInvokeParameter,Int32 a,Int32 b,Int32 dataLen,Byte[] data){}
        }
        */
    }


}
