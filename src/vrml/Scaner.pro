#QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../scaner

SOURCES += \
        ../scaner/SdScaner.cpp \
        ../scaner/SdScanerMultyline.cpp \
        SdScanerVrml.cpp \
        VrmlColor.cpp \
        VrmlNode.cpp \
        VrmlNode1.cpp \
        VrmlNode1Coordinate3.cpp \
        VrmlNode1IndexedFaceSet.cpp \
        VrmlNode1Material.cpp \
        VrmlNode1MaterialBinding.cpp \
        VrmlNode1MatrixTransform.cpp \
        VrmlNode1Normal.cpp \
        VrmlNode1NormalBinding.cpp \
        VrmlNode1Rotation.cpp \
        VrmlNode1Scale.cpp \
        VrmlNode1Separator.cpp \
        VrmlNode1Transform.cpp \
        VrmlNode1Translation.cpp \
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
        VrmlVector.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  ../scaner/SdScaner.h \
  ../scaner/SdScanerMultyline.h \
  SdScanerVrml.h \
  VrmlColor.h \
  VrmlNode.h \
  VrmlNode1.h \
  VrmlNode1Coordinate3.h \
  VrmlNode1IndexedFaceSet.h \
  VrmlNode1Material.h \
  VrmlNode1MaterialBinding.h \
  VrmlNode1MatrixTransform.h \
  VrmlNode1Normal.h \
  VrmlNode1NormalBinding.h \
  VrmlNode1Rotation.h \
  VrmlNode1Scale.h \
  VrmlNode1Separator.h \
  VrmlNode1Transform.h \
  VrmlNode1Translation.h \
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
  VrmlTypes.h \
  VrmlVector.h
