/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#ifndef SDPOINT_H
#define SDPOINT_H

#include "SdAngle.h"
#include <QJsonObject>
#include <QPoint>

//Line enter type
#define SD_ENTER_ORTHO    0  //All lines orthogonal
#define SD_ENTER_DIAGONAL 1  //Lines with angle 45 degree
#define SD_ENTER_VARIABLE 2  //Lines with variable angle

class SdPoint : public QPoint
  {
  public:
    SdPoint() : QPoint() { }
    SdPoint( int xpos, int ypos ) : QPoint(xpos,ypos) { }
    SdPoint( QPoint p ) : QPoint(p) {}
    void    operator /= (int i) { rx() /= i; ry() /= i; }
    void    operator *= (int i) { rx() *= i; ry() *= i; }
    void    operator -= (int i) { rx() -= i; ry() -= i; }
    void    operator += (int i) { rx() += i; ry() += i; }
    void    operator += ( SdPoint p ) { rx() += p.x(); ry() += p.y(); }
    void    operator = ( QPoint p ) { setX(p.x()); setY(p.y()); }
    void    set( int cx, int cy ) { setX(cx); setY(cy); }
    void    rotate( SdPoint origin, SdAngle angle );
    void    mirror( SdPoint origin );
    void    mirror( SdPoint a, SdPoint b );
    void    move( SdPoint offset ) { setX( x() + offset.x() ); setY( y() + offset.y() ); }
    SdPoint complement() const { return SdPoint(-x(),-y()); }
    bool    isEmpty() const { return x() == 0 && y() == 0; }
    SdPoint operator - ( SdPoint b ) { return SdPoint( x() - b.x(), y() - b.y() ); }
    SdPoint operator + ( SdPoint b ) { return SdPoint( x() + b.x(), y() + b.y() ); }
    SdPoint getMiddle( SdPoint b ) { return SdPoint( (x() + b.x()) / 2, (y() + b.y()) ); }
    SdAngle getAngle( SdPoint center = SdPoint() ) const; //Угол поворота до точки относительно центра
    double  getDistance( SdPoint p ) const;

    //Комплексное преобразование
    SdPoint convertImplement( SdPoint origin, SdPoint offset, SdAngle angle, bool mirror );
    SdPoint unConvertImplement( SdPoint origin, SdPoint offset, SdAngle angle, bool mirror );

    //Проверка условий для точки
    bool    isOnCircle( SdPoint center, int radius, int delta = 0 ) const ;         //На окружности?
    bool    isOnArc( SdPoint center, SdPoint start, SdPoint stop, int delta = 0 ) const; //На дуге?
    bool    isLeftHigh( SdPoint a ) const;                              //Левее или выше чем a?

    void    write(const QString name, QJsonObject &obj ) const;
    void    read(const QString name, const QJsonObject obj );

    QJsonObject write() const;
    void        read( const QJsonObject obj );
  };

//Вычисление интересных точек
SdPoint calcMiddlePoint( SdPoint a, SdPoint b, int enter );
SdPoint calcArcStop( SdPoint center, SdPoint start, SdPoint sector );
bool    calcFreeNearIntersect( SdPoint sour, SdPoint a, SdPoint b, SdPoint &dest );
SdAngle calcDirection90( SdPoint sour, SdPoint dest );
int     sred( int a, int b, int gridSize );
bool    isSegmentYAccross( int y, int x1, int x2, SdPoint p1, SdPoint p2, bool inside, int *result = 0 );
bool    isSegmentYAccrossA( int y, int x1, int x2, SdPoint p1, SdPoint p2, bool inside );


#endif // SDPOINT_H
