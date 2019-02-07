/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Plate net list. Contains list of all named nets.
  On list base we build ratnet.
*/
#include "SdPlateNetList.h"

//======================================================================================================================================
//     SdPlateNetList

SdPlateNetList::SdPlateNetList()
  {

  }

SdPlateNetList::~SdPlateNetList()
  {
  clear();
  }





//Add net segment to appropriate net
void SdPlateNetList::addNetSegment(SdGraphTraced *traced, const QString netName, SdStratum s, SdPoint p1, SdPoint p2)
  {
  Q_UNUSED(traced)

  //If net with netName not contained in list, then create new one
  if( !mNetList.contains(netName) )
    //Create new net with netName and insert it in list
    mNetList.insert( netName, new SdPlateNet() );
  //To this moment net with netName already exist
  //Append segment to existing net
  mNetList[netName]->addNetSegment( s, p1, p2 );
  }





//For each net build ratnet
void SdPlateNetList::buildRatNet(SdRatNet *ratNet)
  {
  for( SdPlateNet *net : mNetList )
    net->buildRatNet( ratNet );
  }




//Clear net list
void SdPlateNetList::clear()
  {
  qDeleteAll( mNetList );
  mNetList.clear();
  }




//Find nearest point for netName in net list
SdPoint SdPlateNetList::nearestPoint(const QString netName, SdPoint p)
  {
  //If net with netName not contained in list, then create new one
  if( mNetList.contains(netName) )
    return mNetList[netName]->nearestPoint( p );
  return p;
  }


