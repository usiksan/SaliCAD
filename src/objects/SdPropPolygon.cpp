/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing polygon properties. It contains windows gap, net name which polygon and polygon stratum
*/
#include "SdPropPolygon.h"


void SdPropPolygon::operator =(const SdPropPolygon &prop)
  {
  mGap     = prop.mGap;
  mNetName = prop.mNetName;
  mStratum = prop.mStratum;
  }




void SdPropPolygon::append(const SdPropPolygon &prop)
  {
  mGap.append( prop.mGap );
  mNetName.append( prop.mNetName );
  mStratum.append( prop.mStratum );
  }




bool SdPropPolygon::operator ==(const SdPropPolygon &prop)
  {
  return mGap == prop.mGap && mNetName == prop.mNetName && mStratum == prop.mStratum;
  }




void SdPropPolygon::clear()
  {
  mGap.clear();
  mNetName.clear();
  mStratum.clear();
  }




bool SdPropPolygon::match(const SdPropPolygon &prop)
  {
  return mGap.match( prop.mGap ) && mNetName.match( prop.mNetName ) && mStratum.match( prop.mStratum );
  }




void SdPropPolygon::write(QJsonObject &obj) const
  {
  mGap.write( QStringLiteral("pGap"), obj );
  mStratum.write( QStringLiteral("pStratum"), obj );
  mNetName.write( QStringLiteral("pNet"), obj );
  }




void SdPropPolygon::read(const QJsonObject obj)
  {
  mGap.read( QStringLiteral("pGap"), obj );
  mStratum.read( QStringLiteral("pStratum"), obj );
  mNetName.read( QStringLiteral("pNet"), obj );
  }