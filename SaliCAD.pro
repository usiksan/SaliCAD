#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T21:29:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SaliCAD
TEMPLATE = app


SOURCES += \
    SdMain.cpp \
    windows/SdWMain.cpp

HEADERS  += \
    SdConfig.h \
    windows/SdWMain.h

DISTFILES += \
    license.txt
