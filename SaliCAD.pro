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
    windows/SdWEditor.cpp \
    objects/SdProjectItem.cpp \
    objects/SdSymbol.cpp \
    objects/SdPoint.cpp \
    objects/SdRect.cpp \
    objects/SdPointList.cpp \
    objects/SdCircle.cpp \
    objects/SdAngle.cpp \
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
    windows/SdWCommand.cpp \
    objects/SdPulsar.cpp \
    windows/SdPNewProjectItem_SelectType.cpp \
    windows/SdPNewProjectItem_EnterName.cpp \
    objects/SdIds.cpp \
    windows/SdDGetObject.cpp \
    objects/SdObjectFactory.cpp \
    objects/SdObjectInfo.cpp \
    objects/SdObjectDataBaseRecord.cpp \
    windows/SdWEditorGraph.cpp \
    windows/SdWEditorGraphSymbol.cpp \
    objects/SdScaler.cpp \
    windows/SdDOptions.cpp \
    windows/SdDOptionsPageColors.cpp \
    objects/SdEnvir.cpp \
    windows/SdWView.cpp \
    modes/SdMode.cpp \
    modes/SdModeCommon.cpp \
    windows/windowsCursor.cpp \
    objects/SdLayer.cpp \
    objects/SdContainer.cpp \
    objects/SdGraph.cpp \
    objects/SdGraphLinear.cpp \
    objects/SdPropLine.cpp \
    objects/SdPropLayer.cpp \
    objects/SdGraphLinearLine.cpp \
    objects/SdSelector.cpp \
    windows/SdWEditorGraphPart.cpp \
    objects/SdConverter.cpp \
    objects/SdConverterView.cpp \
    modes/SdModeTemp.cpp \
    modes/SdCursorFactory.cpp \
    modes/SdModeTZoomer.cpp \
    modes/SdModeTZoomWindow.cpp

HEADERS  += \
    SdConfig.h \
    windows/SdWMain.h \
    objects/SdObject.h \
    objects/SdProject.h \
    windows/SdWProjectTree.h \
    windows/SdWProjectList.h \
    windows/SdWEditor.h \
    objects/SdProjectItem.h \
    objects/SdObjectMap.h \
    objects/SdSymbol.h \
    objects/SdPoint.h \
    objects/SdRect.h \
    objects/SdPointList.h \
    objects/SdCircle.h \
    objects/SdAngle.h \
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
    windows/SdWCommand.h \
    objects/SdPulsar.h \
    windows/SdPNewProjectItem_SelectType.h \
    windows/SdPNewProjectItem_EnterName.h \
    objects/SdIds.h \
    windows/SdDGetObject.h \
    objects/SdObjectFactory.h \
    objects/SdObjectInfo.h \
    objects/SdObjectDataBaseRecord.h \
    windows/SdWEditorGraph.h \
    windows/SdWEditorGraphSymbol.h \
    objects/SdScaler.h \
    windows/SdDOptions.h \
    windows/SdDOptionsPageColors.h \
    objects/SdEnvir.h \
    windows/SdWView.h \
    modes/SdMode.h \
    modes/SdModeCommon.h \
    objects/SdLayer.h \
    objects/SdContainer.h \
    objects/SdGraph.h \
    objects/SdGraphLinear.h \
    objects/SdPropLine.h \
    objects/SdPropInt.h \
    objects/SdPropLayer.h \
    objects/SdGraphLinearLine.h \
    objects/SdSelector.h \
    windows/SdWEditorGraphPart.h \
    objects/SdConverter.h \
    objects/SdConverterView.h \
    modes/SdModeTemp.h \
    modes/SdCursorFactory.h \
    modes/SdModeTZoomer.h \
    modes/SdModeIdents.h \
    modes/SdModeTZoomWindow.h

DISTFILES += \
    license.txt

RESOURCES += \
    SaliCADrc.qrc

FORMS += \
    windows/SdDGetObject.ui
