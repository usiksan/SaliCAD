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
  mDistance(DBL_MAX),
  mNetName(),
  mGraph(nullptr)
  {

  }




//Test if snapped to point
void SdSnapInfo::test(SdGraph *graph, SdPoint p, SdSnapMask mask)
  {
  if( (mSnapMask & snapExcludeExcl) && p == mExclude )
    return;
  if( (mSnapMask & snapExcludeSour) && p == mSour )
    return;
  double d = mSour.getSquareDistance(p);
  if( d < mDistance ) {
    mDistance = d;
    mDest     = p;
    mDestMask = mask;
    mGraph    = graph;
    }
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
  mDest     = SdPoint::far();
  mDistance = DBL_MAX;
  mGraph    = nullptr;
  }

