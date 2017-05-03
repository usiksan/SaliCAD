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
#include "SdPoint.h"
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
  obj.insert( QStringLiteral("RefCount"), mRefCount );
  obj.insert( QStringLiteral("Auto"),     mAuto );
  sdParamWrite( QStringLiteral("Parametrs"), mParamTable, obj );
  mOrigin.write( QStringLiteral("Origin"), obj );
  }




void SdProjectItem::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mObjectInfo.readObject( obj );
  mRefCount = obj.value( QStringLiteral("RefCount") ).toInt();
  mAuto     = obj.value( QStringLiteral("Auto") ).toBool();
  sdParamRead( QStringLiteral("Parametrs"), mParamTable, obj );
  mOrigin.read( QStringLiteral("Origin"), obj );
  }




void SdProjectItem::cloneFrom( const SdObject *src )
  {
  SdContainer::cloneFrom( src );
  const SdProjectItem *sour = dynamic_cast<const SdProjectItem*>(src);
  mObjectInfo.copyFrom( sour->mObjectInfo );
  mRefCount   = 0;
  mAuto       = true;
  mParamTable = sour->mParamTable;
  }
