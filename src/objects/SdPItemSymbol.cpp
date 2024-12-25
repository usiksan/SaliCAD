/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Graphical symbol presentation
*/

#include "SdPItemSymbol.h"
#include "SdGraphSymPin.h"
#include "library/SdLibraryStorage.h"


SdPItemSymbol::SdPItemSymbol() :
  SdPItemVariant()
  {

  }




QString SdPItemSymbol::getType() const
  {
  return QStringLiteral(SD_TYPE_SYMBOL);
  }




quint64 SdPItemSymbol::getClass() const
  {
  return dctSymbol;
  }




QString SdPItemSymbol::getIconName() const
  {
  if( !isEditEnable() ) {
    if( mThereNewer )
      return QStringLiteral(":/pic/iconSymbolLockedNew.png");
    return QStringLiteral(":/pic/iconSymbolLocked.png");
    }
  return QStringLiteral(":/pic/iconSymbol.png");
  }




quint64 SdPItemSymbol::getAcceptedObjectsMask() const
  {
  return dctSymbolObjects;
  }







//!
//! \brief sectionIsAvailable Return true if section with sectionIndex available
//! \param sectionIndex       Section index
//! \return                   true if section with sectionIndex available
//!
bool SdPItemSymbol::sectionIsAvailable(int sectionIndex) const
  {
  return sectionIndex >= 0 && sectionIndex < sectionCount();
  }



//!
//! \brief sectionCount Returns section count
//! \return                Section count
//!
int SdPItemSymbol::sectionCount() const
  {
  return 1;// partGet() == nullptr ? 0 : 1;
  }


//!
//! \brief sectionSymbolTitleGet Returns section symbol title for visual presentation
//! \param sectionIndex          Section index
//! \return                      Section symbol title for visual presentation
//!
QString SdPItemSymbol::sectionSymbolTitleGet(int sectionIndex) const
  {
  if( sectionIndex == 0 )
    return getTitle();
  return QString{};
  }



//!
//! \brief sectionSymbolIdGet Returns section symbol id
//! \param sectionIndex       Section index
//! \return                   Section symbol id
//!
QString SdPItemSymbol::sectionSymbolIdGet(int sectionIndex) const
  {
  if( sectionIndex == 0 )
    return getUid();
  return QString{};
  }



//!
//! \brief sectionPinNumberGet Return individual pin number for desired pin name for section
//! \param sectionIndex        Section index
//! \param pinName             Pin name which number must be retrived
//! \return                    Pin number for pin name of section with index sectionIndex
//!
QString SdPItemSymbol::sectionPinNumberGet(int sectionIndex, const QString pinName) const
  {
  Q_UNUSED(sectionIndex)
  //Scan symbol pins and find that with pinName
  QString pinNumber;
  forEachConst( dctSymPin, [&pinNumber,pinName] ( SdObject *obj ) -> bool {
    auto pin = obj->convertConstPtr<SdGraphSymPin>();
    if( pin != nullptr && pin->getPinName() == pinName ) {
      pinNumber = pin->getPinNumber();
      return false;
      }
    return true;
    } );
  return pinNumber;
  }





//!
//! \brief extractSymbolFromFactory Return symbol from section by index
//! \param sectionIndex             Section index
//! \return                         Symbol extracted from factory
//!
SdPItemSymbol *SdPItemSymbol::extractSymbolFromFactory(int sectionIndex) const
  {
  if( sectionIndex == 0 )
    return sdObjectOnly<SdPItemSymbol>( SdLibraryStorage::instance()->cfObjectGet( getUid() ) );
  return nullptr;
  }


