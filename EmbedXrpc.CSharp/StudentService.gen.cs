 using EmbedXrpc; 
 using System; 
 // auto code gen ! DO NOT modify this file! create time 2020-04-05 22:57:07.614 ; 
 namespace StudentService 
 { 
         public enum Sex_t :UInt16 
         { 
             Man= 0, 
             Woman= 1, 
         } 
         public class Result_t 
         { 
             public Int32 Value{get;set;} 
             public Byte NameLen{get;set;} 
                 [ArrayProperty(LenFieldName = "NameLen")] 
             public Byte[] Name{get;set;} 
             public SByte Arg1{get;set;} 
         } 
         public class Student_t 
         { 
             public UInt16 ResultsLen{get;set;} 
                 [ArrayProperty(LenFieldName = "ResultsLen")] 
             public Result_t[] Results{get;set;} 
             public Int32 Age{get;set;} 
                 [ArrayProperty(LenFieldName = "")] 
             public Byte[] Name{get;set;} 
             public Byte StudentIdLen{get;set;} 
                 [ArrayProperty(LenFieldName = "StudentIdLen")] 
             public Int16[] StudentId{get;set;} 
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
             public Student_t[] Students{get;set;} 
         } 
         public class BroadcastDataTimeStruct 
         { 
             public DateTime_t t{get;set;} 
         } 
         [DelegateInfo(Name="BroadcastDataTime")] 
         public partial class BroadcastDataTimeClientImpl :IDelegate 
         { 
         public static readonly UInt32 BroadcastDataTime_ServiceId=16; 
         public override UInt32 GetSid(){ return BroadcastDataTime_ServiceId ; } 
         public override void Invoke(SerializationManager recManager) 
         { 
         BroadcastDataTimeStruct request = Serialization.Deserialize <BroadcastDataTimeStruct>(recManager); 
         BroadcastDataTime(request.t); 
         } 
         //public void BroadcastDataTime(DateTime_t t); 
         } 
         public class BroadcastDataTimeDelegate 
         { 
         private EmbedXrpc.Server Server; 
         public BroadcastDataTimeDelegate(EmbedXrpc.Server server) 
         { Server=server; } 
         public static readonly UInt32 BroadcastDataTime_ServiceId=16; 
         public void BroadcastDataTime( DateTime_t t) 
         { 
         BroadcastDataTimeStruct request=new BroadcastDataTimeStruct(); 
             request.t=t; 
         SerializationManager sm=new SerializationManager(); 
         Serialization.Serialize(sm, request); 
         Server.Send ( BroadcastDataTime_ServiceId , sm.Index,sm.Data.ToArray() ); 
         } 
         } 
             public class GetStudentInfoFormStudentId_Request 
             { 
                 public Byte StudentIdLen{get;set;} 
                     [ArrayProperty(LenFieldName = "StudentIdLen")] 
                 public Byte[] StudentId{get;set;} 
                 public Int32 arg2{get;set;} 
                 public Int32 arg3{get;set;} 
             } 
                 public class GetStudentInfoFormStudentId_Response 
                 { 
                     public ResponseState State{get;set;} 
                     public Student_t ReturnValue{get;set;} 
                 } 
             public class GetStudentsInfoFormAge_Request 
             { 
             } 
                 public class GetStudentsInfoFormAge_Response 
                 { 
                     public ResponseState State{get;set;} 
                     public StudentArray_t ReturnValue{get;set;} 
                 } 
             public class Test_Request 
             { 
             } 
             [ServiceInfo(Name="GetStudentInfoFormStudentId")] 
             public partial class GetStudentInfoFormStudentIdService :IService 
             { 
             public static readonly UInt32 GetStudentInfoFormStudentId_ServiceId=17; 
             public override UInt32 GetSid(){ return GetStudentInfoFormStudentId_ServiceId ; } 
                 private GetStudentInfoFormStudentId_Response Response=new GetStudentInfoFormStudentId_Response() ; 
             public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
             { 
             GetStudentInfoFormStudentId_Request request = Serialization.Deserialize <GetStudentInfoFormStudentId_Request>(recManager); 
             GetStudentInfoFormStudentId(request.StudentIdLen,request.StudentId,request.arg2,request.arg3); 
                 Serialization.Serialize(sendManager, Response); 
             } 
             //public void GetStudentInfoFormStudentId(Byte StudentIdLen,Byte[] StudentId,Int32 arg2,Int32 arg3); 
             } 
             [ServiceInfo(Name="GetStudentsInfoFormAge")] 
             public partial class GetStudentsInfoFormAgeService :IService 
             { 
             public static readonly UInt32 GetStudentsInfoFormAge_ServiceId=18; 
             public override UInt32 GetSid(){ return GetStudentsInfoFormAge_ServiceId ; } 
                 private GetStudentsInfoFormAge_Response Response=new GetStudentsInfoFormAge_Response() ; 
             public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
             { 
             GetStudentsInfoFormAge_Request request = Serialization.Deserialize <GetStudentsInfoFormAge_Request>(recManager); 
             GetStudentsInfoFormAge(); 
                 Serialization.Serialize(sendManager, Response); 
             } 
             //public void GetStudentsInfoFormAge(); 
             } 
             [ServiceInfo(Name="Test")] 
             public partial class TestService :IService 
             { 
             public static readonly UInt32 Test_ServiceId=19; 
             public override UInt32 GetSid(){ return Test_ServiceId ; } 
             public override void Invoke(SerializationManager recManager, SerializationManager sendManager) 
             { 
             Test_Request request = Serialization.Deserialize <Test_Request>(recManager); 
             Test(); 
             } 
             //public void Test(); 
             } 
             [ResponseInfo(Name="GetStudentInfoFormStudentId",ServiceId=17)] 
             [ResponseInfo(Name="GetStudentsInfoFormAge",ServiceId=18)] 
             [ResponseInfo(Name="Test",ServiceId=19)] 
         public class IMyInterfaceClientImpl 
         { 
         private EmbedXrpc.Client Client=null; 
         public IMyInterfaceClientImpl(EmbedXrpc.Client client) 
         { Client=client;} 
             public static readonly UInt32 GetStudentInfoFormStudentId_ServiceId=17; 
                 public GetStudentInfoFormStudentId_Response GetStudentInfoFormStudentId( Byte StudentIdLen,Byte[] StudentId,Int32 arg2,Int32 arg3) 
             { 
             GetStudentInfoFormStudentId_Request request=new GetStudentInfoFormStudentId_Request (); 
                 request.StudentIdLen=StudentIdLen; 
                 request.StudentId=StudentId; 
                 request.arg2=arg2; 
                 request.arg3=arg3; 
             SerializationManager sm=new SerializationManager(); 
             Serialization.Serialize(sm, request); 
             Client.Send ( GetStudentInfoFormStudentId_ServiceId , sm.Index,sm.Data.ToArray() ); 
                 GetStudentInfoFormStudentId_Response response=new GetStudentInfoFormStudentId_Response() ; 
                 ResponseState result=Client.Wait<GetStudentInfoFormStudentId_Response>( GetStudentInfoFormStudentId_ServiceId, out response); 
                 if(response ==null) 
                 { 
                     response=new GetStudentInfoFormStudentId_Response() ; 
                 } 
                 response.State=result; 
                 return response; 
             } 
             public static readonly UInt32 GetStudentsInfoFormAge_ServiceId=18; 
                 public GetStudentsInfoFormAge_Response GetStudentsInfoFormAge( ) 
             { 
             GetStudentsInfoFormAge_Request request=new GetStudentsInfoFormAge_Request (); 
             SerializationManager sm=new SerializationManager(); 
             Serialization.Serialize(sm, request); 
             Client.Send ( GetStudentsInfoFormAge_ServiceId , sm.Index,sm.Data.ToArray() ); 
                 GetStudentsInfoFormAge_Response response=new GetStudentsInfoFormAge_Response() ; 
                 ResponseState result=Client.Wait<GetStudentsInfoFormAge_Response>( GetStudentsInfoFormAge_ServiceId, out response); 
                 if(response ==null) 
                 { 
                     response=new GetStudentsInfoFormAge_Response() ; 
                 } 
                 response.State=result; 
                 return response; 
             } 
             public static readonly UInt32 Test_ServiceId=19; 
                 public void Test( ) 
             { 
             Test_Request request=new Test_Request (); 
             SerializationManager sm=new SerializationManager(); 
             Serialization.Serialize(sm, request); 
             Client.Send ( Test_ServiceId , sm.Index,sm.Data.ToArray() ); 
             } 
         } 
 } 
