 using EmbedXrpc; 
 using System; 
 using System.Reflection; 
 using System.Collections.Generic; 
 // auto code gen ! DO NOT modify this file! create time 2020-05-05 16:45:33.617 ; 
 namespace StudentService 
 { 
         public enum Sex_t :UInt32 
         { 
             Man= 0, 
             Woman= 1, 
             HHHH= 2, 
         } 
         public class Result_t 
         { 
                     public Int32 Value{get;set;} 
                     public Byte NameLen{get;set;} 
                 [ArrayProperty(LenFieldName = "NameLen")] 
                 public Byte[] Name{get;set;}=new Byte [ 0 ]; 
                     public SByte Arg1{get;set;} 
         } 
         public class Student_t 
         { 
                     public UInt16 ResultsLen{get;set;} 
                 [ArrayProperty(LenFieldName = "ResultsLen")] 
                 public Result_t[] Results{get;set;}=new Result_t [ 0 ]; 
                     public Int32 Age{get;set;} 
                 [ArrayProperty(LenFieldName = "")] 
                 public Byte[] Name{get;set;}=new Byte [ 1 ]; 
                     public Byte StudentIdLen{get;set;} 
                 [ArrayProperty(LenFieldName = "StudentIdLen")] 
                 public Byte[] StudentId{get;set;}=new Byte [ 0 ]; 
                     public Sex_t Sex{get;set;} 
         } 
         public class DateTime_t 
         { 
                     public Int32 Year{get;set;} 
                     public Int32 Month{get;set;} 
                     public Int32 Day{get;set;} 
                     public Int32 Hour{get;set;} 
                     public Int32 Min{get;set;} 
                     public Int32 Sec{get;set;} 
         } 
         public class StudentArray_t 
         { 
                     public Byte StudentIdLen{get;set;} 
                 [ArrayProperty(LenFieldName = "StudentIdLen")] 
                 public Student_t[] Students{get;set;}=new Student_t [ 0 ]; 
         } 
         public class BroadcastDataTimeStruct 
         { 
                     public DateTime_t t{get;set;}=new DateTime_t (); 
         } 
             [DelegateInfo(Name="BroadcastDataTime")] 
             public partial class BroadcastDataTimeClientImpl :IDelegate 
             { 
             public static readonly UInt16 BroadcastDataTime_ServiceId=16; 
             public override UInt16 GetSid(){ return BroadcastDataTime_ServiceId ; } 
             public override void Invoke(SerializationManager recManager) 
             { 
             Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
             BroadcastDataTimeStruct request = serialization.Deserialize <BroadcastDataTimeStruct>(recManager); 
             BroadcastDataTime(request.t); 
             } 
             //public void BroadcastDataTime(DateTime_t t); 
             } 
             public class BroadcastDataTimeDelegate 
             { 
             private EmbedXrpc.Server Server; 
             public BroadcastDataTimeDelegate(EmbedXrpc.Server server) 
             { Server=server; } 
             public static readonly UInt16 BroadcastDataTime_ServiceId=16; 
             public void Invoke( DateTime_t t ) 
             { 
             BroadcastDataTimeStruct request=new BroadcastDataTimeStruct(); 
                 request.t=t; 
             SerializationManager sm=new SerializationManager(); 
             Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
             serialization.Serialize(sm, request); 
             List<byte> sendBytes = new List<byte>(); 
             sendBytes.Add( (byte)(BroadcastDataTime_ServiceId&0xff) ); 
             sendBytes.Add( (byte)(BroadcastDataTime_ServiceId>>8&0xff) ); 
             sendBytes.Add( (byte)(Server.TimeOut>>0&0xff) ); 
             sendBytes.Add( (byte)(Server.TimeOut>>8&0xff) ); 
             sendBytes.AddRange(sm.Data); 
             Server.Send ( sendBytes.Count,0,sendBytes.ToArray() ); 
             } 
             } 
             public class IMyInterface_GetStudentInfoFormStudentId_Request 
             { 
                         public Byte StudentIdLen{get;set;} 
                     [ArrayProperty(LenFieldName = "StudentIdLen")] 
                     public Byte[] StudentId{get;set;}=new Byte [ 0 ]; 
                         public Int32 arg2{get;set;} 
                         public Int32 arg3{get;set;} 
             } 
                 public class IMyInterface_GetStudentInfoFormStudentId_Response 
                 { 
                     public ResponseState State{get;set;} 
                     public Student_t ReturnValue{get;set;} 
                 } 
             public class IMyInterface_GetStudentsInfoFormAge_Request 
             { 
             } 
                 public class IMyInterface_GetStudentsInfoFormAge_Response 
                 { 
                     public ResponseState State{get;set;} 
                     public StudentArray_t ReturnValue{get;set;} 
                 } 
             public class IMyInterface_Test_Request 
             { 
                     [ArrayProperty(LenFieldName = "")] 
                     public Byte[] noLen{get;set;}=new Byte [ 1 ]; 
             } 
                 public class IMyInterface_Test_Response 
                 { 
                     public ResponseState State{get;set;} 
                     public Boolean ReturnValue{get;set;} 
                 } 
                 [ServiceInfo(Name="IMyInterface.GetStudentInfoFormStudentId")] 
                 public partial class IMyInterface_GetStudentInfoFormStudentIdService :IService 
                 { 
                 public static readonly UInt16 GetStudentInfoFormStudentId_ServiceId=17; 
                 public override UInt16 GetSid(){ return GetStudentInfoFormStudentId_ServiceId ; } 
                     private IMyInterface_GetStudentInfoFormStudentId_Response Response=new IMyInterface_GetStudentInfoFormStudentId_Response() ; 
                 public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
                 { 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 IMyInterface_GetStudentInfoFormStudentId_Request request = serialization.Deserialize <IMyInterface_GetStudentInfoFormStudentId_Request>(recManager); 
                 GetStudentInfoFormStudentId(request.StudentIdLen,request.StudentId,request.arg2,request.arg3); 
                     // Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                     serialization.Serialize(sendManager, Response); 
                 } 
                 //public void GetStudentInfoFormStudentId(Byte StudentIdLen,Byte[] StudentId,Int32 arg2,Int32 arg3); 
                 } 
                 [ServiceInfo(Name="IMyInterface.GetStudentsInfoFormAge")] 
                 public partial class IMyInterface_GetStudentsInfoFormAgeService :IService 
                 { 
                 public static readonly UInt16 GetStudentsInfoFormAge_ServiceId=18; 
                 public override UInt16 GetSid(){ return GetStudentsInfoFormAge_ServiceId ; } 
                     private IMyInterface_GetStudentsInfoFormAge_Response Response=new IMyInterface_GetStudentsInfoFormAge_Response() ; 
                 public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
                 { 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 IMyInterface_GetStudentsInfoFormAge_Request request = serialization.Deserialize <IMyInterface_GetStudentsInfoFormAge_Request>(recManager); 
                 GetStudentsInfoFormAge(); 
                     // Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                     serialization.Serialize(sendManager, Response); 
                 } 
                 //public void GetStudentsInfoFormAge(); 
                 } 
                 [ServiceInfo(Name="IMyInterface.Test")] 
                 public partial class IMyInterface_TestService :IService 
                 { 
                 public static readonly UInt16 Test_ServiceId=19; 
                 public override UInt16 GetSid(){ return Test_ServiceId ; } 
                     private IMyInterface_Test_Response Response=new IMyInterface_Test_Response() ; 
                 public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
                 { 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 IMyInterface_Test_Request request = serialization.Deserialize <IMyInterface_Test_Request>(recManager); 
                 Test(request.noLen); 
                     // Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                     serialization.Serialize(sendManager, Response); 
                 } 
                 //public void Test(Byte[] noLen); 
                 } 
                 [ResponseInfo(Name="IMyInterface.GetStudentInfoFormStudentId",ServiceId=17)] 
                 [ResponseInfo(Name="IMyInterface.GetStudentsInfoFormAge",ServiceId=18)] 
                 [ResponseInfo(Name="IMyInterface.Test",ServiceId=19)] 
             public class IMyInterfaceClientImpl 
             { 
             private EmbedXrpc.Client Client=null; 
             public IMyInterfaceClientImpl(EmbedXrpc.Client client) 
             { Client=client;} 
                 public static readonly UInt16 GetStudentInfoFormStudentId_ServiceId=17; 
                     public IMyInterface_GetStudentInfoFormStudentId_Response GetStudentInfoFormStudentId( Byte StudentIdLen,Byte[] StudentId,Int32 arg2,Int32 arg3) 
                 { 
                 lock(Client.ObjectMutex) 
                 { 
                 Client.ResponseMessageQueueHandle.Reset(); 
                 IMyInterface_GetStudentInfoFormStudentId_Request request=new IMyInterface_GetStudentInfoFormStudentId_Request (); 
                     request.StudentIdLen=StudentIdLen; 
                     request.StudentId=StudentId; 
                     request.arg2=arg2; 
                     request.arg3=arg3; 
                 SerializationManager sm=new SerializationManager(); 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 serialization.Serialize(sm, request); 
                 List<byte> sendBytes = new List<byte>(); 
                 sendBytes.Add( (byte)(GetStudentInfoFormStudentId_ServiceId&0xff) ); 
                 sendBytes.Add( (byte)(GetStudentInfoFormStudentId_ServiceId>>8&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>0&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>8&0xff) ); 
                 sendBytes.AddRange(sm.Data); 
                 Client.Send ( sendBytes.Count,0,sendBytes.ToArray() ); 
                     IMyInterface_GetStudentInfoFormStudentId_Response response=new IMyInterface_GetStudentInfoFormStudentId_Response() ; 
                     ResponseState result=Client.Wait<IMyInterface_GetStudentInfoFormStudentId_Response>( GetStudentInfoFormStudentId_ServiceId, out response); 
                     if(response ==null ) 
                     { 
                     response=new IMyInterface_GetStudentInfoFormStudentId_Response() ; 
                     } 
                     response.State=result; 
                     return response; 
                 } 
                 } 
                 public static readonly UInt16 GetStudentsInfoFormAge_ServiceId=18; 
                     public IMyInterface_GetStudentsInfoFormAge_Response GetStudentsInfoFormAge( ) 
                 { 
                 lock(Client.ObjectMutex) 
                 { 
                 Client.ResponseMessageQueueHandle.Reset(); 
                 IMyInterface_GetStudentsInfoFormAge_Request request=new IMyInterface_GetStudentsInfoFormAge_Request (); 
                 SerializationManager sm=new SerializationManager(); 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 serialization.Serialize(sm, request); 
                 List<byte> sendBytes = new List<byte>(); 
                 sendBytes.Add( (byte)(GetStudentsInfoFormAge_ServiceId&0xff) ); 
                 sendBytes.Add( (byte)(GetStudentsInfoFormAge_ServiceId>>8&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>0&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>8&0xff) ); 
                 sendBytes.AddRange(sm.Data); 
                 Client.Send ( sendBytes.Count,0,sendBytes.ToArray() ); 
                     IMyInterface_GetStudentsInfoFormAge_Response response=new IMyInterface_GetStudentsInfoFormAge_Response() ; 
                     ResponseState result=Client.Wait<IMyInterface_GetStudentsInfoFormAge_Response>( GetStudentsInfoFormAge_ServiceId, out response); 
                     if(response ==null ) 
                     { 
                     response=new IMyInterface_GetStudentsInfoFormAge_Response() ; 
                     } 
                     response.State=result; 
                     return response; 
                 } 
                 } 
                 public static readonly UInt16 Test_ServiceId=19; 
                     public IMyInterface_Test_Response Test( Byte[] noLen) 
                 { 
                 lock(Client.ObjectMutex) 
                 { 
                 Client.ResponseMessageQueueHandle.Reset(); 
                 IMyInterface_Test_Request request=new IMyInterface_Test_Request (); 
                     request.noLen=noLen; 
                 SerializationManager sm=new SerializationManager(); 
                 Serialization serialization=new Serialization(Assembly.GetExecutingAssembly()); 
                 serialization.Serialize(sm, request); 
                 List<byte> sendBytes = new List<byte>(); 
                 sendBytes.Add( (byte)(Test_ServiceId&0xff) ); 
                 sendBytes.Add( (byte)(Test_ServiceId>>8&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>0&0xff) ); 
                 sendBytes.Add( (byte)(Client.TimeOut>>8&0xff) ); 
                 sendBytes.AddRange(sm.Data); 
                 Client.Send ( sendBytes.Count,0,sendBytes.ToArray() ); 
                     IMyInterface_Test_Response response=new IMyInterface_Test_Response() ; 
                     ResponseState result=Client.Wait<IMyInterface_Test_Response>( Test_ServiceId, out response); 
                     if(response ==null ) 
                     { 
                     response=new IMyInterface_Test_Response() ; 
                     } 
                     response.State=result; 
                     return response; 
                 } 
                 } 
             } 
 } 
