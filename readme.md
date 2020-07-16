# EmbedXrpc 用户手册
# QQ群134161401
#### 这个东西类似于Google的GRPC,但是应用场景是单片机。RPC远程调用极大的方便了开发，使得不必关注于协议解析，数据的序列化和反序列化等繁琐的工作。可是目前还没有在单片机上实现好用的RPC框架，于是我就谋生了做这个RPC框架的想法，所用的技术是：C#做 IDL语言+csscript+自己实现序列化和反序列化+代码生成(Razor Engine) QQ群134161401
#### 应用场景：单片机近距离Client/Server交互场景（USB、串口、CAN（如J1939 、ISO15765协议等），）只要是流协议都支持
#### Sample1工程是例子！除了main.cpp的代码是手工写的之外，其他的代码都是工具生成的！这是个最简单的例子！
#### 此工程演示了：
#### 1.客户端每一秒向服务端发送5、2 服务端计算出来结果后，把结果发送给客户端,客户端打印出来
```
//客户端请求
auto sum=Client.Add(5, 2);
if (sum.State == ResponseState_Ok)//如果接收到了正确的结果就打印出来
{
	printf("sum:%d\n", sum.ReturnValue);
}
```
这时候会通过底层发送到对方，比如通过串口什么的

```
//服务端这边,填鸭式完成如下类方法(这些函数签名都是生成的,你只需要实现就可以了)：
void Inter_AddService::Add(Byte a, Byte b)
{
	Response.ReturnValue = a + b;
}
```
#### 2.服务端每1秒广播当前的时间，客户端打印到控制台上
![](简单示意图.jpg)
![](网图1.jpg)
## 编译EmbedXrpc代码生成器EXE
1.用vs2019 打开EmbedXrpcIdlParser.sln编译好(编译EXE只做一次)。
2.在EmbedXrpcIdlParser\bin\Debug会出现一个EmbedXrpcIdlParser.exe,
## 用户使用步骤(以idltest1.cs为例子)
1.编写你的IDL文件，目录下会有一个idltest1.cs 文件，是例子,你可以参考一下。
2.执行命令
```
.\EmbedXrpcIdlParser.exe -g all -l cpp -i idltest1.cs -o yourPath
```


###### -g 代表生成client还是server 还是all
###### -l 代表生成cpp还是 cs
###### -o 代表生成的代码打算放到哪个目录


5.生成好代码后（我假定你生成在了A文件夹下,
###### 其中：
###### A文件夹所有的文件(包括子目录下的) + EmbedXrpcRuntime/Cpp + EmbedXrpcRuntime/Cpp/Win32.Port = 客户端(上位机端)的源文件(如果上位机用C++开发)
###### A文件夹所有的文件(包括子目录下的) + EmbedXrpcRuntime/CS = 客户端(上位机端)的源文件(如果上位机用C#开发)
###### A文件夹所有的文件(包括子目录下的) + EmbedXrpcRuntime/Cpp + EmbedXrpcRuntime/Cpp/RT-Thread.Port = 服务端(单片机端)的源文件 RT-Thread的移植
###### A文件夹所有的文件(包括子目录下的) + EmbedXrpcRuntime/Cpp + EmbedXrpcRuntime/Cpp/FreeRTOS.Port = 服务端(单片机端)的源文件 FreeRTOS的移植

#### 又一个简单的例子
1.编写idl文件：demo.cs
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
2.执行命令(见上述),生成客户端(电脑端)的代码和服务端代码(这里假定执行命令中,-g参数你写的是all,而不是单独的client和server),接口所示：

```
//客户端
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


//----------服务端-----------
class Inter_GetValueService : public IService
{
public:
    uint16_t GetSid() { return Inter_GetValue_ServiceId; }
    Inter_GetValue_Response Response;
    void GetValue();
    void Invoke(SerializationManager &recManager, SerializationManager &sendManager);
};
```

**3.服务端(单片机)这边,你需要编写代码：**
```
EmbedXrpcServerObject ServerRpcObject(.....);//参数我这里省略了

void Inter_GetValueService::GetValue()
{
    Response.ReturnValue=188;
}

```


**4.客户端(电脑)这边,你需要编写代码：**

```
EmbedXrpcClientObject ClientRpcObject(....Args...);//参数我这里省略了
InterClientImpl Client(&ClientRpcObject);
auto val=Client.GetValue();//这个函数将会把数据发送到服务器(比如单片机)上
if(val.State==ResponseState_Ok)
{
    UInt16 value=val.ReturnValue;//如果不出意外的话，value的值是188
    //你的代码
}
Client.Free_GetValue(&val);//最后用完val,记得调用接口对应的释放函数,形如： Free_你的接口()
```

至此一个完整的RPC调用就完成了
可以看下我自己项目上实现的
```
auto ThreadsInfo=Dev.Request.GetThreadsInfo();
CheckResponseState(ThreadsInfo);//我自己写的一个宏，就是判断State是否是OK，如果不是OK，就返回错误的，简单辅助宏罢了
printf("...");//打印参数

auto EraseResult=Dev.Request.FlashErase(addr,size);
CheckResponseState(EraseResult);
while(...)
{
  auto WriteResult=Dev.Request.FlashWrite(addr,size,bin);
  CheckResponseState(WriteResult);
}
```
是不是非常方便~~


### 原理部分(可以不看)

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
 sub[] b;
 sub c;
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
一般地，数组需要整数型字段（例子中的c_len）作为他的长度表示，如果没有，序列化的时候只序列化一个单位（例子中的MyStructType）长度；
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
具体的看代码吧，暂时看不懂序列化和反序列化没关系。



