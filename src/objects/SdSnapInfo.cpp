/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  SdSnapInfo - info structure for snap system
  Snap system allow link to appropriate points.
*/

#include "SdSnapInfo.h"
#include "SdContainer.h"
#include "SdGraph.h"
#include <float.h>

SdSnapInfo::SdSnapInfo() :
  mDest( SdPoint::far() ),
  mSnapMask(0),
  mDestMask(0),
  mSqDistance(DBL_MAX),
  mNetName(),
  mGraph(nullptr)
  {

  }




//Test if snapped to point
void SdSnapInfo::test(SdGraph *graph, SdPoint p, SdSnapMask mask)
  {
  double distance;
  if( isCandidate( p, distance ) ) {
    mSqDistance = distance;
    mDest       = p;
    mDestMask   = mask;
    mGraph      = graph;
    }
  }




//Test if candidate to snapped point
bool SdSnapInfo::isCandidate(SdPoint p, double &distance )
  {
  if( (mSnapMask & snapExcludeExcl) && p == mExclude )
    return false;
  if( (mSnapMask & snapExcludeSour) && p == mSour )
    return false;
  distance = mSour.getSquareDistance(p);
  return distance < mSqDistance;
  }




//Scan all graph objects of container with this snap
bool SdSnapInfo::scan(SdContainer *object, SdClass mask )
  {
  object->forEach( mask, [this](SdObject *obj) {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph ) graph->snapPoint( this );
    return true;
    });

  return isFound();
  }




//Reset snap to next find
void SdSnapInfo::reset(bool )
  {
  mDest       = SdPoint::far();
  mSqDistance = DBL_MAX;
  mGraph      = nullptr;
  }

