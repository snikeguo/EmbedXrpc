 using EmbedXrpc; 
 using System; 
 using System.Reflection; 
 using System.Collections.Generic; 
 // auto code gen ! DO NOT modify this file! create time 2020-06-11 18:09:45.209 ; 
 namespace Sample1 
 { 
         public class DateTime_t 
         { 
                     public Int32 Year{get;set;} 
                     public Int32 Month{get;set;} 
                     public Int32 Day{get;set;} 
                     public Int32 Hour{get;set;} 
                     public Int32 Min{get;set;} 
                     public Int32 Sec{get;set;} 
         } 
         public class DateTimeChangeStruct 
         { 
                 [ArrayProperty(LenFieldName = "")] 
                 public DateTime_t[] now{get;set;}=new DateTime_t [ 1 ]; 
         } 
             [DelegateInfo(Name="DateTimeChange")] 
             public partial class DateTimeChangeClientImpl :IDelegate 
             { 
             public static readonly UInt16 DateTimeChange_ServiceId=16; 
             public override UInt16 GetSid(){ return DateTimeChange_ServiceId ; } 
             public override void Invoke(SerializationManager recManager) 
             { 
             Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
             DateTimeChangeStruct request = serialization.Deserialize <DateTimeChangeStruct>(recManager); 
             DateTimeChange(request.now); 
             } 
             //public void DateTimeChange(DateTime_t[] now); 
             } 
             public class DateTimeChangeDelegate 
             { 
             private EmbedXrpc.Server Server; 
             public DateTimeChangeDelegate(EmbedXrpc.Server server) 
             { Server=server; } 
             public static readonly UInt16 DateTimeChange_ServiceId=16; 
             public void Invoke( DateTime_t[] now ) 
             { 
             DateTimeChangeStruct request=new DateTimeChangeStruct(); 
                 request.now=now; 
             SerializationManager sm=new SerializationManager(); 
             Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
             serialization.Serialize(sm, request); 
             List<byte> sendBytes = new List<byte>(); 
             sendBytes.Add( (byte)(DateTimeChange_ServiceId&0xff) ); 
             sendBytes.Add( (byte)(DateTimeChange_ServiceId>>8&0xff) ); 
             sendBytes.Add( (byte)(Server.TimeOut>>0&0xff) ); 
             sendBytes.Add( (byte)(Server.TimeOut>>8&0xff) ); 
             sendBytes.AddRange(sm.Data); 
             Server.Send ( sendBytes.Count,0,sendBytes.ToArray() ); 
             } 
             } 
             public class Inter_Add_Request 
             { 
                         public Byte a{get;set;} 
                         public Byte b{get;set;} 
             } 
             public class Inter_Add_RequestResponseContent 
             { 
                 public RequestResponseState State{get;set;} 
                 public Int32 ReturnValue{get;set;} 
             } 
             public class Inter_NoArg_Request 
             { 
             } 
             public class Inter_NoArg_RequestResponseContent 
             { 
                 public RequestResponseState State{get;set;} 
                 public Boolean ReturnValue{get;set;} 
             } 
             public class Inter_NoReturn_Request 
             { 
             } 
             public class Inter_NoReturn_RequestResponseContent 
             { 
                 public RequestResponseState State{get;set;} 
             } 
             public class Inter_NoArgAndReturn_Request 
             { 
             } 
             public class Inter_NoArgAndReturn_RequestResponseContent 
             { 
                 public RequestResponseState State{get;set;} 
             } 
                 [ServiceInfo(Name="Inter.Add")] 
                 public partial class Inter_AddService :IService 
                 { 
                 public static readonly UInt16 Add_ServiceId=17; 
                 public override UInt16 GetSid(){ return Add_ServiceId ; } 
                     private Inter_Add_RequestResponseContent Response=new Inter_Add_RequestResponseContent() ; 
                 public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
                 { 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 Inter_Add_Request request = serialization.Deserialize <Inter_Add_Request>(recManager); 
                 Add(request.a,request.b); 
                     // Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                     serialization.Serialize(sendManager, Response); 
                 } 
                 //public void Add(Byte a,Byte b); 
                 } 
                 [ServiceInfo(Name="Inter.NoArg")] 
                 public partial class Inter_NoArgService :IService 
                 { 
                 public static readonly UInt16 NoArg_ServiceId=18; 
                 public override UInt16 GetSid(){ return NoArg_ServiceId ; } 
                     private Inter_NoArg_RequestResponseContent Response=new Inter_NoArg_RequestResponseContent() ; 
                 public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
                 { 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 Inter_NoArg_Request request = serialization.Deserialize <Inter_NoArg_Request>(recManager); 
                 NoArg(); 
                     // Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                     serialization.Serialize(sendManager, Response); 
                 } 
                 //public void NoArg(); 
                 } 
                 [ServiceInfo(Name="Inter.NoReturn")] 
                 public partial class Inter_NoReturnService :IService 
                 { 
                 public static readonly UInt16 NoReturn_ServiceId=19; 
                 public override UInt16 GetSid(){ return NoReturn_ServiceId ; } 
                 public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
                 { 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 Inter_NoReturn_Request request = serialization.Deserialize <Inter_NoReturn_Request>(recManager); 
                 NoReturn(); 
                 } 
                 //public void NoReturn(); 
                 } 
                 [ServiceInfo(Name="Inter.NoArgAndReturn")] 
                 public partial class Inter_NoArgAndReturnService :IService 
                 { 
                 public static readonly UInt16 NoArgAndReturn_ServiceId=20; 
                 public override UInt16 GetSid(){ return NoArgAndReturn_ServiceId ; } 
                 public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
                 { 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 Inter_NoArgAndReturn_Request request = serialization.Deserialize <Inter_NoArgAndReturn_Request>(recManager); 
                 NoArgAndReturn(); 
                 } 
                 //public void NoArgAndReturn(); 
                 } 
                 [ResponseInfo(Name="Inter.Add",ServiceId=17)] 
                 [ResponseInfo(Name="Inter.NoArg",ServiceId=18)] 
                 [ResponseInfo(Name="Inter.NoReturn",ServiceId=19)] 
                 [ResponseInfo(Name="Inter.NoArgAndReturn",ServiceId=20)] 
             public class InterClientImpl 
             { 
             private EmbedXrpc.EmbedXrpcObject Client=null; 
             public InterClientImpl(EmbedXrpc.EmbedXrpcObject client) 
             { Client=client;} 
                 public static readonly UInt16 Add_ServiceId=17; 
                 public Inter_Add_RequestResponseContent Add( Byte a,Byte b) 
                 { 
                 Inter_Add_RequestResponseContent reqresp=new Inter_Add_RequestResponseContent() ; 
                 lock(Client.ObjectMutex) 
                 { 
                 Client.ResponseMessageQueueHandle.Reset(); 
                 Inter_Add_Request request=new Inter_Add_Request (); 
                     request.a=a; 
                     request.b=b; 
                 SerializationManager sm=new SerializationManager(); 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 serialization.Serialize(sm, request); 
                 List<byte> sendBytes = new List<byte>(); 
                 sendBytes.Add( (byte)(Add_ServiceId&0xff) ); 
                 sendBytes.Add( (byte)(Add_ServiceId>>8&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>0&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>8&0xff) ); 
                 sendBytes.AddRange(sm.Data); 
                 if( Client.Send ( sendBytes.Count,0,sendBytes.ToArray() )==false)
 {
reqresp.State=RequestResponseState.RequestState_Failed;
 goto exi;
}
else
{
reqresp.State=RequestResponseState.RequestState_Ok;
} 
                     var waitstate=Client.Wait<Inter_Add_RequestResponseContent>( Add_ServiceId, out reqresp); 
                     if(reqresp ==null ) 
                     { 
                     reqresp=new Inter_Add_RequestResponseContent() ; 
                     } 
                     reqresp.State=waitstate;
                     goto exi; 
                 } 
                 exi:
return reqresp; 
                 } 
                 public static readonly UInt16 NoArg_ServiceId=18; 
                 public Inter_NoArg_RequestResponseContent NoArg( ) 
                 { 
                 Inter_NoArg_RequestResponseContent reqresp=new Inter_NoArg_RequestResponseContent() ; 
                 lock(Client.ObjectMutex) 
                 { 
                 Client.ResponseMessageQueueHandle.Reset(); 
                 Inter_NoArg_Request request=new Inter_NoArg_Request (); 
                 SerializationManager sm=new SerializationManager(); 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 serialization.Serialize(sm, request); 
                 List<byte> sendBytes = new List<byte>(); 
                 sendBytes.Add( (byte)(NoArg_ServiceId&0xff) ); 
                 sendBytes.Add( (byte)(NoArg_ServiceId>>8&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>0&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>8&0xff) ); 
                 sendBytes.AddRange(sm.Data); 
                 if( Client.Send ( sendBytes.Count,0,sendBytes.ToArray() )==false)
 {
reqresp.State=RequestResponseState.RequestState_Failed;
 goto exi;
}
else
{
reqresp.State=RequestResponseState.RequestState_Ok;
} 
                     var waitstate=Client.Wait<Inter_NoArg_RequestResponseContent>( NoArg_ServiceId, out reqresp); 
                     if(reqresp ==null ) 
                     { 
                     reqresp=new Inter_NoArg_RequestResponseContent() ; 
                     } 
                     reqresp.State=waitstate;
                     goto exi; 
                 } 
                 exi:
return reqresp; 
                 } 
                 public static readonly UInt16 NoReturn_ServiceId=19; 
                 public Inter_NoReturn_RequestResponseContent NoReturn( ) 
                 { 
                 Inter_NoReturn_RequestResponseContent reqresp=new Inter_NoReturn_RequestResponseContent() ; 
                 lock(Client.ObjectMutex) 
                 { 
                 Client.ResponseMessageQueueHandle.Reset(); 
                 Inter_NoReturn_Request request=new Inter_NoReturn_Request (); 
                 SerializationManager sm=new SerializationManager(); 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 serialization.Serialize(sm, request); 
                 List<byte> sendBytes = new List<byte>(); 
                 sendBytes.Add( (byte)(NoReturn_ServiceId&0xff) ); 
                 sendBytes.Add( (byte)(NoReturn_ServiceId>>8&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>0&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>8&0xff) ); 
                 sendBytes.AddRange(sm.Data); 
                 if( Client.Send ( sendBytes.Count,0,sendBytes.ToArray() )==false)
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
                 public static readonly UInt16 NoArgAndReturn_ServiceId=20; 
                 public Inter_NoArgAndReturn_RequestResponseContent NoArgAndReturn( ) 
                 { 
                 Inter_NoArgAndReturn_RequestResponseContent reqresp=new Inter_NoArgAndReturn_RequestResponseContent() ; 
                 lock(Client.ObjectMutex) 
                 { 
                 Client.ResponseMessageQueueHandle.Reset(); 
                 Inter_NoArgAndReturn_Request request=new Inter_NoArgAndReturn_Request (); 
                 SerializationManager sm=new SerializationManager(); 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 serialization.Serialize(sm, request); 
                 List<byte> sendBytes = new List<byte>(); 
                 sendBytes.Add( (byte)(NoArgAndReturn_ServiceId&0xff) ); 
                 sendBytes.Add( (byte)(NoArgAndReturn_ServiceId>>8&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>0&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>8&0xff) ); 
                 sendBytes.AddRange(sm.Data); 
                 if( Client.Send ( sendBytes.Count,0,sendBytes.ToArray() )==false)
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
