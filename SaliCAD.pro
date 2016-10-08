#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T21:29:34
#
#-------------------------------------------------
CONFIG += c++11

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SaliCAD
TEMPLATE = app


SOURCES += \
    SdMain.cpp \
    windows/SdWMain.cpp \
    objects/SdObject.cpp \
    objects/SdProject.cpp \
    windows/SdWProjectTree.cpp \
    windows/SdWProjectList.cpp \
    objects/SdProjectModel.cpp \
    windows/SdWEditor.cpp \
    objects/SdProjectItem.cpp \
    objects/SdSymbol.cpp \
    objects/SdGraphObject.cpp \
    objects/SdPoint.cpp \
    objects/SdRect.cpp \
    objects/SdPointList.cpp \
    objects/SdCircle.cpp \
    objects/SdAngle.cpp \
    objects/SdLineProp.cpp \
    objects/SdProp.cpp \
    objects/SdTextProp.cpp \
    objects/SdContext.cpp \
    objects/SdQuadrangle.cpp \
    objects/SdUndo.cpp \
    objects/SdSegment.cpp \
    objects/SdSnapInfo.cpp \
    objects/SdPolyWindow.cpp \
    objects/SdPolyWindowList.cpp \
    objects/SdRoadProp.cpp \
    objects/SdSymPinProp.cpp \
    objects/SdPartPinProp.cpp \
    objects/SdSymImpProp.cpp \
    objects/SdPartImpProp.cpp \
    objects/SdOwner.cpp \
    windows/SdWCommand.cpp \
    objects/SdPulsar.cpp

HEADERS  += \
    SdConfig.h \
    windows/SdWMain.h \
    objects/SdObject.h \
    objects/SdProject.h \
    windows/SdWProjectTree.h \
    windows/SdWProjectList.h \
    objects/SdProjectModel.h \
    windows/SdWEditor.h \
    objects/SdProjectItem.h \
    objects/SdObjectMap.h \
    objects/SdSymbol.h \
    objects/SdGraphObject.h \
    objects/SdPoint.h \
    objects/SdRect.h \
    objects/SdPointList.h \
    objects/SdCircle.h \
    objects/SdIntProp.h \
    objects/SdAngle.h \
    objects/SdLineProp.h \
    objects/SdProp.h \
    objects/SdTextProp.h \
    objects/SdContext.h \
    objects/SdQuadrangle.h \
    objects/SdUndoRecord.h \
    objects/SdUndo.h \
    objects/SdSegment.h \
    objects/SdSnapInfo.h \
    objects/SdPolyWindow.h \
    objects/SdPolyWindowList.h \
    objects/SdRoadProp.h \
    objects/SdSymPinProp.h \
    objects/SdPartPinProp.h \
    objects/SdSymImpProp.h \
    objects/SdPartImpProp.h \
    objects/SdOwner.h \
    windows/SdWCommand.h \
    objects/SdPulsar.h

DISTFILES += \
    license.txt
