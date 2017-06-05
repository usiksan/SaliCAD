/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdStratum.h"

SdStratum::SdStratum() :
  mStratum(0l)
  {

  }


SdStratum::SdStratum(qint64 str) :
  mStratum(str)
  {

  }




void SdStratum::write(QJsonObject &obj) const
  {
  obj.insert( QStringLiteral("Stratum"), QString::number( mStratum, 16 ) );
  }




void SdStratum::read(const QJsonObject &obj)
  {
  mStratum = obj.value( QStringLiteral("Stratum") ).toString().toLongLong( nullptr, 16 );
  }
