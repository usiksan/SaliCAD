/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3D point object
*/
#ifndef SDPOINT3D_H
#define SDPOINT3D_H

#include "SdPoint.h"

#include <QOpenGLFunctions_2_0>

class Sd3dPoint
  {
    int mX, mY, mZ; //3d coordiantes
  public:
    Sd3dPoint() : mX(0), mY(0), mZ(0) {}
    Sd3dPoint( const SdPoint &p, int cz = 0 ) : mX(p.x()), mY(p.y()), mZ(cz) {}
    Sd3dPoint( int cx, int cy, int cz ) : mX(cx), mY(cy), mZ(cz) {}

    void        set( int cx, int cy, int cz ) { mX = cx; mY = cy; mZ = cz; }

    void        clear() { mX = mY = mZ = 0; }

    int         x() const { return mX; }
    int         y() const { return mY; }
    int         z() const { return mZ; }

    float       fxmm() const { return static_cast<float>(mX) / 1000.0; }
    float       fymm() const { return static_cast<float>(mY) / 1000.0; }
    float       fzmm() const { return static_cast<float>(mZ) / 1000.0; }

    Sd3dPoint   operator + ( const Sd3dPoint &p ) const { return Sd3dPoint( mX + p.x(), mY + p.y(), mZ + p.z() ); }
    bool        operator == ( const Sd3dPoint &p ) const { return mX == p.x() && mY == p.y() && mZ == p.z(); }

    void        vertex( QOpenGLFunctions_2_0 *f ) const { f->glVertex3i( mX, mY, mZ ); }

    void        write(const QString name, QJsonObject &obj ) const;
    void        read(const QString name, const QJsonObject obj );

    void        swap( Sd3dPoint *p );

    QJsonObject write() const;
    void        read( const QJsonObject obj );

  };

#endif // SDPOINT3D_H
