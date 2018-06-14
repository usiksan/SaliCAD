/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rect by origin point, width and height

  Because Y axiz point to down, then "top" in fact is bottom and wise wersa

  Because right() return (left pos) + width - 1, then replace all functions on correct (left pos) + width
*/

#ifndef SSdRect_H
#define SSdRect_H

#include "SdPoint.h"
#include "SdPointList.h"
#include <QRect>
#include <QList>

class SdCircle;

class SdRect : public QRect
  {
  public:
    SdRect() : QRect() {}
    SdRect( const QRect &r ) : QRect( r ) {}
    //Construct rect by two diagonal corners
    SdRect( SdPoint a, SdPoint b );
    //Construct rect by leftTop corner and width and height
    SdRect( int px, int py, int w, int h);

    //Because Y axiz point to down, then "top" in fact is bottom and wise wersa
    int         getTop() const { return top() + height(); }
    int         getBottom() const { return top(); }
    int         getLeft() const { return left(); }
    int         getRight() const { return left() + width(); }

    //Because right() return (left pos) + width - 1, then replace all functions on correct (left pos) + width
    SdPoint     getTopLeft() const { return SdPoint( left(), top() + height() ); }
    SdPoint     getTopRight() const { return SdPoint( left() + width(), top() + height() ); }
    SdPoint     getBottomLeft() const { return SdPoint( left(), top() ); }
    SdPoint     getBottomRight() const { return SdPoint( left() + width(), top() ); }

    void        set( SdPoint a, SdPoint b );
    void        set( QRect r );
    void        grow( SdPoint point );
    void        grow( const SdRect &rect );
    void        move( SdPoint offset ) { translate( offset ); }
    void        mirror( SdPoint origin );
    void        rotate( SdPoint origin, SdPropAngle angle );

    bool        isPointInside( const SdPoint point ) const;
    bool        isRectInside( const SdRect &r ) const { return contains( r ); }
    bool        isCircleInside( SdCircle circle ) const;
    bool        isAccross(const SdPoint p1, const SdPoint p2 ) const;
    bool        isAccross( const SdRect &r ) const;
    void        calcOverPolygon( SdPointList &polygon );
    void        intersect( const SdRect &sour ) { set( intersected(sour) ); }

    QJsonObject write() const;
    void        write( const QString name, QJsonObject &obj ) const;
    void        read( const QJsonObject &obj );
    void        read( const QString name, const QJsonObject obj );

    SdRect      swap( const SdRect &r );
  };


//List of rectangles
typedef QList<SdRect> SdRectList;

#endif // SSdRect_H
