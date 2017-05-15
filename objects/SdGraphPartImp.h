/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#ifndef SDGRAPHPARTIMP_H
#define SDGRAPHPARTIMP_H

#include "SdGraph.h"

class SdPItemPlate;

class SdGraphPartImp : public SdGraph
  {
  public:
    SdGraphPartImp();

    void pinConnectionSet( const QString pinNumber, const QString wireName, bool com );

    SdPItemPlate *getPlate() const;
  };

#endif // SDGRAPHPARTIMP_H
