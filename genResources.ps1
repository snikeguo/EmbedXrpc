param($version)


Copy-Item -Path  .\EmbedXrpcIdlParser\bin\x86\Debug\EmbedXrpcIdlParser.exe -Destination PublishVersion\EmbedXrpcIdlParser.exe
Copy-Item -Path  .\EmbedXrpc.CSharp.NetCore\EmbedXrpcCommon.cs -Destination PublishVersion\Runtime\Cs\EmbedXrpcCommon.cs
Copy-Item -Path  .\EmbedXrpc.CSharp.NetCore\EmbedXrpcObject.cs -Destination PublishVersion\Runtime\Cs\EmbedXrpcObject.cs
Copy-Item -Path  .\EmbedXrpc.CSharp.NetCore\Serialization.cs -Destination PublishVersion\Runtime\Cs\Serialization.cs
Copy-Item -Path  .\Win32Queue.NetCore\Win32Queue.cs -Destination PublishVersion\Runtime\Cs\Win32Queue.cs
Copy-Item -Path  .\Win32Queue.NetCore\Win32Signal.cs -Destination PublishVersion\Runtime\Cs\Win32Signal.cs

Copy-Item -Path  .\EmbedXrpcSerialization\EmbedSerialization.h -Destination PublishVersion\Runtime\Cpp\EmbedSerialization.h
Copy-Item -Path  .\EmbedXrpcSerialization\EmbedXrpcCommon.h -Destination PublishVersion\Runtime\Cpp\EmbedXrpcCommon.h
Copy-Item -Path  .\EmbedXrpcSerialization\EmbedXrpcObject.cpp -Destination PublishVersion\Runtime\Cpp\EmbedXrpcObject.cpp
Copy-Item -Path  .\EmbedXrpcSerialization\EmbedXrpcObject.h -Destination PublishVersion\Runtime\Cpp\EmbedXrpcObject.h
Copy-Item -Path  .\EmbedXrpcSerialization\EmbedXrpcPortInterface.h -Destination PublishVersion\Runtime\Cpp\EmbedXrpcPortInterface.h
Copy-Item -Path  .\EmbedXrpcSerialization\Port\EmbedXrpc.Port.h  -Destination PublishVersion\Runtime\Cpp\EmbedXrpc.Port.h.template
Copy-Item -Path  .\EmbedXrpcSerialization\SoftCrc32.c  -Destination PublishVersion\Runtime\Cpp\SoftCrc32.c
Copy-Item -Path  .\EmbedXrpcSerialization\SoftCrc32.h  -Destination PublishVersion\Runtime\Cpp\SoftCrc32.h

# EmbedLibrary
Copy-Item -Path .\EmbedLibrary\EmbedLibrary.h -Destination PublishVersion\EmbedLibrary\EmbedLibrary.h
Copy-Item -Path .\EmbedLibrary\Win32.Port\El.Win32.Port.cpp  -Destination PublishVersion\EmbedLibrary\Win32.Port\El.Win32.Port.cpp
Copy-Item -Path .\EmbedLibrary\FreeRTOS.Port\El.FreeRTOS.Port.cpp  -Destination PublishVersion\EmbedLibrary\FreeRTOS.Port\El.FreeRTOS.Port.cpp

Copy-Item -Path .\EmbedLibrary\stream_buffer_noos_and_win32.h  -Destination PublishVersion\EmbedLibrary\stream_buffer_noos_and_win32.h
Copy-Item -Path .\EmbedLibrary\queue_noos_and_win32.h  -Destination PublishVersion\EmbedLibrary\queue_noos_and_win32.h
Copy-Item -Path .\EmbedLibrary\message_buffer_noos_and_win32.h  -Destination PublishVersion\EmbedLibrary\message_buffer_noos_and_win32.h
Copy-Item -Path .\EmbedLibrary\stream_buffer_noos_and_win32.c  -Destination PublishVersion\EmbedLibrary\stream_buffer_noos_and_win32.c
Copy-Item -Path .\EmbedLibrary\queue_noos_and_win32.c  -Destination PublishVersion\EmbedLibrary\queue_noos_and_win32.c


# copy Sample
if(Test-Path .\PublishVersion\Sample1)
{
    Remove-Item .\PublishVersion\Sample1 -Recurse
}
if(Test-Path .\PublishVersion\Sample2)
{
    Remove-Item .\PublishVersion\Sample2 -Recurse
}
if(Test-Path .\PublishVersion\Sample3)
{
    Remove-Item .\PublishVersion\Sample3 -Recurse
}

Copy-Item .\Sample1 .\PublishVersion\Sample1 -Recurse
Copy-Item .\Sample2 .\PublishVersion\Sample2 -Recurse
Copy-Item .\Sample3 .\PublishVersion\Sample3 -Recurse

if(Test-Path .\EmbedXrpc.Master.$version.zip)
{
    Remove-Item .\EmbedXrpc.Master.$version.zip
}
Compress-Archive -LiteralPath .\PublishVersion -DestinationPath .\EmbedXrpc.Master.$version.zip