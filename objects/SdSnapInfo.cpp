/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdSnapInfo.h"
#include "SdContainer.h"
#include "SdGraph.h"
#include <float.h>

SdSnapInfo::SdSnapInfo() :
  mSnapMask(0),
  mDestMask(0),
  mDistance(DBL_MAX),
  mFlag(0),
  mNetName()
  {

  }




bool SdSnapInfo::test(SdPoint p, int mask)
  {
  if( (mFlag & dsifExExcl) && p == mExclude )
    return false;
  if( (mFlag & dsifExSour) && p == mSour )
    return false;
  double d = mSour.getDistance(p);
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

