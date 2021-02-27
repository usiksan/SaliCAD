#ifndef SDM3DVALUE_H
#define SDM3DVALUE_H

#include "SdM3dTypes.h"

class SdM3dValue
  {
  public:
    SdM3dValue() {}
    virtual ~SdM3dValue() {}

    virtual char         type() const { return SDM3D_TYPE_NONE; }


    virtual bool         toBool() const { return false; }

    virtual float        toFloat() const { return 0.0; }

    virtual QColor       toColor() const { return QColor(); }

    virtual QString      toString() const { return QString(); }


    virtual SdM3dFlat    toFlat() const { return SdM3dFlat(); }


    virtual QVector3D    toVertex() const { return QVector3D(); }

    virtual QMatrix4x4   toMatrix() const { return QMatrix4x4(); }

    virtual SdM3dSegment toSegment() const { return SdM3dSegment(); }

    virtual SdM3dPath    toPath() const { return SdM3dPath(); }

    virtual SdM3dRegion  toRegion() const { return SdM3dRegion(); }

    virtual SdM3dFace    toFace() const { return SdM3dFace(); }

    virtual SdM3dModel   toModel() const { return SdM3dModel(); }
  };

using SdM3dValuePtr = SdM3dValue*;

using SdM3dValuePtrList = QList<SdM3dValuePtr>;

#endif // SDM3DVALUE_H
