#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T21:29:34
#
#-------------------------------------------------
CONFIG += c++17

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4):  QT += widgets

TARGET = SaliCaptureEditor
TEMPLATE = app


SOURCES += \
    SdCaptureMain.cpp \
    guider/SdGuiderCapture.cpp \
    guider/SdGuiderPlayer.cpp \
    guider/SdGuiderFile.cpp \
    guider/SdGuiderWMain.cpp

HEADERS  += \
    guider/SdGuiderCapture.h \
    guider/SdGuiderPlayer.h \
    guider/SdGuiderFile.h \
    guider/SdGuiderWMain.h

DISTFILES += \
    license.txt

