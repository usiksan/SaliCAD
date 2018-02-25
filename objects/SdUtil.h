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

#include <QString>
#include <QPoint>
#include <QPointF>

#define timeOffsetConstant 1000000000L

class SdUtil
  {
  public:
    //Convert textual representation of physical coords to int
    static int     phys2log( const QString src, double ppm );

    //Convert physical coord to int
    static int     phys2log( double phys, double ppm );

    //Convert physical point to int point
    static QPoint  phys2log( QPointF phys, double ppm );

    //Convert textual representation of physical coord to double phys
    static double  str2phys( const QString src );

    //Convert logical int coords to textual physical
    static QString log2physStr( int log, double ppm );

    //Convert logical int coord to physical
    static double  log2phys( int log, double ppm );

    //Convert logical point to physical
    static QPointF log2phys( QPoint log, double ppm );

    //Get current time from 2000year
    static int     getTime2000();

    //Convert time from 2000year to time from epoch
    static qint64  time2000toEpoch( int time );

    //Test if string contains only english symbols
    static bool    isEnglish( const QString str );

    //Swap int
    static void    swapInt( int &src, int *dst );

    //Swap bool
    static void    swapBool( bool &src, bool *dst );

  };

#endif // SDUTIL_H
