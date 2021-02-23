/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function add extrudes the model from the top side of source model in the direction of the vector
*/
#include "SdM3dFunModelAddExtrude.h"
#include "SdM3dFunModelWall.h"
#include "SdM3dFunRegionTranslate.h"

SdM3dFunModelAddExtrude::SdM3dFunModelAddExtrude() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_MODEL, SDM3D_TYPE_VERTEX, SDM3D_TYPE_COLOR )
  {

  }


SdM3dModel SdM3dFunModelAddExtrude::toModel() const
  {
  return modelAddExtrude( mParamList[0]->toModel(), mParamList[1]->toVertex(), mParamList[2]->toColor() );
  }



SdM3dModel SdM3dFunModelAddExtrude::modelAddExtrude(SdM3dModel src, QVector3D vector, QColor color)
  {
  SdM3dModel md(src);

  //Remove bottom side
  SdM3dFace bot = md.takeLast();

  SdM3dFace top;
  top.mContour = SdM3dFunRegionTranslate::regionTranslate( bot.mContour, vector );
  top.mColor   = color;

  //Side walls
  md.append( SdM3dFunModelWall::modelWalls( bot.mContour, top.mContour, color, true ) );

  //Top side
  md.append( top );

  return md;
  }
