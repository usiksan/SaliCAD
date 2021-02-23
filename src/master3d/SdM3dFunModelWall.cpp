#include "SdM3dFunModelWall.h"
#include "SdM3dFunRegionTranslate.h"

SdM3dFunModelWall::SdM3dFunModelWall() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION, SDM3D_TYPE_VERTEX, SDM3D_TYPE_COLOR, SDM3D_TYPE_BOOL )
  {

  }


SdM3dModel SdM3dFunModelWall::toModel() const
  {
  return modelWall( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toColor(), mParamList[3]->toBool() );
  }




SdM3dModel SdM3dFunModelWall::modelWall(SdM3dRegion region, QVector3D grow, QColor color, bool close )
  {
  SdM3dRegion otherRegion = SdM3dFunRegionTranslate::regionTranslate( region, grow );
  return modelWalls( region, otherRegion, color, close );
  }



//!
//! \brief modelWalls Builds walls on base bottom and top regions. Walls builded with color
//! \param bottom     Bottom region of walls
//! \param top        Top region of walls
//! \param color      Color of faces for the walls
//! \param close      If true then append wall with n-1 and 0 index vertex
//! \return           Model of walls
//!
SdM3dModel SdM3dFunModelWall::modelWalls(SdM3dRegion bottom, SdM3dRegion top, QColor color, bool close)
  {
  SdM3dModel model;
  SdM3dFace face;
  face.mColor = color;
  for( int i = 0; i < bottom.count() - 1; i++ ) {
    face.mContour.clear();
    face.mContour.append( bottom.at(i) );
    face.mContour.append( bottom.at(i+1) );
    face.mContour.append( top.at(i+1) );
    face.mContour.append( top.at(i) );
    model.append( face );
    }
  if( close ) {
    //Append wall with n-1 and 0 vertex
    face.mContour.clear();
    face.mContour.append( bottom.last() );
    face.mContour.append( bottom.first() );
    face.mContour.append( top.first() );
    face.mContour.append( top.last() );
    model.append( face );
    }
  return model;
  }
