/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Plate net. Contains list of all net segments.
  On list base we build ratnet.
*/
#include "SdPlateNet.h"
#include "SdPoint.h"

#include <QMap>

//======================================================================================================================================
//     SdPlateNet



SdPlateNet::SdPlateNet() :
  mSubNetIndex(1)
  {

  }


//Append one net segment. Check and manage subnets
void SdPlateNet::addNetSegment(SdStratum s, SdPoint p1, SdPoint p2 )
  {
  //Prepare pair to append
  SdPlateNetSegment segment;
  segment.mSegment.set(p1,p2);
  segment.mStratum = s;
  segment.mSubNet  = 0;

  //find subnet whose contains this pair
  for( int i = 0; i < mSegmentList.count(); i++ )
    if( segment.intersect(mSegmentList.at(i)) ) {
      //This segment interseced with other subnet
      if( segment.mSubNet )
        //Subnet already assigned, replace
        replaceSubNet( segment.mSubNet, mSegmentList.at(i).mSubNet );
      //Replace subnet in current segment
      segment.mSubNet = mSegmentList.at(i).mSubNet;
      }

  if( segment.mSubNet == 0 )
    //Subnet is not found. Assign to new subnet
    segment.mSubNet = ++mSubNetIndex;

  mSegmentList.append( segment );
  }







//Build ratnet as pair for each-to-other subnets
void SdPlateNet::buildRatNet(SdRatNet *ratNet) const
  {
  //At first, build subnet map
  QMap<int,SdPoint> mSubNetMap;

  //Scan all segments and check subnet
  //for each sub net we append one ratnet pair with shortest distance to other subnets
  for( const SdPlateNetSegment &segment : mSegmentList ) {
    if( !mSubNetMap.contains(segment.mSubNet) ) {
      //Append new subnet to map
      if( mSubNetMap.count() ) {
        //Find shortest ratSegment from this point to any other
        auto iter = mSubNetMap.cbegin();
        //First point set as shortest at first
        SdPoint p(iter.value());
        double d = p.getSquareDistance( segment.mSegment.getP1() );
        iter++;
        while( iter != mSubNetMap.cend() ) {
          //Test the next point
          SdPoint tp( iter.value() );
          double td = tp.getSquareDistance( segment.mSegment.getP1() );
          //If new point distance is shortest then reassign
          if( td < d ) {
            d = td;
            p = tp;
            }
          iter++;
          }
        //Append pair to ratnet
        ratNet->append( SdRatNetPair(p,segment.mSegment.getP1() ) );
        }
      mSubNetMap.insert( segment.mSubNet, segment.mSegment.getP1() );
      }
    }

  }





//Neares point
SdPoint SdPlateNet::nearestPoint(SdPoint p)
  {
  if( mSegmentList.count() == 0 )
    return p;

  //Scan all segments and find nearest point
  SdPoint nearest = mSegmentList.at(0).mSegment.getP1();
  double d = p.getSquareDistance( nearest );
  for( const SdPlateNetSegment &segment : mSegmentList ) {
    double td = p.getSquareDistance( segment.mSegment.getP1() );
    //If new point distance is shortest then reassign
    if( td < d ) {
      d = td;
      nearest = segment.mSegment.getP1();
      }

    td = p.getSquareDistance( segment.mSegment.getP2() );
    //If new point distance is shortest then reassign
    if( td < d ) {
      d = td;
      nearest = segment.mSegment.getP2();
      }
    }
  return nearest;
  }






//For each points if point subnet equals oldSubNet then replace on newSubNet
void SdPlateNet::replaceSubNet(int oldSubNet, int newSubNet)
  {
  for( SdPlateNetSegment &np : mSegmentList )
    if( np.mSubNet == oldSubNet )
      np.mSubNet = newSubNet;
  }






