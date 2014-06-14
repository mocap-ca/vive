#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T14:05:05
#
#-------------------------------------------------

CONFIG   += qt
#CONFIG   += vicon
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
    localServer.cpp \
    baseclient.cpp

HEADERS  += mainwindow.h \
    server.h \
    mocapSubject.h \
    testClient.h \
    profile.h \
    localServer.h \
    baseclient.h

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
