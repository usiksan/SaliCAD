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
  }



void SdPropRoad::append(const SdPropRoad &prop)
  {
  mWidth.append( prop.mWidth );
  }



bool SdPropRoad::operator ==(const SdPropRoad &prop)
  {
  return mWidth == prop.mWidth;
  }



void SdPropRoad::clear()
  {
  mWidth.clear();
  }



bool SdPropRoad::match(const SdPropRoad &prop)
  {
  return mWidth.match( prop.mWidth );
  }




void SdPropRoad::write(QJsonObject &obj) const
  {
  mWidth.write( QStringLiteral("RoadWidth"), obj );
//  mLayer.write( QStringLiteral("RoadLayer"), obj );
  }




void SdPropRoad::read(const QJsonObject obj)
  {
  mWidth.read( QStringLiteral("RoadWidth"), obj );
//  mLayer.read( QStringLiteral("RoadLayer"), obj );
  }

