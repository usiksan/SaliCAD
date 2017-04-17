#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T21:29:34
#
#-------------------------------------------------
CONFIG += c++11

QT       += core gui  network

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
    objects/SdContext.cpp \
    objects/SdQuadrangle.cpp \
    objects/SdUndo.cpp \
    objects/SdSegment.cpp \
    objects/SdSnapInfo.cpp \
    objects/SdPolyWindow.cpp \
    objects/SdPolyWindowList.cpp \
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
    modes/SdModeTZoomWindow.cpp \
    modes/SdModeCLinear.cpp \
    objects/SdPropPartImp.cpp \
    objects/SdPropPartPin.cpp \
    objects/SdPropRoad.cpp \
    objects/SdPropSymImp.cpp \
    objects/SdPropSymPin.cpp \
    objects/SdPropText.cpp \
    windows/SdPropBar.cpp \
    windows/SdPropBarLinear.cpp \
    modes/SdModeCLinearRect.cpp \
    objects/SdGraphLinearRect.cpp \
    objects/SdUndoRecordInsert.cpp \
    modes/SdModeCLinearRectFilled.cpp \
    objects/SdGraphLinearRectFilled.cpp \
    modes/SdModeCLinearLine.cpp \
    objects/SdUndoRecordPropLineAnd2Points.cpp \
    windows/SdPropBarTextual.cpp \
    modes/SdModeCText.cpp \
    objects/SdGraphText.cpp \
    objects/SdConverterText.cpp \
    objects/SdUndoRecordPropTextAndText.cpp \
    modes/SdModeCTextual.cpp \
    windows/SdWLabel.cpp

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
    objects/SdContext.h \
    objects/SdQuadrangle.h \
    objects/SdUndoRecord.h \
    objects/SdUndo.h \
    objects/SdSegment.h \
    objects/SdSnapInfo.h \
    objects/SdPolyWindow.h \
    objects/SdPolyWindowList.h \
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
    modes/SdModeTZoomWindow.h \
    modes/SdModeCLinear.h \
    objects/SdPropText.h \
    objects/SdPropSymPin.h \
    objects/SdPropSymImp.h \
    objects/SdPropRoad.h \
    objects/SdPropPartPin.h \
    objects/SdPropPartImp.h \
    windows/SdPropBar.h \
    windows/SdPropBarLinear.h \
    modes/SdModeCLinearRect.h \
    objects/SdGraphLinearRect.h \
    objects/SdUndoRecordInsert.h \
    objects/SdUndoRecordBegin.h \
    modes/SdModeCLinearRectFilled.h \
    objects/SdGraphLinearRectFilled.h \
    modes/SdModeCLinearLine.h \
    objects/SdUndoRecordPropLineAnd2Points.h \
    windows/SdPropBarTextual.h \
    modes/SdModeCText.h \
    objects/SdGraphText.h \
    objects/SdConverterText.h \
    objects/SdUndoRecordPropTextAndText.h \
    modes/SdModeCTextual.h \
    windows/SdWLabel.h

DISTFILES += \
    license.txt

RESOURCES += \
    SaliCADrc.qrc

FORMS += \
    windows/SdDGetObject.ui
