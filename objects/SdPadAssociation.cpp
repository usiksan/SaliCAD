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
  undo->padAssociation( &mAuthor, &mName, &mMap );
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





void SdPadAssociation::cloneFrom(const SdObject *src)
  {
  const SdPadAssociation *pads = dynamic_cast<const SdPadAssociation*>(src);
  if( pads != nullptr ) {
    SdObject::cloneFrom( src );
    mMap = pads->mMap;
    mName = pads->mName;
    mAuthor = pads->mAuthor;
    }
  }





void SdPadAssociation::writeObject(QJsonObject &obj) const
  {
  SdObject::writeObject( obj );
  obj.insert( QStringLiteral("pad association name"), mName );
  obj.insert( QStringLiteral("pad association author"), mAuthor );
  QJsonObject ar;
  for( auto iter = mMap.cbegin(); iter != mMap.cend(); iter++ )
    ar.insert( iter.key(), iter.value().write() );
  obj.insert( QStringLiteral("association"), ar );
  }



void SdPadAssociation::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdObject::readObject( map, obj );
  mName = obj.value( QStringLiteral("pad association name") ).toString();
  mAuthor = obj.value( QStringLiteral("pad association author") ).toString();
  QJsonObject ar = obj.value( QStringLiteral("association") ).toObject();
  SdPad pad;
  for( auto iter = ar.constBegin(); iter != ar.constEnd(); iter++ ) {
    pad.read( iter.value().toObject() );
    mMap.insert( iter.key(), pad );
    }
  }


