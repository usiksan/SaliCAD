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

void SdPropLine::write(QJsonObject &obj) const
  {
  mWidth.write( QStringLiteral("lineWidth"), obj );
  mType.write( QStringLiteral("lineType"), obj );
  mLayer.write( QStringLiteral("lineLayer"), obj );
  }

void SdPropLine::read(const QJsonObject obj)
  {
  mWidth.read( QStringLiteral("lineWidth"), obj );
  mType.read( QStringLiteral("lineType"), obj );
  mLayer.read( QStringLiteral("lineLayer"), obj );
  }

void SdPropLine::saveState(SdUndo *undo)
  {
  undo->propInt( &mWidth );
  undo->propInt( &mType );
  undo->propLayer( &mLayer );
  }
