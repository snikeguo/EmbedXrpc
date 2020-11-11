using System;
using System.Reflection;
using System.Collections.Generic;
using EmbedXrpc;
// auto code gen ! DO NOT modify this file! create time 2020 - 11 - 11 19: 22:16.017;
//C# Code Generater Version:1.7.3.0
namespace Sample1
{
using UInt8 = Byte;
using Int8 = SByte;
public class DateTime_t
{
[FieldNumber( 10086) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Year{get;set;}
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Month{get;set;}
[FieldNumber( 3) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Day{get;set;}
[FieldNumber( 4) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Hour{get;set;}
[FieldNumber( 5) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Min{get;set;}
[FieldNumber( 6) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 Sec{get;set;}
}
public class DateTimeChange_Parameter
{
[ArrayProperty(LenFieldName = "")]
[FieldNumber( 1) ] 
public DateTime_t[] now{get;set;}=new DateTime_t[1];
}
[DelegateInfo(Name="DateTimeChange")]
public partial class DateTimeChangeClientImpl:IDelegate
{
public static readonly UInt16 DateTimeChange_ServiceId=16;//0x10
public override UInt16 GetSid(){ return DateTimeChange_ServiceId;}
public override void Invoke(SerializationManager recManager)
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
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.IsEnableMataDataEncode,new List<byte>());
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
public class Inter_Add_Parameter
{
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 b{get;set;}
}
public class Inter_Add_Return
{
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 ReturnValue{get;set;}
}
[ResponseServiceInfo(Name="Inter_Add",ServiceId=17)]
public partial class Inter_AddService:IService
{
public static readonly UInt16 Add_ServiceId=17;//0x11
public override UInt16 GetSid(){ return Add_ServiceId;}
private Inter_Add_Return Response = new Inter_Add_Return();
public override void Invoke(SerializationManager recManager, SerializationManager sendManager)
{
Inter_Add_Parameter request = recManager.Deserialize < Inter_Add_Parameter>();
Add(request.a,request.b);
sendManager.Serialize(Response, 0);
}
//public void Add(Int32 a,Int32 b);
}
[RequestServiceInfo(Name="Inter_Add",ServiceId=17)]
public class Inter_Add:IRequestService
{
private EmbedXrpcObject XrpcObject=null;
public Inter_Add(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 Add_ServiceId=17;//0x11
public override UInt16 GetSid(){ return Add_ServiceId;}
public Inter_Add_Return Add(Int32 a,Int32 b)
{
Inter_Add_Return reqresp=new Inter_Add_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.ResponseMessageQueueHandle.Reset();
Inter_Add_Parameter request =new Inter_Add_Parameter();
request.a=a;
request.b=b;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.IsEnableMataDataEncode,new List<byte>());
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
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}
[FieldNumber( 2) ] 
[ArrayLenFieldFlag( false ) ]
public UInt8 ReturnValue{get;set;}
}
[ResponseServiceInfo(Name="Inter_NoArg",ServiceId=18)]
public partial class Inter_NoArgService:IService
{
public static readonly UInt16 NoArg_ServiceId=18;//0x12
public override UInt16 GetSid(){ return NoArg_ServiceId;}
private Inter_NoArg_Return Response = new Inter_NoArg_Return();
public override void Invoke(SerializationManager recManager, SerializationManager sendManager)
{
Inter_NoArg_Parameter request = recManager.Deserialize < Inter_NoArg_Parameter>();
NoArg();
sendManager.Serialize(Response, 0);
}
//public void NoArg();
}
[RequestServiceInfo(Name="Inter_NoArg",ServiceId=18)]
public class Inter_NoArg:IRequestService
{
private EmbedXrpcObject XrpcObject=null;
public Inter_NoArg(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoArg_ServiceId=18;//0x12
public override UInt16 GetSid(){ return NoArg_ServiceId;}
public Inter_NoArg_Return NoArg()
{
Inter_NoArg_Return reqresp=new Inter_NoArg_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.ResponseMessageQueueHandle.Reset();
Inter_NoArg_Parameter request =new Inter_NoArg_Parameter();
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.IsEnableMataDataEncode,new List<byte>());
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
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public Int32 a{get;set;}
}
public class Inter_NoReturn_Return
{
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}
}
[ResponseServiceInfo(Name="Inter_NoReturn",ServiceId=19)]
public partial class Inter_NoReturnService:IService
{
public static readonly UInt16 NoReturn_ServiceId=19;//0x13
public override UInt16 GetSid(){ return NoReturn_ServiceId;}
public override void Invoke(SerializationManager recManager, SerializationManager sendManager)
{
Inter_NoReturn_Parameter request = recManager.Deserialize < Inter_NoReturn_Parameter>();
NoReturn(request.a);
}
//public void NoReturn(Int32 a);
}
[RequestServiceInfo(Name="Inter_NoReturn",ServiceId=19)]
public class Inter_NoReturn:IRequestService
{
private EmbedXrpcObject XrpcObject=null;
public Inter_NoReturn(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoReturn_ServiceId=19;//0x13
public override UInt16 GetSid(){ return NoReturn_ServiceId;}
public Inter_NoReturn_Return NoReturn(Int32 a)
{
Inter_NoReturn_Return reqresp=new Inter_NoReturn_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.ResponseMessageQueueHandle.Reset();
Inter_NoReturn_Parameter request =new Inter_NoReturn_Parameter();
request.a=a;
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.IsEnableMataDataEncode,new List<byte>());
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
[FieldNumber( 1) ] 
[ArrayLenFieldFlag( false ) ]
public RequestResponseState State{get;set;}
}
[ResponseServiceInfo(Name="Inter_NoArgAndReturn",ServiceId=20)]
public partial class Inter_NoArgAndReturnService:IService
{
public static readonly UInt16 NoArgAndReturn_ServiceId=20;//0x14
public override UInt16 GetSid(){ return NoArgAndReturn_ServiceId;}
public override void Invoke(SerializationManager recManager, SerializationManager sendManager)
{
Inter_NoArgAndReturn_Parameter request = recManager.Deserialize < Inter_NoArgAndReturn_Parameter>();
NoArgAndReturn();
}
//public void NoArgAndReturn();
}
[RequestServiceInfo(Name="Inter_NoArgAndReturn",ServiceId=20)]
public class Inter_NoArgAndReturn:IRequestService
{
private EmbedXrpcObject XrpcObject=null;
public Inter_NoArgAndReturn(EmbedXrpcObject xrpcObject)
{
XrpcObject=xrpcObject;
}
public static readonly UInt16 NoArgAndReturn_ServiceId=20;//0x14
public override UInt16 GetSid(){ return NoArgAndReturn_ServiceId;}
public Inter_NoArgAndReturn_Return NoArgAndReturn()
{
Inter_NoArgAndReturn_Return reqresp=new Inter_NoArgAndReturn_Return();
lock(XrpcObject.ObjectMutex) 
{
XrpcObject.ResponseMessageQueueHandle.Reset();
Inter_NoArgAndReturn_Parameter request =new Inter_NoArgAndReturn_Parameter();
SerializationManager sm=new SerializationManager(Assembly.GetExecutingAssembly(),XrpcObject.IsEnableMataDataEncode,new List<byte>());
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
