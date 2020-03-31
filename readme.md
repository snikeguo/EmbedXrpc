# mbedRpc �û��ֲ�
#### Ӧ�ó�������Ƭ��������Client/Server����������USB�����ڡ�CAN����J1939 ��ISO15765Э��ȣ�����ֻҪ����Э�鶼֧��
## �û�ʹ�ò���
1.��vs2019 ��EmbedXrpcIdlParser.sln����á�
2.��EmbedXrpcIdlParser\bin\Debug�����һ��EmbedXrpcIdlParser.exe,
3.��д���IDL�ļ���Ŀ¼�»���һ��idltest1.cs �ļ��������ӡ���������������ļ��Ļ�����ǵø�һ������ļ�����������
```
[GenerationOptionParameter(OutPutFileName= "StudentService",OutPutPath = @"D:\VSProject\EmbedXrpcIdlParser\TestEmbeXrpc\")]
```
��OutPutPath�ĳ�����Ҫ��·�������·�������ɴ����·�����ǵñ���

4.ִ������
```
./EmbedXrpcIdlParser.exe cpp -i ���idl�ļ� 
```
��
```
./EmbedXrpcIdlParser.exe cpp -i idltest1.cs
```
�ǵ�д���ļ���չ��

5.���ɺô�����Ҽٶ�����������A�ļ����£����㻹Ҫ��EmbedXrpcSerializationĿ¼�µ��Ƕ�Cpp��H�ļ�Ҳ���Ƶ�A�ļ����£���������˹�����
###### ���У�
###### A�ļ����µ��ļ�+ ClientĿ¼�µĴ���=�ͻ�����Ҫ��x86 PC�ˣ� ��֧�ִ��
###### A�ļ����µ��ļ�+ ServerĿ¼�µĴ���=�������Ҫ����Ƭ���ˣ�




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
 MyStructType[] b;
MyStructType c;
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
����Ŀ�����ɣ���ʱ���������л��ͷ����л�û��ϵ��������д�ĵ��ˡ�



