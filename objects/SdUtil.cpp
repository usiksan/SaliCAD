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
#include "SdUtil.h"


//Convert textual representation of physical coords to logical int
int SdUtil::phys2log(const QString src, double ppm)
  {
  QString txt = src;
  txt.replace( QChar(','), QString(".") );
  if( ppm == 0 ) ppm = 1.0;
  return txt.toDouble() / ppm;
  }




//Convert logical int coords to textual physical
QString SdUtil::log2physStr(int log, double ppm)
  {

  }


