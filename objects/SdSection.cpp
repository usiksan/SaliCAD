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







void SdSection::setSymbolId(const QString id, SdUndo *undo)
  {
  SdObject *obj = SdObjectFactory::extractObject( id, true, nullptr );
  SdPItemSymbol *symbol = dynamic_cast<SdPItemSymbol*>(obj);
  if( symbol != nullptr ) {
    //Store previous state
    undo->string2( &mSymbolId, &mSymbolTitle );
    undo->stringMap( &mAssociationTable );
    //Setup new symbol info
    mSymbolId = symbol->getId();
    mSymbolTitle = QString( "%1 (%2)" ).arg(symbol->getTitle()).arg(symbol->getAuthor());
    //Accum pins
    SdPinAssociation pins;
    symbol->forEach( dctSymPin, [&pins, this] (SdObject *obj) -> bool {
      SdGraphSymPin *pin = dynamic_cast<SdGraphSymPin*>( obj );
      if( pin != nullptr )
        pins.insert( pin->getPinName(), mAssociationTable.value(pin->getPinName()) );
      return true;
      } );
    mAssociationTable = pins;
    }
  if( obj != nullptr )
    delete obj;
  }




SdPItemSymbol *SdSection::extractFromFactory( bool soft, QWidget *parent) const
  {
  return dynamic_cast<SdPItemSymbol*>( SdObjectFactory::extractObject( mSymbolId, soft, parent ) );
  }




void SdSection::setPinNumber(const QString name, const QString number, SdUndo *undo)
  {
  undo->stringMapItem( &mAssociationTable, name );
  mAssociationTable.insert( name, number );
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
    mAssociationTable = section->mAssociationTable;
    }
  }




void SdSection::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  obj.insert( QStringLiteral("SymbolTitle"), mSymbolTitle );
  obj.insert( QStringLiteral("SymbolId"), mSymbolId );
  sdStringMapWrite( QStringLiteral("PinAssotiation"), mAssociationTable, obj );
  }




void SdSection::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mSymbolTitle = obj.value( QStringLiteral("SymbolTitle") ).toString();
  mSymbolId    = obj.value( QStringLiteral("SymbolId") ).toString();
  sdStringMapRead( QStringLiteral("PinAssotiation"), mAssociationTable, obj );
  }

