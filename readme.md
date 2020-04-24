# EmbedRpc �û��ֲ�
#### [��Ƶ�̳�](https://v.qq.com/x/page/v0943tykmh8.html)
#### Ӧ�ó�������Ƭ��������Client/Server����������USB�����ڡ�CAN����J1939 ��ISO15765Э��ȣ�����ֻҪ����Э�鶼֧��
![avatar](��ʾ��ͼ.png)
![avatar](��ͼ1.png)
## �û�ʹ�ò���
1.��vs2019 ��EmbedXrpcIdlParser.sln����á�
2.��EmbedXrpcIdlParser\bin\Debug�����һ��EmbedXrpcIdlParser.exe,
3.��д���IDL�ļ���Ŀ¼�»���һ��idltest1.cs �ļ��������ӡ���������������ļ��Ļ�����ǵø�һ������ļ����������
```
[FileName("idltest1.cs")]
public class OptionProcess:IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "StudentService";//������ļ���
        option.CSharpNameSpace = "StudentService";//�������C#�ļ�,�������ɵ������ռ�
        option.UserIncludes.Add("UserIncFile");//�������Cpp�ļ�,���ɵĴ��뽫�����UserIncFile.h�ļ�.����㲻��Ҫ������д�������
        option.UserNamespace.Add("UserNameSpace");//�������C#�ļ�,���ɵĴ��뽫�����using UserNameSpace;���.����㲻��Ҫ������д�������
        return option;
    }
}

```
�ǵñ���.

4.ִ������
```
.\EmbedXrpcIdlParser.exe -g all -l cpp -i idltest1.cs -o yourPath
```


###### -g ��������client����server ����all
###### -l ��������cpp���� cs
###### -o �������ɵĴ������ŵ��ĸ�Ŀ¼


5.���ɺô�����Ҽٶ�����������A�ļ�����,
###### ���У�
###### A�ļ������е��ļ�(������Ŀ¼�µ�) + EmbedXrpcRuntime/Cpp + EmbedXrpcRuntime/Cpp/Win32.Port = �ͻ���(��λ����)��Դ�ļ�(�����λ����C++����)
###### A�ļ������е��ļ�(������Ŀ¼�µ�) + EmbedXrpcRuntime/CS = �ͻ���(��λ����)��Դ�ļ�(�����λ����C#����)
###### A�ļ������е��ļ�(������Ŀ¼�µ�) + EmbedXrpcRuntime/Cpp + EmbedXrpcRuntime/Cpp/RT-Thread.Port = �����(��Ƭ����)��Դ�ļ� Ŀǰ֧��RT-Thread

#### һ���򵥵�����
1.��дidl�ļ���demo.cs
```
using System;
using EmbedXrpcIdlParser;

[FileName("demo.cs")]
interface Inter
{
    UInt16 GetValue();
}
[FileName("demo.cs")]
public class OptionProcess:IOptionProcess
{
    public GenerationOption Process()
    {
        GenerationOption option = new GenerationOption();
        option.OutPutFileName = "Demo";
        option.CSharpNameSpace = "Demo";
        return option;
    }
}
```
2.ִ������(������),���ɿͻ���(���Զ�)�Ĵ���ͷ���˴���(����ٶ�ִ��������,-g������д����all,�����ǵ�����client��server),�ӿ���ʾ��

```
//�ͻ���
class InterClientImpl
{
public:
    EmbedXrpcClientObject *RpcClientObject = nullptr;
    InterClientImpl(EmbedXrpcClientObject *rpcobj)
    {
        this->RpcClientObject = rpcobj;
    }
    Inter_GetValue_Response &GetValue();
    void Free_GetValue(Inter_GetValue_Response *response);
};


//----------�����-----------
class Inter_GetValueService : public IService
{
public:
    uint16_t GetSid() { return Inter_GetValue_ServiceId; }
    Inter_GetValue_Response Response;
    void GetValue();
    void Invoke(SerializationManager &recManager, SerializationManager &sendManager);
};
```

**3.�����(��Ƭ��)���,����Ҫ��д���룺**
```
EmbedXrpcServerObject ServerRpcObject(.....);//����������ʡ����

void Inter_GetValueService::GetValue()
{
    Response.ReturnValue=188;
}

```


