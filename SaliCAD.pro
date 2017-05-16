#-------------------------------------------------
#
# Project created by QtCreator 2016-10-06T21:29:34
#
#-------------------------------------------------
CONFIG += c++11

QT       += core gui  network sql

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
    windows/SdDGetObject.cpp \
    objects/SdObjectFactory.cpp \
    objects/SdObjectInfo.cpp \
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
    windows/SdWLabel.cpp \
    objects/SdGraphLinearCircle.cpp \
    objects/SdUndoRecordPropLinePointInt.cpp \
    objects/SdGraphLinearCircleFilled.cpp \
    objects/SdGraphLinearRegion.cpp \
    objects/SdUndoRecordPropLineAndPointsList.cpp \
    objects/SdGraphSymPin.cpp \
    objects/SdGraphPartPin.cpp \
    objects/SdGraphSymImp.cpp \
    objects/SdGraphPartImp.cpp \
    objects/SdPItemPart.cpp \
    objects/SdPItemSymbol.cpp \
    objects/SdPItemComponent.cpp \
    objects/SdSection.cpp \
    objects/SdPropString.cpp \
    objects/SdParamTable.cpp \
    modes/SdModeView.cpp \
    objects/SdPartVariant.cpp \
    objects/SdUndoRecordPropSymPin.cpp \
    modes/SdModeCSymPin.cpp \
    windows/SdPropBarSymPin.cpp \
    modes/SdModeCOrigin.cpp \
    objects/SdUndoRecordPoint.cpp \
    windows/SdWEditorComponent.cpp \
    objects/SdPItemSheet.cpp \
    objects/SdPItemPlate.cpp \
    windows/SdWSection.cpp \
    objects/SdUndoRecordPropPartPin.cpp \
    modes/SdModeCPartPin.cpp \
    windows/SdPropBarPartPin.cpp \
    modes/SdModeCOriginPart.cpp \
    windows/SdWEditorGraphSheet.cpp \
    windows/SdWEditorGraphPlate.cpp \
    modes/SdModeCSymImp.cpp \
    objects/SdConverterImplement.cpp \
    windows/SdPropBarSymImp.cpp \
    objects/SdGraphWiring.cpp \
    objects/SdGraphWiringWire.cpp \
    objects/SdContainerSheetNet.cpp

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
    windows/SdDGetObject.h \
    objects/SdObjectFactory.h \
    objects/SdObjectInfo.h \
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
    windows/SdWLabel.h \
    objects/SdGraphLinearCircle.h \
    objects/SdUndoRecordPropLinePointInt.h \
    objects/SdGraphLinearCircleFilled.h \
    objects/SdGraphLinearRegion.h \
    objects/SdUndoRecordPropLineAndPointsList.h \
    objects/SdGraphSymPin.h \
    objects/SdGraphPartPin.h \
    objects/SdGraphSymImp.h \
    objects/SdGraphPartImp.h \
    objects/SdPItemPart.h \
    objects/SdPItemSymbol.h \
    objects/SdPItemComponent.h \
    objects/SdSection.h \
    objects/SdPropString.h \
    objects/SdParamTable.h \
    modes/SdModeView.h \
    objects/SdPartVariant.h \
    objects/SdUndoRecordPropSymPin.h \
    modes/SdModeCSymPin.h \
    windows/SdPropBarSymPin.h \
    modes/SdModeCOrigin.h \
    objects/SdUndoRecordPoint.h \
    windows/SdWEditorComponent.h \
    objects/SdPItemSheet.h \
    objects/SdPItemPlate.h \
    windows/SdWSection.h \
    objects/SdUndoRecordPropPartPin.h \
    modes/SdModeCPartPin.h \
    windows/SdPropBarPartPin.h \
    modes/SdModeCOriginPart.h \
    windows/SdWEditorGraphSheet.h \
    windows/SdWEditorGraphPlate.h \
    modes/SdModeCSymImp.h \
    objects/SdConverterImplement.h \
    windows/SdPropBarSymImp.h \
    objects/SdGraphWiring.h \
    objects/SdGraphWiringWire.h \
    objects/SdContainerSheetNet.h

DISTFILES += \
    license.txt

RESOURCES += \
    SaliCADrc.qrc

FORMS += \
    windows/SdDGetObject.ui
