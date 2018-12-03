/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
  Component inheritance which specify component params
  Component is symbol without graph bun contains symbol sections with pin (name-number) assotiation and component part
*/
#include "SdPItemInheritance.h"
#include "SdObjectFactory.h"
#include "SdTime2x.h"

SdPItemInheritance::SdPItemInheritance() :
  SdPItemVariant()
  {

  }






//Set component uid
void SdPItemInheritance::setComponentUid(const QString uid, SdUndo *undo)
  {
  SdLibraryHeader hdr;
  if( SdObjectFactory::extractHeader( uid, hdr ) ) {
    undo->string2( &mComponentUid, &mComponentTitle );
    mComponentTitle = QString("%1 (%2) [%3]").arg(hdr.mName).arg(hdr.mAuthor).arg(SdTime2x::toLocalString(hdr.mTime));
    mComponentUid   = uid;
    }
  }




QString SdPItemInheritance::getType() const
  {
  return QStringLiteral( SD_TYPE_INHERITANCE );
  }




SdClass SdPItemInheritance::getClass() const
  {
  return dctInheritance;
  }




QString SdPItemInheritance::getIconName() const
  {
  if( !isEditEnable() ) {
    if( mThereNewer )
      return QStringLiteral(":/pic/iconInheritanceNew.png");
    return QStringLiteral(":/pic/iconInheritanceLocked.png");
    }
  return QStringLiteral(":/pic/iconInheritance.png");
  }




quint64 SdPItemInheritance::getAcceptedObjectsMask() const
  {
  return 0;
  }




void SdPItemInheritance::getHeader(SdLibraryHeader &hdr) const
  {
  SdPItemVariant::getHeader( hdr );
  hdr.mInherit = mComponentUid;
  }




void SdPItemInheritance::cloneFrom(const SdObject *src)
  {
  SdPItemVariant::cloneFrom( src );
  const SdPItemInheritance *inh = dynamic_cast<const SdPItemInheritance*>(src);
  if( inh ) {
    mComponentUid   = inh->mComponentUid;
    mComponentTitle = inh->mComponentTitle;
    }
  }




void SdPItemInheritance::writeObject(QJsonObject &obj) const
  {
  SdPItemVariant::writeObject( obj );
  obj.insert( QStringLiteral("CompUid"), mComponentUid );
  obj.insert( QStringLiteral("CompTitle"), mComponentTitle );
  }




void SdPItemInheritance::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdPItemVariant::readObject( map, obj );
  mComponentUid   = obj.value( QStringLiteral("CompUid") ).toString();
  mComponentTitle = obj.value( QStringLiteral("CompTitle") ).toString();
  }

