/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SSdRect_H
#define SSdRect_H

#include "SdPoint.h"
#include "SdPointList.h"
#include <QRect>

class SdCircle;

class SdRect : public QRect
  {
  public:
    SdRect() : QRect() {}
    SdRect( SdPoint a, SdPoint b );
    SdRect( int px, int py, int w, int h);

    void        set( SdPoint a, SdPoint b );
    void        set( QRect r );
    void        grow( SdPoint point );
    void        grow( const SdRect &rect );
    void        move( SdPoint offset ) { translate( offset ); }
    void        mirror( SdPoint origin );
    void        rotate( SdPoint origin, SdAngle angle );

    bool        isPointInside( const SdPoint point ) const { return contains( point ); }
    bool        isRectInside( const SdRect &r ) const { return contains( r ); }
    bool        isCircleInside( SdCircle circle ) const;
    bool        isAccross(const SdPoint p1, const SdPoint p2 ) const;
    bool        isAccross( const SdRect &r ) const { return intersects( r ); }
    void        calcOverPolygon( SdPointList &polygon );
    void        intersect( const SdRect &sour ) { set( intersected(sour) ); }

    QJsonObject write() const;
    void        write( const QString name, QJsonObject &obj ) const;
    void        read( const QJsonObject &obj );
    void        read( const QString name, const QJsonObject obj );

    SdRect      swap( const SdRect &r );
  };

#endif // SSdRect_H
