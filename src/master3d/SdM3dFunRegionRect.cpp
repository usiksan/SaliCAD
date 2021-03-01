/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  3d model programming language

  The function builds rectangle region with center in 0
*/
#include "SdM3dFunRegionRect.h"


SdM3dFunRegionRect::SdM3dFunRegionRect() :
  SdM3dFunction( SDM3D_TYPE_REGION, SDM3D_TYPE_FLOAT, SDM3D_TYPE_FLOAT )
  {

  }



SdM3dRegion SdM3dFunRegionRect::toRegion() const
  {
  return regionRect( mParamList[0]->toFloat(), mParamList[1]->toFloat() );
  }



//!
//! \brief regionRect Builds rectangle region with center in 0
//! \param width      Width (X) of rectangle (in flat view)
//! \param height     Height (Y) of rectangle (in flat view)
//! \return           Rectangle region with center in 0
//!
SdM3dRegion SdM3dFunRegionRect::regionRect(float width, float height, QVector3D offset)
  {
  width /= 2.0;
  height /= 2.0;
  SdM3dRegion region;
  region.append( QVector3D(-width + offset.x(), -height + offset.y(), offset.z() ) );
  region.append( QVector3D(width + offset.x(), -height + offset.y(), offset.z() ) );
  region.append( QVector3D(width + offset.x(), height + offset.y(), offset.z() ) );
  region.append( QVector3D(-width + offset.x(), height + offset.y(), offset.z() ) );
  return region;
  }
