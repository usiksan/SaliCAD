/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Rich and plain text representation
*/
#include "SdPItemRich.h"
#include "SdJsonIO.h"

#include <QTextEdit>

SdPItemRich::SdPItemRich() :
  mTextEditor(nullptr)
  {

  }




QString SdPItemRich::getType() const
  {
  return QStringLiteral(SD_TYPE_RICH);
  }




SdClass SdPItemRich::getClass() const
  {
  return dctRich;
  }




//!
//! \brief cloneFrom Overrided function. We copy object from source
//! \param src       Source of object from which copy must be made
//! \param copyMap   Structure for mapping copying substitutes
//! \param next      Make simple or next copy. Next copy available not for all objects.
//!                  For example: pin name A23 with next copy return A24
//!
void SdPItemRich::cloneFrom(const SdObject *src, SdCopyMap &copyMap, bool next)
  {
  SdProjectItem::cloneFrom( src, copyMap, next );
  SdPtrConst<SdPItemRich> rich(src);
  if( rich.isValid() )
    mContents = rich->mContents;
  }




//!
//! \brief json Overloaded function to write object content into json writer
//!             Overrided function
//! \param js   Json writer
//!
void SdPItemRich::json(SdJsonWriter &js) const
  {
  if( mTextEditor )
    js.jsonString( QStringLiteral("contents"), mTextEditor->toHtml() );
  else
    js.jsonString( QStringLiteral("contents"), mContents );
  SdProjectItem::json( js );
  }




//!
//! \brief json Overloaded function to read object content from json reader
//!             Overrided function
//! \param js   Json reader
//!
void SdPItemRich::json(const SdJsonReader &js)
  {
  js.jsonString( QStringLiteral("contents"), mContents );
  SdProjectItem::json( js );
  }







QString SdPItemRich::getIconName() const
  {
  return QStringLiteral(":/pic/iconRich.png");
  }




quint64 SdPItemRich::getAcceptedObjectsMask() const
  {
  return 0ul;
  }
