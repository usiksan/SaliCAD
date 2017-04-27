/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdSection.h"
#include "SdPItemSymbol.h"

SdSection::SdSection() :
  SdObject(),
  mSymbol(0)
  {

  }




QString SdSection::getType() const
  {
  return QString( SD_TYPE_SECTION );
  }




quint64 SdSection::getClass() const
  {
  return dctSection;
  }




void SdSection::cloneFrom(const SdObject *src)
  {
  SdObject::cloneFrom( src );
  const SdSection *section = dynamic_cast<const SdSection*>(src);
  if( section ) {
    mSymbol           = section->mSymbol;
    mAssotiationTable = section->mAssotiationTable;
    }
  }




void SdSection::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  writePtr( mSymbol, QString("Symbol"), obj );

  }




void SdSection::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  }

