#ifndef VRMLVECTOR_H
#define VRMLVECTOR_H

#include "VrmlTypes.h"

struct VrmlVector
  {
    VrmlFloat mX;
    VrmlFloat mY;
    VrmlFloat mZ;

    VrmlVector( float mx = 0.0, float my = 0.0, float mz = 0.0 ) : mX(mx), mY(my), mZ(mz) {}

    void parse( SdScanerVrml *scaner );
  };

using VrmlVectorList = QList<VrmlVector>;


#endif // VRMLVECTOR_H
