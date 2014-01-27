#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T14:05:05
#
#-------------------------------------------------

CONFIG   += qt
QT       += core gui network
INCLUDEPATH += C:/cpp/api/viconDataStream/Win32/CPP/
DEPENDPATH  += C:/cpp/api/viconDataStream/Win32/CPP/

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MocapSocket
TEMPLATE = app

#win32: INCLUDEPATH += C:\cpp\api\viconDataStream\Win32\CPP
#win64: INCLUDEPATH += C:\cpp\api\viconDataStream\Win64\CPP

QMAKE_LFLAGS += /INCREMENTAL:NO

LIBS += C:/cpp/api/viconDataStream/Win32/CPP/ViconDataStreamSDK_CPP.lib

SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    workthread.cpp \
	mocapSubject.cpp \
        viconClient.cpp \
	testClient.cpp

HEADERS  += mainwindow.h \
    server.h \
    workthread.h \
	mocapSubject.h \
        testClient.h \
    viconClient.h

FORMS    += mainwindow.ui
