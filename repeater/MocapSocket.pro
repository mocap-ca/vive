#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T14:05:05
#
#-------------------------------------------------

CONFIG   += qt
CONFIG   += vicon
CONFIG   += naturalpoint
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MocapSocket
TEMPLATE = app

QMAKE_LFLAGS += -INCREMENTAL:NO

SOURCES += main.cpp\
    mainwindow.cpp \
    server.cpp \
    mocapSubject.cpp \
    testClient.cpp \
    profilesx.cpp \
    baseclient.cpp \
    mocapModel.cpp \
    vivec3d.cpp \
    ../fileTranslators/asfAmcFile.cpp \
    ../fileTranslators/bvhFile.cpp \
    ../fileTranslators/c3dFile.cpp \
    ../fileTranslators/htrFile.cpp \
    ../fileTranslators/trcFile.cpp

HEADERS  += mainwindow.h \
    server.h \
    mocapSubject.h \
    testClient.h \
    profile.h \
    baseclient.h \
    mocapModel.h \
    defines.h \
    vivec3d.h \
    ../fileTranslators/asfAmcFile.h \
    ../fileTranslators/bvhFile.h \
    ../fileTranslators/c3dFile.h \
    ../fileTranslators/htrFile.h \
    ../fileTranslators/trcFile.h \
    ../fileTranslators/fileTranslator.h \
    ../fileTranslators/joint.h \
    ../fileTranslators/vectorMath.h

FORMS    += mainwindow.ui

CONFIG(vicon) {
    DEFINES += VICON_CLIENT
    INCLUDEPATH += C:/cpp/api/viconDataStream/Win64/CPP/
    DEPENDPATH  += C:/cpp/api/viconDataStream/Win64/CPP/
    LIBS += C:/cpp/api/viconDataStream/Win64/CPP/ViconDataStreamSDK_CPP.lib
    SOURCES += viconClient.cpp
    HEADERS += viconClient.h
#win32: INCLUDEPATH += C:\cpp\api\viconDataStream\Win32\CPP
#win64: INCLUDEPATH += C:\cpp\api\viconDataStream\Win64\CPP
}

CONFIG(naturalpoint) {
    DEFINES += NATURALPOINT_CLIENT
    SOURCES += naturalpointClient.cpp CMNatNetPacketParser.cpp
    HEADERS += naturalpointClient.h NatNetTypes.h CMNatNetPacketParser.h
}
