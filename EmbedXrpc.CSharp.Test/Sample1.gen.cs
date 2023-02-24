using System;
using System.Reflection;
using System.Collections.Generic;
using EmbedXrpc;
// auto code gen ! DO NOT modify this file!
//C# Code Generater Version:3.2.0.0
namespace Sample1
{
using UInt8 = Byte;
using Int8 = SByte;
public enum Sex:UInt64
{
Man=2,
WoMan=9,
}
public class AddResult:IEmbedXrpcSerialization
{
public const int AddResult_boolTest_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public bool boolTest{get;set;}


public const int AddResult_Sum_FieldNumber=2;
[BitField(11)]
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public UInt32 Sum{get;set;}


public const int AddResult_Sum2_FieldNumber=3;
[BitField(19)]
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public UInt32 Sum2{get;set;}


public const int AddResult_Sum3_FieldNumber=4;
[BitField(2)]
[FieldNumber( 4) ] 
[ArrayLenFieldFlag( false ) ]
public UInt32 Sum3{get;set;}


public const int AddResult_dataLen_FieldNumber=5;
[FieldNumber( 5) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 dataLen{get;set;}


public const int AddResult_data_FieldNumber=6;
[NoSerialization]
[ArrayProperty(LenFieldName = "dataLen")]
[FieldNumber( 6) ] 
public byte[] data{get;set;}=new byte[0];


public const int AddResult_Sum4_FieldNumber=7;
[BitField(11)]
[FieldNumber( 7) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 Sum4{get;set;}


public const int AddResult_Sum5_FieldNumber=8;
[BitField(19)]
[FieldNumber( 8) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 Sum5{get;set;}


public const int AddResult_Sum6_FieldNumber=9;
[BitField(2)]
[FieldNumber( 9) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 Sum6{get;set;}


public const int AddResult_Sum7_FieldNumber=10;
[FieldNumber( 10) ] 
[ArrayLenFieldFlag( false ) ]
public UInt32 Sum7{get;set;}


public const int AddResult_PtrTest1_FieldNumber=11;
[NoSerialization]
[FieldNumber( 11) ] 
[ArrayLenFieldFlag( false ) ]
public IntPtr PtrTest1{get;set;}


public const int AddResult_data1_FieldNumber=12;
[ArrayProperty(LenFieldName = "")]
[FieldNumber( 12) ] 
public byte[] data1{get;set;}=new byte[1];


public void Serialize(SerializationManager sm)
{
sm.Buf.AddRange(sm.ToBytes(boolTest,typeof(System.Boolean)));
sm.Index+=sizeof(bool);
UInt32 bitsTempValue0=0;
bitsTempValue0 |= ((UInt32)(Sum))<< 0 ;
bitsTempValue0 |= ((UInt32)(Sum2))<< 11 ;
bitsTempValue0 |= ((UInt32)(Sum3))<< 30 ;
sm.Buf.AddRange(sm.ToBytes(bitsTempValue0,typeof(System.UInt32)));
sm.Index+=sizeof(UInt32);
sm.Buf.AddRange(sm.ToBytes(dataLen,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
//! //data:NoSerialization
UInt64 bitsTempValue1=0;
bitsTempValue1 |= ((UInt64)(Sum4))<< 0 ;
bitsTempValue1 |= ((UInt64)(Sum5))<< 11 ;
bitsTempValue1 |= ((UInt64)(Sum6))<< 30 ;
sm.Buf.AddRange(sm.ToBytes(bitsTempValue1,typeof(System.UInt64)));
sm.Index+=sizeof(UInt64);
sm.Buf.AddRange(sm.ToBytes(Sum7,typeof(System.UInt32)));
sm.Index+=sizeof(UInt32);
//! //PtrTest1:NoSerialization
for(UInt32 data1_index=0;data1_index<1;data1_index++)
{
sm.Buf.AddRange(sm.ToBytes(data1[data1_index],typeof(System.Byte)));
sm.Index+=sizeof(byte);
}
}

public void Deserialize(SerializationManager sm)
{
boolTest=(bool)sm.DeserializeField(typeof(System.Boolean));
UInt32 bitsTempValue0=0;
bitsTempValue0=(UInt32)sm.DeserializeField(typeof(System.UInt32));
Sum=bitsTempValue0>>0;
Sum2=bitsTempValue0>>11;
Sum3=bitsTempValue0>>30;
dataLen=(Int32)sm.DeserializeField(typeof(System.Int32));
//! //data:NoSerialization
UInt64 bitsTempValue1=0;
bitsTempValue1=(UInt64)sm.DeserializeField(typeof(System.UInt64));
Sum4=bitsTempValue1>>0;
Sum5=bitsTempValue1>>11;
Sum6=bitsTempValue1>>30;
Sum7=(UInt32)sm.DeserializeField(typeof(System.UInt32));
//! //PtrTest1:NoSerialization
data1=new byte[1];
for(UInt32 data1_index=0;data1_index<1;data1_index++)
{
data1[data1_index]=(byte)sm.DeserializeField(typeof(System.Byte));
}
}

}
public class Student:IEmbedXrpcSerialization
{
#if MyMacro==true
public const int Student_a_FieldNumber=1;
[NoSerialization]
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}
#endif // #if MyMacro==true


public const int Student_b_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 b{get;set;}


public const int Student_AddressType_FieldNumber=3;
[UnionTargetType]
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 AddressType{get;set;}


public const int Student_u1_FieldNumber=4;
[UnionField]
[FieldNumber( 4) ] 
[ArrayLenFieldFlag( false ) ]
public byte u1{get;set;}


#if MyMacro==true
public const int Student_u2_FieldNumber=5;
[UnionField]
[FieldNumber( 5) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 u2{get;set;}
#endif // #if MyMacro==true


public const int Student_u3_FieldNumber=6;
[UnionField]
[FieldNumber( 6) ] 
[ArrayLenFieldFlag( false ) ]
public Sex u3{get;set;}


public const int Student_u4_FieldNumber=7;
[UnionField]
[FieldNumber( 7) ] 
public AddResult u4{get;set;}=new AddResult();


public const int Student_Sum_FieldNumber=8;
[BitField(11)]
[FieldNumber( 8) ] 
[ArrayLenFieldFlag( false ) ]
public UInt32 Sum{get;set;}


public const int Student_Sum2_FieldNumber=9;
[BitField(19)]
[FieldNumber( 9) ] 
[ArrayLenFieldFlag( false ) ]
public UInt32 Sum2{get;set;}


public const int Student_Sum3_FieldNumber=10;
[BitField(2)]
[FieldNumber( 10) ] 
[ArrayLenFieldFlag( false ) ]
public UInt32 Sum3{get;set;}


public const int Student_uend1_FieldNumber=100;
[FieldNumber( 100) ] 
[ArrayLenFieldFlag( false ) ]
public byte uend1{get;set;}


public const int Student_uend2_FieldNumber=101;
[FieldNumber( 101) ] 
[ArrayLenFieldFlag( false ) ]
public byte uend2{get;set;}


public const int Student_Sum4_FieldNumber=11;
[BitField(11)]
[FieldNumber( 11) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 Sum4{get;set;}


public const int Student_Sum5_FieldNumber=12;
[BitField(19)]
[FieldNumber( 12) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 Sum5{get;set;}


public const int Student_Sum6_FieldNumber=13;
[BitField(2)]
[FieldNumber( 13) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 Sum6{get;set;}


public const int Student_Sum7_FieldNumber=14;
[FieldNumber( 14) ] 
[ArrayLenFieldFlag( false ) ]
public UInt32 Sum7{get;set;}


public void Serialize(SerializationManager sm)
{
//! //a:NoSerialization
sm.Buf.AddRange(sm.ToBytes(b,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(AddressType,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
switch(AddressType)
{
case Student_u1_FieldNumber:
sm.Buf.AddRange(sm.ToBytes(u1,typeof(System.Byte)));
sm.Index+=sizeof(byte);
break;
#if MyMacro==true
case Student_u2_FieldNumber:
sm.Buf.AddRange(sm.ToBytes(u2,typeof(System.UInt64)));
sm.Index+=sizeof(UInt64);
break;
#endif // #if MyMacro==true
case Student_u3_FieldNumber:
sm.Buf.AddRange(sm.ToBytes(u3,typeof(System.UInt64)));
sm.Index+=sizeof(UInt64);
break;
case Student_u4_FieldNumber:
u4.Serialize(sm);
break;
}
UInt32 bitsTempValue0=0;
bitsTempValue0 |= ((UInt32)(Sum))<< 0 ;
bitsTempValue0 |= ((UInt32)(Sum2))<< 11 ;
bitsTempValue0 |= ((UInt32)(Sum3))<< 30 ;
sm.Buf.AddRange(sm.ToBytes(bitsTempValue0,typeof(System.UInt32)));
sm.Index+=sizeof(UInt32);
sm.Buf.AddRange(sm.ToBytes(uend1,typeof(System.Byte)));
sm.Index+=sizeof(byte);
sm.Buf.AddRange(sm.ToBytes(uend2,typeof(System.Byte)));
sm.Index+=sizeof(byte);
UInt64 bitsTempValue1=0;
bitsTempValue1 |= ((UInt64)(Sum4))<< 0 ;
bitsTempValue1 |= ((UInt64)(Sum5))<< 11 ;
bitsTempValue1 |= ((UInt64)(Sum6))<< 30 ;
sm.Buf.AddRange(sm.ToBytes(bitsTempValue1,typeof(System.UInt64)));
sm.Index+=sizeof(UInt64);
sm.Buf.AddRange(sm.ToBytes(Sum7,typeof(System.UInt32)));
sm.Index+=sizeof(UInt32);
}

public void Deserialize(SerializationManager sm)
{
//! //a:NoSerialization
b=(Int32)sm.DeserializeField(typeof(System.Int32));
AddressType=(Int32)sm.DeserializeField(typeof(System.Int32));
switch(AddressType)
{
case Student_u1_FieldNumber:
u1=(byte)sm.DeserializeField(typeof(System.Byte));
break;
#if MyMacro==true
case Student_u2_FieldNumber:
u2=(UInt64)sm.DeserializeField(typeof(System.UInt64));
break;
#endif // #if MyMacro==true
case Student_u3_FieldNumber:
u3=(Sex)sm.DeserializeField(typeof(System.UInt64));
break;
case Student_u4_FieldNumber:
u4=new AddResult();
u4.Deserialize(sm);
break;
}
UInt32 bitsTempValue0=0;
bitsTempValue0=(UInt32)sm.DeserializeField(typeof(System.UInt32));
Sum=bitsTempValue0>>0;
Sum2=bitsTempValue0>>11;
Sum3=bitsTempValue0>>30;
uend1=(byte)sm.DeserializeField(typeof(System.Byte));
uend2=(byte)sm.DeserializeField(typeof(System.Byte));
UInt64 bitsTempValue1=0;
bitsTempValue1=(UInt64)sm.DeserializeField(typeof(System.UInt64));
Sum4=bitsTempValue1>>0;
Sum5=bitsTempValue1>>11;
Sum6=bitsTempValue1>>30;
Sum7=(UInt32)sm.DeserializeField(typeof(System.UInt32));
}

}
public class DateTime_t:IEmbedXrpcSerialization
{
public const int DateTime_t_Year_FieldNumber=10086;
[FieldNumber( 10086) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Year{get;set;}


public const int DateTime_t_Month_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Month{get;set;}


public const int DateTime_t_Day_FieldNumber=3;
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Day{get;set;}


public const int DateTime_t_Hour_FieldNumber=4;
[FieldNumber( 4) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Hour{get;set;}


public const int DateTime_t_Min_FieldNumber=5;
[FieldNumber( 5) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Min{get;set;}


public const int DateTime_t_Sec_FieldNumber=6;
[FieldNumber( 6) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Sec{get;set;}


public const int DateTime_t_Sex_FieldNumber=8;
[FieldNumber( 8) ] 
[ArrayLenFieldFlag( false ) ]
public Sex Sex{get;set;}


public const int DateTime_t_David_FieldNumber=7;
[FieldNumber( 7) ] 
public Student David{get;set;}=new Student();


public void Serialize(SerializationManager sm)
{
sm.Buf.AddRange(sm.ToBytes(Year,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(Month,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(Day,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(Hour,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(Min,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(Sec,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(Sex,typeof(System.UInt64)));
sm.Index+=sizeof(UInt64);
David.Serialize(sm);
}

public void Deserialize(SerializationManager sm)
{
Year=(Int32)sm.DeserializeField(typeof(System.Int32));
Month=(Int32)sm.DeserializeField(typeof(System.Int32));
Day=(Int32)sm.DeserializeField(typeof(System.Int32));
Hour=(Int32)sm.DeserializeField(typeof(System.Int32));
Min=(Int32)sm.DeserializeField(typeof(System.Int32));
Sec=(Int32)sm.DeserializeField(typeof(System.Int32));
Sex=(Sex)sm.DeserializeField(typeof(System.UInt64));
David=new Student();
David.Deserialize(sm);
}

}
public class DateTimeChange_Parameter:IEmbedXrpcSerialization
{
public const int DateTimeChange_Parameter_now_FieldNumber=1;
[ArrayProperty(LenFieldName = "")]
[FieldNumber( 1) ] 
public DateTime_t[] now{get;set;}=new DateTime_t[1];


public void Serialize(SerializationManager sm)
{
for(UInt32 now_index=0;now_index<1;now_index++)
{
now[now_index].Serialize(sm);
}
}

public void Deserialize(SerializationManager sm)
{
now=new DateTime_t[1];
for(UInt32 now_index=0;now_index<1;now_index++)
{
now[now_index]=new DateTime_t();
now[now_index].Deserialize(sm);
}
}

}
public class DateTimeChange_Return:IEmbedXrpcSerialization
{
public const int DateTimeChange_Return_State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


public void Serialize(SerializationManager sm)
{
sm.Buf.AddRange(sm.ToBytes(State,typeof(System.Byte)));
sm.Index+=sizeof(byte);
}

public void Deserialize(SerializationManager sm)
{
State=(RequestResponseState)sm.DeserializeField(typeof(System.Byte));
}

}
#if MyMacro==true
public class DateTimeChange_Requester<DTL>:IRequestService<DTL> where DTL:struct
{
private EmbedXrpcObject<DTL> XrpcObject=null;
public DateTimeChange_Requester(EmbedXrpcObject<DTL> xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 DateTimeChange_ServiceId=16;//0x10
public UInt16 GetSid(){ return DateTimeChange_ServiceId;}
public DateTimeChange_Return Invoke(DTL userDataOfTransportLayer,DateTime_t[] now)
{
DateTimeChange_Return reqresp=new DateTimeChange_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
DateTimeChange_Parameter request =new DateTimeChange_Parameter();
request.now=now;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());
request.Serialize(sm);
List<byte> sendBytes = new List<byte>();
sendBytes.Add((byte)(DateTimeChange_ServiceId&0xff));
sendBytes.Add((byte)(DateTimeChange_ServiceId>>8&0xff));
sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));
sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));

sendBytes.Add((byte)(sm.Index&0xff));
sendBytes.Add((byte)(sm.Index>>8&0xff));
sendBytes.Add((byte)(sm.Index>>16&0xff));
sendBytes.Add((byte)(sm.Index>>24&0xff));

UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf.ToArray(),0,sm.Index);
sendBytes.Add((byte)(bufcrc&0xff));
sendBytes.Add((byte)(bufcrc>>8&0xff));
sendBytes.Add((byte)(bufcrc>>16&0xff));
sendBytes.Add((byte)(bufcrc>>24&0xff));
sendBytes.AddRange(sm.Buf);
if( XrpcObject.Send ( userDataOfTransportLayer,sendBytes.Count,0,sendBytes.ToArray() )==false)
 {
reqresp.State=RequestResponseState.RequestState_Failed;
 goto exi;
}
else
{
reqresp.State=RequestResponseState.RequestState_Ok;
}
}
exi:
return reqresp;
}
}
#endif // #if MyMacro==true
#if MyMacro==true
[ResponseServiceInfo(Name="DateTimeChange",ServiceId=16)]
public partial class DateTimeChange_Service<DTL>:IService<DTL> where DTL:struct
{
public static readonly UInt16 DateTimeChange_ServiceId=16;//0x10
public UInt16 GetSid(){ return DateTimeChange_ServiceId;}
public  void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
{
DateTimeChange_Parameter request = new DateTimeChange_Parameter();
request.Deserialize(recManager);
DateTimeChange(ref serviceInvokeParameter,request.now);
}
//public void DateTimeChange(ref serviceInvokeParameter,DateTime_t[] now);
}
#endif // #if MyMacro==true


public class Add_Parameter:IEmbedXrpcSerialization
{
public const int Add_Parameter_a_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}


public const int Add_Parameter_b_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 b{get;set;}


public const int Add_Parameter_dataLen_FieldNumber=3;
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 dataLen{get;set;}


public const int Add_Parameter_data_FieldNumber=4;
[ArrayProperty(LenFieldName = "dataLen")]
[FieldNumber( 4) ] 
public byte[] data{get;set;}=new byte[0];


public void Serialize(SerializationManager sm)
{
sm.Buf.AddRange(sm.ToBytes(a,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(b,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
sm.Buf.AddRange(sm.ToBytes(dataLen,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
for(Int32 data_index=0;data_index<dataLen;data_index++)
{
sm.Buf.AddRange(sm.ToBytes(data[data_index],typeof(System.Byte)));
sm.Index+=sizeof(byte);
}
}

public void Deserialize(SerializationManager sm)
{
a=(Int32)sm.DeserializeField(typeof(System.Int32));
b=(Int32)sm.DeserializeField(typeof(System.Int32));
dataLen=(Int32)sm.DeserializeField(typeof(System.Int32));
data=new byte[dataLen];
for(Int32 data_index=0;data_index<dataLen;data_index++)
{
data[data_index]=(byte)sm.DeserializeField(typeof(System.Byte));
}
}

}
public class Add_Return:IEmbedXrpcSerialization
{
public const int Add_Return_State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


public const int Add_Return_ReturnValue_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 ReturnValue{get;set;}


public void Serialize(SerializationManager sm)
{
sm.Buf.AddRange(sm.ToBytes(State,typeof(System.Byte)));
sm.Index+=sizeof(byte);
sm.Buf.AddRange(sm.ToBytes(ReturnValue,typeof(System.Int32)));
sm.Index+=sizeof(Int32);
}

public void Deserialize(SerializationManager sm)
{
State=(RequestResponseState)sm.DeserializeField(typeof(System.Byte));
ReturnValue=(Int32)sm.DeserializeField(typeof(System.Int32));
}

}
public class Add_Requester<DTL>:IRequestService<DTL> where DTL:struct
{
private EmbedXrpcObject<DTL> XrpcObject=null;
public Add_Requester(EmbedXrpcObject<DTL> xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 Add_ServiceId=17;//0x11
public UInt16 GetSid(){ return Add_ServiceId;}
public Add_Return Invoke(DTL userDataOfTransportLayer,Int32 a,Int32 b,Int32 dataLen,Byte[] data)
{
Add_Return reqresp=new Add_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
Add_Parameter request =new Add_Parameter();
request.a=a;
request.b=b;
request.dataLen=dataLen;
request.data=data;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());
request.Serialize(sm);
List<byte> sendBytes = new List<byte>();
sendBytes.Add((byte)(Add_ServiceId&0xff));
sendBytes.Add((byte)(Add_ServiceId>>8&0xff));
sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));
sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));

sendBytes.Add((byte)(sm.Index&0xff));
sendBytes.Add((byte)(sm.Index>>8&0xff));
sendBytes.Add((byte)(sm.Index>>16&0xff));
sendBytes.Add((byte)(sm.Index>>24&0xff));

UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf.ToArray(),0,sm.Index);
sendBytes.Add((byte)(bufcrc&0xff));
sendBytes.Add((byte)(bufcrc>>8&0xff));
sendBytes.Add((byte)(bufcrc>>16&0xff));
sendBytes.Add((byte)(bufcrc>>24&0xff));
sendBytes.AddRange(sm.Buf);
if( XrpcObject.Send ( userDataOfTransportLayer,sendBytes.Count,0,sendBytes.ToArray() )==false)
 {
reqresp.State=RequestResponseState.RequestState_Failed;
 goto exi;
}
else
{
reqresp.State=RequestResponseState.RequestState_Ok;
}
var waitstate=XrpcObject.Wait<Add_Return>(Add_ServiceId, ref reqresp);
if(reqresp==null)
{reqresp=new Add_Return();}
reqresp.State=waitstate;
goto exi;
}
exi:
return reqresp;
}
}
[ResponseServiceInfo(Name="Add",ServiceId=17)]
public partial class Add_Service<DTL>:IService<DTL> where DTL:struct
{
public static readonly UInt16 Add_ServiceId=17;//0x11
public UInt16 GetSid(){ return Add_ServiceId;}
private Add_Return Response = new Add_Return();
public  void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
{
Add_Parameter request = new Add_Parameter();
request.Deserialize(recManager);
Add(ref serviceInvokeParameter,request.a,request.b,request.dataLen,request.data);
Response.Serialize(sendManager);
}
//public void Add(ref serviceInvokeParameter,Int32 a,Int32 b,Int32 dataLen,Byte[] data);
}


}
