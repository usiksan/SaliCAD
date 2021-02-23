/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds cylinder model from its size
*/
#include "SdM3dFunModelCylinder.h"
#include "SdM3dFunModelExtrude.h"
#include "SdM3dFunRegionCircle.h"

SdM3dFunModelCylinder::SdM3dFunModelCylinder() :
  SdM3dFunction( SDM3D_TYPE_MODEL, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT, SDM3D_TYPE_COLOR )
  {

  }


SdM3dModel SdM3dFunModelCylinder::toModel() const
  {
  return modelCylinder( mParamList[0]->toFloat(), mParamList[1]->toFloat(), mParamList[2]->toColor() );
  }



//!
//! \brief modelCylinder Builds cylinder model from its size
//! \param radius        Radius of circle of footing of cylinder
//! \param height        Height of cylinder
//! \param color         Color of cylinder
//! \return              Cylinder model
//!
SdM3dModel SdM3dFunModelCylinder::modelCylinder(float radius, float height, QColor color)
  {
  return SdM3dFunModelExtrude::modelExtrude( SdM3dFunRegionCircle::regionCircle( radius ), QVector3D(0,0,height), color );
  }
