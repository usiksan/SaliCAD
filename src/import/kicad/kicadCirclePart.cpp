#include "SdKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdGraphLinearCircle.h"
#include "../../objects/SdGraphLinearCircleFilled.h"



void kicadGraphCircle( SdScanerKiCad *scaner, SdPItemPart *part )
  {
  SdKiCadCircle circleInfo;
  circleInfo.scan( scaner );
  if( circleInfo.isValid() ) {
    SdPropLine prop;
    kicadFillPropLine( prop, circleInfo.mStroke, circleInfo.mLayer );

    int radius = qAbs(circleInfo.mCenter.mPoint.x() - circleInfo.mEnd.mPoint.x());

    //Build line primitive on lineInfo base
    if( circleInfo.mFilled.isYes() )
      part->insertChild( new SdGraphLinearCircleFilled( circleInfo.mCenter.mPoint, radius, prop ), nullptr );
    else
      part->insertChild( new SdGraphLinearCircle( circleInfo.mCenter.mPoint, radius, prop ), nullptr );
    }

  }
