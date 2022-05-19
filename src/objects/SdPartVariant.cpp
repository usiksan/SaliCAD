/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Part variant contains one part variant for component. Variants may be
  different component placement with the same part or different parts of one component.
*/
#include "SdPartVariant.h"
#include "SdObjectFactory.h"
#include "SdPItemPart.h"

SdPartVariant::SdPartVariant() :
  SdObject(),
  mDefault(false)
  {

  }



SdPItemPart *SdPartVariant::extractFromFactory(bool soft, QWidget *parent) const
  {
  return dynamic_cast<SdPItemPart*>( SdObjectFactory::extractObject( mPartId, soft, parent ) );
  }




void SdPartVariant::setPartId(const QString id, SdUndo *undo)
  {
  SdLibraryHeader hdr;
  if( SdObjectFactory::extractHeader(id, hdr) ) {
    if( undo != nullptr )
      undo->string2( &mPartId, &mPartTitle );
    mPartId = id;
    mPartTitle = QString( "%1 (%2)" ).arg(hdr.mName).arg(hdr.mAuthor);
    }
  }








QString SdPartVariant::getType() const
  {
  return QStringLiteral( SD_TYPE_PART_VARIANT );
  }





quint64 SdPartVariant::getClass() const
  {
  return dctPartVariant;
  }






//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdPartVariant::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdObject::cloneFrom( src, copyMap, next );
  SdPtrConst<SdPartVariant> part(src);
  if( part.isValid() ) {
    mPartTitle = part->mPartTitle;
    mPartId    = part->mPartId;
    mDefault   = part->mDefault;
    }
  }









void SdPartVariant::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  obj.insert( QString("PartTitle"), mPartTitle );
  obj.insert( QString("PartId"), mPartId );
  obj.insert( QString("Default"), mDefault );
  }




void SdPartVariant::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mPartTitle = obj.value( QString("PartTitle") ).toString();
  mPartId    = obj.value( QString("PartId") ).toString();
  mDefault   = obj.value( QString("Default") ).toBool();
  }


