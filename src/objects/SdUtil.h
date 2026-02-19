/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Some utils
*/
#ifndef SDUTIL_H
#define SDUTIL_H

#include "SdPoint.h"

#include <QString>
#include <QPoint>
#include <QPointF>
#include <QPolygonF>


class SdUtil
  {
  public:
    inline static const double EPSILON = 1e-10;

    //Extract value from string
    static double    extractValueFromString( const QString str, int &start );

    //Extract logical value from string
    static int       extractLogFromString( const QString str, int &start, double ppm );

    //Convert textual representation of physical coords to int
    static int       phys2log( const QString src, double ppm );

    //Convert physical coord to int
    static int       phys2log( double phys, double ppm );

    //Convert physical point to int point
    static QPoint    phys2log( QPointF phys, double ppm );

    //Convert textual representation of physical coord to double phys
    static double    str2phys( const QString src );

    //Convert logical int coords to textual physical
    static QString   log2physStr( int log, double ppm );

    //Convert logical int coord to physical
    static double    log2phys( int log, double ppm );

    //Convert logical point to physical
    static QPointF   log2phys( QPoint log, double ppm );

    //Convert logical um int coord to mm minimum string
    static QString   um2mm(int log );

    //Test if string contains only english symbols
    static bool      isEnglish( const QString str );

    template <typename Type>
    static void      swap( Type &src, Type *dst )
      {
      if( dst != nullptr ) {
        Type tmp(*dst);
        *dst = src;
        src = tmp;
        }
      }

    //Create octagon over circle
    static QPolygonF octagon( int centerx, int centery, int radius );

    //!
    //! \brief polygonOverCircle Creates regular polygon circumscribed around circle
    //! \param centerx           Center X coordinate
    //! \param centery           Center Y coordinate
    //! \param radius            Circle radius
    //! \param sides             Number of polygon sides (minimum 3)
    //! \return                  Polygon points in clockwise order
    //!
    static QPolygonF polygonOverCircle(int centerx, int centery, int radius, int sides);


    //!
    //! \brief roundrect Creates polygon for rectangle with rounded corners
    //! \param r         Rectangle in integer coordinates (SdPoint system: Y up)
    //! \param ratio     Rounding ratio in internal units (same as radius)
    //! \param sides     Total number of points in polygon (should be multiple of 4)
    //!                  Minimum 4 (just rectangle), recommended 16-32 for smooth corners
    //! \return          Polygon approximating rounded rectangle with specified total points
    //!                  Points are in SdPoint coordinate system (Y up)
    //!
    static QPolygonF roundrect(QRect r, int ratio, int sides);


    //!
    //! \brief ellipse Creates polygon approximation of ellipse
    //! \param r       Rectangle defining ellipse bounds
    //! \param sides   Total number of points in polygon (minimum 4, recommended 16-32)
    //! \return        Polygon approximating the ellipse with specified number of points
    //!
    static QPolygonF ellipse(QRect r, int sides);

    // Нормализация вектора
    static QPointF   normalize(const QPointF& v);

    // Получение нормали к ребру
    static QPointF   getEdgeNormal(const QPointF& p1, const QPointF& p2);

    // Определение ориентации полигона
    static bool      isCounterClockwise(const QPolygon &polygon);

    // Проверка, лежит ли точка между двумя другими на прямой
    static bool      isPointBetween(const QPointF& p, const QPointF& a, const QPointF& b);

    // Вычисление угла между векторами в градусах
    static double    angleBetween(const QPointF& v1, const QPointF& v2);

    static QPolygonF equidistant( const QPolygon &src, double gap );

    //Value limitation
    static int       iLimit( int val, int vmin, int vmax );

    // Поворот вектора на 90 градусов против часовой стрелки
    inline static QPointF rotate90(const QPointF& v) { return QPointF(-v.y(), v.x()); }

    // Сравнение чисел с плавающей точкой
    inline static bool    isZero(double val) { return qAbs(val) < EPSILON; }

    inline static bool    isEqual(double a, double b) { return isZero(a - b); }

    inline static bool    isEqual(const QPointF& p1, const QPointF& p2) { return isZero(p1.x() - p2.x()) && isZero(p1.y() - p2.y()); }
    };

#endif // SDUTIL_H
