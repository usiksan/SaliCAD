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

QString SdSection::getTitle() const
  {
  return QString( "%1 (%2)" ).arg(mSymbolTitle).arg(mSymbolAuthor);
  }



void SdSection::updateFromSymbol(SdPItemSymbol *symbol)
  {
  mSymbolTitle = symbol->getTitle();
  mSymbolAuthor = symbol->getAuthor();
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
  return dynamic_cast<SdPItemSymbol*>( SdObjectFactory::extractObject( mSymbolTitle, mSymbolAuthor, soft, parent ) );
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
    mSymbolTitle       = section->mSymbolTitle;
    mSymbolAuthor     = section->mSymbolAuthor;
    mAssotiationTable = section->mAssotiationTable;
    }
  }




void SdSection::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  obj.insert( QStringLiteral("SymbolName"), mSymbolTitle );
  obj.insert( QStringLiteral("SymbolAuthor"), mSymbolAuthor );
  QJsonObject pins;
  for( auto i = mAssotiationTable.constBegin(); i != mAssotiationTable.constEnd(); i++ )
    pins.insert( i.key(), i.value() );
  obj.insert( QStringLiteral("PinAssotiation"), pins );
  }




void SdSection::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mSymbolTitle   = obj.value( QStringLiteral("SymbolName") ).toString();
  mSymbolAuthor = obj.value( QStringLiteral("SymbolAuthor") ).toString();
  QJsonObject pins = obj.value( QStringLiteral("PinAssotiation") ).toObject();
  for( auto i = pins.constBegin(); i != pins.constEnd(); i++ ) {
//    mPinList.append( i.key() );
    mAssotiationTable.insert( i.key(), i.value().toString() );
    }
  }

