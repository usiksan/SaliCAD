/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Component is symbol without graph bun contains symbol sections with pin (name-number) assotiation and component part
*/
#include "SdPItemComponent.h"
#include "SdPItemSymbol.h"
#include "library/SdLibraryStorage.h"
#include "SdProject.h"

#include <QDebug>

SdPItemComponent::SdPItemComponent() :
  SdPItemVariant()
  {

  }




//!
//! \brief sectionIsAvailable Return true if section with sectionIndex available
//! \param sectionIndex       Section index
//! \return                   true if section with sectionIndex available
//!
bool SdPItemComponent::sectionIsAvailable(int sectionIndex) const
  {
  return sectionGet( sectionIndex ) != nullptr;
  }








//Return section count
int SdPItemComponent::sectionCount() const
  {
  int cnt = 0;
  //Scan all objects and count SdSection
  forEachConst( dctSection, [&cnt] (SdObject *obj) -> bool {
    SdSection *section = dynamic_cast<SdSection*>( obj );
    Q_ASSERT( section != nullptr );
    cnt++;
    return true;
    });
  return cnt;
  }




//Append section with symbol id. May be empty
void SdPItemComponent::sectionAppend(const QString id, SdUndo *undo)
  {
  //qDebug() << "appendSection" << id;
  SdSection *section = new SdSection();
  insertChild( section, undo );
  section->setSymbolId( id, undo );
  }




//Section symbol title for visual presentation
QString SdPItemComponent::sectionSymbolTitleGet(int sectionIndex) const
  {
  SdSection *sec = sectionGet( sectionIndex );
  if( sec != nullptr )
    return sec->getSymbolTitle();
  return QString();
  }




//Section symbol id
QString SdPItemComponent::sectionSymbolIdGet(int sectionIndex) const
  {
  SdSection *sec = sectionGet( sectionIndex );
  if( sec != nullptr )
    return sec->getSymbolId();
  return QString();
  }




//Setup new section symbol id
void SdPItemComponent::sectionSymbolIdSet(const QString id, int sectionIndex, SdUndo *undo)
  {
  SdSection *sec = sectionGet( sectionIndex );
  if( sec != nullptr )
    sec->setSymbolId( id, undo );
  }



//Return section by index
SdSection *SdPItemComponent::sectionGet(int sectionIndex) const
  {
  if( sectionIndex < 0 )
    return nullptr;
  SdSectionPtr section = nullptr;
  forEachConst( dctSection, [&section,&sectionIndex] (SdObject *obj) -> bool {
    if( sectionIndex == 0 ) {
      section = dynamic_cast<SdSection*>( obj );
      Q_ASSERT( section != nullptr );
      //Break repeation
      return false;
      }
    sectionIndex--;
    return true;
    });
  return section;
  }




//Return symbol from section by index
SdPItemSymbol *SdPItemComponent::extractSymbolFromFactory(int sectionIndex) const
  {
  SdSection *sec = sectionGet( sectionIndex );
  if( sec != nullptr )
    return sec->extractFromFactory();
  return nullptr;
  }




//Remove section
void SdPItemComponent::sectionRemove(int sectionIndex, SdUndo *undo)
  {
  SdSection *sec = sectionGet( sectionIndex );
  if( sec != nullptr )
    sec->deleteObject( undo );
  }




//Return full section pin assotiation table
SdPinAssociation SdPItemComponent::sectionPinsGet(int sectionIndex) const
  {
  SdSection *sec = sectionGet( sectionIndex );
  if( sec != nullptr )
    return sec->getPins();
  return SdPinAssociation();
  }




//Return individual pin number for desired pin name for section
QString SdPItemComponent::sectionPinNumberGet(int sectionIndex, const QString pinName) const
  {
  SdSection *sec = sectionGet( sectionIndex );
  if( sec != nullptr )
    return sec->getPinNumber( pinName );
  return QString();
  }




//Setup new pin number for desired pin name for section
void SdPItemComponent::sectionPinNumberSet(int sectionIndex, const QString pinName, const QString pinNumber, SdUndo *undo)
  {
  SdSection *sec = sectionGet( sectionIndex );
  if( sec != nullptr )
    return sec->setPinNumber( pinName, pinNumber, undo );
  }








QString SdPItemComponent::getType() const
  {
  return QString(SD_TYPE_COMPONENT);
  }




quint64 SdPItemComponent::getClass() const
  {
  return dctComponent;
  }




QString SdPItemComponent::getIconName() const
  {
  if( !isEditEnable() ) {
    if( mThereNewer )
      return QStringLiteral(":/pic/iconCompLockedNew.png");
    return QStringLiteral(":/pic/iconCompLocked.png");
    }
  return QStringLiteral(":/pic/iconComp.png");
  }






//Create default component with single section with given symbol
SdPItemComponent *sdCreateDefaultComponent(SdPItemSymbol *symbol, bool appendDefaultPart)
  {
  //Create component
  SdPItemComponent *comp = new SdPItemComponent();

  //Set same title as symbol
  comp->setTitle( symbol->getTitle(), QString("Component creation") );

  //Insert self to project
  symbol->getProject()->insertChild( comp, symbol->getUndo() );

  //Append symbol to library
  SdLibraryStorage::instance()->cfObjectInsert( symbol );
  //Append section with symbol
  comp->sectionAppend( symbol->getUid(), symbol->getUndo() );

  if( appendDefaultPart ) {
    //Setup default part
    comp->partIdSet( QString("Part\rplug part\rsalicad"), symbol->getUndo() );

    //Setup default packing info
    SdPinAssociation pins = comp->sectionPinsGet(0);
    int pin = 1;
    for( auto iter = pins.cbegin(); iter != pins.cend(); ++iter ) {
      comp->sectionPinNumberSet( 0, iter.key(), QString::number(pin++), symbol->getUndo() );
      }

    //Fix component
    //comp->setEditEnable( false, QString("Component fixed") );
    }
  return comp;
  }
