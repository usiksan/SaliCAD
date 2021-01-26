#ifndef VRMLCOLOR_H
#define VRMLCOLOR_H

#include "VrmlTypes.h"

struct VrmlColor
  {
    VrmlFloat mRed;
    VrmlFloat mGreen;
    VrmlFloat mBlue;

    VrmlColor( float red = 0.0, float green = 0.0, float blue = 0.0 ) : mRed(red), mGreen(green), mBlue(blue) {}

    void parse( SdScanerVrml *scaner );
  };

using VrmlColorList = QList<VrmlColor>;

#endif // VRMLCOLOR_H
