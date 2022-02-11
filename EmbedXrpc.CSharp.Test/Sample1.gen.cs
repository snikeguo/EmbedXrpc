using System;
using System.Reflection;
using System.Collections.Generic;
using EmbedXrpc;
// auto code gen ! DO NOT modify this file!
//C# Code Generater Version:2.4.1.0
namespace Sample1
{
    using UInt8 = Byte;
    using Int8 = SByte;
    public enum Sex : ulong
    {
        Man = 2,
        WoMan = 9,
    }
    public class AddResult : IEmbedXrpcSerialization
    {
        public const int AddResult_Sum_FieldNumber = 1;
        [BitField(11)]
        [FieldNumber(1)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum { get; set; }


        public const int AddResult_Sum2_FieldNumber = 2;
        [BitField(19)]
        [FieldNumber(2)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum2 { get; set; }


        public const int AddResult_Sum3_FieldNumber = 3;
        [BitField(2)]
        [FieldNumber(3)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum3 { get; set; }


        public const int AddResult_dataLen_FieldNumber = 4;
        [FieldNumber(4)]
        [ArrayLenFieldFlag(false)]
        public Int32 dataLen { get; set; }


        public const int AddResult_data_FieldNumber = 5;
        [NoSerialization]
        [ArrayProperty(LenFieldName = "dataLen")]
        [FieldNumber(5)]
        public UInt8[] data { get; set; } = new UInt8[0];


        public const int AddResult_Sum4_FieldNumber = 6;
        [BitField(11)]
        [FieldNumber(6)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum4 { get; set; }


        public const int AddResult_Sum5_FieldNumber = 7;
        [BitField(19)]
        [FieldNumber(7)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum5 { get; set; }


        public const int AddResult_Sum6_FieldNumber = 8;
        [BitField(2)]
        [FieldNumber(8)]
        [ArrayLenFieldFlag(false)]
        public UInt64 Sum6 { get; set; }


        public const int AddResult_Sum7_FieldNumber = 9;
        [FieldNumber(9)]
        [ArrayLenFieldFlag(false)]
        public UInt32 Sum7 { get; set; }


        public void Serialize(SerializationManager sm)
        {
            UInt32 bitsTempValue0 = 0;
            bitsTempValue0 |= ((UInt32)(Sum)) << 0;
            bitsTempValue0 |= ((UInt32)(Sum2)) << 11;
            bitsTempValue0 |= ((UInt32)(Sum3)) << 30;
            sm.Buf.AddRange(SerializationManager.ToBytes(bitsTempValue0, Type_t.TYPE_UINT32));
            sm.Index += sizeof(UInt32);
            sm.Buf.AddRange(SerializationManager.ToBytes(dataLen, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            //data :NoSerialization
            UInt64 bitsTempValue1 = 0;
            bitsTempValue1 |= ((UInt64)(Sum4)) << 0;
            bitsTempValue1 |= ((UInt64)(Sum5)) << 11;
            bitsTempValue1 |= ((UInt64)(Sum6)) << 30;
            sm.Buf.AddRange(SerializationManager.ToBytes(bitsTempValue1, Type_t.TYPE_UINT64));
            sm.Index += sizeof(UInt64);
            sm.Buf.AddRange(SerializationManager.ToBytes(Sum7, Type_t.TYPE_UINT32));
            sm.Index += sizeof(UInt32);
        }

        public void Deserialize(SerializationManager sm)
        {
            UInt32 bitsTempValue0 = 0;
            bitsTempValue0 = (UInt32)sm.DeserializeField(Type_t.TYPE_UINT32);
            Sum = bitsTempValue0 >> 0;
            Sum2 = bitsTempValue0 >> 11;
            Sum3 = bitsTempValue0 >> 30;
            dataLen = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            //data :NoSerialization
            UInt64 bitsTempValue1 = 0;
            bitsTempValue1 = (UInt64)sm.DeserializeField(Type_t.TYPE_UINT64);
            Sum4 = bitsTempValue1 >> 0;
            Sum5 = bitsTempValue1 >> 11;
            Sum6 = bitsTempValue1 >> 30;
            Sum7 = (UInt32)sm.DeserializeField(Type_t.TYPE_UINT32);
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
        public UInt8 u1 { get; set; }


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
        public UInt8 uend1 { get; set; }


        public const int Student_uend2_FieldNumber = 101;
        [FieldNumber(101)]
        [ArrayLenFieldFlag(false)]
        public UInt8 uend2 { get; set; }


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
            //a :NoSerialization
            sm.Buf.AddRange(SerializationManager.ToBytes(b, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(AddressType, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            switch (AddressType)
            {
                case Student_u1_FieldNumber:
                    sm.Buf.AddRange(SerializationManager.ToBytes(u1, Type_t.TYPE_UINT8));
                    sm.Index += sizeof(UInt8);
                    break;
#if MyMacro == true
                case Student_u2_FieldNumber:
                    sm.Buf.AddRange(SerializationManager.ToBytes(u2, Type_t.TYPE_UINT64));
                    sm.Index += sizeof(UInt64);
                    break;
#endif // #if MyMacro==true
                case Student_u3_FieldNumber:
                    sm.Buf.AddRange(SerializationManager.ToBytes(u3, Type_t.TYPE_UINT64));
                    sm.Index += sizeof(UInt64);
                    break;
                case Student_u4_FieldNumber:
                    u4.Serialize(sm);
                    break;
            }
            UInt32 bitsTempValue0 = 0;
            bitsTempValue0 |= ((UInt32)(Sum)) << 0;
            bitsTempValue0 |= ((UInt32)(Sum2)) << 11;
            bitsTempValue0 |= ((UInt32)(Sum3)) << 30;
            sm.Buf.AddRange(SerializationManager.ToBytes(bitsTempValue0, Type_t.TYPE_UINT32));
            sm.Index += sizeof(UInt32);
            sm.Buf.AddRange(SerializationManager.ToBytes(uend1, Type_t.TYPE_UINT8));
            sm.Index += sizeof(UInt8);
            sm.Buf.AddRange(SerializationManager.ToBytes(uend2, Type_t.TYPE_UINT8));
            sm.Index += sizeof(UInt8);
            UInt64 bitsTempValue1 = 0;
            bitsTempValue1 |= ((UInt64)(Sum4)) << 0;
            bitsTempValue1 |= ((UInt64)(Sum5)) << 11;
            bitsTempValue1 |= ((UInt64)(Sum6)) << 30;
            sm.Buf.AddRange(SerializationManager.ToBytes(bitsTempValue1, Type_t.TYPE_UINT64));
            sm.Index += sizeof(UInt64);
            sm.Buf.AddRange(SerializationManager.ToBytes(Sum7, Type_t.TYPE_UINT32));
            sm.Index += sizeof(UInt32);
        }

        public void Deserialize(SerializationManager sm)
        {
            //a :NoSerialization
            b = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            AddressType = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            switch (AddressType)
            {
                case Student_u1_FieldNumber:
                    u1 = (UInt8)sm.DeserializeField(Type_t.TYPE_UINT8);
                    break;
#if MyMacro == true
                case Student_u2_FieldNumber:
                    u2 = (UInt64)sm.DeserializeField(Type_t.TYPE_UINT64);
                    break;
#endif // #if MyMacro==true
                case Student_u3_FieldNumber:
                    u3 = (Sex)sm.DeserializeField(Type_t.TYPE_UINT64);
                    break;
                case Student_u4_FieldNumber:
                    u4 = new AddResult();
                    u4.Deserialize(sm);
                    break;
            }
            UInt32 bitsTempValue0 = 0;
            bitsTempValue0 = (UInt32)sm.DeserializeField(Type_t.TYPE_UINT32);
            Sum = bitsTempValue0 >> 0;
            Sum2 = bitsTempValue0 >> 11;
            Sum3 = bitsTempValue0 >> 30;
            uend1 = (UInt8)sm.DeserializeField(Type_t.TYPE_UINT8);
            uend2 = (UInt8)sm.DeserializeField(Type_t.TYPE_UINT8);
            UInt64 bitsTempValue1 = 0;
            bitsTempValue1 = (UInt64)sm.DeserializeField(Type_t.TYPE_UINT64);
            Sum4 = bitsTempValue1 >> 0;
            Sum5 = bitsTempValue1 >> 11;
            Sum6 = bitsTempValue1 >> 30;
            Sum7 = (UInt32)sm.DeserializeField(Type_t.TYPE_UINT32);
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
            sm.Buf.AddRange(SerializationManager.ToBytes(Year, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(Month, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(Day, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(Hour, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(Min, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(Sec, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(Sex, Type_t.TYPE_UINT64));
            sm.Index += sizeof(UInt64);
            David.Serialize(sm);
        }

        public void Deserialize(SerializationManager sm)
        {
            Year = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            Month = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            Day = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            Hour = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            Min = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            Sec = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            Sex = (Sex)sm.DeserializeField(Type_t.TYPE_UINT64);
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
            for (Byte now_index = 0; now_index < 1; now_index++)
            {
                now[now_index].Serialize(sm);
            }
        }

        public void Deserialize(SerializationManager sm)
        {
            now = new DateTime_t[1];
            for (Byte now_index = 0; now_index < 1; now_index++)
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
            sm.Buf.AddRange(SerializationManager.ToBytes(State, Type_t.TYPE_UINT8));
            sm.Index += sizeof(UInt8);
        }

        public void Deserialize(SerializationManager sm)
        {
            State = (RequestResponseState)sm.DeserializeField(Type_t.TYPE_UINT8);
        }

    }
#if MyMacro == true
    [RequestServiceInfo(Name = "DateTimeChange", ServiceId = 16)]
    public class DateTimeChange_Requester<DTL> : IRequestService<DTL> where DTL : struct
    {
        private EmbedXrpcObject<DTL> XrpcObject = null;
        public DateTimeChange_Requester(EmbedXrpcObject<DTL> xrpcObject)
        {
            XrpcObject = xrpcObject;
        }
        public static readonly UInt16 DateTimeChange_ServiceId = 16;//0x10
        public UInt16 GetSid() { return DateTimeChange_ServiceId; }
        public DateTimeChange_Return Invoke(DTL userDataOfTransportLayer, DateTime_t[] now)
        {
            DateTimeChange_Return reqresp = new DateTimeChange_Return();
            lock (XrpcObject.ObjectMutex)
            {
                XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
                DateTimeChange_Parameter request = new DateTimeChange_Parameter();
                request.now = now;
                SerializationManager sm = new SerializationManager(Assembly.GetExecutingAssembly(), new List<byte>());
                request.Serialize(sm);
                List<byte> sendBytes = new List<byte>();
                sendBytes.Add((byte)(DateTimeChange_ServiceId & 0xff));
                sendBytes.Add((byte)(DateTimeChange_ServiceId >> 8 & 0xff));
                sendBytes.Add((byte)(XrpcObject.TimeOut & 0xff));
                sendBytes.Add((byte)(((XrpcObject.TimeOut >> 8 & 0xff) & 0x3F) | ((byte)ReceiveType.Request) << 6));
                sendBytes.AddRange(sm.Buf);
                if (XrpcObject.Send(userDataOfTransportLayer, sendBytes.Count, 0, sendBytes.ToArray()) == false)
                {
                    reqresp.State = RequestResponseState.RequestState_Failed;
                    goto exi;
                }
                else
                {
                    reqresp.State = RequestResponseState.RequestState_Ok;
                }
            }
        exi:
            return reqresp;
        }
    }
#endif // #if MyMacro==true
#if MyMacro == true
    [ResponseServiceInfo(Name = "DateTimeChange", ServiceId = 16)]
    public partial class DateTimeChange_Service<DTL> : IService<DTL> where DTL : struct
    {
        public static readonly UInt16 DateTimeChange_ServiceId = 16;//0x10
        public UInt16 GetSid() { return DateTimeChange_ServiceId; }
        public void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
        {
            DateTimeChange_Parameter request = new DateTimeChange_Parameter();
            request.Deserialize(recManager);
            DateTimeChange(ref serviceInvokeParameter, request.now);
        }
        //public void DateTimeChange(ref serviceInvokeParameter,DateTime_t[] now);
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
        public UInt8[] data { get; set; } = new UInt8[0];


        public void Serialize(SerializationManager sm)
        {
            sm.Buf.AddRange(SerializationManager.ToBytes(a, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(b, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            sm.Buf.AddRange(SerializationManager.ToBytes(dataLen, Type_t.TYPE_INT32));
            sm.Index += sizeof(Int32);
            for (Int32 data_index = 0; data_index < dataLen; data_index++)
            {
                sm.Buf.AddRange(SerializationManager.ToBytes(data[data_index], Type_t.TYPE_UINT8));
                sm.Index += sizeof(UInt8);
            }
        }

        public void Deserialize(SerializationManager sm)
        {
            a = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            b = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            dataLen = (Int32)sm.DeserializeField(Type_t.TYPE_INT32);
            data = new UInt8[dataLen];
            for (Int32 data_index = 0; data_index < dataLen; data_index++)
            {
                data[data_index] = (UInt8)sm.DeserializeField(Type_t.TYPE_UINT8);
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
        public AddResult ReturnValue { get; set; } = new AddResult();


        public void Serialize(SerializationManager sm)
        {
            sm.Buf.AddRange(SerializationManager.ToBytes(State, Type_t.TYPE_UINT8));
            sm.Index += sizeof(UInt8);
            ReturnValue.Serialize(sm);
        }

        public void Deserialize(SerializationManager sm)
        {
            State = (RequestResponseState)sm.DeserializeField(Type_t.TYPE_UINT8);
            ReturnValue = new AddResult();
            ReturnValue.Deserialize(sm);
        }

    }
    [RequestServiceInfo(Name = "Add", ServiceId = 17)]
    public class Add_Requester<DTL> : IRequestService<DTL> where DTL : struct
    {
        private EmbedXrpcObject<DTL> XrpcObject = null;
        public Add_Requester(EmbedXrpcObject<DTL> xrpcObject)
        {
            XrpcObject = xrpcObject;
        }
        public static readonly UInt16 Add_ServiceId = 17;//0x11
        public UInt16 GetSid() { return Add_ServiceId; }
        public Add_Return Invoke(DTL userDataOfTransportLayer, Int32 a, Int32 b, Int32 dataLen, Byte[] data)
        {
            Add_Return reqresp = new Add_Return();
            lock (XrpcObject.ObjectMutex)
            {
                XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
                Add_Parameter request = new Add_Parameter();
                request.a = a;
                request.b = b;
                request.dataLen = dataLen;
                request.data = data;
                SerializationManager sm = new SerializationManager(Assembly.GetExecutingAssembly(), new List<byte>());
                request.Serialize(sm);
                List<byte> sendBytes = new List<byte>();
                sendBytes.Add((byte)(Add_ServiceId & 0xff));
                sendBytes.Add((byte)(Add_ServiceId >> 8 & 0xff));
                sendBytes.Add((byte)(XrpcObject.TimeOut & 0xff));
                sendBytes.Add((byte)(((XrpcObject.TimeOut >> 8 & 0xff) & 0x3F) | ((byte)ReceiveType.Request) << 6));
                sendBytes.AddRange(sm.Buf);
                if (XrpcObject.Send(userDataOfTransportLayer, sendBytes.Count, 0, sendBytes.ToArray()) == false)
                {
                    reqresp.State = RequestResponseState.RequestState_Failed;
                    goto exi;
                }
                else
                {
                    reqresp.State = RequestResponseState.RequestState_Ok;
                }
                var waitstate = XrpcObject.Wait<Add_Return>(Add_ServiceId, out reqresp);
                if (reqresp == null)
                { reqresp = new Add_Return(); }
                reqresp.State = waitstate;
                goto exi;
            }
        exi:
            return reqresp;
        }
    }
    [ResponseServiceInfo(Name = "Add", ServiceId = 17)]
    public partial class Add_Service<DTL> : IService<DTL> where DTL : struct
    {
        public static readonly UInt16 Add_ServiceId = 17;//0x11
        public UInt16 GetSid() { return Add_ServiceId; }
        private Add_Return Response = new Add_Return();
        public void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
        {
            Add_Parameter request = new Add_Parameter();
            request.Deserialize(recManager);
            Add(ref serviceInvokeParameter, request.a, request.b, request.dataLen, request.data);
            Response.Serialize(sendManager);
        }
        //public void Add(ref serviceInvokeParameter,Int32 a,Int32 b,Int32 dataLen,Byte[] data);
    }


}
