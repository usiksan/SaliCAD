#ifndef SDM3DVALUE_H
#define SDM3DVALUE_H

#include "SdM3dTypes.h"

class SdM3dValue
  {
  public:
    SdM3dValue() {}
    virtual ~SdM3dValue() {}

    virtual float        toFloat() const { return 0.0; }

    virtual int          toInt() const { return 0; }

    virtual QString      toString() const { return QString(); }

    virtual QVector3D    toVertex() const { return QVector3D(); }

    virtual QMatrix4x4   toMatrix() const { return QMatrix4x4(); }

    virtual SdM3dSegment toSegment() const { return SdM3dSegment(); }

    virtual SdM3dPath    toPath() const { return SdM3dPath(); }

    virtual SdM3dFace    toFace() const { return SdM3dFace(); }

    virtual SdM3dFaceSet toFaceSet() const { return SdM3dFaceSet(); }
  };

using SdM3dValuePtr = SdM3dValue*;

#endif // SDM3DVALUE_H
