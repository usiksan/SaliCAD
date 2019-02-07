#include "SdPlateNetGraph.h"



SdPlateNetGraph::SdPlateNetGraph(const QString netName) :
  SdPlateNetContainer(),
  mNetName(netName)
  {

  }





SdPlateNetGraphPath SdPlateNetGraph::findLoop(SdPoint src, SdStratum stratumSrc, SdPoint dst, SdStratum stratumDst)
  {
  int nodeSrc = addNode( stratumSrc, src );
  int nodeDst = addNode( stratumDst, dst );

  int path = findLoopPath( nodeSrc, nodeDst );
  if( path >= 0 )
    return mPathList[path];
  return SdPlateNetGraphPath();
  }




void SdPlateNetGraph::addNetSegment(SdGraphTraced *traced, const QString netName, SdStratum s, SdPoint p1, SdPoint p2)
  {
  if( netName == mNetName ) {
    if( traced->getClass() == dctPartImp ) {
      //This is node
      addNode( s, p1 );
      }
    else {
      //Segment
      SdPlateNetGraphSegment segment;
      segment.mTraced  = traced;
      segment.mStratum = s;
      segment.p1       = p1;
      segment.p2       = p2;
      mSegmentList.append( segment );
      }
    }
  }




int SdPlateNetGraph::addNode(SdStratum s, SdPoint p)
  {
  int i;
  for( i = 0; i < mNodeList.count() && mNodeList.at(i).mOrigin != p; i++ );
  if( i < mNodeList.count() )
    mNodeList[i].mStratum |= s;
  else {
    SdPlateNetGraphNode node;
    node.mOrigin  = p;
    node.mStratum = s;
    mNodeList.append( node );
    }
  return i;
  }




int SdPlateNetGraph::findLoopPath(int nodeSrc, int nodeDst, int nodeBack )
  {
  if( nodeSrc == nodeDst )
    return -1;

  //Find direct connection
  for( int i = 0; i < mPathList.count(); ++i )
    if( mPathList.at(i).getConnectedNode(nodeSrc) == nodeDst ) return i;

  //Find indirect connection
  for( int i = 0; i < mPathList.count(); ++i )
    if( mPathList.at(i).isConnectedToNode(nodeSrc) && mPathList.at(i).getConnectedNode(nodeSrc) != nodeBack ) {
      int r = findLoopPath( mPathList.at(i).getConnectedNode(nodeSrc), nodeDst, nodeSrc );
      if( r >= 0 ) return r;
      }

  //No connection
  return -1;
  }
