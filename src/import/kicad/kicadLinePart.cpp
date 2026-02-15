#include "SdKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdGraphLinearLine.h"



void kicadGraphLine( SdScanerKiCad *scaner, SdPItemPart *part )
  {
  SdKiCadLine lineInfo;
  lineInfo.scan( scaner );

  if( lineInfo.isValid() ) {
    SdPropLine prop;
    kicadFillPropLine( prop, lineInfo.mStroke, lineInfo.mLayer );

    //Build line primitive on lineInfo base
    part->insertChild( new SdGraphLinearLine( lineInfo.mStart.mPoint, lineInfo.mEnd.mPoint, prop ), nullptr );
    }
  }
