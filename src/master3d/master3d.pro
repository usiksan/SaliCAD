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
        SdM3dBuildSegment.cpp \
        SdM3dFloat.cpp \
        SdM3dFloatSumm.cpp \
        SdM3dFunBuildBox.cpp \
        SdM3dFunBuildRect.cpp \
        SdM3dFunBuildVertex.cpp \
        SdM3dFunExtrudeModel.cpp \
        SdM3dFunTransferRegion.cpp \
        SdM3dFunction.cpp \
        SdM3dOperation.cpp \
        SdM3dOperator.cpp \
        SdM3dOperatorAssign.cpp \
        SdM3dOperatorBlock.cpp \
        SdM3dParser.cpp \
        SdM3dReference.cpp \
        SdM3dScaner.cpp \
        SdM3dSumm.cpp \
        SdM3dUnary.cpp \
        SdM3dUnaryFloatMinus.cpp \
        SdM3dValue.cpp \
        SdM3dVariable.cpp \
        SdM3dVariableFloat.cpp \
        SdM3dVariableString.cpp \
        SdM3dVariableVertex.cpp \
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
  SdM3dBinaryFloatAdd.h \
  SdM3dBinaryFloatDiv.h \
  SdM3dBinaryFloatMult.h \
  SdM3dBinaryFloatSub.h \
  SdM3dBuildSegment.h \
  SdM3dFloat.h \
  SdM3dFloatSumm.h \
  SdM3dFunBuildBox.h \
  SdM3dFunBuildRect.h \
  SdM3dFunBuildVertex.h \
  SdM3dFunExtrudeModel.h \
  SdM3dFunTransferRegion.h \
  SdM3dFunction.h \
  SdM3dOperation.h \
  SdM3dOperator.h \
  SdM3dOperatorAssign.h \
  SdM3dOperatorBlock.h \
  SdM3dParser.h \
  SdM3dReference.h \
  SdM3dScaner.h \
  SdM3dSumm.h \
  SdM3dTypes.h \
  SdM3dUnary.h \
  SdM3dUnaryFloatMinus.h \
  SdM3dValue.h \
  SdM3dVariable.h \
  SdM3dVariableFloat.h \
  SdM3dVariableString.h \
  SdM3dVariableVertex.h
