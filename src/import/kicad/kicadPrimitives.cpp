#include "SdScanerKiCad.h"
#include "objects/SdPoint.h"
#include "objects/SdPropLine.h"


void kicadPoint( SdScanerKiCad *scaner, SdPoint &point )
  {
  point.setX( scaner->tokenNeedPartCoord() );
  point.setY( scaner->tokenNeedPartCoord() );
  scaner->tokenNeed( ')' );
  }




void kicadLineType( SdScanerKiCad *scaner, SdPropInt &type )
  {
  QString lineType( scaner->tokenNeedValue('n') );
  if( lineType == "solid" ) type = dltSolid;
  else type = dltSolid;
  scaner->tokenNeed( ')' );
  }


void kicadLineWidth( SdScanerKiCad *scaner, SdPropInt &width )
  {
  width = scaner->tokenNeedPartCoord();
  scaner->tokenNeed( ')' );
  }


QString kicadLayerMap( const QString kicadLayerName )
  {
  static QMap<QString,QString> mLayerMap;
  if( mLayerMap.isEmpty() ) {
    //Fill layer map
    //mLayerMap.insert( "F.SilkS", "" ); //Silk
    //mLayerMap.insert( "F.CrtYd", "" );
    mLayerMap.insert( "F.Fab",   LID0_COMPONENT );
    mLayerMap.insert( "F.Paste", LID0_STENCIL LID1_TOP );
    mLayerMap.insert( "F.Cu",    LID0_PAD LID1_TOP );
    mLayerMap.insert( "F.Mask",  LID0_SOLDER_MASK LID1_TOP );
    }
  return mLayerMap.value( kicadLayerName, LID0_INVISIBLE );
  }


void kicadLayer( SdScanerKiCad *scaner, SdPropLayer &layer )
  {
  QString kicadLayerName( scaner->tokenNeedValue('s') );
  QString layerId( kicadLayerMap(kicadLayerName) );
  layer.set( layerId );
  scaner->tokenNeed( ')' );
  }


void kicadStrokeLineType( SdScanerKiCad *scaner, SdPropLine *prop )
  {
  kicadLineType( scaner, prop->mType );
  }


void kicadStrokeLineWidth( SdScanerKiCad *scaner, SdPropLine *prop )
  {
  kicadLineWidth( scaner, prop->mWidth );
  }


void kicadStroke( SdScanerKiCad *scaner, SdPropLine *prop )
  {
  static QMap<QString,std::function<void( SdScanerKiCad*, SdPropLine* )> >
      tokenMap( { { QString("type"),  kicadStrokeLineType },
                  { QString("width"), kicadStrokeLineWidth }
                } );
  scaner->parse( tokenMap, prop );
  }
