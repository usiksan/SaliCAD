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
#ifndef SDBARRIER_H
#define SDBARRIER_H

#include <QString>
#include <QPolygonF>
#include <QList>

struct SdBarrier
  {

    QString   mNetName; //Name of net which have this barrier
    QPolygonF mPolygon; //Barrier polygon
  };

typedef QList<SdBarrier> SdBarrierList;

#endif // SDBARRIER_H
