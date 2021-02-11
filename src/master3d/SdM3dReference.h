#ifndef SDM3DREFERENCE_H
#define SDM3DREFERENCE_H

#include "SdM3dValue.h"

class SdM3dReference : public SdM3dValue
  {
    SdM3dValue *mReference;
  public:
    SdM3dReference( SdM3dValue *ref ) : SdM3dValue(), mReference(ref) {}



    // SdM3dValue interface
  public:
    virtual char         type() const override      { return mReference->type(); }
    virtual float        toFloat() const override   { return mReference->toFloat(); }
    virtual QColor       toColor() const override   { return mReference->toColor(); }
    virtual QString      toString() const override  { return mReference->toString(); }
    virtual QVector3D    toVertex() const override  { return mReference->toVertex(); }
    virtual QMatrix4x4   toMatrix() const override  { return mReference->toMatrix(); }
    virtual SdM3dSegment toSegment() const override { return mReference->toSegment(); }
    virtual SdM3dPath    toPath() const override    { return mReference->toPath(); }
    virtual SdM3dRegion  toRegion() const override  { return mReference->toRegion(); }
    virtual SdM3dFace    toFace() const override    { return mReference->toFace(); }
    virtual SdM3dModel   toModel() const override   { return mReference->toModel(); }
  };

#endif // SDM3DREFERENCE_H
