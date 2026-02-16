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
#include "SdPropStratum.h"
#include "SdRuleBlock.h"
#include "SdBarrier.h"
#include "SdPolyWindowList.h"


class SdPItemPlate;
class SdPlateNetContainer;

class SdGraphTraced : public SdGraph
  {
  public:
    SdGraphTraced();

    //Information
    //Rapid plate retrive
    SdPItemPlate        *getPlate() const;
    //Stratum of object
    virtual SdPropStratum    stratum() const = 0;

    //Set flag to update rat net. Rat net will be clear and reconstructed
    void                 setDirtyRatNet();

    //Accumulate traced linked to point
    void                 accumLinkedTrace(SdGraphTraced *from, SdPoint a, QString netName, SdSelector *sel );

    //Defined here
    virtual bool         isPointOnNet( SdPoint p, SdPropStratum stratum, QString *netName, int *destStratum ) = 0;
    virtual void         accumNetSegments( SdPlateNetContainer *netContainer ) = 0;
    virtual void         drawStratum( SdContext *dcx, int stratum ) = 0;
    virtual void         accumBarriers( SdBarrierList &dest, int stratum, SdRuleId toWhich, const SdRuleBlock &blk ) const = 0;
    virtual bool         isMatchNetAndStratum( const QString netName, SdPropStratum stratum ) const;
    virtual void         accumWindows( SdPolyWindowList &dest, int stratum, int gap, const QString netName ) const = 0;
    virtual bool         isLinked( SdPoint a, SdPropStratum stratum, QString netName ) const = 0;


    // SdGraph interface
  public:
    virtual void         draw(SdContext *dc) override;
  };

typedef SdGraphTraced *SdGraphTracedPtr;

#endif // SDGRAPHTRACED_H
