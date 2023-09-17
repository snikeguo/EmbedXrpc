using System;
using System.Reflection;
using System.Collections.Generic;
using EmbedXrpc;
// auto code gen ! DO NOT modify this file!
//C# Code Generater Version:4.2.1.0
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


public const int AddResult_PtrTest_FieldNumber=11;
[NoSerialization]
[FieldNumber( 11) ] 
[ArrayLenFieldFlag( false ) ]
public IntPtr PtrTest{get;set;}


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(boolTest,typeof(System.Boolean));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
UInt32 bitsTempValue0=0;
bitsTempValue0 |= ((UInt32)(Sum))<< 0 ;
bitsTempValue0 |= ((UInt32)(Sum2))<< 11 ;
bitsTempValue0 |= ((UInt32)(Sum3))<< 30 ;
if(sm.Buf!=null) bytes=sm.ToBytes(bitsTempValue0,typeof(System.UInt32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(dataLen,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
//data:NoSerialization
UInt64 bitsTempValue1=0;
bitsTempValue1 |= ((UInt64)(Sum4))<< 0 ;
bitsTempValue1 |= ((UInt64)(Sum5))<< 11 ;
bitsTempValue1 |= ((UInt64)(Sum6))<< 30 ;
if(sm.Buf!=null) bytes=sm.ToBytes(bitsTempValue1,typeof(System.UInt64));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=8;
if(sm.Buf!=null) bytes=sm.ToBytes(Sum7,typeof(System.UInt32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
//PtrTest:NoSerialization
}

public void Deserialize(SerializationManager sm)
{
boolTest=(bool)sm.DeserializeField(typeof(System.Boolean),1);
UInt32 bitsTempValue0=0;
bitsTempValue0=(UInt32)sm.DeserializeField(typeof(System.UInt32),4);
Sum=bitsTempValue0>>0;
Sum2=bitsTempValue0>>11;
Sum3=bitsTempValue0>>30;
dataLen=(Int32)sm.DeserializeField(typeof(System.Int32),4);
//data:NoSerialization
UInt64 bitsTempValue1=0;
bitsTempValue1=(UInt64)sm.DeserializeField(typeof(System.UInt64),8);
Sum4=bitsTempValue1>>0;
Sum5=bitsTempValue1>>11;
Sum6=bitsTempValue1>>30;
Sum7=(UInt32)sm.DeserializeField(typeof(System.UInt32),4);
//PtrTest:NoSerialization
}

}
public class Student:IEmbedXrpcSerialization
{
#if MyMacro==TRUE
public const int Student_a_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}
#endif // #if MyMacro==TRUE


public const int Student_b_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 b{get;set;}


public const int Student_AddressType_FieldNumber=3;
[UnionTargetType]
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 AddressType{get;set;}


#if MyMacro==TRUE
public const int Student_u1_FieldNumber=4;
[UnionField]
[FieldNumber( 4) ] 
[ArrayLenFieldFlag( false ) ]
public byte u1{get;set;}
#endif // #if MyMacro==TRUE


#if MyMacro==TRUE
public const int Student_u2_FieldNumber=5;
[UnionField]
[FieldNumber( 5) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 u2{get;set;}
#endif // #if MyMacro==TRUE


#if MyMacro==TRUE
public const int Student_u3_FieldNumber=6;
[UnionField]
[FieldNumber( 6) ] 
[ArrayLenFieldFlag( false ) ]
public Sex u3{get;set;}
#endif // #if MyMacro==TRUE


#if MyMacro==TRUE
public const int Student_u4_FieldNumber=7;
[UnionField]
[FieldNumber( 7) ] 
public AddResult u4{get;set;}=new AddResult();
#endif // #if MyMacro==TRUE


#if MyMacro==TRUE
public const int Student_uend1_FieldNumber=100;
[FieldNumber( 100) ] 
[ArrayLenFieldFlag( false ) ]
public byte uend1{get;set;}
#endif // #if MyMacro==TRUE


#if MyMacro==TRUE
public const int Student_uend2_FieldNumber=8;
[FieldNumber( 8) ] 
[ArrayLenFieldFlag( false ) ]
public byte uend2{get;set;}
#endif // #if MyMacro==TRUE


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
#if MyMacro==TRUE
if(sm.Buf!=null) bytes=sm.ToBytes(a,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
#endif // #if MyMacro==TRUE
if(sm.Buf!=null) bytes=sm.ToBytes(b,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(AddressType,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
switch(AddressType)
{
#if MyMacro==TRUE
case Student_u1_FieldNumber:
if(sm.Buf!=null) bytes=sm.ToBytes(u1,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u2_FieldNumber:
if(sm.Buf!=null) bytes=sm.ToBytes(u2,typeof(System.UInt64));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=8;
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u3_FieldNumber:
if(sm.Buf!=null) bytes=sm.ToBytes(u3,typeof(System.UInt64));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=8;
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u4_FieldNumber:
u4.Serialize(sm);
break;
#endif // #if MyMacro==TRUE
}
#if MyMacro==TRUE
if(sm.Buf!=null) bytes=sm.ToBytes(uend1,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
if(sm.Buf!=null) bytes=sm.ToBytes(uend2,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
#endif // #if MyMacro==TRUE
}

public void Deserialize(SerializationManager sm)
{
#if MyMacro==TRUE
a=(Int32)sm.DeserializeField(typeof(System.Int32),4);
#endif // #if MyMacro==TRUE
b=(Int32)sm.DeserializeField(typeof(System.Int32),4);
AddressType=(Int32)sm.DeserializeField(typeof(System.Int32),4);
switch(AddressType)
{
#if MyMacro==TRUE
case Student_u1_FieldNumber:
u1=(byte)sm.DeserializeField(typeof(System.Byte),1);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u2_FieldNumber:
u2=(UInt64)sm.DeserializeField(typeof(System.UInt64),8);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u3_FieldNumber:
u3=(Sex)sm.DeserializeField(typeof(System.UInt64),8);
break;
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
case Student_u4_FieldNumber:
u4=new AddResult();
u4.Deserialize(sm);
break;
#endif // #if MyMacro==TRUE
}
#if MyMacro==TRUE
uend1=(byte)sm.DeserializeField(typeof(System.Byte),1);
#endif // #if MyMacro==TRUE
#if MyMacro==TRUE
uend2=(byte)sm.DeserializeField(typeof(System.Byte),1);
#endif // #if MyMacro==TRUE
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


public const int DateTime_t_DateStringLen_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public byte DateStringLen{get;set;}


#if MyMacro==TRUE
public const int DateTime_t_DateString_FieldNumber=7;
[ArrayProperty(LenFieldName = "DateStringLen")]
[FieldNumber( 7) ] 
public byte[] DateString{get;set;}=new byte[0];
#endif // #if MyMacro==TRUE


public const int DateTime_t_David_FieldNumber=9;
[FieldNumber( 9) ] 
public Student David{get;set;}=new Student();


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(Year,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(Month,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(Day,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(Hour,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(Min,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(Sec,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(Sex,typeof(System.UInt64));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=8;
if(sm.Buf!=null) bytes=sm.ToBytes(DateStringLen,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
#if MyMacro==TRUE
for(byte DateString_index=0;DateString_index<DateStringLen;DateString_index++)
{
if(sm.Buf!=null) bytes=sm.ToBytes(DateString[DateString_index],typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
}
#endif // #if MyMacro==TRUE
David.Serialize(sm);
}

public void Deserialize(SerializationManager sm)
{
Year=(Int32)sm.DeserializeField(typeof(System.Int32),4);
Month=(Int32)sm.DeserializeField(typeof(System.Int32),4);
Day=(Int32)sm.DeserializeField(typeof(System.Int32),4);
Hour=(Int32)sm.DeserializeField(typeof(System.Int32),4);
Min=(Int32)sm.DeserializeField(typeof(System.Int32),4);
Sec=(Int32)sm.DeserializeField(typeof(System.Int32),4);
Sex=(Sex)sm.DeserializeField(typeof(System.UInt64),8);
DateStringLen=(byte)sm.DeserializeField(typeof(System.Byte),1);
#if MyMacro==TRUE
DateString=new byte[DateStringLen];
for(byte DateString_index=0;DateString_index<DateStringLen;DateString_index++)
{
DateString[DateString_index]=(byte)sm.DeserializeField(typeof(System.Byte),1);
}
#endif // #if MyMacro==TRUE
David=new Student();
David.Deserialize(sm);
}

}
public class TestSerialize:IEmbedXrpcSerialization
{
public const int TestSerialize_EnumArrayLen_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 EnumArrayLen{get;set;}


public const int TestSerialize_EnumArray_FieldNumber=2;
[ArrayProperty(LenFieldName = "EnumArrayLen")]
[FieldNumber( 2) ] 
public Sex[] EnumArray{get;set;}=new Sex[0];


public const int TestSerialize_ObjectArrayLen_FieldNumber=3;
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 ObjectArrayLen{get;set;}


public const int TestSerialize_DateTimeArray_FieldNumber=4;
[ArrayProperty(LenFieldName = "EnumArrayLen")]
[FieldNumber( 4) ] 
public DateTime_t[] DateTimeArray{get;set;}=new DateTime_t[0];


public const int TestSerialize_FiexDateTimeArray_FieldNumber=5;
[ArrayProperty(LenFieldName = "EnumArrayLen")]
[FieldNumber( 5) ] 
public DateTime_t[] FiexDateTimeArray{get;set;}=new DateTime_t[0];


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(EnumArrayLen,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
for(Int32 EnumArray_index=0;EnumArray_index<EnumArrayLen;EnumArray_index++)
{
if(sm.Buf!=null) bytes=sm.ToBytes(EnumArray[EnumArray_index],typeof(Sex));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=8;
}
if(sm.Buf!=null) bytes=sm.ToBytes(ObjectArrayLen,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
for(Int32 DateTimeArray_index=0;DateTimeArray_index<EnumArrayLen;DateTimeArray_index++)
{
DateTimeArray[DateTimeArray_index].Serialize(sm);
}
for(Int32 FiexDateTimeArray_index=0;FiexDateTimeArray_index<EnumArrayLen;FiexDateTimeArray_index++)
{
FiexDateTimeArray[FiexDateTimeArray_index].Serialize(sm);
}
}

public void Deserialize(SerializationManager sm)
{
EnumArrayLen=(Int32)sm.DeserializeField(typeof(System.Int32),4);
EnumArray=new Sex[EnumArrayLen];
for(Int32 EnumArray_index=0;EnumArray_index<EnumArrayLen;EnumArray_index++)
{
EnumArray[EnumArray_index]=(UInt64)sm.DeserializeField(typeof(System.UInt64),8);
}
ObjectArrayLen=(Int32)sm.DeserializeField(typeof(System.Int32),4);
DateTimeArray=new DateTime_t[EnumArrayLen];
for(Int32 DateTimeArray_index=0;DateTimeArray_index<EnumArrayLen;DateTimeArray_index++)
{
DateTimeArray[DateTimeArray_index]=new DateTime_t();
DateTimeArray[DateTimeArray_index].Deserialize(sm);
}
FiexDateTimeArray=new DateTime_t[EnumArrayLen];
for(Int32 FiexDateTimeArray_index=0;FiexDateTimeArray_index<EnumArrayLen;FiexDateTimeArray_index++)
{
FiexDateTimeArray[FiexDateTimeArray_index]=new DateTime_t();
FiexDateTimeArray[FiexDateTimeArray_index].Deserialize(sm);
}
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
byte[] bytes=null;
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
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(State,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
}

public void Deserialize(SerializationManager sm)
{
State=(RequestResponseState)sm.DeserializeField(typeof(System.Byte),1);
}

}
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
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.RequestBuffer,12);
request.Serialize(sm);
XrpcObject.RequestBuffer[0]=((byte)(DateTimeChange_ServiceId&0xff));
XrpcObject.RequestBuffer[1]=((byte)(DateTimeChange_ServiceId>>8&0xff));
XrpcObject.RequestBuffer[2]=((byte)(XrpcObject.TimeOut&0xff));
XrpcObject.RequestBuffer[3]=((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));

var requestObject_dataLen=sm.Index-12;
XrpcObject.RequestBuffer[4]=((byte)(requestObject_dataLen&0xff));
XrpcObject.RequestBuffer[5]=((byte)(requestObject_dataLen>>8&0xff));
XrpcObject.RequestBuffer[6]=((byte)(requestObject_dataLen>>16&0xff));
XrpcObject.RequestBuffer[7]=((byte)(requestObject_dataLen>>24&0xff));

UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf,12,requestObject_dataLen);
XrpcObject.RequestBuffer[8]=((byte)(bufcrc&0xff));
XrpcObject.RequestBuffer[9]=((byte)(bufcrc>>8&0xff));
XrpcObject.RequestBuffer[10]=((byte)(bufcrc>>16&0xff));
XrpcObject.RequestBuffer[11]=((byte)(bufcrc>>24&0xff));
if( XrpcObject.Send ( userDataOfTransportLayer,sm.Index,0,XrpcObject.RequestBuffer )==false)
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


public class Test2_Parameter:IEmbedXrpcSerialization
{
public const int Test2_Parameter_now_FieldNumber=1;
[ArrayProperty(LenFieldName = "")]
[FieldNumber( 1) ] 
public DateTime_t[] now{get;set;}=new DateTime_t[1];


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
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
public class Test2_Return:IEmbedXrpcSerialization
{
public const int Test2_Return_State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(State,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
}

public void Deserialize(SerializationManager sm)
{
State=(RequestResponseState)sm.DeserializeField(typeof(System.Byte),1);
}

}
public class Test2_Requester<DTL>:IRequestService<DTL> where DTL:struct
{
private EmbedXrpcObject<DTL> XrpcObject=null;
public Test2_Requester(EmbedXrpcObject<DTL> xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 Test2_ServiceId=17;//0x11
public UInt16 GetSid(){ return Test2_ServiceId;}
public Test2_Return Invoke(DTL userDataOfTransportLayer,DateTime_t[] now)
{
Test2_Return reqresp=new Test2_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
Test2_Parameter request =new Test2_Parameter();
request.now=now;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.RequestBuffer,12);
request.Serialize(sm);
XrpcObject.RequestBuffer[0]=((byte)(Test2_ServiceId&0xff));
XrpcObject.RequestBuffer[1]=((byte)(Test2_ServiceId>>8&0xff));
XrpcObject.RequestBuffer[2]=((byte)(XrpcObject.TimeOut&0xff));
XrpcObject.RequestBuffer[3]=((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));

var requestObject_dataLen=sm.Index-12;
XrpcObject.RequestBuffer[4]=((byte)(requestObject_dataLen&0xff));
XrpcObject.RequestBuffer[5]=((byte)(requestObject_dataLen>>8&0xff));
XrpcObject.RequestBuffer[6]=((byte)(requestObject_dataLen>>16&0xff));
XrpcObject.RequestBuffer[7]=((byte)(requestObject_dataLen>>24&0xff));

UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf,12,requestObject_dataLen);
XrpcObject.RequestBuffer[8]=((byte)(bufcrc&0xff));
XrpcObject.RequestBuffer[9]=((byte)(bufcrc>>8&0xff));
XrpcObject.RequestBuffer[10]=((byte)(bufcrc>>16&0xff));
XrpcObject.RequestBuffer[11]=((byte)(bufcrc>>24&0xff));
if( XrpcObject.Send ( userDataOfTransportLayer,sm.Index,0,XrpcObject.RequestBuffer )==false)
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
[ResponseServiceInfo(Name="Test2",ServiceId=17)]
public partial class Test2_Service<DTL>:IService<DTL> where DTL:struct
{
public static readonly UInt16 Test2_ServiceId=17;//0x11
public UInt16 GetSid(){ return Test2_ServiceId;}
public  void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
{
Test2_Parameter request = new Test2_Parameter();
request.Deserialize(recManager);
Test2(ref serviceInvokeParameter,request.now);
}
//public void Test2(ref serviceInvokeParameter,DateTime_t[] now);
}


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


public const int Add_Parameter_test_FieldNumber=5;
[ArrayProperty(LenFieldName = "")]
[FieldNumber( 5) ] 
public TestSerialize[] test{get;set;}=new TestSerialize[1];


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(a,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(b,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
if(sm.Buf!=null) bytes=sm.ToBytes(dataLen,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
for(Int32 data_index=0;data_index<dataLen;data_index++)
{
if(sm.Buf!=null) bytes=sm.ToBytes(data[data_index],typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
}
for(UInt32 test_index=0;test_index<1;test_index++)
{
test[test_index].Serialize(sm);
}
}

public void Deserialize(SerializationManager sm)
{
a=(Int32)sm.DeserializeField(typeof(System.Int32),4);
b=(Int32)sm.DeserializeField(typeof(System.Int32),4);
dataLen=(Int32)sm.DeserializeField(typeof(System.Int32),4);
data=new byte[dataLen];
for(Int32 data_index=0;data_index<dataLen;data_index++)
{
data[data_index]=(byte)sm.DeserializeField(typeof(System.Byte),1);
}
test=new TestSerialize[1];
for(UInt32 test_index=0;test_index<1;test_index++)
{
test[test_index]=new TestSerialize();
test[test_index].Deserialize(sm);
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
public AddResult ReturnValue{get;set;}=new AddResult();


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(State,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
ReturnValue.Serialize(sm);
}

public void Deserialize(SerializationManager sm)
{
State=(RequestResponseState)sm.DeserializeField(typeof(System.Byte),1);
ReturnValue=new AddResult();
ReturnValue.Deserialize(sm);
}

}
public class Add_Requester<DTL>:IRequestService<DTL> where DTL:struct
{
private EmbedXrpcObject<DTL> XrpcObject=null;
public Add_Requester(EmbedXrpcObject<DTL> xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 Add_ServiceId=18;//0x12
public UInt16 GetSid(){ return Add_ServiceId;}
public Add_Return Invoke(DTL userDataOfTransportLayer,Int32 a,Int32 b,Int32 dataLen,Byte[] data,TestSerialize[] test)
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
request.test=test;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.RequestBuffer,12);
request.Serialize(sm);
XrpcObject.RequestBuffer[0]=((byte)(Add_ServiceId&0xff));
XrpcObject.RequestBuffer[1]=((byte)(Add_ServiceId>>8&0xff));
XrpcObject.RequestBuffer[2]=((byte)(XrpcObject.TimeOut&0xff));
XrpcObject.RequestBuffer[3]=((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));

var requestObject_dataLen=sm.Index-12;
XrpcObject.RequestBuffer[4]=((byte)(requestObject_dataLen&0xff));
XrpcObject.RequestBuffer[5]=((byte)(requestObject_dataLen>>8&0xff));
XrpcObject.RequestBuffer[6]=((byte)(requestObject_dataLen>>16&0xff));
XrpcObject.RequestBuffer[7]=((byte)(requestObject_dataLen>>24&0xff));

UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf,12,requestObject_dataLen);
XrpcObject.RequestBuffer[8]=((byte)(bufcrc&0xff));
XrpcObject.RequestBuffer[9]=((byte)(bufcrc>>8&0xff));
XrpcObject.RequestBuffer[10]=((byte)(bufcrc>>16&0xff));
XrpcObject.RequestBuffer[11]=((byte)(bufcrc>>24&0xff));
if( XrpcObject.Send ( userDataOfTransportLayer,sm.Index,0,XrpcObject.RequestBuffer )==false)
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
[ResponseServiceInfo(Name="Add",ServiceId=18)]
public partial class Add_Service<DTL>:IService<DTL> where DTL:struct
{
public static readonly UInt16 Add_ServiceId=18;//0x12
public UInt16 GetSid(){ return Add_ServiceId;}
private Add_Return Response = new Add_Return();
public  void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
{
Add_Parameter request = new Add_Parameter();
request.Deserialize(recManager);
Add(ref serviceInvokeParameter,request.a,request.b,request.dataLen,request.data,request.test);
Response.Serialize(sendManager);
}
//public void Add(ref serviceInvokeParameter,Int32 a,Int32 b,Int32 dataLen,Byte[] data,TestSerialize[] test);
}


public class NoArg_Parameter:IEmbedXrpcSerialization
{
public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
}

public void Deserialize(SerializationManager sm)
{
}

}
public class NoArg_Return:IEmbedXrpcSerialization
{
public const int NoArg_Return_State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


public const int NoArg_Return_ReturnValue_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public bool ReturnValue{get;set;}


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(State,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
if(sm.Buf!=null) bytes=sm.ToBytes(ReturnValue,typeof(System.Boolean));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
}

public void Deserialize(SerializationManager sm)
{
State=(RequestResponseState)sm.DeserializeField(typeof(System.Byte),1);
ReturnValue=(bool)sm.DeserializeField(typeof(System.Boolean),1);
}

}
public class NoArg_Requester<DTL>:IRequestService<DTL> where DTL:struct
{
private EmbedXrpcObject<DTL> XrpcObject=null;
public NoArg_Requester(EmbedXrpcObject<DTL> xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoArg_ServiceId=19;//0x13
public UInt16 GetSid(){ return NoArg_ServiceId;}
public NoArg_Return Invoke(DTL userDataOfTransportLayer)
{
NoArg_Return reqresp=new NoArg_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
NoArg_Parameter request =new NoArg_Parameter();
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.RequestBuffer,12);
request.Serialize(sm);
XrpcObject.RequestBuffer[0]=((byte)(NoArg_ServiceId&0xff));
XrpcObject.RequestBuffer[1]=((byte)(NoArg_ServiceId>>8&0xff));
XrpcObject.RequestBuffer[2]=((byte)(XrpcObject.TimeOut&0xff));
XrpcObject.RequestBuffer[3]=((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));

var requestObject_dataLen=sm.Index-12;
XrpcObject.RequestBuffer[4]=((byte)(requestObject_dataLen&0xff));
XrpcObject.RequestBuffer[5]=((byte)(requestObject_dataLen>>8&0xff));
XrpcObject.RequestBuffer[6]=((byte)(requestObject_dataLen>>16&0xff));
XrpcObject.RequestBuffer[7]=((byte)(requestObject_dataLen>>24&0xff));

UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf,12,requestObject_dataLen);
XrpcObject.RequestBuffer[8]=((byte)(bufcrc&0xff));
XrpcObject.RequestBuffer[9]=((byte)(bufcrc>>8&0xff));
XrpcObject.RequestBuffer[10]=((byte)(bufcrc>>16&0xff));
XrpcObject.RequestBuffer[11]=((byte)(bufcrc>>24&0xff));
if( XrpcObject.Send ( userDataOfTransportLayer,sm.Index,0,XrpcObject.RequestBuffer )==false)
 {
reqresp.State=RequestResponseState.RequestState_Failed;
 goto exi;
}
else
{
reqresp.State=RequestResponseState.RequestState_Ok;
}
var waitstate=XrpcObject.Wait<NoArg_Return>(NoArg_ServiceId, ref reqresp);
if(reqresp==null)
{reqresp=new NoArg_Return();}
reqresp.State=waitstate;
goto exi;
}
exi:
return reqresp;
}
}
[ResponseServiceInfo(Name="NoArg",ServiceId=19)]
public partial class NoArg_Service<DTL>:IService<DTL> where DTL:struct
{
public static readonly UInt16 NoArg_ServiceId=19;//0x13
public UInt16 GetSid(){ return NoArg_ServiceId;}
private NoArg_Return Response = new NoArg_Return();
public  void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
{
NoArg_Parameter request = new NoArg_Parameter();
request.Deserialize(recManager);
NoArg(ref serviceInvokeParameter);
Response.Serialize(sendManager);
}
//public void NoArg(ref serviceInvokeParameter);
}


public class NoReturn_Parameter:IEmbedXrpcSerialization
{
public const int NoReturn_Parameter_a_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(a,typeof(System.Int32));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=4;
}

public void Deserialize(SerializationManager sm)
{
a=(Int32)sm.DeserializeField(typeof(System.Int32),4);
}

}
public class NoReturn_Return:IEmbedXrpcSerialization
{
public const int NoReturn_Return_State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(State,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
}

public void Deserialize(SerializationManager sm)
{
State=(RequestResponseState)sm.DeserializeField(typeof(System.Byte),1);
}

}
public class NoReturn_Requester<DTL>:IRequestService<DTL> where DTL:struct
{
private EmbedXrpcObject<DTL> XrpcObject=null;
public NoReturn_Requester(EmbedXrpcObject<DTL> xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoReturn_ServiceId=20;//0x14
public UInt16 GetSid(){ return NoReturn_ServiceId;}
public NoReturn_Return Invoke(DTL userDataOfTransportLayer,Int32 a)
{
NoReturn_Return reqresp=new NoReturn_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
NoReturn_Parameter request =new NoReturn_Parameter();
request.a=a;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.RequestBuffer,12);
request.Serialize(sm);
XrpcObject.RequestBuffer[0]=((byte)(NoReturn_ServiceId&0xff));
XrpcObject.RequestBuffer[1]=((byte)(NoReturn_ServiceId>>8&0xff));
XrpcObject.RequestBuffer[2]=((byte)(XrpcObject.TimeOut&0xff));
XrpcObject.RequestBuffer[3]=((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));

var requestObject_dataLen=sm.Index-12;
XrpcObject.RequestBuffer[4]=((byte)(requestObject_dataLen&0xff));
XrpcObject.RequestBuffer[5]=((byte)(requestObject_dataLen>>8&0xff));
XrpcObject.RequestBuffer[6]=((byte)(requestObject_dataLen>>16&0xff));
XrpcObject.RequestBuffer[7]=((byte)(requestObject_dataLen>>24&0xff));

UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf,12,requestObject_dataLen);
XrpcObject.RequestBuffer[8]=((byte)(bufcrc&0xff));
XrpcObject.RequestBuffer[9]=((byte)(bufcrc>>8&0xff));
XrpcObject.RequestBuffer[10]=((byte)(bufcrc>>16&0xff));
XrpcObject.RequestBuffer[11]=((byte)(bufcrc>>24&0xff));
if( XrpcObject.Send ( userDataOfTransportLayer,sm.Index,0,XrpcObject.RequestBuffer )==false)
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
[ResponseServiceInfo(Name="NoReturn",ServiceId=20)]
public partial class NoReturn_Service<DTL>:IService<DTL> where DTL:struct
{
public static readonly UInt16 NoReturn_ServiceId=20;//0x14
public UInt16 GetSid(){ return NoReturn_ServiceId;}
public  void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
{
NoReturn_Parameter request = new NoReturn_Parameter();
request.Deserialize(recManager);
NoReturn(ref serviceInvokeParameter,request.a);
}
//public void NoReturn(ref serviceInvokeParameter,Int32 a);
}


public class NoArgAndReturn_Parameter:IEmbedXrpcSerialization
{
public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
}

public void Deserialize(SerializationManager sm)
{
}

}
public class NoArgAndReturn_Return:IEmbedXrpcSerialization
{
public const int NoArgAndReturn_Return_State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


public void Serialize(SerializationManager sm)
{
byte[] bytes=null;
if(sm.Buf!=null) bytes=sm.ToBytes(State,typeof(System.Byte));
if(sm.Buf!=null) Array.Copy(bytes,0,sm.Buf,sm.Index,bytes.Length);
sm.Index+=1;
}

public void Deserialize(SerializationManager sm)
{
State=(RequestResponseState)sm.DeserializeField(typeof(System.Byte),1);
}

}
public class NoArgAndReturn_Requester<DTL>:IRequestService<DTL> where DTL:struct
{
private EmbedXrpcObject<DTL> XrpcObject=null;
public NoArgAndReturn_Requester(EmbedXrpcObject<DTL> xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoArgAndReturn_ServiceId=21;//0x15
public UInt16 GetSid(){ return NoArgAndReturn_ServiceId;}
public NoArgAndReturn_Return Invoke(DTL userDataOfTransportLayer)
{
NoArgAndReturn_Return reqresp=new NoArgAndReturn_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.MessageQueueOfRequestServiceHandle.Reset();
NoArgAndReturn_Parameter request =new NoArgAndReturn_Parameter();
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.RequestBuffer,12);
request.Serialize(sm);
XrpcObject.RequestBuffer[0]=((byte)(NoArgAndReturn_ServiceId&0xff));
XrpcObject.RequestBuffer[1]=((byte)(NoArgAndReturn_ServiceId>>8&0xff));
XrpcObject.RequestBuffer[2]=((byte)(XrpcObject.TimeOut&0xff));
XrpcObject.RequestBuffer[3]=((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));

var requestObject_dataLen=sm.Index-12;
XrpcObject.RequestBuffer[4]=((byte)(requestObject_dataLen&0xff));
XrpcObject.RequestBuffer[5]=((byte)(requestObject_dataLen>>8&0xff));
XrpcObject.RequestBuffer[6]=((byte)(requestObject_dataLen>>16&0xff));
XrpcObject.RequestBuffer[7]=((byte)(requestObject_dataLen>>24&0xff));

UInt32 bufcrc=XrpcObject.GetBufferCrc(sm.Buf,12,requestObject_dataLen);
XrpcObject.RequestBuffer[8]=((byte)(bufcrc&0xff));
XrpcObject.RequestBuffer[9]=((byte)(bufcrc>>8&0xff));
XrpcObject.RequestBuffer[10]=((byte)(bufcrc>>16&0xff));
XrpcObject.RequestBuffer[11]=((byte)(bufcrc>>24&0xff));
if( XrpcObject.Send ( userDataOfTransportLayer,sm.Index,0,XrpcObject.RequestBuffer )==false)
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
[ResponseServiceInfo(Name="NoArgAndReturn",ServiceId=21)]
public partial class NoArgAndReturn_Service<DTL>:IService<DTL> where DTL:struct
{
public static readonly UInt16 NoArgAndReturn_ServiceId=21;//0x15
public UInt16 GetSid(){ return NoArgAndReturn_ServiceId;}
public  void Invoke(ref ServiceInvokeParameter<DTL> serviceInvokeParameter, SerializationManager recManager, SerializationManager sendManager)
{
NoArgAndReturn_Parameter request = new NoArgAndReturn_Parameter();
request.Deserialize(recManager);
NoArgAndReturn(ref serviceInvokeParameter);
}
//public void NoArgAndReturn(ref serviceInvokeParameter);
}


}
