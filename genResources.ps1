copy  .\EmbedXrpcIdlParser\bin\x86\Debug\EmbedXrpcIdlParser.exe .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\CodeGenTool\EmbedXrpcIdlParser.exe
copy  .\EmbedXrpc.CSharp.NetCore\EmbedXrpcCommon.cs .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cs\EmbedXrpcCommon.cs
copy  .\EmbedXrpc.CSharp.NetCore\EmbedXrpcObject.cs .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cs\EmbedXrpcObject.cs
copy  .\EmbedXrpc.CSharp.NetCore\Serialization.cs .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cs\Serialization.cs
copy  .\EmbedXrpc.CSharp.NetCore\Win32Queue.cs .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cs\Win32Queue.cs
copy  .\EmbedXrpc.CSharp.NetCore\Win32Signal.cs .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cs\Win32Signal.cs
copy  .\EmbedXrpcSerialization\BlockBufferProvider.cpp .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\BlockBufferProvider.cpp
copy  .\EmbedXrpcSerialization\BlockBufferProvider.h .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\BlockBufferProvider.h
copy  .\EmbedXrpcSerialization\EmbedSerialization.cpp .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\EmbedSerialization.cpp
copy  .\EmbedXrpcSerialization\EmbedSerialization.h .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\EmbedSerialization.h
copy  .\EmbedXrpcSerialization\EmbedXrpcCommon.h .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\EmbedXrpcCommon.h
copy  .\EmbedXrpcSerialization\EmbedXrpcObject.h .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\EmbedXrpcObject.h
copy  .\EmbedXrpcSerialization\EmbedXrpcPortInterface.h .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\EmbedXrpcPortInterface.h
copy  .\EmbedXrpcSerialization\ringbuffer.cpp .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\ringbuffer.cpp
copy  .\EmbedXrpcSerialization\ringbuffer.h .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\ringbuffer.h


copy -Recurse -Force .\EmbedXrpcSerialization\FreeRTOS.Port .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\FreeRTOS.Port
copy -Recurse -Force .\EmbedXrpcSerialization\FreeRTOS.Port .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\FreeRTOS.Port
copy -Recurse -Force .\EmbedXrpcSerialization\Win32.Port .\EmbedXrpcIdlParser\bin\x86\EmbedXrpc.Master\Runtime\Cpp\Win32.Port
