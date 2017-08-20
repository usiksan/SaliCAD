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

class SdUtil
  {
  public:
    //Convert textual representation of physical coords to int
    static int     phys2log( const QString src, double ppm );

    //Convert logical int coords to textual physical
    static QString log2physStr( int log, double ppm );
  };

#endif // SDUTIL_H
