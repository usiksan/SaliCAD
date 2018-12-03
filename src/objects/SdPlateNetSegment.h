#ifndef SDPLATENETSEGMENT_H
#define SDPLATENETSEGMENT_H

#include "SdPoint.h"
#include "SdStratum.h"
#include "SdSegment.h"


struct SdPlateNetSegment
  {
    SdSegment mSegment;
    SdStratum mStratum;
    int       mSubNet;

    bool intersect( const SdPlateNetSegment &p ) const {
      return mSubNet != p.mSubNet && mStratum.match(p.mStratum) && (p.mSegment.isPointOn(mSegment.getP1()) || p.mSegment.isPointOn(mSegment.getP2()));
      }
  };

#endif // SDPLATENETSEGMENT_H
