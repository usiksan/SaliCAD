/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/

#include "SdProjectItem.h"
#include "SdProject.h"
#include "SdPulsar.h"
#include "SdIds.h"

SdProjectItem::SdProjectItem() :
  mRefCount(0)
  {

  }

quint64 SdProjectItem::getId() const
  {
  if( mId == 0 ) mId = getGlobalId();
  return mId;
  }

void SdProjectItem::setTitle(const QString title)
  {
  mId = 0;
  mTitle = title;
  SdPulsar::pulsar->emitRenameItem( this );
  }

SdProject *SdProjectItem::getProject() const
  {
  return dynamic_cast<SdProject*>( getParent() );
  }



void SdProjectItem::writeObject(QJsonObject &obj) const
  {
  SdContainer::writeObject( obj );
  obj.insert( QStringLiteral("Title"), mTitle );
  obj.insert( QStringLiteral("RefCount"), mRefCount );
  }


void SdProjectItem::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mTitle = obj.value( QStringLiteral("Title") ).toString();
  mRefCount = obj.value( QStringLiteral("RefCount") ).toInt();
  }
