QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Sd3dReaderVrml.cpp \
        Sd3dVrmlNode.cpp \
        SdScaner.cpp \
        SdScanerMultyline.cpp \
        VrmlColor.cpp \
        VrmlMFColor.cpp \
        VrmlNode.cpp \
        VrmlNodeApperance.cpp \
        VrmlNodeColor.cpp \
        VrmlNodeCompound.cpp \
        VrmlNodeGroup.cpp \
        VrmlNodeIndexedFaceSet.cpp \
        VrmlNodeMaterial.cpp \
        VrmlNodeShape.cpp \
        VrmlNodeTransform.cpp \
        VrmlRotation.cpp \
        VrmlVector.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  Sd3dReaderVrml.h \
  Sd3dVrmlNode.h \
  SdScaner.h \
  SdScanerMultyline.h \
  VrmlColor.h \
  VrmlMFColor.h \
  VrmlNode.h \
  VrmlNodeApperance.h \
  VrmlNodeColor.h \
  VrmlNodeCompound.h \
  VrmlNodeGroup.h \
  VrmlNodeIndexedFaceSet.h \
  VrmlNodeMaterial.h \
  VrmlNodeShape.h \
  VrmlNodeTransform.h \
  VrmlRotation.h \
  VrmlTypes.h \
  VrmlVector.h
