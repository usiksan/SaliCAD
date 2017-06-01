/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graph objects triting as connection point
*/
#ifndef SDGRAPHWIRING_H
#define SDGRAPHWIRING_H

#include "SdGraph.h"

class SdGraphWiring : public SdGraph
  {
  public:
    SdGraphWiring();

    virtual bool getNetOnPoint( SdPoint p, QString &destName ) = 0;

  };

#endif // SDGRAPHWIRING_H
