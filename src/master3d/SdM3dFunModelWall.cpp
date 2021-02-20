#include "SdM3dFunModelWall.h"
#include "SdM3dFunRegionTranslate.h"

SdM3dFunModelWall::SdM3dFunModelWall() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX, SDM3D_TYPE_COLOR )
  {

  }


SdM3dModel SdM3dFunModelWall::toModel() const
  {
  return modelWall( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor() );
  }




SdM3dModel SdM3dFunModelWall::modelWall(SdM3dRegion region, QVector3D grow, QColor color)
  {
  SdM3dRegion otherRegion = SdM3dFunRegionTranslate::regionTranslate( region, grow );
  SdM3dModel model;
  SdM3dFace face;
  face.mColor = color;
  for( int i = 0; region.count() - 1; i++ ) {
    face.mContour.clear();
    face.mContour.append( region.at(i) );
    face.mContour.append( region.at(i+1) );
    face.mContour.append( otherRegion.at(i+1) );
    face.mContour.append( otherRegion.at(i) );
    model.append( face );
    }
  return model;
  }
