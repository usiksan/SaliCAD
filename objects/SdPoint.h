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

struct SdPoint
  {
    int x,y;

    SdPoint() : x(0),y(0) { }
    SdPoint( int _x, int _y ) : x(_x), y(_y) { }
    void    operator /= (int i) { x /= i; y /= i; }
    void    operator *= (int i) { x *= i; y *= i; }
    void    operator -= (int i) { x -= i; y -= i; }
    void    operator += (int i) { x += i; y += i; }
    void    set( int cx, int cy ) { x = cx; y = cy; }
    void    rotate( SdPoint origin, SdAngle angle );
    void    mirror( SdPoint origin );
    void    mirror( SdPoint a, SdPoint b );
    void    grow( SdPoint a, int direction, double factor );
    void    move( SdPoint offset );
    SdPoint complement() const { return SdPoint(-x,-y); }
    bool    operator != (const SdPoint &dest) const { return x != dest.x || y != dest.y; }
    bool    operator == (const SdPoint &dest) const { return x == dest.x && y == dest.y; }
    bool    isEmpty() const { return x == 0 && y == 0; }
    SdPoint operator - ( SdPoint b );
    SdPoint operator + ( SdPoint b );
    SdPoint getMiddle( SdPoint b );
    SdAngle getAngle( SdPoint center = SdPoint() ) const; //Угол поворота до точки относительно центра
    double  getDistance( SdPoint p ) const;

    //Комплексное преобразование
    SdPoint convertImplement( SdPoint origin, SdPoint offset, SdAngle angle, bool mirror );
    SdPoint unConvertImplement( SdPoint origin, SdPoint offset, SdAngle angle, bool mirror );

    //Проверка условий для точки
    bool    isOnCircle( SdPoint center, int radius ) const ;         //На окружности?
    bool    isOnArc( SdPoint center, SdPoint start, SdPoint stop ) const; //На дуге?
    bool    isOnSection( SdPoint a, SdPoint b ) const;                   //На отрезке
    bool    isLeftHigh( SdPoint a ) const;                              //Левее и выше чем?

    void    write(const QString name, QJsonObject &obj );
    void    read(const QString name, const QJsonObject obj );

    QJsonObject write() const;
    void        read( const QJsonObject obj );
  };

#endif // SDPOINT_H
