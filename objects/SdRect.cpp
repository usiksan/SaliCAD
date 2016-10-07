/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdRect.h"

SdRect::SdRect()
  {

  }

QJsonObject SdRect::write() const
  {
  QJsonObject obj;
  obj.insert( QStringLiteral("x"), x );
  obj.insert( QStringLiteral("y"), y );
  obj.insert( QStringLiteral("w"), width );
  obj.insert( QStringLiteral("h"), height );
  return obj;
  }

void SdRect::write(const QString name, QJsonObject &obj) const
  {
  obj.insert( name, write() );
  }

void SdRect::read(const QJsonObject &obj)
  {
  x      = obj.value( QStringLiteral("x") );
  y      = obj.value( QStringLiteral("y") );
  width  = obj.value( QStringLiteral("w") );
  height = obj.value( QStringLiteral("h") );
  }

void SdRect::read(const QString name, const QJsonObject obj)
  {
  read( obj.value(name).toObject() );
  }

