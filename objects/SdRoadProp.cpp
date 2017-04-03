/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdRoadProp.h"



void SdRoadProp::operator =(const SdRoadProp &prop)
  {
  mWidth = prop.mWidth;
  mLayer = prop.mLayer;
  }

void SdRoadProp::append(const SdRoadProp &prop)
  {
  mWidth.append( prop.mWidth );
  mLayer.append( prop.mLayer );
  }

bool SdRoadProp::operator ==(const SdRoadProp &prop)
  {
  return mWidth == prop.mWidth && mLayer == prop.mLayer;
  }

void SdRoadProp::clear()
  {
  mWidth.clear();
  mLayer.clear();
  }

bool SdRoadProp::match(const SdRoadProp &prop)
  {
  return mWidth.match( prop.mWidth ) && mLayer.match( prop.mLayer );
  }

void SdRoadProp::write(QJsonObject &obj) const
  {
  mWidth.write( QStringLiteral("RoadWidth"), obj );
  mLayer.write( QStringLiteral("RoadLayer"), obj );
  }

void SdRoadProp::read(const QJsonObject obj)
  {
  mWidth.read( QStringLiteral("RoadWidth"), obj );
  mLayer.read( QStringLiteral("RoadLayer"), obj );
  }

