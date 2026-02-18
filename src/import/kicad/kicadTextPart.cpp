#include "SdKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdGraphText.h"
#include "../../objects/SdGraphIdent.h"

struct KiCadText
  {
    QString    mType;
    QString    mText;
    SdPoint    mOrg;
    SdPropText mPropText;

  };


static void kicadTextAt( SdScanerKiCad *scaner, KiCadText *text )
  {
  // text->mOrg.setX( scaner->tokenNeedPartCoord() );
  // text->mOrg.setY( scaner->tokenNeedPartCoord() );
  scaner->tokenNeedValue('n');
  scaner->tokenNeedValue(')');
  }


static void kicadTextLayer( SdScanerKiCad *scaner, KiCadText *text )
  {

  }

static void kicadTextEffects( SdScanerKiCad *scaner, KiCadText *text )
  {

  }




void kicadTextPart( SdScanerKiCad *scaner, SdPItemPart *part )
  {
  static QMap<QString,std::function<void( SdScanerKiCad*, KiCadText* )> >
      tokenMap( { { QString("at"),      kicadTextAt },
                  { QString("layer"),   kicadTextLayer },
                  { QString("effects"), kicadTextEffects }
                } );

  KiCadText textInfo;
  textInfo.mType = scaner->tokenNeedValue('n');
  textInfo.mType = scaner->tokenNeedValue('s');


  if( textInfo.mType == "reference" ) {
    //Default properties for reference id
    textInfo.mPropText.mLayer.set( QString(LID0_IDENT LID1_TOP) );    //Layer of ident
    textInfo.mPropText.mFont   = 0;
    textInfo.mPropText.mSize   = 1000;
    textInfo.mPropText.mDir    = da0;
    textInfo.mPropText.mHorz   = dhjCenter;
    textInfo.mPropText.mVert   = dvjMiddle;
    textInfo.mPropText.mMirror = 0;

    scaner->parse( tokenMap, &textInfo );

    int half = textInfo.mPropText.mSize.value() / 2;
    SdRect r( textInfo.mOrg.x() - half, textInfo.mOrg.y() - half, half * 2, half * 2 );
    part->identGet()->updateIdent( textInfo.mOrg, r, &textInfo.mPropText );
    }
  }
