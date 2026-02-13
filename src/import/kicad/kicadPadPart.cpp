#include "SdScanerKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdGraphPartPin.h"


struct KiCadPad
  {
    SdPoint     mPos;
    int         mAngleRotation;
    SdPoint     mSize;
    QStringList mLayerList;
    int         mDrill;
    double      mRoundRectRatio;

    KiCadPad() : mAngleRotation(0), mDrill(0), mRoundRectRatio(0) {}
  };

static void kicadPadAt( SdScanerKiCad *scaner, KiCadPad *pad )
  {
  kicadPointWithRotation( scaner, pad->mPos, pad->mAngleRotation );
  }

static void kicadPadSize( SdScanerKiCad *scaner, KiCadPad *pad )
  {
  kicadPoint( scaner, pad->mSize );
  }

static void kicadPadLayerList( SdScanerKiCad *scaner, KiCadPad *pad )
  {
  kicadLayerList( scaner, pad->mLayerList );
  }

static void kicadPadDrill( SdScanerKiCad *scaner, KiCadPad *pad )
  {
  pad->mDrill = scaner->tokenNeedPartCoord();
  scaner->tokenNeed( ')' );
  }



void kicadPadPart( SdScanerKiCad *scaner, SdPItemPart *part )
  {
  static QMap<QString,std::function<void( SdScanerKiCad*, KiCadPad* )> >
      tokenMap( { { QString("at"),     kicadPadAt },
                  { QString("size"),   kicadPadSize },
                  { QString("drill"),  kicadPadDrill },
                  { QString("layers"), kicadPadLayerList }
                } );
  QString padNumber;
  if( !scaner->tokenNeedValue( 's', padNumber, QObject::tr("Need pad number") ) ) return;

  QString padType;
  if( !scaner->tokenNeedValue( 'n', padType, QObject::tr("Need pad type") ) ) return;

  QString padForm;
  if( !scaner->tokenNeedValue( 'n', padForm, QObject::tr("Need pad form") ) ) return;

  KiCadPad pad;
  scaner->parse( tokenMap, &pad );

  //Build pin on component with pad
  // part->insertChild( new SdGraphLinearLine( lineInfo.mA, lineInfo.mB, lineInfo.mProp ), nullptr );
  }
