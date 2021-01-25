#ifndef VRMLVECTOR_H
#define VRMLVECTOR_H

#include "VrmlTypes.h"

struct VrmlVector
  {
    VrmlFloat mX;
    VrmlFloat mY;
    VrmlFloat mZ;

    void parse( SdScanerVrml *scaner );
  };

using VrmlVectorList = QList<VrmlVector>;

#endif // VRMLVECTOR_H
