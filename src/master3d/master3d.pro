#QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../scaner/SdScaner.cpp \
        ../scaner/SdScanerMultyline.cpp \
        SdM3dArrayFace.cpp \
        SdM3dArraySegment.cpp \
        SdM3dArrayVertex.cpp \
        SdM3dFloat.cpp \
        SdM3dFunBuildBox.cpp \
        SdM3dFunBuildRect.cpp \
        SdM3dFunBuildVertex.cpp \
        SdM3dFunExtrudeModel.cpp \
        SdM3dFunFaceBuild.cpp \
        SdM3dFunTransferRegion.cpp \
        SdM3dFunction.cpp \
        SdM3dOperatorBlock.cpp \
        SdM3dOperatorIf.cpp \
        SdM3dOperatorWhile.cpp \
        SdM3dParser.cpp \
        SdM3dPartModel.cpp \
        SdM3dProgramm.cpp \
        SdM3dScaner.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  ../scaner/SdScaner.h \
  ../scaner/SdScanerMultyline.h \
  SdM3dArray.h \
  SdM3dArrayFace.h \
  SdM3dArraySegment.h \
  SdM3dArrayVertex.h \
  SdM3dBinary.h \
  SdM3dBinaryBool.h \
  SdM3dBinaryBoolAnd.h \
  SdM3dBinaryBoolFloatLess.h \
  SdM3dBinaryBoolOr.h \
  SdM3dBinaryFloat.h \
  SdM3dBinaryFloatAdd.h \
  SdM3dBinaryFloatDiv.h \
  SdM3dBinaryFloatMult.h \
  SdM3dBinaryFloatSub.h \
  SdM3dFloat.h \
  SdM3dFunBuildBox.h \
  SdM3dFunBuildRect.h \
  SdM3dFunBuildVertex.h \
  SdM3dFunExtrudeModel.h \
  SdM3dFunFaceBuild.h \
  SdM3dFunTransferRegion.h \
  SdM3dFunction.h \
  SdM3dOperator.h \
  SdM3dOperatorAssign.h \
  SdM3dOperatorBlock.h \
  SdM3dOperatorIf.h \
  SdM3dOperatorWhile.h \
  SdM3dParser.h \
  SdM3dPartModel.h \
  SdM3dProgramm.h \
  SdM3dReference.h \
  SdM3dScaner.h \
  SdM3dTypes.h \
  SdM3dUnary.h \
  SdM3dUnaryBoolNot.h \
  SdM3dUnaryFloatMinus.h \
  SdM3dValue.h \
  SdM3dVariable.h \
  SdM3dVariableBool.h \
  SdM3dVariableColor.h \
  SdM3dVariableFace.h \
  SdM3dVariableFloat.h \
  SdM3dVariableGraph.h \
  SdM3dVariableMatrix.h \
  SdM3dVariableModel.h \
  SdM3dVariablePath.h \
  SdM3dVariablePin.h \
  SdM3dVariableRegion.h \
  SdM3dVariableSegment.h \
  SdM3dVariableString.h \
  SdM3dVariableText.h \
  SdM3dVariableVertex.h
