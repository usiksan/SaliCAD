/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdPropRoad.h"



void SdPropRoad::operator =(const SdPropRoad &prop)
  {
  mWidth = prop.mWidth;
  mLayer = prop.mLayer;
  }

void SdPropRoad::append(const SdPropRoad &prop)
  {
  mWidth.append( prop.mWidth );
  mLayer.append( prop.mLayer );
  }

bool SdPropRoad::operator ==(const SdPropRoad &prop)
  {
  return mWidth == prop.mWidth && mLayer == prop.mLayer;
  }

void SdPropRoad::clear()
  {
  mWidth.clear();
  mLayer.clear();
  }

bool SdPropRoad::match(const SdPropRoad &prop)
  {
  return mWidth.match( prop.mWidth ) && mLayer.match( prop.mLayer );
  }

void SdPropRoad::write(QJsonObject &obj) const
  {
  mWidth.write( QStringLiteral("RoadWidth"), obj );
  mLayer.write( QStringLiteral("RoadLayer"), obj );
  }

void SdPropRoad::read(const QJsonObject obj)
  {
  mWidth.read( QStringLiteral("RoadWidth"), obj );
  mLayer.read( QStringLiteral("RoadLayer"), obj );
  }

