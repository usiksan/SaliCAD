/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical traced object. Traced object is object which represents printed road or metallic hole.
*/
#ifndef SDGRAPHTRACED_H
#define SDGRAPHTRACED_H

#include "SdGraph.h"
#include "SdStratum.h"


class SdPItemPlate;
class SdPlateNetList;

class SdGraphTraced : public SdGraph
  {
  public:
    SdGraphTraced();

    //Information
    SdPItemPlate        *getPlate() const;

    //Set flag to update rat net. Rat net will be clear and reconstructed
    void                 setDirtyRatNet();

    //Defined here
    virtual bool         isPointOnNet( SdPoint p, SdStratum stratum, QString &wireName ) = 0;
    virtual void         accumNetPoints( SdPlateNetList &netList ) = 0;
    //virtual void         accumLinked( SdPoint a, SdPoint b, SdLayer *layer, SdSelector *sel ) = 0;

  };

#endif // SDGRAPHTRACED_H