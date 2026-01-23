#include "SdScanerKiCad.h"
#include "../../objects/SdPItemPart.h"
#include "../../objects/SdGraphLinearCircle.h"
#include "../../objects/SdGraphLinearCircleFilled.h"

struct KiCadCircle
  {
    SdPoint    mCenter;
    SdPoint    mEnd;
    SdPropLine mProp;
    bool       mFill = false;

    int radius() const { return mCenter.getDistanceInt(mEnd); }
  };

static void kicadCircleCenterPoint( SdScanerKiCad *scaner, KiCadCircle *circle )
  {
  kicadPoint( scaner, circle->mCenter );
  }

static void kicadCircleEndPoint( SdScanerKiCad *scaner, KiCadCircle *circle )
  {
  kicadPoint( scaner, circle->mEnd );
  }

static void kicadCircleStroke( SdScanerKiCad *scaner, KiCadCircle *circle )
  {
  kicadStroke( scaner, &(circle->mProp) );
  }

static void kicadCircleLayer( SdScanerKiCad *scaner, KiCadCircle *circle )
  {
  kicadLayer( scaner, circle->mProp.mLayer );
  }

static void kicadCircleFill( SdScanerKiCad *scaner, KiCadCircle *circle )
  {
  circle->mFill = scaner->tokenNeedValue( 'n' ) == "yes";
  scaner->tokenNeed( ')' );
  }


void kicadCirclePart( SdScanerKiCad *scaner, SdPItemPart *part )
  {
  static QMap<QString,std::function<void( SdScanerKiCad*, KiCadCircle* )> >
      tokenMap( { { QString("center"), kicadCircleCenterPoint },
                  { QString("end"),    kicadCircleEndPoint },
                  { QString("stroke"), kicadCircleStroke },
                  { QString("layer"),  kicadCircleLayer },
                  { QString("fill"),   kicadCircleFill }
                } );
  KiCadCircle circleInfo;
  scaner->parse( tokenMap, &circleInfo );

  //Build line primitive on lineInfo base
  if( circleInfo.mFill )
    part->insertChild( new SdGraphLinearCircleFilled( circleInfo.mCenter, circleInfo.radius(), circleInfo.mProp ), nullptr );
  else
    part->insertChild( new SdGraphLinearCircle( circleInfo.mCenter, circleInfo.radius(), circleInfo.mProp ), nullptr );
  }
