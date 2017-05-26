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
#include "SdSection.h"
#include "SdPartVariant.h"
#include "SdObjectFactory.h"
#include "SdGraphIdent.h"
#include "SdProject.h"

SdPItemSymbol::SdPItemSymbol()
  {

  }




//Return section count
int SdPItemSymbol::getSectionCount() const
  {
  int cnt = 0;
  for( SdObject *obj : mChildList ) {
    if( obj && !obj->isDeleted() && obj->getClass() == dctSection ) {
      SdSection *section = dynamic_cast<SdSection*>( obj );
      Q_ASSERT( section != 0 );
      cnt++;
      }
    }
  return cnt;
  }




SdSection *SdPItemSymbol::getSection(int sectionIndex) const
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




SdPartVariant *SdPItemSymbol::getPart(int partIndex) const
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




SdPartVariant *SdPItemSymbol::getDefaultPart() const
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




//Set new default part
void SdPItemSymbol::setDefaultPart(SdPartVariant *partVar)
  {
  forEach( dctPartVariant, [partVar] (SdObject *obj) -> bool {
    SdPartVariant *pv = dynamic_cast<SdPartVariant*>( obj );
    Q_ASSERT(pv);
    pv->setDefault( pv == partVar );
    return true;
    });
  }




SdPItemSymbol *SdPItemSymbol::extractSymbolFromFactory(int sectionIndex, bool soft, QWidget *parent) const
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec )
    return sec->extractFromFactory( soft, parent );
  return nullptr;
  }




SdPItemPart *SdPItemSymbol::extractPartFromFactory(int partIndex, bool soft, QWidget *parent) const
  {
  SdPartVariant *prt = getPart( partIndex );
  if( prt ) prt->extractFromFactory( soft, parent );
  return nullptr;
  }




SdPItemPart *SdPItemSymbol::extractDefaultPartFromFacory(bool soft, QWidget *parent) const
  {
  SdPartVariant *prt = getDefaultPart();
  if( prt ) prt->extractFromFactory( soft, parent );
  return nullptr;
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
  return QString(":/pic/iconSymbol.png");
  }


void SdPItemSymbol::cloneFrom( const SdObject *src )
  {
  SdProjectItem::cloneFrom( src );
  }



SdGraphIdent *SdPItemSymbol::createIdent()
  {
  SdGraphIdent *ident = new SdGraphIdent( SdPoint(), QStringLiteral("Id"), SdRect(), sdGlobalProp->mSymIdentProp );
  SdProject *prj = getProject();
  Q_ASSERT( prj != nullptr );
  insertChild( ident, prj->getUndo() );
  return ident;
  }
