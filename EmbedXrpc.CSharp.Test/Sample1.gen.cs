using System;
using System.Reflection;
using System.Collections.Generic;
using EmbedXrpc;
// auto code gen ! DO NOT modify this file! create time 2021 - 04 - 29 10: 46:45.964;
//C# Code Generater Version:1.9.3.0
namespace Sample1
{
using UInt8 = Byte;
using Int8 = SByte;
public enum Sex:ulong
{
Man=2,
WoMan=9,
}
public class Student
{
public const int a_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}


public const int b_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 b{get;set;}


public const int AddressType_FieldNumber=3;
[UnionTargetType]
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 AddressType{get;set;}


public const int u1_FieldNumber=4;
[UnionField]
[FieldNumber( 4) ] 
[ArrayLenFieldFlag( false ) ]
public UInt8 u1{get;set;}


public const int u2_FieldNumber=5;
[UnionField]
[FieldNumber( 5) ] 
[ArrayLenFieldFlag( false ) ]
public UInt64 u2{get;set;}


public const int u3_FieldNumber=6;
[UnionField]
[FieldNumber( 6) ] 
[ArrayLenFieldFlag( false ) ]
public Sex u3{get;set;}


public const int u4_FieldNumber=7;
[UnionField]
[FieldNumber( 7) ] 
public AddResult u4{get;set;}=new AddResult();


public const int uend1_FieldNumber=100;
[FieldNumber( 100) ] 
[ArrayLenFieldFlag( false ) ]
public UInt8 uend1{get;set;}


public const int uend2_FieldNumber=101;
[FieldNumber( 101) ] 
[ArrayLenFieldFlag( false ) ]
public UInt8 uend2{get;set;}


}
public class DateTime_t
{
public const int Year_FieldNumber=10086;
[FieldNumber( 10086) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Year{get;set;}


public const int Month_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Month{get;set;}


public const int Day_FieldNumber=3;
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Day{get;set;}


public const int Hour_FieldNumber=4;
[FieldNumber( 4) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Hour{get;set;}


public const int Min_FieldNumber=5;
[FieldNumber( 5) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Min{get;set;}


public const int Sec_FieldNumber=6;
[FieldNumber( 6) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Sec{get;set;}


public const int Sex_FieldNumber=8;
[FieldNumber( 8) ] 
[ArrayLenFieldFlag( false ) ]
public Sex Sex{get;set;}


public const int David_FieldNumber=7;
[FieldNumber( 7) ] 
public Student David{get;set;}=new Student();


}
public class AddResult
{
public const int Sum_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Sum{get;set;}


public const int dataLen_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 dataLen{get;set;}


public const int data_FieldNumber=3;
[ArrayProperty(LenFieldName = "dataLen")]
[FieldNumber( 3) ] 
public UInt8[] data{get;set;}=new UInt8[0];


}
public class DateTimeChange_Parameter
{
public const int now_FieldNumber=1;
[ArrayProperty(LenFieldName = "")]
[FieldNumber( 1) ] 
public DateTime_t[] now{get;set;}=new DateTime_t[1];


}
[DelegateInfo(Name="DateTimeChange")]
public partial class DateTimeChangeClientImpl:IDelegate
{
public static readonly UInt16 DateTimeChange_ServiceId=16;//0x10
public UInt16 GetSid(){ return DateTimeChange_ServiceId;}
public void Invoke(SerializationManager recManager)
{
DateTimeChange_Parameter request = recManager.Deserialize <DateTimeChange_Parameter>();
DateTimeChange(request.now);//call function!
}
//public void DateTimeChange(DateTime_t[] now);
}
public class DateTimeChangeDelegate
{
private EmbedXrpcObject XrpcObject; 
public DateTimeChangeDelegate(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 DateTimeChange_ServiceId=16;//0x10
public void Invoke(DateTime_t[] now)
{
DateTimeChange_Parameter request =new DateTimeChange_Parameter();
request.now = now;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());
sm.Serialize(request,0);
List<byte> sendBytes = new List<byte>();
sendBytes.Add((byte)(DateTimeChange_ServiceId&0xff));
sendBytes.Add((byte)(DateTimeChange_ServiceId>>8&0xff));
sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));
sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Delegate)<<6));
sendBytes.AddRange(sm.Data);
XrpcObject.Send ( sendBytes.Count,0,sendBytes.ToArray() );
}
}
public class TestDelegate_Parameter
{
public const int now_FieldNumber=1;
[ArrayProperty(LenFieldName = "")]
[FieldNumber( 1) ] 
public DateTime_t[] now{get;set;}=new DateTime_t[1];


}
[DelegateInfo(Name="TestDelegate")]
public partial class TestDelegateClientImpl:IDelegate
{
public static readonly UInt16 TestDelegate_ServiceId=17;//0x11
public UInt16 GetSid(){ return TestDelegate_ServiceId;}
public void Invoke(SerializationManager recManager)
{
TestDelegate_Parameter request = recManager.Deserialize <TestDelegate_Parameter>();
TestDelegate(request.now);//call function!
}
//public void TestDelegate(DateTime_t[] now);
}
public class TestDelegateDelegate
{
private EmbedXrpcObject XrpcObject; 
public TestDelegateDelegate(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 TestDelegate_ServiceId=17;//0x11
public void Invoke(DateTime_t[] now)
{
TestDelegate_Parameter request =new TestDelegate_Parameter();
request.now = now;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());
sm.Serialize(request,0);
List<byte> sendBytes = new List<byte>();
sendBytes.Add((byte)(TestDelegate_ServiceId&0xff));
sendBytes.Add((byte)(TestDelegate_ServiceId>>8&0xff));
sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));
sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Delegate)<<6));
sendBytes.AddRange(sm.Data);
XrpcObject.Send ( sendBytes.Count,0,sendBytes.ToArray() );
}
}
public class Inter_Add_Parameter
{
public const int a_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}


