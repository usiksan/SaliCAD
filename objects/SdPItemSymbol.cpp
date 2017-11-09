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

SdPItemSymbol::SdPItemSymbol() :
  SdProjectItem()
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



//Return section with desired index
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








//Return default part
SdPartVariant *SdPItemSymbol::getPart() const
  {
  for( SdObject *obj : mChildList ) {
    if( obj && !obj->isDeleted() && obj->getClass() == dctPartVariant ) {
      SdPartVariant *part = dynamic_cast<SdPartVariant*>( obj );
      Q_ASSERT( part != 0 );
      return part;
      }
    }
  return nullptr;
  }








//Return symbol from section by index
SdPItemSymbol *SdPItemSymbol::extractSymbolFromFactory(int sectionIndex, bool soft, QWidget *parent) const
  {
  SdSection *sec = getSection( sectionIndex );
  if( sec )
    return sec->extractFromFactory( soft, parent );
  return nullptr;
  }





//Return part descripted part variant
SdPItemPart *SdPItemSymbol::extractPartFromFactory(bool soft, QWidget *parent) const
  {
  SdPartVariant *prt = getPart();
  if( prt )
    return prt->extractFromFactory( soft, parent );
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




quint64 SdPItemSymbol::getAcceptedObjectsMask() const
  {
  return dctSymbolObjects;
  }





//void SdPItemSymbol::insertObjects(SdPoint offset, SdSelector *sel, SdUndo *undo, SdWEditorGraph *editor, SdSelector *dest, bool next)
//  {

//  }








void SdPItemSymbol::cloneFrom( const SdObject *src )
  {
  SdProjectItem::cloneFrom( src );
  }





void SdPItemSymbol::insertChild(SdObject *child, SdUndo *undo)
  {
  Q_ASSERT( child != nullptr );
  if( child->getClass() == dctPartVariant ) {
    //If part present then remove it
    SdPartVariant *part = getPart();
    if( part != nullptr )
      part->deleteObject( undo );
    //Insert new part
    SdProjectItem::insertChild( child, undo );
    }
  else SdProjectItem::insertChild( child, undo );
  }
