/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Pin-to-pad association table map as object. It allow to save it in library
  and retrive from external library for share using.
*/
#include "SdPadAssociation.h"
#include "SdUtil.h"

SdPadAssociation::SdPadAssociation()
  {
  mId = QString("PinToPadAssociation %1").arg( SdUtil::getTime2000() );
  }

SdPadAssociation::SdPadAssociation(const SdPadMap map, const QString name, const QString author) :
  mMap(map),
  mName(name)
  {
  mId = QString("PinToPadAssociation %1 %2 %3").arg(name).arg(author).arg( SdUtil::getTime2000() );
  }






QString SdPadAssociation::getType() const
  {
  return QStringLiteral(SD_TYPE_PAD_ASSOCIATION);
  }





quint64 SdPadAssociation::getClass() const
  {
  return dctPadAssociation;
  }





void SdPadAssociation::cloneFrom(const SdObject *src)
  {
  const SdPadAssociation *pads = dynamic_cast<const SdPadAssociation*>(src);
  if( pads != nullptr ) {
    SdObject::cloneFrom( src );
    mMap = pads->mMap;
    }
  }





void SdPadAssociation::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  obj.insert( QStringLiteral("pad association name"), mName );
  QJsonObject ar;
  for( auto iter = mMap.cbegin(); iter != mMap.cend(); iter++ )
    ar.insert( iter.key(), iter.value().write() );
  obj.insert( QStringLiteral("association"), ar );
  }



void SdPadAssociation::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mName = obj.value( QStringLiteral("pad association name") ).toString();
  QJsonObject ar = obj.value( QStringLiteral("association") ).toObject();
  SdPad pad;
  for( auto iter = ar.constBegin(); iter != ar.constEnd(); iter++ ) {
    pad.read( iter.value().toObject() );
    mMap.insert( iter.key(), pad );
    }
  }


QString SdPadAssociation::getId() const
  {
  }
