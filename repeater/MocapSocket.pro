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

win32:LIBS += -lsetupapi -lWs2_32

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
    ../fileTranslators/trcFile.cpp \
    viveClient.cpp \
    fileClient.cpp \
    filewriter.cpp

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
    ../fileTranslators/vectorMath.h \
    viveClient.h \
    fileClient.h \
    filewriter.h

FORMS    += mainwindow.ui

CONFIG(vicon) {
    DEFINES += VICON_CLIENT
win32:contains(QMAKE_HOST.arch, x86_64) {
    VICONDIR = C:/cpp/api/viconDataStream/Win64
} else {
    VICONDIR = C:/cpp/api/viconDataStream/Win32
}
    INCLUDEPATH += $${VICONDIR}/CPP/
    DEPENDPATH  += $${VICONDIR}/CPP/
    LIBS += $${VICONDIR}/CPP/ViconDataStreamSDK_CPP.lib

    SOURCES += viconClient.cpp
    HEADERS += viconClient.h
}

CONFIG(naturalpoint) {
    NATNETDIR = C:\cpp\git\vive\repeater\NatNetSDK2.5
    DEFINES += NATURALPOINT_CLIENT
    SOURCES += naturalpointClient.cpp
    HEADERS += naturalpointClient.h NatNetTypes.h
win32:contains(QMAKE_HOST.arch, x86_64) {
    LIBS += -L$${NATNETDIR}/lib/x64 -lNatNetLibStatic
} else {
    LIBS += -L$${NATNETDIR}/lib -lNatNetLibStatic
}
    INCLUDEPATH += $${NATNETDIR}/include

}
