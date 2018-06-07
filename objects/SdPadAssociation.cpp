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
  //TODO D035 write pad association
  }



void SdPadAssociation::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  //TODO D036 read pad association
  }
