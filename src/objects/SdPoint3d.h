#ifndef SDPOINT3D_H
#define SDPOINT3D_H

#include "SdPoint.h"

#include <QOpenGLFunctions_2_0>

class SdPoint3d
  {
    int mX, mY, mZ; //3d coordiantes
  public:
    SdPoint3d() : mX(0), mY(0), mZ(0) {}
    SdPoint3d( const SdPoint &p, int cz = 0 ) : mX(p.x()), mY(p.y()), mZ(cz) {}
    SdPoint3d( int cx, int cy, int cz ) : mX(cx), mY(cy), mZ(cz) {}

    int x() const { return mX; }
    int y() const { return mY; }
    int z() const { return mZ; }

    void vertex( QOpenGLFunctions_2_0 *f ) { f->glVertex3i( mX, mY, mZ ); }
  };

#endif // SDPOINT3D_H
