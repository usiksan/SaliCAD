/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function moves the region in the direction perpendicular to the plane of the region by the specified amount
*/
#include "SdM3dFunRegionShift.h"
#include "SdM3dFunRegionTranslate.h"

SdM3dFunRegionShift::SdM3dFunRegionShift() :
  SdM3dFunction( SDM3D_TYPE_REGION, SDM3D_TYPE_REGION, SDM3D_TYPE_FLOAT )
  {

  }


SdM3dRegion SdM3dFunRegionShift::toRegion() const
  {
  return regionShift( mParamList[0]->toRegion(), mParamList[1]->toFloat() );
  }




//!
//! \brief regionShift The function moves the region in the direction perpendicular
//!                    to the plane of the region by the specified shift amount
//! \param region      Source region to move
//! \param shift       Shift amount
//! \return            Moved region
//!
SdM3dRegion SdM3dFunRegionShift::regionShift(SdM3dRegion region, float shift)
  {
  if( region.count() < 3 ) return region;
  //Normal vector
  QVector3D normal = QVector3D::normal( region.at(0), region.at(1), region.at(2) );
  normal *= -shift;
  return SdM3dFunRegionTranslate::regionTranslate( region, normal );
  }
