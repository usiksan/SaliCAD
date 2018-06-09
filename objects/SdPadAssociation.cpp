#include "SdPadAssociation.h"

SdPadAssociation::SdPadAssociation()
  {

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
  QJsonObject ar;
  for( auto iter = mMap.cbegin(); iter != mMap.cend(); iter++ )
    ar.insert( iter.key(), iter.value().write() );
  obj.insert( QStringLiteral("association"), ar );
  }



void SdPadAssociation::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  QJsonObject ar = obj.value( QStringLiteral("association") ).toObject();
  SdPad pad;
  for( auto iter = ar.constBegin(); iter != ar.constEnd(); iter++ ) {
    pad.read( iter.value().toObject() );
    mMap.insert( iter.key(), pad );
    }
  }
