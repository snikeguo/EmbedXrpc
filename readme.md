# mbedRpc 用户手册
#### 应用场景：单片机近距离Client/Server交互场景（USB、串口、CAN（如J1939 、ISO15765协议等），）只要是流协议都支持
## 用户使用步骤
1.用vs2019 打开EmbedXrpcIdlParser.sln编译好。
2.在EmbedXrpcIdlParser\bin\Debug会出现一个EmbedXrpcIdlParser.exe,
3.编写你的IDL文件，目录下会有一个idltest1.cs 文件，是例子。如果你打算用这个文件的话，请记得改一下这个文件里的这条语句
```
[GenerationOptionParameter(OutPutFileName= "StudentService",OutPutPath = @"D:\VSProject\EmbedXrpcIdlParser\TestEmbeXrpc\")]
```
把OutPutPath改成你想要的路径，这个路径是生成代码的路径。记得保存

4.执行命令
```
./EmbedXrpcIdlParser.exe cpp -i 你的idl文件 
```
如
```
./EmbedXrpcIdlParser.exe cpp -i idltest1.cs
```
记得写上文件扩展名

5.生成好代码后（我假定你生成在了A文件夹下），你还要把EmbedXrpcSerialization目录下的那堆Cpp和H文件也复制到A文件夹下，至此完成了工作。
###### 其中：
###### A文件夹下的文件+ Client目录下的代码=客户端需要（x86 PC端） 不支持大端
###### A文件夹下的文件+ Server目录下的代码=服务端需要（单片机端）




#### 序列化部分：
例如有如下结构体：
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
这个IDL文件通过exe后会生成序列化描述源文件，功能和高级语言的反射差不多。
假定生成代码后，有这样的语句：
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
序列化库会根据描述源文件 会把instance序列化成字节流；
0x1,0x2,0x0(小端序)，3,4,5,6,7,8；（并没有使用TLV或者TTLV编码。因为要减少内存消耗和提高效率，如果你想改成TLV，自己可以很简单改好。）
支持一维固定数组和一维动态数组（指针），不支持2维；
一般地，数组需要整数型字段（例子中的c_len）作为他的长度表示，如果没有，序列化的时候值序列化一个单位（例子中的MyStructType）长度；
总之他最终序列化的基本单位是整数u8.i8...i64  因为任何数据都是这个的集合体（不支持位域）

#### RPC部分：
序列化库和RPC是解耦的，你可以单独使用序列化，也可以两个一起用
假定有如下Service接口：
```
MyType GetValue(var a1,var a2,)
```
会生成一个结构体 GetValue_Request ,客户端生成GetValue同名函数，并生成如下代码：
```
GetValue（var a1,var a2,）
{
//填充结构体
Memcpy(GetValue_Request.a1,a1,sizeof(a1));
Memcpy....
GetValue_Request_Type.serialize(buf,request)
ClientRpcObject->Send(buf)//通过串口啥的发送出去
GetValue_Response response;
 wait(..&response)//等待串口的回复 如果接受成功，反序列化到response
Return 
}
```
## 关于使用注意：
**例如例子中 结构体ss中的b这个动态数组，如果b的值是无效的。也就是说当b指向一块无效的内存的时候，这时候执行序列化代码会发生异常，因为序列化库不管这些，他只是忠实的序列化b指向的内容。所以这些部分由用户去控制。**
具体的看代码吧，暂时看不懂序列化和反序列化没关系。最讨厌写文档了。



