#include "SdM3dFunModelCurve.h"
#include "SdM3dFunModelWall.h"

#include <math.h>

SdM3dFunModelCurve::SdM3dFunModelCurve() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_REGION )
  {

  }


SdM3dModel SdM3dFunModelCurve::toModel() const
  {
  return modelCurve( mParamList[0]->toRegion(), mParamList[1]->toVertex(), mParamList[2]->toFloat(), mParamList[3]->toColor() );
  }



SdM3dModel SdM3dFunModelCurve::modelCurve(SdM3dRegion region, QVector3D rotationAxis, float angle, QColor color )
  {
  SdM3dModel model;

  //Bottom face
  SdM3dFace botFace;
  botFace.mColor = color;
  botFace.mContour = region;
  model.append( botFace );

  //Walls
  SdM3dRegion bot( region );
  if( angle > 0 ) {
    float addon = 10.0;
    do {
      //Build matrix to rotate to current angle
      QMatrix4x4 matrix;
      matrix.rotate( qMin( addon, angle ), rotationAxis );

      //Build rotated redion
      SdM3dRegion top;
      for( auto const &v : region )
        top.append( matrix.map( v ) );

      //Append walls
      model.append( SdM3dFunModelWall::modelWalls( bot, top, color, true ) );

      bot = top;
      addon += 10.0;
      }
    while( addon < angle );
    }
  else {
    float addon = -10.0;
    do {
      //Build matrix to rotate to current angle
      QMatrix4x4 matrix;
      matrix.rotate( qMax( addon, angle ), rotationAxis );

      //Build rotated redion
      SdM3dRegion top;
      for( auto const &v : region )
        top.append( matrix.map( v ) );

      //Append walls
      model.append( SdM3dFunModelWall::modelWalls( bot, top, color, true ) );

      bot = top;
      addon -= 10.0;
      }
    while( addon > angle );
    }

  //Top face
  SdM3dFace topFace;
  topFace.mColor = color;
  topFace.mContour = bot;
  model.append( topFace );

  return model;
  }
