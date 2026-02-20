/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Road net segment. Also representates vias. Used for rat net build.
*/
#ifndef SDPLATENETSEGMENT_H
#define SDPLATENETSEGMENT_H

#include "SdPvStratum.h"
#include "SdSegment.h"


struct SdPlateNetSegment
  {
    SdSegment     mSegment;
    SdPvStratum   mStratum;
    int           mSubNet;

    bool intersect( const SdPlateNetSegment &p ) const {
      return mSubNet != p.mSubNet && mStratum == p.mStratum && (p.mSegment.isPointOn(mSegment.getP1()) || p.mSegment.isPointOn(mSegment.getP2()));
      }
  };

#endif // SDPLATENETSEGMENT_H
