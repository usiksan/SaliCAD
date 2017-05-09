/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdPItemComponent.h"
#include "SdSection.h"
#include "SdPartVariant.h"
#include "SdObjectFactory.h"

SdPItemComponent::SdPItemComponent()
  {

  }

SdSection *SdPItemComponent::getSection(int sectionIndex) const
  {
  for( SdObject *obj : mChildList ) {
    if( obj && !obj->isDeleted() && obj->getClass() == dctSection ) {
      SdSection *section = dynamic_cast<SdSection*>( obj );
      Q_ASSERT( section != 0 );
      if( sectionIndex == 0 ) return section;
      sectionIndex--;
      }
    }
  return 0;
  }




SdPartVariant *SdPItemComponent::getPart(int partIndex) const
  {
  for( SdObject *obj : mChildList ) {
    if( obj && !obj->isDeleted() && obj->getClass() == dctPartVariant ) {
      SdPartVariant *part = dynamic_cast<SdPartVariant*>( obj );
      Q_ASSERT( part != 0 );
      if( partIndex == 0 ) return part;
      partIndex--;
      }
    }
  return 0;
  }




SdPartVariant *SdPItemComponent::getDefaultPart() const
  {
  for( SdObject *obj : mChildList ) {
    if( obj && !obj->isDeleted() && obj->getClass() == dctPartVariant ) {
      SdPartVariant *part = dynamic_cast<SdPartVariant*>( obj );
      Q_ASSERT( part != 0 );
      if( part->isDefault() ) return part;
      }
    }
  return 0;
  }




SdPItemSymbol *SdPItemComponent::extractSymbolFromFactory(int sectionIndex, bool soft, QWidget *parent) const
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec )
    return sec->extractFromFactory( soft, parent );
  return nullptr;
  }




SdPItemPart *SdPItemComponent::extractPartFromFactory(int partIndex, bool soft, QWidget *parent) const
  {
  SdPartVariant *prt = getPart( partIndex );
  if( prt ) prt->extractFromFactory( soft, parent );
  return nullptr;
  }




SdPItemPart *SdPItemComponent::extractDefaultPartFromFacory(bool soft, QWidget *parent) const
  {
  SdPartVariant *prt = getDefaultPart();
  if( prt ) prt->extractFromFactory( soft, parent );
  return nullptr;
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
  return QString(":/pic/iconProject.png");
  }
