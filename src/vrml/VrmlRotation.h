#ifndef VRMLROTATION_H
#define VRMLROTATION_H

#include "VrmlTypes.h"

#include <QVector3D>

struct VrmlRotation
  {
    //The first three values specify a normalized rotation axis vector about which the rotation takes place.
    VrmlFloat mVectorX;
    VrmlFloat mVectorY;
    VrmlFloat mVectorZ;
    //The fourth value specifies the amount of right-handed rotation about that axis in radians.
    VrmlFloat mAngle;

    VrmlRotation( float vectorX = 0.0, float vectorY = 0.0, float vectorZ = 0.0, float angle = 0.0 );

    void      parse( SdScanerVrml *scaner );

    QVector3D vector3d() const { return QVector3D( mVectorX, mVectorY, mVectorZ ); }
  };

#endif // VRMLROTATION_H
