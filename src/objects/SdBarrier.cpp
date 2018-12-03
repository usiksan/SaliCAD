/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Barrier formed by tracing object with assiciated net name
*/

#include "SdBarrier.h"


//Check if point p is inside any barrier and return true or false if not
bool sdIsBarrierListContains(const SdBarrierList &list, QString netName, SdPoint p)
  {
  QPointF pf(p.toPointF());
  for( const SdBarrier &bar : list )
    if( bar.mNetName != netName && bar.mPolygon.containsPoint(pf, Qt::OddEvenFill) )
      return true;
  return false;
  }
