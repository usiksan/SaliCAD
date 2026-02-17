/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Pin-to-pad association table map as object. It allow to save it in library
  and retrive from external library for share using.
*/
#include "SdPadAssociation.h"
#include "SdProjectItem.h"
#include "SdUtil.h"
#include "SdJsonIO.h"
#include "library/SdLibraryHeader.h"

SdPadAssociation::SdPadAssociation()
  {
  }

SdPadAssociation::SdPadAssociation(const SdPadMap map, const QString name, const QString author) :
  mMap(map),
  mName(name),
  mAuthor(author)
  {
  }




QString SdPadAssociation::getUid() const
  {
  return headerUid( getType(), mName, mAuthor );
  }




void SdPadAssociation::setMap(const QString name, const SdPadMap &src, SdUndo *undo)
  {
  undo->prop( &mAuthor, &mName, &mMap );
  mName   = name;
  mMap    = src;
  mAuthor = SdProjectItem::getDefaultAuthor();
  }






QString SdPadAssociation::getType() const
  {
  return QStringLiteral(SD_TYPE_PAD_ASSOCIATION);
  }





quint64 SdPadAssociation::getClass() const
  {
  return dctPadAssociation;
  }




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdPadAssociation::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdPtrConst<SdPadAssociation> pads(src);
  if( pads.isValid() ) {
    SdObject::cloneFrom( src, copyMap, next );
    mMap = pads->mMap;
    mName = pads->mName;
    mAuthor = pads->mAuthor;
    }
  }



//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdPadAssociation::json(SdJsonWriter &js) const
  {
  js.jsonString( QStringLiteral("pad association name"), mName );
  js.jsonString( QStringLiteral("pad association author"), mAuthor );
  js.jsonMap( js, QStringLiteral("association"), mMap );
  SdObject::json( js );
  }



//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdPadAssociation::json(const SdJsonReader &js)
  {
  js.jsonString( QStringLiteral("pad association name"), mName );
  js.jsonString( QStringLiteral("pad association author"), mAuthor );
  js.jsonMap( js, QStringLiteral("association"), mMap );
  SdObject::json( js );
  }