**4.�ͻ���(����)���,����Ҫ��д���룺**

```
EmbedXrpcClientObject ClientRpcObject(....Args...);//����������ʡ����
InterClientImpl Client(&ClientRpcObject);
auto val=Client.GetValue();//���������������ݷ��͵�������(���絥Ƭ��)��
if(val.State==ResponseState_Ok)
{
    UInt16 value=val.ReturnValue;//�����������Ļ���value��ֵ��188
    //��Ĵ���
}
Client.Free_GetValue(&val);//�������val,�ǵõ��ýӿڶ�Ӧ���ͷź���,���磺 Free_��Ľӿ�()
```

����һ��������RPC���þ������
���Կ������Լ���Ŀ��ʵ�ֵ�
```
auto ThreadsInfo=Dev.Request.GetThreadsInfo();
CheckResponseState(ThreadsInfo);//���Լ�д��һ���꣬�����ж�State�Ƿ���OK���������OK���ͷ��ش���ģ��򵥸��������
printf("...");//��ӡ����

auto EraseResult=Dev.Request.FlashErase(addr,size);
CheckResponseState(EraseResult);
while(...)
{
  auto WriteResult=Dev.Request.FlashWrite(addr,size,bin);
  CheckResponseState(WriteResult);
}
```
�ǲ��Ƿǳ�����~~


### ԭ����

#### ���л����֣�
���������½ṹ�壺
```
Struct sub
{
Uint8 v1;
Uint8 v2;
}
Struct ss
{
 Uint8  a;
 Uint16 b_len;
 sub[] b;
 sub c;
}
```
���IDL�ļ�ͨ��exe����������л�����Դ�ļ������ܺ͸߼����Եķ����ࡣ
�ٶ����ɴ��������������䣺
```
Struct ss  instance;
Instance.a=1;
Instance. b_len=2;
Instance.b=malloc(....);
Instance.b[0].v1=3;
Instance.b[0].v2=4;
Instance.b[1].v1=5;
Instance.b[1].v2=6;
Instance.c.v1=7;
Instance.c.v2=8;
```
���л�����������Դ�ļ� ���instance���л����ֽ�����
0x1,0x2,0x0(С����)��3,4,5,6,7,8������û��ʹ��TLV����TTLV���롣��ΪҪ�����ڴ����ĺ����Ч�ʣ��������ĳ�TLV���Լ����Ժܼ򵥸ĺá���
֧��һά�̶������һά��̬���飨ָ�룩����֧��2ά��
һ��أ�������Ҫ�������ֶΣ������е�c_len����Ϊ���ĳ��ȱ�ʾ�����û�У����л���ʱ��ֵ���л�һ����λ�������е�MyStructType�����ȣ�
��֮���������л��Ļ�����λ������u8.i8...i64  ��Ϊ�κ����ݶ�������ļ����壨��֧��λ��

#### RPC���֣�
���л����RPC�ǽ���ģ�����Ե���ʹ�����л���Ҳ��������һ����
�ٶ�������Service�ӿڣ�
```
MyType GetValue(var a1,var a2,)
```
������һ���ṹ�� GetValue_Request ,�ͻ�������GetValueͬ�����������������´��룺
```
GetValue��var a1,var a2,��
{
//���ṹ��
Memcpy(GetValue_Request.a1,a1,sizeof(a1));
Memcpy....
GetValue_Request_Type.serialize(buf,request)
ClientRpcObject->Send(buf)//ͨ������ɶ�ķ��ͳ�ȥ
GetValue_Response response;
 wait(..&response)//�ȴ����ڵĻظ� ������ܳɹ��������л���response
Return 
}
```
## ����ʹ��ע�⣺
**���������� �ṹ��ss�е�b�����̬���飬���b��ֵ����Ч�ġ�Ҳ����˵��bָ��һ����Ч���ڴ��ʱ����ʱ��ִ�����л�����ᷢ���쳣����Ϊ���л��ⲻ����Щ����ֻ����ʵ�����л�bָ������ݡ�������Щ�������û�ȥ���ơ�**
����Ŀ�����ɣ���ʱ���������л��ͷ����л�û��ϵ��



