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

struct SdDistanceInfo {
    SdPoint mSource;     //Source point
    SdPoint mDest;       //Dest point
    double  mDistance;   //Distance from source point to mDest
    int     mDestSubNet; //Dest sub net id

    SdDistanceInfo() : mDistance(-1), mDestSubNet(0) {}
  };

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
    virtual int          getSubNet( const QString netName, SdPoint p, SdStratum s ) = 0;
    virtual void         getMinDistance( int subNet, const QString netName, SdPoint p, SdDistanceInfo *dst ) = 0;
    virtual void         getSubNetDistance( int subNet, SdDistanceInfo *dst ) = 0;
    //virtual void         accumLinked( SdPoint a, SdPoint b, SdLayer *layer, SdSelector *sel ) = 0;
    virtual void         unionSubNet( int srcSubNet, int dstSubNet ) = 0;
    virtual void         forsedSubNet( SdPItemPlate *plate ) = 0;

  };

#endif // SDGRAPHTRACED_H
