echo off
echo  ..\EmbedXrpcIdlParser\bin\x86\Debug\EmbedXrpcIdlParser.exe -i sample1.cs -g all -l cpp-nano -o XrpcCodeGen
EmbedXrpcIdlParser.exe -i sample1.cs -g all -l cpp-nano -o XrpcCodeGen
EmbedXrpcIdlParser.exe -i sample1.cs -g all -l cs -o XrpcCodeGen