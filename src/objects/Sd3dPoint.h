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
#include <QList>
#include <QVector3D>

class Sd3dPoint : public QVector3D
  {
  public:
    Sd3dPoint() : QVector3D() {}
    Sd3dPoint( const SdPoint &p, int cz = 0 ) { setAsMcm( p.x(), p.y(), cz ); }
    Sd3dPoint( float cx, float cy, float cz ) : QVector3D(cx,cy,cz) {}
    Sd3dPoint( const QVector3D &v ) : QVector3D(v) {}

    //!
    //! \brief projectionXY Return projection of 3d point to XY surface
    //! \return             2d projection of 3d point to XY surface
    //!
    SdPoint     projectionXY() const { return SdPoint(x()*1000000.0,y()*1000000.0); }

    void        setAsMcm( int cx, int cy, int cz );

    void        clear();

//    float       fxmm() const { return static_cast<float>(mX) / 1000.0; }
//    float       fymm() const { return static_cast<float>(mY) / 1000.0; }
//    float       fzmm() const { return static_cast<float>(mZ) / 1000.0; }

//    Sd3dPoint   operator + ( const Sd3dPoint &p ) const { return Sd3dPoint( mX + p.x(), mY + p.y(), mZ + p.z() ); }
    //bool        operator == ( const Sd3dPoint &p ) const { return mX == p.x() && mY == p.y() && mZ == p.z(); }

    void        vertex( QOpenGLFunctions_2_0 *f ) const { f->glVertex3f( x(), y(), z() ); }

    void        normal( QOpenGLFunctions_2_0 *f ) const { f->glNormal3f( x(), y(), z() ); }

    void        write(const QString name, QJsonObject &obj ) const;
    void        read(const QString name, const QJsonObject obj );

    void        swap( Sd3dPoint *p );

    QJsonObject write() const;
    void        read( const QJsonObject obj );

  };

using Sd3dPointList = QList<Sd3dPoint>;

QJsonArray sd3dPointListWrite( const Sd3dPointList &list );

void       sd3dPointListRead( Sd3dPointList &list, const QJsonArray &array );

#endif // SDPOINT3D_H
