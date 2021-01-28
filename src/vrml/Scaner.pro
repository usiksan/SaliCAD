#QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        SdScaner.cpp \
        SdScanerMultyline.cpp \
        SdScanerVrml.cpp \
        VrmlColor.cpp \
        VrmlNode.cpp \
        VrmlNodeAppearance.cpp \
        VrmlNodeColor.cpp \
        VrmlNodeCompound.cpp \
        VrmlNodeCoordinate.cpp \
        VrmlNodeGroup.cpp \
        VrmlNodeIndexedFaceSet.cpp \
        VrmlNodeMaterial.cpp \
        VrmlNodeNormal.cpp \
        VrmlNodeShape.cpp \
        VrmlNodeTransform.cpp \
        VrmlRotation.cpp \
        VrmlSmartNodePtr.cpp \
        VrmlVector.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  SdScaner.h \
  SdScanerMultyline.h \
  SdScanerVrml.h \
  VrmlColor.h \
  VrmlNode.h \
  VrmlNodeAppearance.h \
  VrmlNodeColor.h \
  VrmlNodeCompound.h \
  VrmlNodeCoordinate.h \
  VrmlNodeGroup.h \
  VrmlNodeIndexedFaceSet.h \
  VrmlNodeMaterial.h \
  VrmlNodeNormal.h \
  VrmlNodeShape.h \
  VrmlNodeTransform.h \
  VrmlRotation.h \
  VrmlSmartNodePtr.h \
  VrmlTypes.h \
  VrmlVector.h
