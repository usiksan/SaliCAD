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
#include "SdGraphSymPin.h"
#include "SdObjectFactory.h"

SdSection::SdSection() :
  SdObject()
  {

  }




void SdSection::updateFromSymbol(SdPItemSymbol *symbol)
  {
  mSymbolId = symbol->getId();
  mSymbolTitle = QString( "%1 (%2)" ).arg(symbol->getTitle()).arg(symbol->getAuthor());
  SdPinAssotiation pins;
  symbol->forEach( dctSymPin, [&pins, this] (SdObject *obj) -> bool {
    SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>( obj );
    if( pin ) {
      pins.insert( pin->getPinName(), mAssotiationTable.value(pin->getPinName()) );
      }
    return true;
    } );
  mAssotiationTable = pins;
  }




SdPItemSymbol *SdSection::extractFromFactory( bool soft, QWidget *parent) const
  {
  return dynamic_cast<SdPItemSymbol*>( SdObjectFactory::extractObject( mSymbolId, soft, parent ) );
  }




void SdSection::setPinNumber(const QString name, const QString number)
  {
  mAssotiationTable.insert( name, number );
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
    mSymbolTitle      = section->mSymbolTitle;
    mSymbolId         = section->mSymbolId;
    mAssotiationTable = section->mAssotiationTable;
    }
  }




void SdSection::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  obj.insert( QStringLiteral("SymbolTitle"), mSymbolTitle );
  obj.insert( QStringLiteral("SymbolId"), mSymbolId );
  QJsonObject pins;
  for( auto i = mAssotiationTable.constBegin(); i != mAssotiationTable.constEnd(); i++ )
    pins.insert( i.key(), i.value() );
  obj.insert( QStringLiteral("PinAssotiation"), pins );
  }




void SdSection::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mSymbolTitle = obj.value( QStringLiteral("SymbolTitle") ).toString();
  mSymbolId    = obj.value( QStringLiteral("SymbolId") ).toString();
  QJsonObject pins = obj.value( QStringLiteral("PinAssotiation") ).toObject();
  for( auto i = pins.constBegin(); i != pins.constEnd(); i++ )
    mAssotiationTable.insert( i.key(), i.value().toString() );
  }

