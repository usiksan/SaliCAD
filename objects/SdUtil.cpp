#include "SdUtil.h"


//Convert textual representation of physical coords to int
int SdUtil::coord2int(const QString src, double ppm)
  {
  QString txt = src;
  txt.replace( QChar(','), QString(".") );
  if( ppm == 0 ) ppm = 1.0;
  return txt.toDouble() / ppm;
  }
