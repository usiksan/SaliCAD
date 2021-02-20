#include "SdM3dFunModelExtrude.h"
#include "SdM3dFunRegionTranslate.h"


SdM3dFunModelExtrude::SdM3dFunModelExtrude() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX, SDM3D_TYPE_COLOR )
  {

  }




SdM3dModel SdM3dFunModelExtrude::toModel() const
  {
  return modelExtrude( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor() );
  }




SdM3dModel SdM3dFunModelExtrude::modelExtrude(SdM3dRegion region, QVector3D vector, QColor color )
  {
  SdM3dModel md;
  SdM3dFace bot;
  bot.mContour = region;
  bot.mColor   = color;
  md.append( bot );

  SdM3dFace top;
  top.mContour = SdM3dFunRegionTranslate::regionTranslate( region, vector );
  top.mColor   = color;
  md.append( top );

  //Side walls
  for( int i = 0; i < region.count(); i++ ) {
    SdM3dFace side;
    int j = i < region.count() - 1 ? i + 1 : 0;
    side.mContour.append( bot.mContour.at(i) );
    side.mContour.append( bot.mContour.at(j) );
    side.mContour.append( top.mContour.at(j) );
    side.mContour.append( top.mContour.at(i) );
    md.append( side );
    }

  return md;
  }
