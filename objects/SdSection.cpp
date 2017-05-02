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

SdSection::SdSection() :
  SdObject()
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
    mSymbolName       = section->mSymbolName;
    mSymbolAuthor     = section->mSymbolAuthor;
    mAssotiationTable = section->mAssotiationTable;
    }
  }




void SdSection::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  obj.insert( QString("SymbolName"), mSymbolName );
  obj.insert( QString("SymbolAuthor"), mSymbolAuthor );
  //TODO save pin assotioation
  //mAssotiationTable

  }




void SdSection::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mSymbolName   = obj.value( QString("SymbolName") ).toString();
  mSymbolAuthor = obj.value( QString("SymbolAuthor") ).toString();
  //TODO read pin assotiation
  }

