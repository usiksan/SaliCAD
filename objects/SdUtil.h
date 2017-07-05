#ifndef SDUTIL_H
#define SDUTIL_H

#include <QString>

class SdUtil
  {
  public:
    //Convert textual representation of physical coords to int
    static int coord2int( const QString src, double ppm );
  };

#endif // SDUTIL_H
