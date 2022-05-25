/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Tracing via properties. It contains via pad type, net name which via and via stratum
*/
#include "SdPropVia.h"




//Assign properties with pattern accordings [Назначить свойства в соответствии с шаблоном]
void SdPropVia::operator =(const SdPropVia &prop)
  {
  mStratum = prop.mStratum; //Stratum of via
  mPadType = prop.mPadType; //Pin type for pad assotiation
  mNetName = prop.mNetName; //Net name which via is
  }




//Append properties [Добавить свойства]
void SdPropVia::append(const SdPropVia &prop)
  {
  mStratum.append( prop.mStratum );
  mPadType.append( prop.mPadType );
  mNetName.append( prop.mNetName );
  }




//Compare properties [Сравнить свойства]
bool SdPropVia::operator ==(const SdPropVia &prop)
  {
  return mStratum == prop.mStratum && mPadType == prop.mPadType && mNetName == prop.mNetName;
  }




//Set to undefined state. When undefined state then appending equals coping [Установить в неопределенное состояние]
void SdPropVia::clear()
  {
  mStratum.clear();
  mPadType.clear();
  mNetName.clear();
  }




//Compare with pattern [Сравнить на совпадение с эталоном]
bool SdPropVia::match(const SdPropVia &prop)
  {
  return mStratum.match( prop.mStratum ) && mPadType.match( prop.mPadType ) && mNetName.match( prop.mNetName );
  }



//!
//! \brief json Overloaded function to write object content into json writer
//! \param js   Json writer
//!
void SdPropVia::json(SvJsonWriter &js) const
  {
  mStratum.json( QStringLiteral("vStratum"), js );
  mNetName.json( QStringLiteral("vNet"), js );
  mPadType.json( QStringLiteral("vPad"), js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//! \param js   Json reader
//!
void SdPropVia::json(const SvJsonReader &js)
  {
  mStratum.json( QStringLiteral("vStratum"), js );
  mNetName.json( QStringLiteral("vNet"), js );
  mPadType.json( QStringLiteral("vPad"), js );
  }

