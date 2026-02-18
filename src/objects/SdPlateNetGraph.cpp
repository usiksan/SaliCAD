#include "SdPlateNetGraph.h"



SdPlateNetGraph::SdPlateNetGraph(const QString netName) :
  SdPlateNetContainer(),
  mNetName(netName)
  {

  }





//Find presented loop from src to dst
SdGraphTracedPtrList SdPlateNetGraph::findLoop(SdPoint src, SdPvStratum stratumSrc, SdPoint dst, SdPvStratum stratumDst)
  {
  int nodeSrc = addNode( stratumSrc, src );
  int nodeDst = addNode( stratumDst, dst );

  for( auto it = mPathList.begin(); it != mPathList.end(); it++ )
    it->mVisited = false;

  int path = findLoopPath( nodeSrc, nodeDst );
  if( path >= 0 )
    return mPathList[path].mTracedList;
  return SdGraphTracedPtrList();
  }




void SdPlateNetGraph::addNetSegment(SdGraphTraced *traced, const QString netName, SdPvStratum s, SdPoint p1, SdPoint p2)
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
      segment.mUsed    = false;
      mSegmentList.append( segment );
      }
    }
  }



//From separate net segments we fill paths which connect nodes
//Create path list and node graph
void SdPlateNetGraph::fillPath()
  {
  //Remove all via nodes
  for( auto iter = mSegmentList.begin(); iter != mSegmentList.end(); iter++ )
    testViaNode( iter );

  //Add all T-nodes
  for( auto iter = mSegmentList.begin(); iter != mSegmentList.end(); iter++ )
    testTNode( iter );


  //Build all roads and leaved vias to paths
  for( auto iter = mSegmentList.begin(); iter != mSegmentList.end(); iter++ )
    if( !iter->mUsed && iter->mTraced->getClass() == dctTraceRoad )
      buildPath( iter );
  }




void SdPlateNetGraph::buildPath(  QList<SdPlateNetGraphSegment>::iterator iter)
  {
  SdPlateNetGraphPath path;
  path.mTracedList.append( iter->mTraced );
  iter->mUsed = true;
  SdPoint p = iter->p1;
  SdPvStratum s = iter->mStratum;
  int node = findNode( s, p );
  while( node < 0 ) {
    SdPoint pp = p;
    SdPvStratum sp = s;
    for( auto it = mSegmentList.begin(); it != mSegmentList.end(); it++ )
      if( !it->mUsed && it->isMatch( p, s ) ) {
        //Append segment to path list
        path.mTracedList.append( it->mTraced );
        it->mUsed = true;
        if( it->isMatch1( p, s ) ) {
          p = it->p2;
          s = it->mStratum;
          }
        else {
          p = it->p1;
          s = it->mStratum;
          }
        break;
        }

    //Test unconnected segment
    if( p == pp && s == sp ) {
      node = addNode( s, p );
      break;
      }
    node = findNode( s, p );
    }
  path.mNode1 = node;

  p = iter->p2;
  s = iter->mStratum;
  node = findNode( s, p );
  while( node < 0 ) {
    SdPoint pp = p;
    SdPvStratum sp = s;
    for( auto it = mSegmentList.begin(); it != mSegmentList.end(); it++ )
      if( !it->mUsed && it->isMatch( p, s ) ) {
        //Append segment to path list
        path.mTracedList.append( it->mTraced );
        it->mUsed = true;
        if( it->isMatch1( p, s ) ) {
          p = it->p2;
          s = it->mStratum;
          }
        else {
          p = it->p1;
          s = it->mStratum;
          }
        break;
        }
    //Test unconnected segment
    if( p == pp && s == sp ) {
      node = addNode( s, p );
      break;
      }
    node = findNode( s, p );
    }
  path.mNode2 = node;

  mPathList.append( path );
  }



//If via connected more then two stratum then via is node and remove from view
void SdPlateNetGraph::testViaNode( QList<SdPlateNetGraphSegment>::iterator iter)
  {
  if( !iter->mUsed && iter->mTraced->getClass() == dctTraceVia ) {
    SdPoint p = iter->p1;
    SdPvStratum s = iter->mStratum;
    int ms1 = 0, ms2 = 0;
    for( auto it = mSegmentList.begin(); it != mSegmentList.end(); it++ )
      if( it->mTraced->getClass() == dctTraceRoad && it->isMatch( p, s ) ) {
        int ds = it->mStratum & s;
        if( ms1 == 0 || ms1 == ds ) ms1 = ds;
        else if( ms2 == 0 || ms2 == ds ) ms2 = ds;
        else {
          //More then two stratum. Via is node
          iter->mUsed = true;
          addNode( s, p );
          }
        }
    }
  }




void SdPlateNetGraph::testTNode( QList<SdPlateNetGraphSegment>::iterator iter )
  {
  if( !iter->mUsed && iter->mTraced->getClass() == dctTraceRoad ) {
    SdPoint p = iter->p1;
    SdPvStratum s = iter->mStratum;
    int count = 0;
    for( auto it = mSegmentList.begin(); it != mSegmentList.end(); it++ )
      if( !it->mUsed && iter != it && it->isMatch( p, s ) )
        count++;
    if( count > 1 )
      addNode( s, p );

    p = iter->p2;
    count = 0;
    for( auto it = mSegmentList.begin(); it != mSegmentList.end(); it++ )
      if( !it->mUsed && iter != it  && it->isMatch( p, s ) )
        count++;
    if( count > 1 )
      addNode( s, p );
    }
  }




//Add node to node list if it not present in list
int SdPlateNetGraph::addNode(SdPvStratum s, SdPoint p)
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




int SdPlateNetGraph::findNode(SdPvStratum s, SdPoint p)
  {
  for( int i = 0; i < mNodeList.count(); i++ )
    if( mNodeList.at(i).isMatch( p, s ) )
      return i;
  return -1;
  }




int SdPlateNetGraph::findLoopPath(int nodeSrc, int nodeDst)
  {
  if( nodeSrc == nodeDst )
    return -1;

  //Find direct connection
  for( int i = 0; i < mPathList.count(); ++i )
    if( mPathList.at(i).getConnectedNode(nodeSrc) == nodeDst ) return i;

  //Find indirect connection
  for( auto it = mPathList.begin(); it != mPathList.end(); it++ )
    if( !it->mVisited && it->isConnectedToNode(nodeSrc) ) {
      it->mVisited = true;
      int r = findLoopPath( it->getConnectedNode(nodeSrc), nodeDst );
      if( r >= 0 ) return r;
      }

  //No connection
  return -1;
  }
