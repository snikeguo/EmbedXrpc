# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Tools.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.
message("You are running qmake on a generated .pro file. This may not work!")


HEADERS += ../EmbedXrpcSerialization/EmbedSerialization.h \
    ../EmbedXrpcSerialization/EmbedSerializationBaseType.h \
    ../EmbedXrpcSerialization/EmbedXrpc.Port.h \
    ../EmbedXrpcSerialization/EmbedXrpcCommon.h \
    ../EmbedXrpcSerialization/EmbedXrpcServerObject.h \
    ./BlockQueue.h \
    ./Server/StudentService.Server.h \
    ./StudentService.EmbedXrpcSerialization.h \
    ./StudentService.h \
    ./TestEmbedXrpcForQt.h \
    ./Win32EmbedXrpcPort.h
SOURCES += ../EmbedXrpcSerialization/EmbedSerialization.cpp \
    ../EmbedXrpcSerialization/EmbedXrpc.Port.cpp \
    ./BlockQueue.cpp \
    ./main.cpp \
    ./Server/StudentService.Server.cpp \
    ./StudentService.cpp \
    ./StudentService.EmbedXrpcSerialization.cpp \
    ./TestEmbedXrpcForQt.cpp \
    ./Win32EmbedXrpcPort.cpp
FORMS += ./TestEmbedXrpcForQt.ui
RESOURCES += TestEmbedXrpcForQt.qrc
