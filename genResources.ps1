param($version)


Copy-Item -Path  .\EmbedXrpcIdlParser\bin\x86\Debug\EmbedXrpcIdlParser.exe -Destination PublishVersion\CodeGenTool\EmbedXrpcIdlParser.exe
Copy-Item -Path  .\EmbedXrpc.CSharp.NetCore\EmbedXrpcCommon.cs -Destination PublishVersion\Runtime\Cs\EmbedXrpcCommon.cs
Copy-Item -Path  .\EmbedXrpc.CSharp.NetCore\EmbedXrpcObject.cs -Destination PublishVersion\Runtime\Cs\EmbedXrpcObject.cs
Copy-Item -Path  .\EmbedXrpc.CSharp.NetCore\Serialization.cs -Destination PublishVersion\Runtime\Cs\Serialization.cs
Copy-Item -Path  .\EmbedXrpc.CSharp.NetCore\Win32Queue.cs -Destination PublishVersion\Runtime\Cs\Win32Queue.cs
Copy-Item -Path  .\EmbedXrpc.CSharp.NetCore\Win32Signal.cs -Destination PublishVersion\Runtime\Cs\Win32Signal.cs

Copy-Item -Path  .\EmbedXrpcSerialization\BlockBufferProvider.h -Destination PublishVersion\Runtime\Cpp\BlockBufferProvider.h
Copy-Item -Path  .\EmbedXrpcSerialization\EmbedSerialization.cpp -Destination PublishVersion\Runtime\Cpp\EmbedSerialization.cpp
Copy-Item -Path  .\EmbedXrpcSerialization\EmbedSerialization.h -Destination PublishVersion\Runtime\Cpp\EmbedSerialization.h
Copy-Item -Path  .\EmbedXrpcSerialization\EmbedXrpcCommon.h -Destination PublishVersion\Runtime\Cpp\EmbedXrpcCommon.h
Copy-Item -Path  .\EmbedXrpcSerialization\EmbedXrpcObject.h -Destination PublishVersion\Runtime\Cpp\EmbedXrpcObject.h

Copy-Item -Path  .\EmbedXrpcSerialization\ringbuffer.c -Destination PublishVersion\Runtime\Cpp\ringbuffer.c
Copy-Item -Path  .\EmbedXrpcSerialization\ringbuffer.h -Destination PublishVersion\Runtime\Cpp\ringbuffer.h
Copy-Item -Path  .\Sample1\sample1.cs -Destination PublishVersion\CodeGenTool\sample1.cs

# EmbedLibrary
Copy-Item -Path .\EmbedLibrary\EmbedLibrary.h -Destination PublishVersion\EmbedLibrary\EmbedLibrary.h
Copy-Item -Path .\EmbedLibrary\FreeRTOS.Port\El.FreeRTOS.Port.cpp  -Destination PublishVersion\EmbedLibrary\FreeRTOS.Port\El.FreeRTOS.Port.cpp
Copy-Item -Path .\EmbedLibrary\Win32.Port\BlockQueue.cpp  -Destination PublishVersion\EmbedLibrary\Win32.Port\BlockQueue.cpp
Copy-Item -Path .\EmbedLibrary\Win32.Port\BlockQueue.h  -Destination PublishVersion\EmbedLibrary\Win32.Port\BlockQueue.h
Copy-Item -Path .\EmbedLibrary\Win32.Port\El.Win32.Port.cpp  -Destination PublishVersion\EmbedLibrary\Win32.Port\El.Win32.Port.cpp


if(Test-Path .\EmbedXrpc.Master.$version.zip)
{
    Remove-Item .\EmbedXrpc.Master.$version.zip
}
Compress-Archive -LiteralPath .\PublishVersion -DestinationPath .\EmbedXrpc.Master.$version.zip