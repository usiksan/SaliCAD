/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Symbol section is one of more schematic sections where all section
  declare full component.
*/
#include "SdSection.h"
#include "SdPItemSymbol.h"
#include "SdGraphSymPin.h"
#include "SdObjectFactory.h"
#include "SdJsonIO.h"

#include <memory>

SdSection::SdSection() :
  SdObject()
  {

  }







//Set new symbol id or update previously setuped
void SdSection::setSymbolId(const QString id, SdUndo *undo)
  {
  if( id.isEmpty() )
    return;
  std::unique_ptr<SdObject> obj( SdObjectFactory::extractObject( id, true, nullptr ) );
  SdPtr<SdPItemSymbol> symbol( obj.get() );
  if( symbol.isValid() ) {
    //Store previous state
    undo->string2( &mSymbolId, &mSymbolTitle );
    undo->stringMap( &mAssociationTable );
    //Setup new symbol info
    mSymbolId = symbol->getUid();
    mSymbolTitle = QString( "%1 (%2)" ).arg(symbol->getTitle()).arg(symbol->getAuthor());
    //Accum pins
    SdPinAssociation pins;
    symbol->forEach( dctSymPin, [&pins, this] (SdObject *obj) -> bool {
      SdPtr<SdGraphSymPin> pin( obj );
      if( pin.isValid() )
        pins.insert( pin->getPinName(), mAssociationTable.value(pin->getPinName()) );
      return true;
      } );
    mAssociationTable = pins;
    }
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




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdSection::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdObject::cloneFrom( src, copyMap, next );
  SdPtrConst<SdSection> section(src);
  if( section.isValid() ) {
    mSymbolTitle      = section->mSymbolTitle;
    mSymbolId         = section->mSymbolId;
    mAssociationTable = section->mAssociationTable;
    }
  }



//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdSection::json(SdJsonWriter &js) const
  {
  js.jsonString( QStringLiteral("SymbolTitle"), mSymbolTitle );
  js.jsonString( QStringLiteral("SymbolId"), mSymbolId );
  js.jsonMapString( QStringLiteral("PinAssotiation"), mAssociationTable );
  SdObject::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdSection::json(const SdJsonReader &js)
  {
  js.jsonString( QStringLiteral("SymbolTitle"), mSymbolTitle );
  js.jsonString( QStringLiteral("SymbolId"), mSymbolId );
  js.jsonMapString( QStringLiteral("PinAssotiation"), mAssociationTable );
  SdObject::json( js );
  }












void SdSection::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mSymbolTitle = obj.value( QStringLiteral("SymbolTitle") ).toString();
  mSymbolId    = obj.value( QStringLiteral("SymbolId") ).toString();
  sdStringMapRead( QStringLiteral("PinAssotiation"), mAssociationTable, obj );
  }

