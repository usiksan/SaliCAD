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
#include "library/SdLibraryStorage.h"
#include "SdPItemPart.h"
#include "SdJsonIO.h"

SdPartVariant::SdPartVariant() :
  SdObject(),
  mDefault(false)
  {

  }



SdPItemPart *SdPartVariant::extractFromFactory() const
  {
  return sdObjectOnly<SdPItemPart>( SdLibraryStorage::instance()->cfObjectGet( mPartId ) );
  }




void SdPartVariant::setPartId(const QString id, SdUndo *undo)
  {
  SdLibraryHeader hdr;
  if( SdLibraryStorage::instance()->header( id, hdr) ) {
    if( undo != nullptr )
      undo->string2( &mPartId, &mPartTitle );
    mPartId = id;
    mPartTitle = QString( "%1 (%2)" ).arg( hdr.mName, hdr.authorGlobalName() );
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



//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdPartVariant::json(SdJsonWriter &js) const
  {
  js.jsonString( QStringLiteral("PartTitle"), mPartTitle );
  js.jsonString( QStringLiteral("PartId"), mPartId );
  js.jsonBool( QString("Default"), mDefault );
  SdObject::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdPartVariant::json(const SdJsonReader &js)
  {
  js.jsonString( QStringLiteral("PartTitle"), mPartTitle );
  js.jsonString( QStringLiteral("PartId"), mPartId );
  js.jsonBool( QString("Default"), mDefault );
  SdObject::json( js );
  }



