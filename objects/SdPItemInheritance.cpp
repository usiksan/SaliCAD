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
  SdProjectItem()
  {

  }



//Return component uid
QString SdPItemInheritance::componentUid() const
  {
  return paramGet( SD_COMPONENT_UID );
  }




//Return component title
QString SdPItemInheritance::componentTitle() const
  {
  return paramGet( SD_COMPONENT_TITLE );
  }




//Set component uid
void SdPItemInheritance::setComponentUid(const QString uid, SdUndo *undo)
  {
  SdLibraryHeader hdr;
  if( SdObjectFactory::extractHeader( uid, hdr ) ) {
    paramSet( SD_COMPONENT_TITLE, QString("%1 (%2) [%3]").arg(hdr.mName).arg(hdr.mAuthor).arg(SdTime2x::toLocalString(hdr.mTime)), undo );
    paramSet( SD_COMPONENT_UID, uid, undo );
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
  return QString(":/pic/iconInheritance.png");
  }




quint64 SdPItemInheritance::getAcceptedObjectsMask() const
  {
  return 0;
  }
