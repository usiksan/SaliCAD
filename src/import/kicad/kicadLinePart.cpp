#include "SdScanerKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdGraphLinearLine.h"

struct KiCadLine
  {
    SdPoint    mA;
    SdPoint    mB;
    SdPropLine mProp;
  };

static void kicadLineStartPoint( SdScanerKiCad *scaner, KiCadLine *line )
  {
  kicadPoint( scaner, line->mA );
  }

static void kicadLineEndPoint( SdScanerKiCad *scaner, KiCadLine *line )
  {
  kicadPoint( scaner, line->mB );
  }

static void kicadLineStroke( SdScanerKiCad *scaner, KiCadLine *line )
  {
  kicadStroke( scaner, &(line->mProp) );
  }

static void kicadLineLayer( SdScanerKiCad *scaner, KiCadLine *line )
  {
  kicadLayer( scaner, line->mProp.mLayer );
  }


void kicadLinePart( SdScanerKiCad *scaner, SdPItemPart *part )
  {
  static QMap<QString,std::function<void( SdScanerKiCad*, KiCadLine* )> >
      tokenMap( { { QString("start"),  kicadLineStartPoint },
                  { QString("end"),    kicadLineEndPoint },
                  { QString("stroke"), kicadLineStroke },
                  { QString("layer"),  kicadLineLayer }
                } );
  KiCadLine lineInfo;
  scaner->parse( tokenMap, &lineInfo );

  //Build line primitive on lineInfo base
  part->insertChild( new SdGraphLinearLine( lineInfo.mA, lineInfo.mB, lineInfo.mProp ), nullptr );

  }