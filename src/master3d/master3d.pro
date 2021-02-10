#QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        SdM3dBinary.cpp \
        SdM3dBuildSegment.cpp \
        SdM3dOperation.cpp \
        SdM3dOperator.cpp \
        SdM3dSumm.cpp \
        SdM3dUnary.cpp \
        SdM3dValue.cpp \
        SdM3dValueFloat.cpp \
        SdM3dVariable.cpp \
        SdM3dVariableFloat.cpp \
        SdM3dVariableInt.cpp \
        SdM3dVariableString.cpp \
        SdM3dVariableVertex.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  SdM3dBinary.h \
  SdM3dBuildSegment.h \
  SdM3dOperation.h \
  SdM3dOperator.h \
  SdM3dSumm.h \
  SdM3dTypes.h \
  SdM3dUnary.h \
  SdM3dValue.h \
  SdM3dValueFloat.h \
  SdM3dVariable.h \
  SdM3dVariableFloat.h \
  SdM3dVariableInt.h \
  SdM3dVariableString.h \
  SdM3dVariableVertex.h
