/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing road properties. It contains road width, net name which road and road stratum
*/

#include "SdPropRoad.h"



void SdPropRoad::operator =(const SdPropRoad &prop)
  {
  mWidth   = prop.mWidth;
  mNetName = prop.mNetName;
  mStratum = prop.mStratum;
  }



void SdPropRoad::append(const SdPropRoad &prop)
  {
  mWidth.append( prop.mWidth );
  mNetName.append( prop.mNetName );
  mStratum.append( prop.mStratum );
  }



bool SdPropRoad::operator ==(const SdPropRoad &prop)
  {
  return mWidth == prop.mWidth && mNetName == prop.mNetName && mStratum == prop.mStratum;
  }



void SdPropRoad::clear()
  {
  mWidth.clear();
  mNetName.clear();
  mStratum.clear();
  }



bool SdPropRoad::match(const SdPropRoad &prop)
  {
  return mWidth.match( prop.mWidth ) && mNetName.match( prop.mNetName ) && mStratum.match( prop.mStratum );
  }




void SdPropRoad::write(QJsonObject &obj) const
  {
  mWidth.write( QStringLiteral("rWidth"), obj );
  mStratum.write( QStringLiteral("rStratum"), obj );
  mNetName.write( QStringLiteral("rNet"), obj );
  }




void SdPropRoad::read(const QJsonObject obj)
  {
  mWidth.read( QStringLiteral("rWidth"), obj );
  mStratum.read( QStringLiteral("rStratum"), obj );
  mNetName.read( QStringLiteral("rNet"), obj );
  }

