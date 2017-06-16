#include "SdPlateNet.h"


//======================================================================================================================================
//     SdSubNet
//Find minimal distance
SdSubNetDistance SdSubNet::findMinimalDistance()
  {
  SdSubNetDistance minDist = first();
  for( const SdSubNetDistance &dist : (*this) )
    if( minDist.mDistance > dist.mDistance )
      minDist = dist;
  return minDist;
  }


//Add distance from this sub net to another (dest) sub net
//Calculate distance and call addDistance( int mDestSubNet, double distance, SdPoint sour, SdPoint dest );
void SdSubNet::addDistance(int mDestSubNet, SdPoint sour, SdPoint dest)
  {
  addDistance( mDestSubNet, sour.getDistance(dest), sour, dest );
  }



//Add distance from this sub net to another (dest) sub net
//When addition we check if distance is minimal
void SdSubNet::addDistance(int mDestSubNet, double distance, SdPoint sour, SdPoint dest)
  {
  if( !contains(mDestSubNet) || value(mDestSubNet).mDistance > distance )
    insert( mDestSubNet, SdSubNetDistance( mDestSubNet, distance, sour, dest ) );
  }



//Add all distances from net to current sub net. When addition we check if distance is minimal
void SdSubNet::addSubNet(int curSubNet, const SdSubNet &net)
  {
  for( const SdSubNetDistance &dist : net )
    if( dist.mDestSubNet != curSubNet )
      addDistance( dist.mDestSubNet, dist.mDistance, dist.mSour, dist.mDest );
  }


//======================================================================================================================================
//     SdSubNet



SdPlateNet::SdPlateNet()
  {

  }



int SdPlateNet::addNetPoint(SdStratum s, SdPoint org, int subNet)
  {
  for( SdPlateNetPoint &np : mNetPointList )
    if( np.mStratum.match(s) && np.mOrigin == org ) {
      if( np.mSubNet != subNet ) {
        if( subNet == 0 )
          //Sub net of point equals
          subNet = np.mSubNet;
        else {
          //Replace sub net
          replaceSubNet( subNet, np.mSubNet );
          }
        }
      }
  if( subNet == 0 ) subNet = mNetPointList.count();
  SdPlateNetPoint p;
  p.mStratum = s;
  p.mOrigin  = org;
  p.mSubNet  = subNet;
  mNetPointList.append( p );
  return subNet;
  }



void SdPlateNet::buildRatNet(SdRatNet *ratNet)
  {
  //We link sub net pair with minimal distance
  //Then linked sub nets union into one sub net
  //Process continue while present more then one sub nets
  while( mSubNetList.count() > 1 ) {
    //Find minimal distance
    SdSubNetDistance distance = mSubNetList.first().findMinimalDistance();
    //Add into rat net link pair
    ratNet->append( SdRatNetPair(distance.mSour, distance.mDest) );
    //Move dest sub net to current sub net
    mSubNetList.first().addSubNet( mSubNetList.firstKey(), mSubNetList.value(distance.mDestSubNet) );
    mSubNetList.remove(distance.mDestSubNet);
    }
  }




//For each points if point subnet equals oldSubNet then replace on newSubNet
void SdPlateNet::replaceSubNet(int newSubNet, int oldSubNet)
  {
  for( SdPlateNetPoint &np : mNetPointList )
    if( np.mSubNet == oldSubNet )
      np.mSubNet = newSubNet;
  }




SdPlateNetList::SdPlateNetList()
  {

  }

SdPlateNetList::~SdPlateNetList()
  {
  qDeleteAll( mNetList );
  }



int SdPlateNetList::addNetPoint(const QString netName, SdStratum s, SdPoint org, int subNetIndex )
  {
  if( !mNetList.contains(netName) )
    mNetList.insert( netName, new SdPlateNet() );
  return mNetList[netName]->addNetPoint( s, org, subNetIndex );
  }




void SdPlateNetList::buildRatNet(SdRatNet *ratNet)
  {
  for( SdPlateNet *net : mNetList )
    net->buildRatNet( ratNet );
  }
