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
#include "SdPartVariant.h"
#include "library/SdLibraryStorage.h"
#include "SdProject.h"
#include "SdConfig.h"

#include <QDebug>

SdPItemComponent::SdPItemComponent() :
  SdPItemVariant()
  {

  }






//Part title for visual presentation
QString SdPItemComponent::getPartTitle() const
  {
  SdPartVariant *part = getPart();
  if( part != nullptr )
    return part->getPartTitle();
  return QString();
  }



//Part id
QString SdPItemComponent::getPartId() const
  {
  SdPartVariant *part = getPart();
  if( part != nullptr )
    return part->getPartId();
  return QString();
  }




//Setup new part id
void SdPItemComponent::setPartId(const QString id, SdUndo *undo)
  {
  SdPartVariant *part = getPart();
  if( part == nullptr ) {
    //Part yet not present
    //Insert part variant
    part = new SdPartVariant();
    insertChild( part, undo );
    }
  part->setPartId( id, undo );
  }




//Return part descripted part variant
SdPItemPart *SdPItemComponent::extractPartFromFactory() const
  {
  SdPartVariant *part = getPart();
  if( part != nullptr )
    return part->extractFromFactory();
  return nullptr;
  }




//Return default part
SdPartVariant *SdPItemComponent::getPart() const
  {
  //Pointer where reside SdPartVariant
  SdPartVariantPtr part = nullptr;
  //Scan all objects and find SdPartVariant
  forEachConst( dctPartVariant, [&part] (SdObject *obj) -> bool {
    part = dynamic_cast<SdPartVariant*>(obj);
    Q_ASSERT( part != nullptr );
    return false;
    });
  return part;
  }






//Return section count
int SdPItemComponent::getSectionCount() const
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
void SdPItemComponent::appendSection(const QString id, SdUndo *undo)
  {
  //qDebug() << "appendSection" << id;
  SdSection *section = new SdSection();
  insertChild( section, undo );
  section->setSymbolId( id, undo );
  }




//Section symbol title for visual presentation
QString SdPItemComponent::getSectionSymbolTitle(int sectionIndex) const
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec != nullptr )
    return sec->getSymbolTitle();
  return QString();
  }




//Section symbol id
QString SdPItemComponent::getSectionSymbolId(int sectionIndex) const
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec != nullptr )
    return sec->getSymbolId();
  return QString();
  }




//Setup new section symbol id
void SdPItemComponent::setSectionSymbolId(const QString id, int sectionIndex, SdUndo *undo)
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec != nullptr )
    sec->setSymbolId( id, undo );
  }



//Return section by index
SdSection *SdPItemComponent::getSection(int sectionIndex) const
  {
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
  SdSection *sec = getSection( sectionIndex );
  if( sec != nullptr )
    return sec->extractFromFactory();
  return nullptr;
  }




//Remove section
void SdPItemComponent::removeSection(int sectionIndex, SdUndo *undo)
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec != nullptr )
    sec->deleteObject( undo );
  }




//Return full section pin assotiation table
SdPinAssociation SdPItemComponent::getSectionPins(int sectionIndex) const
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec != nullptr )
    return sec->getPins();
  return SdPinAssociation();
  }




//Return individual pin number for desired pin name for section
QString SdPItemComponent::getSectionPinNumber(int sectionIndex, const QString pinName)
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec != nullptr )
    return sec->getPinNumber( pinName );
  return QString();
  }




//Setup new pin number for desired pin name for section
void SdPItemComponent::setSectionPinNumber(int sectionIndex, const QString pinName, const QString pinNumber, SdUndo *undo)
  {
  SdSection *sec = getSection( sectionIndex );
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
  comp->appendSection( symbol->getUid(), symbol->getUndo() );

  if( appendDefaultPart ) {
    //Setup default part
    comp->setPartId( QString("Part\rplug part\rsalicad"), symbol->getUndo() );

    //Setup default packing info
    SdPinAssociation pins = comp->getSectionPins(0);
    int pin = 1;
    for( auto iter = pins.cbegin(); iter != pins.cend(); ++iter ) {
      comp->setSectionPinNumber( 0, iter.key(), QString::number(pin++), symbol->getUndo() );
      }

    //Fix component
    //comp->setEditEnable( false, QString("Component fixed") );
    }
  return comp;
  }
