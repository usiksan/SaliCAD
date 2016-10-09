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

SdProjectItem::SdProjectItem()
  {

  }

void SdProjectItem::setTitle(const QString title)
  {
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
  }


void SdProjectItem::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mTitle = obj.value( QStringLiteral("Title") ).toString();
  }
