/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Project item object which can contains part link object (part variant)
*/
#include "SdPItemWithPart.h"

SdPItemWithPart::SdPItemWithPart()
  {

  }




//!
//! \brief getPartTitle Returns part title for visual presentation
//! \param id           Id of part variant which title need to be returned. If empty then return primary part title
//! \return             Part title for visual presentation
//!
QString SdPItemWithPart::getPartTitle(const QString id) const
  {
  SdPartVariant *part = getPart( id );
  if( part != nullptr )
    return part->getPartTitle();
  return QString();
  }




//!
//! \brief getPartId Returns part id for primary part variant
//! \return          Part id for primary part variant
//!
QString SdPItemWithPart::getPartId() const
  {
  SdPartVariant *part = getPart();
  if( part != nullptr )
    return part->getPartId();
  return QString();
  }





//!
//! \brief getPartIdList Returns part ids for all parts variants
//! \return              Part ids for all parts variants
//!
QStringList SdPItemWithPart::getPartIdList() const
  {
  QStringList idList;
  //Scan all objects and find SdPartVariant
  forEachConst( dctPartVariant, [&idList] (SdObject *obj) -> bool {
    SdPartVariant *part = dynamic_cast<SdPartVariant*>(obj);
    Q_ASSERT( part != nullptr );
    idList.append( part->getPartId() );
    return true;
    });
  return idList;
  }





//!
//! \brief setPartId Setup new part id for primary part variant
//! \param id        New id for primary part
//! \param undo
//!
void SdPItemWithPart::setPartId(const QString id, SdUndo *undo)
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




//!
//! \brief extractPartFromFactory Return part descripted part variant
//! \param id                     Id of part which need to returned. If empty return primary part
//! \return                       Part descripted part variant
//!
SdPItemPart *SdPItemWithPart::extractPartFromFactory(const QString id) const
  {
  SdPartVariant *part = getPart( id );
  if( part != nullptr )
    return part->extractFromFactory();
  return nullptr;
  }




//!
//! \brief getPart Returns part variant for id. If id is empty then return primary part variant
//! \param id      Id of part which variant need to returned. If empty return primary part variant
//! \return
//!
SdPartVariant *SdPItemWithPart::getPart(const QString id) const
  {
  //Pointer where reside SdPartVariant
  SdPartVariantPtr part = nullptr;
  //Scan all objects and find SdPartVariant
  forEachConst( dctPartVariant, [&part,id] (SdObject *obj) -> bool {
    part = dynamic_cast<SdPartVariant*>(obj);
    Q_ASSERT( part != nullptr );
    return !(id.isEmpty() || part->getPartId() == id);
    });
  return part;
  }




//!
//! \brief removePart Removes part variant from part list
//! \param id         Id of part which variant need to be removed
//! \param undo
//!
void SdPItemWithPart::removePart(const QString id, SdUndo *undo)
  {
  SdPartVariant *part = getPart( id );
  if( part != nullptr )
    deleteChild( part, undo );
  }


