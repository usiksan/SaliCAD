/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdTextImplement.h"


void SdTextImplement::write(const QString name, QJsonObject &obj) const
  {
  mProp.write( name, obj );
  mOrigin.write( name + QString("org"), obj );
  }




void SdTextImplement::read(const QString name, const QJsonObject &obj)
  {
  mProp.read( name, obj );
  mOrigin.read( name + QString("org"), obj );
  }
