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

    void cloneNodeTransform( VrmlNodeTransform *destNode ) const;
    // VrmlNode interface
  public:
    virtual VrmlNode *copy() const override { return new VrmlNodeTransform( this ); }
    virtual void      parse(SdScanerVrml *scaner) override;
  };

#endif // VRMLNODETRANSFORM_H
