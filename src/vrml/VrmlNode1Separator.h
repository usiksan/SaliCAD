#ifndef VRMLNODE1SEPARATOR_H
#define VRMLNODE1SEPARATOR_H

#include "VrmlNode1.h"

class VrmlNode1Separator : public VrmlNode1
  {
    VrmlNode *mCoordinate3;
    VrmlNode *mIndexedFaceSet;
    VrmlNode *mMaterial;
    VrmlNode *mMaterialBinding;
    VrmlNode *mMatrixTransform;
    VrmlNode *mNormal;
    VrmlNode *mNormalBinding;
    VrmlNode *mRotation;
    VrmlNode *mScale;
    VrmlNode *mTransform;
    VrmlNode *mTranslation;
  public:
    VrmlNode1Separator();
    ~VrmlNode1Separator();

    // VrmlNode interface
  public:
    virtual void generateFaces(std::function<void (const QVector3DList &, const QVector3DList &, const VrmlNodeMaterial *)> appendFace) const override;
    virtual bool parse(SdScanerVrml *scaner, const QString &fieldType) override;
  };

#endif // VRMLNODE1SEPARATOR_H
