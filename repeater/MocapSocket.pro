

CONFIG   += qt
QT       += core gui network widgets

# download from http://www.optitrack.com/downloads/developer-tools.html
NATNETDIR = C:/cpp/api/natnet2.7
CONFIG   += naturalpoint

# download from: http://www.vicon.com/products/software/datastream-sdk
VICONDIR = C:\cpp\api\ViconDataStream1.5
CONFIG   += vicon


TARGET = MocapSocket
TEMPLATE = app

win32:LIBS += -lsetupapi -lWs2_32

QMAKE_LFLAGS += -INCREMENTAL:NO

SOURCES += ../src/mocapData.cpp \
    src/main.cpp         \
    src/mainwindow.cpp   \
    src/server.cpp       \
    src/mocapSubject.cpp \
    src/testClient.cpp   \
    src/profilesx.cpp    \
    src/baseclient.cpp   \
    src/mocapModel.cpp   \
    src/vivec3d.cpp      \
    ../fileTranslators/asfAmcFile.cpp \
    ../fileTranslators/bvhFile.cpp \
    ../fileTranslators/c3dFile.cpp \
    ../fileTranslators/htrFile.cpp \
    ../fileTranslators/trcFile.cpp \
    src/viveClient.cpp \
    src/fileClient.cpp \
    src/filewriter.cpp

INCLUDEPATH += src/ ../fileTranslators ../src/

HEADERS  += ../src/mocapData.h \
    src/mainwindow.h \
    src/server.h \
    src/mocapSubject.h \
    src/testClient.h \
    src/profile.h \
    src/baseclient.h \
    src/mocapModel.h \
    src/defines.h \
    src/vivec3d.h \
    src/viveClient.h \
    src/fileClient.h \
    src/filewriter.h \
    ../fileTranslators/asfAmcFile.h \
    ../fileTranslators/bvhFile.h \
    ../fileTranslators/c3dFile.h \
    ../fileTranslators/htrFile.h \
    ../fileTranslators/trcFile.h \
    ../fileTranslators/fileTranslator.h \
    ../fileTranslators/joint.h \
    ../fileTranslators/vectorMath.h

FORMS    += src/mainwindow.ui

#########
# VICON #
#########

CONFIG(vicon) {
    DEFINES += VICON_CLIENT

win32:contains(QMAKE_HOST.arch, x86_64) {
    VICONDIR = $${VICONDIR}/Win64/CPP
} else {
    VICONDIR = $${VICONDIR}/Win32/CPP
}

    INCLUDEPATH += $${VICONDIR}
    DEPENDPATH  += $${VICONDIR}
    LIBS += $${VICONDIR}/ViconDataStreamSDK_CPP.lib

    SOURCES += src/viconClient.cpp
    HEADERS += src/viconClient.h
}

################
# NATURALPOINT #
################

CONFIG(naturalpoint) {
    DEFINES += NATURALPOINT_CLIENT
    SOURCES += src/naturalpointClient.cpp
    HEADERS += src/naturalpointClient.h
win32:contains(QMAKE_HOST.arch, x86_64) {
    LIBS += -L$${NATNETDIR}/lib/x64 -lNatNetLibStatic
} else {
    LIBS += -L$${NATNETDIR}/lib -lNatNetLibStatic
}
    INCLUDEPATH += $${NATNETDIR}/include

}
