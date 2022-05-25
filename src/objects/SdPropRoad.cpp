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



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPropRoad::json(SvJsonWriter &js) const
  {
  mWidth.json( QStringLiteral("rWidth"), js );
  mStratum.json( QStringLiteral("rStratum"), js );
  mNetName.json( QStringLiteral("rNet"), js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPropRoad::json(const SvJsonReader &js)
  {
  mWidth.json( QStringLiteral("rWidth"), js );
  mStratum.json( QStringLiteral("rStratum"), js );
  mNetName.json( QStringLiteral("rNet"), js );
  }








void SdPropRoad::read(const QJsonObject obj)
  {
  mWidth.read( QStringLiteral("rWidth"), obj );
  mStratum.read( QStringLiteral("rStratum"), obj );
  mNetName.read( QStringLiteral("rNet"), obj );
  }

