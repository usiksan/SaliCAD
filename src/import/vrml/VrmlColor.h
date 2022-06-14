#ifndef VRMLCOLOR_H
#define VRMLCOLOR_H

#include "VrmlTypes.h"

#include <QColor>

struct VrmlColor
  {
    VrmlFloat mRed;
    VrmlFloat mGreen;
    VrmlFloat mBlue;

    VrmlColor( float red = 0.0, float green = 0.0, float blue = 0.0 ) : mRed(red), mGreen(green), mBlue(blue) {}

    void    parse( SdScanerVrml *scaner );

    bool    isValid() const { return mRed >= 0.0; }

    float   color( int ic ) const { return ic == 0 ? mRed : (ic == 1 ? mGreen : mBlue); }

    quint32 toInt() const;

    QColor  toColor( float mult = 1.0 ) const;
  };

using VrmlColorList = QList<VrmlColor>;

#endif // VRMLCOLOR_H
