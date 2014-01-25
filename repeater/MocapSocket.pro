#-------------------------------------------------
#
# Project created by QtCreator 2013-12-18T14:05:05
#
#-------------------------------------------------

CONFIG   += qt
QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MocapSocket
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    workthread.cpp

HEADERS  += mainwindow.h \
    server.h \
    workthread.h

FORMS    += mainwindow.ui
