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
  mSnapMask(0),
  mDestMask(0),
  mDistance(DBL_MAX),
  mNetName()
  {

  }




bool SdSnapInfo::test(SdPoint p, SdSnapMask mask)
  {
  if( (mSnapMask & snapExcludeExcl) && p == mExclude )
    return false;
  if( (mSnapMask & snapExcludeSour) && p == mSour )
    return false;
  double d = mSour.getSquareDistance(p);
  if( d < mDistance ) {
    mDistance = d;
    mDest     = p;
    mDestMask = mask;
    return true;
    }
  return false;
  }




void SdSnapInfo::calculate(SdContainer *object)
  {
  object->forEach( dctAll, [this](SdObject *obj) {
    SdGraph *graph = dynamic_cast<SdGraph*>(obj);
    if( graph ) graph->snapPoint( this );
    return true;
    });
  }




void SdSnapInfo::reset(bool )
  {
  mDistance = DBL_MAX;
  }

