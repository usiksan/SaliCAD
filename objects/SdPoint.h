/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  2D point object
*/

#ifndef SDPOINT_H
#define SDPOINT_H

#include "SdPropAngle.h"
#include <QJsonObject>
#include <QPoint>

//Line enter type
#define SD_ENTER_ORTHO    0  //All lines orthogonal
#define SD_ENTER_DIAGONAL 1  //Lines with angle 45 degree
#define SD_ENTER_VARIABLE 2  //Lines with variable angle

class SdPoint : public QPoint
  {
  public:
    enum CLS {LEFT,  RIGHT,  BEYOND,  BEHIND, BETWEEN, ORIGIN, DESTINATION};
    //    СЛЕВА, СПРАВА, ВПЕРЕДИ, ПОЗАДИ, МЕЖДУ,   НАЧАЛО, КОНЕЦ

    static const int farCoord = 2000000000;

    SdPoint() : QPoint() { }
    SdPoint( int xpos, int ypos ) : QPoint(xpos,ypos) { }
    SdPoint( QPoint p ) : QPoint(p) {}

    void        operator /= (int i) { rx() /= i; ry() /= i; }
    void        operator *= (int i) { rx() *= i; ry() *= i; }
    void        operator -= (int i) { rx() -= i; ry() -= i; }
    void        operator += (int i) { rx() += i; ry() += i; }
    void        operator += ( SdPoint p ) { rx() += p.x(); ry() += p.y(); }
    void        operator = ( QPoint p ) { setX(p.x()); setY(p.y()); }
    void        set( int cx, int cy ) { setX(cx); setY(cy); }
    void        rotate( SdPoint origin, SdPropAngle angle );
    void        mirror( SdPoint origin );
    void        mirror( SdPoint a, SdPoint b );
    void        move( SdPoint offset ) { setX( x() + offset.x() ); setY( y() + offset.y() ); }
    SdPoint     complement() const { return SdPoint(-x(),-y()); }
    bool        isEmpty() const { return x() == 0 && y() == 0; }
    SdPoint     sub( SdPoint b ) { return SdPoint( x() - b.x(), y() - b.y() ); }
    SdPoint     operator - ( SdPoint b ) { return sub(b); }
    SdPoint     operator + ( SdPoint b ) { return SdPoint( x() + b.x(), y() + b.y() ); }
    SdPoint     getMiddle( SdPoint b ) { return SdPoint( (x() + b.x()) / 2, (y() + b.y()) / 2 ); }
    SdPropAngle getAngle( SdPoint center = SdPoint() ) const; //Угол поворота до точки относительно центра
    double      getAngleDegree(SdPoint center) const;
    QPointF     toPointF() const { return QPointF(x(),y()); }

    //Return distance between two points - this and p
    double      getDistance( SdPoint p ) const;

    //Return distance as int between two points - this and p
    int         getDistanceInt( SdPoint p ) const { return static_cast<int>(getDistance(p)); }

    //Return square distance between two points - this and p
    double      getSquareDistance( SdPoint p ) const;

    CLS         classify( SdPoint p0, SdPoint p1 ) const;
    double      getLenght() const;
    bool        isEqual( SdPoint p ) const { return x() == p.x() && y() == p.y(); }

    //Комплексное преобразование
    SdPoint     convertImplement( SdPoint origin, SdPoint offset, SdPropAngle angle, bool mirror );
    SdPoint     unConvertImplement( SdPoint origin, SdPoint offset, SdPropAngle angle, bool mirror );

    //Проверка условий для точки
    bool        isOnCircle( SdPoint center, int radius, int delta = 0 ) const ;         //На окружности?
    bool        isInsideCircle( SdPoint center, int radius ) const;
    bool        isOnArc( SdPoint center, SdPoint start, SdPoint stop, int delta = 0 ) const; //На дуге?
    bool        isLeftHigh( SdPoint a ) const;                              //Левее или выше чем a?
    bool        isOnSegment( SdPoint a, SdPoint b ) const;

    void        write(const QString name, QJsonObject &obj ) const;
    void        read(const QString name, const QJsonObject obj );

    void        swap( SdPoint *p );

    QJsonObject write() const;
    void        read( const QJsonObject obj );

    //Return far point, point with big coords
    static SdPoint far() { return SdPoint(farCoord,farCoord); }

    //Test if point is far
    bool        isFar() const { return x() == farCoord && y() == farCoord; }
  };



//Interesting points calculation [Вычисление интересных точек]
//Get intermediate point with 45 degree step vertex
//    result  --------+ b
//           /
//          /
//       a +
SdPoint get45( SdPoint a, SdPoint b );

//Get intermediate point with 90 degree step vertex
// result  -----------+ b
//         |
//         |
//       a +
SdPoint get90( SdPoint a, SdPoint b );

//Get intermediate point in according enter type
SdPoint calcMiddlePoint(SdPoint a, SdPoint b, int enterType );

//Calculate stop point of arc from sector point
SdPoint calcArcStop( SdPoint center, SdPoint start, SdPoint sector );

bool    calcFreeNearIntersect( SdPoint sour, SdPoint a, SdPoint b, SdPoint &dest );
SdPropAngle calcDirection90( SdPoint sour, SdPoint dest );
int     sred( int a, int b, int gridSize );
bool    isSegmentYAccross( int y, int x1, int x2, SdPoint p1, SdPoint p2, bool inside, int *result = nullptr );
bool    isSegmentYAccrossA( int y, int x1, int x2, SdPoint p1, SdPoint p2, bool inside );


#endif // SDPOINT_H
