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
    SdPvStratum  mStratum;
    SdGraphTraced *mTraced;
    bool           mUsed;

    bool isMatch1( SdPoint p, SdPvStratum s ) { return mStratum.match(s) && p1 == p; }
    bool isMatch2( SdPoint p, SdPvStratum s ) { return mStratum.match(s) && p2 == p; }
    bool isMatch( SdPoint p, SdPvStratum s ) { return mStratum.match(s) && (p1 == p || p2 == p); }
  };

typedef QList<SdGraphTracedPtr> SdGraphTracedPtrList;


struct SdPlateNetGraphPath {
    SdGraphTracedPtrList mTracedList;
    bool                 mVisited;
    int                  mNode1;
    int                  mNode2;
    SdPlateNetGraphPath() : mVisited(false), mNode1(-1),mNode2(-1) {}

    bool isConnectedToNode( int node ) const { return mNode1 == node || mNode2 == node; }
    int  getConnectedNode( int node ) const { return mNode1 == node ? mNode2 : (mNode2 == node ? mNode1 : -1); }
  };





struct SdPlateNetGraphNode {
    SdPvStratum mStratum;
    SdPoint   mOrigin;

    bool isMatch( SdPoint p, SdPvStratum s ) const { return mStratum.match(s) && p == mOrigin; }
  };




class SdPlateNetGraph : public SdPlateNetContainer
  {
    QString                       mNetName;
    QList<SdPlateNetGraphSegment> mSegmentList;
    QList<SdPlateNetGraphNode>    mNodeList;
    QList<SdPlateNetGraphPath>    mPathList;
  public:
    SdPlateNetGraph( const QString netName );

    //From separate net segments we fill paths which connect nodes
    //Create path list and node graph
    void                 fillPath();

    //Find presented loop from src to dst
    SdGraphTracedPtrList findLoop( SdPoint src, SdPvStratum stratumSrc, SdPoint dst, SdPvStratum stratumDst );

    //Add node to node list if it not present in list
    int  addNode( SdPvStratum s, SdPoint p );

    // SdPlateNetContainer interface
  public:
    virtual void addNetSegment(SdGraphTraced *traced, const QString netName, SdPvStratum s, SdPoint p1, SdPoint p2) override;

  private:

    void buildPath( QList<SdPlateNetGraphSegment>::iterator iter );

    void testViaNode( QList<SdPlateNetGraphSegment>::iterator iter );

    void testTNode( QList<SdPlateNetGraphSegment>::iterator iter );

    int  findNode( SdPvStratum s, SdPoint p );

    int  findLoopPath(int nodeSrc, int nodeDst);

  };

#endif // SDPLATENETGRAPH_H
