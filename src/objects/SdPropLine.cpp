/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropLine.h"
#include "SdUndo.h"

void SdPropLine::operator = ( SdPropLine const &sour ) {
  mWidth = sour.mWidth;
  mType  = sour.mType;
  mLayer = sour.mLayer;
  }

void SdPropLine::append( SdPropLine const &sour ) {
  mWidth.append( sour.mWidth );
  mType.append( sour.mType );
  mLayer.append( sour.mLayer );
  }

void SdPropLine::clear() {
  mWidth.clear();
  mType.clear();
  mLayer.clear();
  }

bool SdPropLine::operator == ( SdPropLine const &prop ) {
  return (prop.mWidth < 0 || prop.mWidth == mWidth) &&
         (prop.mType < 0  || prop.mType == mType) &&
         (prop.mLayer == mLayer);
  }

bool SdPropLine::match( SdPropLine const &prop ) {
  return mWidth.match( prop.mWidth ) &&
         mType.match( prop.mType ) &&
      mLayer.match( prop.mLayer );
  }




void SdPropLine::json(SvJsonWriter &js) const
  {
  mWidth.json( QStringLiteral("lineWidth"), js );
  mType.json( QStringLiteral("lineType"), js );
  mLayer.json( QStringLiteral("lineLayer"), js );
  }




void SdPropLine::json(const SvJsonReader &js)
  {
  mWidth.json( QStringLiteral("lineWidth"), js );
  mType.json( QStringLiteral("lineType"), js );
  mLayer.json( QStringLiteral("lineLayer"), js );
  }




void SdPropLine::saveState(SdPropLineState *dst)
  {
  dst->mWidth = mWidth.getValue();
  dst->mType  = mType.getValue();
  dst->mLayer = mLayer.layer();
  }


void SdPropLine::swapState(SdPropLineState *src)
  {
  src->mWidth = mWidth.swap( src->mWidth );
  src->mType  = mType.swap( src->mType );
  src->mLayer = mLayer.swap( src->mLayer );
  }
