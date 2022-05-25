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



void SdPropPolygon::json(SvJsonWriter &js) const
  {
  mGap.json( QStringLiteral("pGap"), js );
  mStratum.json( QStringLiteral("pStratum"), js );
  mNetName.json( QStringLiteral("pNet"), js );
  }



void SdPropPolygon::json(const SvJsonReader &js)
  {
  mGap.json( QStringLiteral("pGap"), js );
  mStratum.json( QStringLiteral("pStratum"), js );
  mNetName.json( QStringLiteral("pNet"), js );
  }








void SdPropPolygon::saveState(SdPropPolygonState *dst)
  {
  dst->mGap     = mGap.getValue();
  dst->mNetName = mNetName.str();
  dst->mStratum = mStratum.getValue();
  }




void SdPropPolygon::swapState(SdPropPolygonState *src)
  {
  src->mGap     = mGap.swap( src->mGap );
  src->mStratum = mStratum.swap( src->mStratum );
  mNetName.swap( &(src->mNetName) );
  }