public const int b_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 b{get;set;}


public const int dataLen_FieldNumber=3;
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 dataLen{get;set;}


public const int data_FieldNumber=4;
[ArrayProperty(LenFieldName = "dataLen")]
[FieldNumber( 4) ] 
public UInt8[] data{get;set;}=new UInt8[0];


}
public class Inter_Add_Return
{
public const int State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


public const int ReturnValue_FieldNumber=2;
[FieldNumber( 2) ] 
public AddResult ReturnValue{get;set;}=new AddResult();


}
[ResponseServiceInfo(Name="Inter_Add",ServiceId=18)]
public partial class Inter_AddService:IService
{
public static readonly UInt16 Add_ServiceId=18;//0x12
public UInt16 GetSid(){ return Add_ServiceId;}
private Inter_Add_Return Response = new Inter_Add_Return();
public  void Invoke(SerializationManager recManager, SerializationManager sendManager)
{
Inter_Add_Parameter request = recManager.Deserialize < Inter_Add_Parameter>();
Add(request.a,request.b,request.dataLen,request.data);
sendManager.Serialize(Response, 0);
}
//public void Add(Int32 a,Int32 b,Int32 dataLen,Byte[] data);
}
[RequestServiceInfo(Name="Inter_Add",ServiceId=18)]
public class Inter_Add:IRequestService
{
private EmbedXrpcObject XrpcObject=null;
public Inter_Add(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 Add_ServiceId=18;//0x12
public UInt16 GetSid(){ return Add_ServiceId;}
public Inter_Add_Return Invoke(Int32 a,Int32 b,Int32 dataLen,Byte[] data)
{
Inter_Add_Return reqresp=new Inter_Add_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.ResponseMessageQueueHandle.Reset();
Inter_Add_Parameter request =new Inter_Add_Parameter();
request.a=a;
request.b=b;
request.dataLen=dataLen;
request.data=data;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());
sm.Serialize(request,0);
List<byte> sendBytes = new List<byte>();
sendBytes.Add((byte)(Add_ServiceId&0xff));
sendBytes.Add((byte)(Add_ServiceId>>8&0xff));
sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));
sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));
sendBytes.AddRange(sm.Data);
if( XrpcObject.Send ( sendBytes.Count,0,sendBytes.ToArray() )==false)
 {
reqresp.State=RequestResponseState.RequestState_Failed;
 goto exi;
}
else
{
reqresp.State=RequestResponseState.RequestState_Ok;
}
var waitstate=XrpcObject.Wait<Inter_Add_Return>(Add_ServiceId, out reqresp);
if(reqresp==null)
{reqresp=new Inter_Add_Return();}
reqresp.State=waitstate;
goto exi;
}
exi:
return reqresp;
}
}
public class Inter_NoArg_Parameter
{
}
public class Inter_NoArg_Return
{
public const int State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


public const int ReturnValue_FieldNumber=2;
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public UInt8 ReturnValue{get;set;}


}
[ResponseServiceInfo(Name="Inter_NoArg",ServiceId=19)]
public partial class Inter_NoArgService:IService
{
public static readonly UInt16 NoArg_ServiceId=19;//0x13
public UInt16 GetSid(){ return NoArg_ServiceId;}
private Inter_NoArg_Return Response = new Inter_NoArg_Return();
public  void Invoke(SerializationManager recManager, SerializationManager sendManager)
{
Inter_NoArg_Parameter request = recManager.Deserialize < Inter_NoArg_Parameter>();
NoArg();
sendManager.Serialize(Response, 0);
}
//public void NoArg();
}
[RequestServiceInfo(Name="Inter_NoArg",ServiceId=19)]
public class Inter_NoArg:IRequestService
{
private EmbedXrpcObject XrpcObject=null;
public Inter_NoArg(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoArg_ServiceId=19;//0x13
public UInt16 GetSid(){ return NoArg_ServiceId;}
public Inter_NoArg_Return Invoke()
{
Inter_NoArg_Return reqresp=new Inter_NoArg_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.ResponseMessageQueueHandle.Reset();
Inter_NoArg_Parameter request =new Inter_NoArg_Parameter();
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());
sm.Serialize(request,0);
List<byte> sendBytes = new List<byte>();
sendBytes.Add((byte)(NoArg_ServiceId&0xff));
sendBytes.Add((byte)(NoArg_ServiceId>>8&0xff));
sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));
sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));
sendBytes.AddRange(sm.Data);
if( XrpcObject.Send ( sendBytes.Count,0,sendBytes.ToArray() )==false)
 {
reqresp.State=RequestResponseState.RequestState_Failed;
 goto exi;
}
else
{
reqresp.State=RequestResponseState.RequestState_Ok;
}
var waitstate=XrpcObject.Wait<Inter_NoArg_Return>(NoArg_ServiceId, out reqresp);
if(reqresp==null)
{reqresp=new Inter_NoArg_Return();}
reqresp.State=waitstate;
goto exi;
}
exi:
return reqresp;
}
}
public class Inter_NoReturn_Parameter
{
public const int a_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}


}
public class Inter_NoReturn_Return
{
public const int State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


}
[ResponseServiceInfo(Name="Inter_NoReturn",ServiceId=20)]
public partial class Inter_NoReturnService:IService
{
public static readonly UInt16 NoReturn_ServiceId=20;//0x14
public UInt16 GetSid(){ return NoReturn_ServiceId;}
public  void Invoke(SerializationManager recManager, SerializationManager sendManager)
{
Inter_NoReturn_Parameter request = recManager.Deserialize < Inter_NoReturn_Parameter>();
NoReturn(request.a);
}
//public void NoReturn(Int32 a);
}
[RequestServiceInfo(Name="Inter_NoReturn",ServiceId=20)]
public class Inter_NoReturn:IRequestService
{
private EmbedXrpcObject XrpcObject=null;
public Inter_NoReturn(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoReturn_ServiceId=20;//0x14
public UInt16 GetSid(){ return NoReturn_ServiceId;}
public Inter_NoReturn_Return Invoke(Int32 a)
{
Inter_NoReturn_Return reqresp=new Inter_NoReturn_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.ResponseMessageQueueHandle.Reset();
Inter_NoReturn_Parameter request =new Inter_NoReturn_Parameter();
request.a=a;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());
sm.Serialize(request,0);
List<byte> sendBytes = new List<byte>();
sendBytes.Add((byte)(NoReturn_ServiceId&0xff));
sendBytes.Add((byte)(NoReturn_ServiceId>>8&0xff));
sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));
sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));
sendBytes.AddRange(sm.Data);
if( XrpcObject.Send ( sendBytes.Count,0,sendBytes.ToArray() )==false)
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
public class Inter_NoArgAndReturn_Parameter
{
}
public class Inter_NoArgAndReturn_Return
{
public const int State_FieldNumber=1;
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}


}
[ResponseServiceInfo(Name="Inter_NoArgAndReturn",ServiceId=21)]
public partial class Inter_NoArgAndReturnService:IService
{
public static readonly UInt16 NoArgAndReturn_ServiceId=21;//0x15
public UInt16 GetSid(){ return NoArgAndReturn_ServiceId;}
public  void Invoke(SerializationManager recManager, SerializationManager sendManager)
{
Inter_NoArgAndReturn_Parameter request = recManager.Deserialize < Inter_NoArgAndReturn_Parameter>();
NoArgAndReturn();
}
//public void NoArgAndReturn();
}
[RequestServiceInfo(Name="Inter_NoArgAndReturn",ServiceId=21)]
public class Inter_NoArgAndReturn:IRequestService
{
private EmbedXrpcObject XrpcObject=null;
public Inter_NoArgAndReturn(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoArgAndReturn_ServiceId=21;//0x15
public UInt16 GetSid(){ return NoArgAndReturn_ServiceId;}
public Inter_NoArgAndReturn_Return Invoke()
{
Inter_NoArgAndReturn_Return reqresp=new Inter_NoArgAndReturn_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.ResponseMessageQueueHandle.Reset();
Inter_NoArgAndReturn_Parameter request =new Inter_NoArgAndReturn_Parameter();
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),new List<byte>());
sm.Serialize(request,0);
List<byte> sendBytes = new List<byte>();
sendBytes.Add((byte)(NoArgAndReturn_ServiceId&0xff));
sendBytes.Add((byte)(NoArgAndReturn_ServiceId>>8&0xff));
sendBytes.Add((byte)(XrpcObject.TimeOut&0xff));
sendBytes.Add((byte)(((XrpcObject.TimeOut>>8&0xff)&0x3F)|((byte)ReceiveType.Request)<<6));
sendBytes.AddRange(sm.Data);
if( XrpcObject.Send ( sendBytes.Count,0,sendBytes.ToArray() )==false)
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
}
