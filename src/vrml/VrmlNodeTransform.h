#ifndef VRMLNODETRANSFORM_H
#define VRMLNODETRANSFORM_H

#include "VrmlVector.h"
#include "VrmlRotation.h"
#include "VrmlNodeGroup.h"


class VrmlNodeTransform : public VrmlNodeGroup
  {
    VrmlVector   mCenter;
    VrmlRotation mRotation;
    VrmlVector   mScale;
    VrmlRotation mScaleOrientation;
    VrmlVector   mTranslation;
  public:
    VrmlNodeTransform();
    VrmlNodeTransform( const VrmlNodeTransform *transform );

    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeTransform( this ); }
    virtual bool      parse(SdScanerVrml *scaner, const QString &fieldType) override;
    virtual void      generateFaces(std::function<void (const QVector3DList &, const QVector3DList &, const VrmlNodeMaterial *)> appendFace) const override;
  };

#endif // VRMLNODETRANSFORM_H