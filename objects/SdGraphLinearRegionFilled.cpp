/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical object Filled Region. It's region bounded lines and filled with color
*/
#include "SdGraphLinearRegionFilled.h"
#include "SdContext.h"
#include "SdSelector.h"



SdGraphLinearRegionFilled::SdGraphLinearRegionFilled() :
  SdGraphLinearRegion()
  {

  }




SdGraphLinearRegionFilled::SdGraphLinearRegionFilled(const SdPointList list, const SdPropLine &propLine) :
  SdGraphLinearRegion( list, propLine )
  {

  }




QString SdGraphLinearRegionFilled::getType() const
  {
  return QStringLiteral(SD_TYPE_REGION_FILLED);
  }






void SdGraphLinearRegionFilled::draw(SdContext *dc)
  {
  dc->regionFill( mList, mProp );
  }
