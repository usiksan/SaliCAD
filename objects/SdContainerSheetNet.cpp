/*
Project "Electronic schematic and pcb CAD"

Author
  Sibilev Alexander S.

Web
  www.saliLab.com
  www.saliLab.ru

Description
*/
#include "SdContainerSheetNet.h"

SdContainerSheetNet::SdContainerSheetNet() :
  SdContainer()
  {

  }




SdContainerSheetNet::SdContainerSheetNet(const QString netName) :
  SdContainer(),
  mNetName(netName)
  {

  }



void SdContainerSheetNet::cloneFrom(const SdObject *src)
  {
  SdContainer::cloneFrom( src );
  const SdContainerSheetNet *net = dynamic_cast<const SdContainerSheetNet*>(src);
  Q_ASSERT( net != nullptr );
  mNetName = net->mNetName;
  }




void SdContainerSheetNet::writeObject(QJsonObject &obj) const
  {
  SdContainer::writeObject(obj);
  obj.insert( QStringLiteral("NetName"), mNetName );
  }



void SdContainerSheetNet::readObject(SdObjectMap *map, const QJsonObject obj)
  {
  SdContainer::readObject( map, obj );
  mNetName = obj.value( QStringLiteral("NetName") ).toString();
  }

