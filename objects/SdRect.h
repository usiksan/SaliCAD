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

struct SdRect
  {
    int x,y,width,height;

    SdRect();
    SdRect();
    SdRect( SdPoint _a, SdPoint _b );
    SdRect( int lx, int ly, int rx, int ry);

    SdPoint leftTop() const { return SdPoint(x,y); }
    SdPoint rightTop() const { return SdPoint(x+width,y); }
    SdPoint leftBottom() const { return SdPoint(x,y+height); }
    SdPoint rightBottom() const { return SdPoint(x+width,y+height); }
    SdPoint center() const { return SdPoint(x+width/2,y+height/2); }
    SdPoint size() const { return SdPoint(width,height); }

    void    set( int lx, int ly, int rx, int ry);
    void    grow( SdPoint point );
    void    grow( const SdRect &rect );
    void    move( SdPoint offset );
    void    mirror( SdPoint origin );
    void    mirror( SdPoint a, SdPoint b );
    void    rotate( SdPoint origin, DAngle angle );

    bool    isPointInside( const SdPoint point ) const;
    bool    isCircleInside( SdCircle circle ) const;
    bool    isRectInside( const SdRect &r ) const;
    bool    isAccross( const SdPoint a, const SdPoint b ) const;
    bool    isAccross( const SdRect &r ) const;
    bool    isAccross( const SdPoint center, const int radius ) const;
    void    calcOverPolygon( SdPointList &polygon );
    void    intersect( const SdRect &sour );

    QJsonObject write() const;
    void        write( const QString name, QJsonObject &obj ) const;
    void        read( const QJsonObject &obj );
    void        read( const QString name, const QJsonObject obj );
  };

#endif // SSdRect_H
