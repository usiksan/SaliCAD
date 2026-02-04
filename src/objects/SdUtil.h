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

    //Value limitation
    static int       iLimit( int val, int vmin, int vmax );
  };

#endif // SDUTIL_H
