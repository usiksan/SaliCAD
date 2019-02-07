/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Plate net graph. Contains list of all segments of single net.
  On list base we build ratnet.
*/
#ifndef SDPLATENETGRAPH_H
#define SDPLATENETGRAPH_H

#include "SdPlateNetContainer.h"

#include <QList>

struct SdPlateNetGraphSegment {
    SdPoint        p1;
    SdPoint        p2;
    SdStratum      mStratum;
    SdGraphTraced *mTraced;
  };

typedef QList<SdGraphTracedPtr> SdGraphTracedPtrList;

struct SdPlateNetGraphPath {
    SdGraphTracedPtrList mTracedList;
    int                  mNode1;
    int                  mNode2;

    bool isConnectedToNode( int node ) const { return mNode1 == node || mNode2 == node; }
    int  getConnectedNode( int node ) const { return mNode1 == node ? mNode2 : (mNode2 == node ? mNode1 : -1); }
  };



//struct SdPlateNetGraphEdge {
//    SdPoint        p1;
//    SdStratum      mStratum1;
//    SdPoint        p2;
//    SdStratum      mStratum2;

//  };

struct SdPlateNetGraphNode {
    SdStratum mStratum;
    SdPoint   mOrigin;
  };

class SdPlateNetGraph : public SdPlateNetContainer
  {
    QString                       mNetName;
    QList<SdPlateNetGraphSegment> mSegmentList;
    QList<SdPlateNetGraphNode>    mNodeList;
    QList<SdPlateNetGraphPath>    mPathList;
  public:
    SdPlateNetGraph( const QString netName );

    SdPlateNetGraphPath findLoop( SdPoint src, SdStratum stratumSrc, SdPoint dst, SdStratum stratumDst );

    // SdPlateNetContainer interface
  public:
    virtual void addNetSegment(SdGraphTraced *traced, const QString netName, SdStratum s, SdPoint p1, SdPoint p2) override;

  private:
    int  addNode( SdStratum s, SdPoint p );

    int  findNode( SdStratum s, SdPoint p );

    int  findLoopPath(int nodeSrc, int nodeDst , int nodeBack);

  };

#endif // SDPLATENETGRAPH_H
