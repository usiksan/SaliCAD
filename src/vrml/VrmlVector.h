#ifndef VRMLVECTOR_H
#define VRMLVECTOR_H

#include "VrmlTypes.h"

#include <QVector3D>

struct VrmlVector
  {
    VrmlFloat mX;
    VrmlFloat mY;
    VrmlFloat mZ;

    VrmlVector( float mx = 0.0, float my = 0.0, float mz = 0.0 ) : mX(mx), mY(my), mZ(mz) {}

    void parse( SdScanerVrml *scaner );

    QVector3D toVector3d() const { return QVector3D( mX, mY, mZ ); }
  };

using VrmlVectorList = QList<VrmlVector>;

using QVector3DList = QList<QVector3D>;

#endif // VRMLVECTOR_H
