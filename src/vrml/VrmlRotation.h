#ifndef VRMLROTATION_H
#define VRMLROTATION_H

#include "VrmlTypes.h"

struct VrmlRotation
  {
    //The first three values specify a normalized rotation axis vector about which the rotation takes place.
    VrmlFloat mVectorX;
    VrmlFloat mVectorY;
    VrmlFloat mVectorZ;
    //The fourth value specifies the amount of right-handed rotation about that axis in radians.
    VrmlFloat mAngle;

    VrmlRotation( float vectorX = 0.0, float vectorY = 0.0, float vectorZ = 0.0, float angle = 0.0 );

    void parse( SdScanerVrml *scaner );
  };

#endif // VRMLROTATION_H
