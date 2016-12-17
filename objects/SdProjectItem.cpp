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
#include <QSettings>

SdProjectItem::SdProjectItem() :
  mRefCount(0),
  mAuto(true)
  {

  }




QString SdProjectItem::getId() const
  {
  return mObjectInfo.getId();
  }




QString SdProjectItem::getIdFileName() const
  {
  return mObjectInfo.getIdFileName();
  }





QString SdProjectItem::getExtendTitle() const
  {
  return mObjectInfo.getExtendTitle();
  }




void SdProjectItem::setTitle(const QString title)
  {
  mObjectInfo.setTitle( title );
  //If false - item in edit state, when true - item can place in library
  mFixed    = false;
  SdPulsar::pulsar->emitRenameItem( this );
  }




SdProject *SdProjectItem::getProject() const
  {
  return dynamic_cast<SdProject*>( getParent() );
  }





void SdProjectItem::writeObject(QJsonObject &obj) const
  {
  SdContainer::writeObject( obj );
  mObjectInfo.writeObject( obj );
  obj.insert( QStringLiteral("Fixed"),    mFixed );
  obj.insert( QStringLiteral("RefCount"), mRefCount );
  obj.insert( QStringLiteral("Auto"),     mAuto );
  }




void SdProjectItem::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mObjectInfo.readObject( obj );
  mFixed    = obj.value( QStringLiteral("Fixed") ).toBool();
  mRefCount = obj.value( QStringLiteral("RefCount") ).toInt();
  mAuto     = obj.value( QStringLiteral("Auto") ).toBool();
  }




void SdProjectItem::cloneFrom( const SdObject *src )
  {
  SdContainer::cloneFrom( src );
  const SdProjectItem *sour = dynamic_cast<const SdProjectItem*>(src);
  mObjectInfo.copyFrom( sour->mObjectInfo );
  mFixed    = sour->mFixed;
  mRefCount = 0;
  mAuto     = true;
  }
