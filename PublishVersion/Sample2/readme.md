## DEMO工程
### 实现功能：
- 1.client每秒向server请求Add服务,用来计算两个数之和。server计算出来后返回给client,client打印出来。
- 2.server每秒向连接的client广播当前时刻

本例子将使用EmbedXrpc各个支持的模式,做全测试,结果：
cppmode包含有两种模式反射版本(cpp-ref下面表格写做0)和生成函数版本(cpp-nano下面表格写做1)

| cpp mode  | is use ringbuffer mode   |  is check sum  | test result  |
| :------------: | :------------: | :------------: | :------------: |
| 0  | 0  | 0  | √  |
| 0  | 0  | 1  | √  |
| 0  | 1  | 0  | √  |
| 0  | 1  | 1  | √  |
| 1  | 0  | 0  | √  |
| 1  | 0  | 1  | √  |
| 1  | 1  | 0  | √  |
| 1  | 1  | 1  | √  |



