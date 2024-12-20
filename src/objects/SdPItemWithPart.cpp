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
//! \brief partTitleGet Returns part title for visual presentation
//! \param id           Id of part variant which title need to be returned. If empty then return primary part title
//! \return             Part title for visual presentation
//!
QString SdPItemWithPart::partTitleGet() const
  {
  SdPartVariant *part = partGet();
  if( part != nullptr )
    return part->getPartTitle();
  return QString();
  }




//!
//! \brief partIdGet Returns part id for primary part variant
//! \return          Part id for primary part variant
//!
QString SdPItemWithPart::partIdGet() const
  {
  SdPartVariant *part = partGet();
  if( part != nullptr )
    return part->getPartId();
  return QString();
  }





//!
//! \brief partIdSet Setup new part id for primary part variant
//! \param id        New id for primary part
//! \param undo
//!
void SdPItemWithPart::partIdSet(const QString id, SdUndo *undo)
  {
  SdPartVariant *part = partGet();
  if( part == nullptr ) {
    //Part yet not present
    //Insert part variant
    part = new SdPartVariant();
    insertChild( part, undo );
    }
  part->setPartId( id, undo );
  }




//!
//! \brief partExtractFromFactory Return part descripted part variant
//! \return                       Part descripted part variant
//!
SdPItemPart *SdPItemWithPart::partExtractFromFactory() const
  {
  SdPartVariant *part = partGet();
  if( part != nullptr )
    return part->extractFromFactory();
  return nullptr;
  }




//!
//! \brief partGet Returns part variant for id. If id is empty then return primary part variant
//! \return
//!
SdPartVariant *SdPItemWithPart::partGet() const
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




//!
//! \brief partRemove Removes part variant from part list
//! \param id         Id of part which variant need to be removed
//! \param undo
//!
void SdPItemWithPart::partRemove(SdUndo *undo)
  {
  SdPartVariant *part = partGet();
  if( part != nullptr )
    deleteChild( part, undo );
  }


