#include "SdScanerKiCad.h"
#include "../../objects/SdPItemPart.h"
/*
#include "../../objects/SdGraphPartPin.h"

/*
struct KiCadPin
  {
    SdPoint    mPos;
    SdPoint    mSize;
    int        mDrill;
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
<<<<<<< HEAD

*/

void kicadPadPart( SdScanerKiCad *scaner, SdPItemPart *part )
  {
  // static QMap<QString,std::function<void( SdScanerKiCad*, KiCadLine* )> >
  //     tokenMap( { { QString("start"),  kicadLineStartPoint },
  //                 { QString("end"),    kicadLineEndPoint },
  //                 { QString("stroke"), kicadLineStroke },
  //                 { QString("layer"),  kicadLineLayer }
  //               } );
  QString padNumber;
  if( !scaner->tokenNeedValue( 's', padNumber, QObject::tr("Need pad number") ) ) return;

  QString padType;
  if( !scaner->tokenNeedValue( 'n', padType, QObject::tr("Need pad type") ) ) return;

  QString padForm;
  if( !scaner->tokenNeedValue( 'n', padForm, QObject::tr("Need pad form") ) ) return;

  // KiCadLine lineInfo;
  // scaner->parse( tokenMap, &lineInfo );

  // //Build line primitive on lineInfo base
  // part->insertChild( new SdGraphLinearLine( lineInfo.mA, lineInfo.mB, lineInfo.mProp ), nullptr );
=======
*/

void kicadPadPart( SdScanerKiCad *scaner, SdPItemPart *part )
  {/*
  static QMap<QString,std::function<void( SdScanerKiCad*, KiCadLine* )> >
      tokenMap( { { QString("start"),  kicadLineStartPoint },
                  { QString("end"),    kicadLineEndPoint },
                  { QString("stroke"), kicadLineStroke },
                  { QString("layer"),  kicadLineLayer }
                } );
  KiCadLine lineInfo;
  scaner->parse( tokenMap, &lineInfo );

  //Build line primitive on lineInfo base
  part->insertChild( new SdGraphLinearLine( lineInfo.mA, lineInfo.mB, lineInfo.mProp ), nullptr ); */
>>>>>>> e4693313ae7ac0ce21ae271bea0dc01d93c1842d
  }
