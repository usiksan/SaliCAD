#ifndef VRMLCOLOR_H
#define VRMLCOLOR_H

#include "VrmlTypes.h"

struct VrmlColor
  {
    VrmlFloat mRed;
    VrmlFloat mGreen;
    VrmlFloat mBlue;
  };

using VrmlColorList = QList<VrmlColor>;

#endif // VRMLCOLOR_H
