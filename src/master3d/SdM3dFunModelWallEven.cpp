#include "SdM3dFunModelWallEven.h"
#include "SdM3dFunRegionTranslate.h"


SdM3dFunModelWallEven::SdM3dFunModelWallEven() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX, SDM3D_TYPE_COLOR )
  {

  }


SdM3dModel SdM3dFunModelWallEven::toModel() const
  {
  return modelWallEven( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor() );
  }

SdM3dModel SdM3dFunModelWallEven::modelWallEven(SdM3dRegion regionPair, QVector3D grow, QColor color)
  {
  SdM3dRegion otherRegion = SdM3dFunRegionTranslate::regionTranslate( regionPair, grow );
  SdM3dModel model;
  SdM3dFace face;
  face.mColor = color;
  for( int i = 0; regionPair.count() - 1; i += 2 ) {
    face.mContour.clear();
    face.mContour.append( regionPair.at(i) );
    face.mContour.append( regionPair.at(i+1) );
    face.mContour.append( otherRegion.at(i+1) );
    face.mContour.append( otherRegion.at(i) );
    model.append( face );
    }
  return model;
  }
